#include "Tello.hpp"

#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h>
#include <libavcodec/avcodec.h>
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/photo.hpp"
#include <iostream>
#include <stdio.h> 

#define PORT    11111 
#define MAXLINE 2048
#define FRAME_SIZE 1460

void print_buffer(unsigned char *buffer, ssize_t buffer_length) {
    for ( int i = 0; i < buffer_length; i++) {
        printf("%u ", buffer[i]);
    }
    printf("\n");
}

void append_buffer(unsigned char *buffer, ssize_t buffer_length, unsigned char *in_buffer, ssize_t in_buffer_size) {

    for (int i = 0; i < in_buffer_size; i++) {
        buffer[buffer_length + i] = in_buffer[i];
    }

}

void Tello::disp_frame(AVFrame rgb_frame) {

    cv::Mat cvFrame(rgb_frame.height, rgb_frame.width, CV_8UC3, rgb_frame.data[0], rgb_frame.linesize[0]);
    // Converts to 
    cv::cvtColor(cvFrame, cvFrame, cv::COLOR_RGB2BGR);
    // cv::fastNlMeansDenoisingColored(cvFrame, cvFrame);
    cv::imshow("Stream", cvFrame);

    char key = (char) cv::waitKey(25);
    if (key == 'q' || key == 27)
    {
        return;
    }
    // printf("Width: %d, Height: %d\n", rgb_frame.width, rgb_frame.height);
}

void Tello::show_video() {

    // while (true) {
    //     int frames_available = frames.size();
    //     if (!frames_available) {
    //         continue;
    //     }
    //     for (int i = 0; i < frames_available; i ++) {
    //         AVFrame frame = frames.front();
    //         frames.pop();
    //         disp_frame(frame);
    //     }
    // }
}



void Tello::decode(ssize_t data_in_size) {
    // Deep copy in data
    int data_in_cpy_end = data_in_size;
    // unsigned char data_in_cpy[data_in_size];
    // for (int i = 0; i < data_in_size; i++) {
    //     frame_buffer_cpy[i] = frame_buffer[i];
    // }

    int data_in_cpy_start = 0;

    try
    {
        while (data_in_cpy_end > 0)
        {
        ssize_t num_consumed = 0;
        bool is_frame_available = false;
        
        try
        {

            num_consumed = decoder.parse((frame_buffer + data_in_cpy_start), data_in_cpy_end);
            
            if (is_frame_available = decoder.is_frame_available())
            {
                const auto &frame = decoder.decode_frame();
                int w, h; std::tie(w,h) = width_height(frame);
                ssize_t out_size = converter.predict_size(w,h);

                unsigned char out_buffer[out_size];

                AVFrame rgb_frame = converter.convert(frame, out_buffer);
                disp_frame(rgb_frame);
                // m.lock();
                // frames.push(rgb_frame);
                // m.unlock();
                //TODO finetune this sleep, stack will be smashed if not, or alternatively make a max size queue.
                // usleep(10000);
            }

        }
        catch (const H264DecodeFailure &e)
        {
            if (num_consumed <= 0)
            // This case is fatal because we cannot continue to move ahead in the stream.
            throw e;
        }
        
        data_in_cpy_end -= num_consumed;
        data_in_cpy_start += num_consumed;
        }
    }
    catch (const H264DecodeFailure &e)
    {
    }
}

void Tello::receive_video() {
    int sockfd; 
    unsigned char buffer[MAXLINE]; 

    struct sockaddr_in servaddr;

    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 

    memset(&servaddr, 0, sizeof(servaddr));

    // Filling server information 
    servaddr.sin_family    = AF_INET; // IPv4 
    servaddr.sin_addr.s_addr = INADDR_ANY; 
    servaddr.sin_port = htons(PORT); 


    if ( bind(sockfd, (const struct sockaddr *)&servaddr,  
            sizeof(servaddr)) < 0 ) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 

    ssize_t udp_data_size = 0;
    socklen_t len; 

    ssize_t frame_buffer_data_length = 0;

    while (true) {
        udp_data_size = recvfrom(sockfd, buffer, MAXLINE,  
                    MSG_WAITALL, nullptr, 
                    nullptr); 


        ssize_t frame_buffer_remaining_length = FRAME_BUFFER_SIZE - frame_buffer_data_length;
        if (frame_buffer_remaining_length > udp_data_size) {
            append_buffer(frame_buffer, frame_buffer_data_length, buffer, udp_data_size);
            frame_buffer_data_length += udp_data_size;
        } else {
            printf("Buffer overflow\n");
            frame_buffer_data_length = 0;
        }
        
        // printf("udp data bytes: %lu, size of buffer: %lu, curent data length: %lu\n", udp_data_size, sizeof(frame_buffer), frame_buffer_data_length);

        if (udp_data_size != FRAME_SIZE) { //A packet of less size than 1460 bytes marks the end of the frame being transmissioned.
            decode(frame_buffer_data_length);
            frame_buffer_data_length = 0;
        }
    }
}
