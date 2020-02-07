#pragma once

#include <stdio.h> 
#include "h264decoder.hpp"


#define FRAME_BUFFER_SIZE 32768

void print_buffer(unsigned char *buffer, ssize_t buffer_length);

void append_buffer(unsigned char *buffer, ssize_t buffer_length, unsigned char *in_buffer, ssize_t in_buffer_size);

class Tello {
    private:
        H264Decoder decoder;
        ConverterRGB24 converter;


        unsigned char frame_buffer[FRAME_BUFFER_SIZE];
        // unsigned char frame_buffer_cpy[FRAME_BUFFER_SIZE];


    public:
        void disp_frame(AVFrame rgb_frame);
        void receive_video();
        void decode(ssize_t data_in_size);

        void show_video();
};
