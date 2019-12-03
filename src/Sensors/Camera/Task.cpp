//***************************************************************************
// Copyright 2007-2019 Universidade do Porto - Faculdade de Engenharia      *
// Laboratório de Sistemas e Tecnologia Subaquática (LSTS)                  *
//***************************************************************************
// This file is part of DUNE: Unified Navigation Environment.               *
//                                                                          *
// Commercial Licence Usage                                                 *
// Licencees holding valid commercial DUNE licences may use this file in    *
// accordance with the commercial licence agreement provided with the       *
// Software or, alternatively, in accordance with the terms contained in a  *
// written agreement between you and Faculdade de Engenharia da             *
// Universidade do Porto. For licensing terms, conditions, and further      *
// information contact lsts@fe.up.pt.                                       *
//                                                                          *
// Modified European Union Public Licence - EUPL v.1.1 Usage                *
// Alternatively, this file may be used under the terms of the Modified     *
// EUPL, Version 1.1 only (the "Licence"), appearing in the file LICENCE.md *
// included in the packaging of this file. You may not use this work        *
// except in compliance with the Licence. Unless required by applicable     *
// law or agreed to in writing, software distributed under the Licence is   *
// distributed on an "AS IS" basis, WITHOUT WARRANTIES OR CONDITIONS OF     *
// ANY KIND, either express or implied. See the Licence for the specific    *
// language governing permissions and limitations at                        *
// https://github.com/LSTS/dune/blob/master/LICENCE.md and                  *
// http://ec.europa.eu/idabc/eupl.html.                                     *
//***************************************************************************
// Author: Marius Eskedal                                                   *
//***************************************************************************

// DUNE headers.
#include <DUNE/DUNE.hpp>
#include "opencv2/opencv.hpp"
#include "opencv2/aruco.hpp"

namespace Sensors
{
  //! Insert short task description here.
  //!
  //! Insert explanation on task behaviour here.
  //! @author Marius Eskedal
  namespace Camera
  {
    using DUNE_NAMESPACES;

    struct Arguments
    {
      std::string video_device_path;
    };

    struct Task: public DUNE::Tasks::Task
    {
      Arguments m_args;

      cv::VideoCapture m_cap;
      cv::Mat m_frame, m_frame_detection;
      cv::Ptr<cv::aruco::Dictionary> m_arUco_dict = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_ARUCO_ORIGINAL);
      const std::string m_input_window_name = "Input Feed";
      const std::string m_detection_window_name = "Detection Feed";

      //! Constructor.
      //! @param[in] name task name.
      //! @param[in] ctx context.
      Task(const std::string& name, Tasks::Context& ctx):
        DUNE::Tasks::Task(name, ctx)
      {
        param("Video device path", m_args.video_device_path)
        .defaultValue("/dev/video0");
        spew(m_args.video_device_path.c_str());
        m_cap.open("/dev/video0");

        cv::namedWindow(m_input_window_name);
        cv::namedWindow(m_detection_window_name);
      }

      //! Update internal state with new parameter values.
      void
      onUpdateParameters(void)
      {
      }

      //! Reserve entity identifiers.
      void
      onEntityReservation(void)
      {
      }

      //! Resolve entity names.
      void
      onEntityResolution(void)
      {

      }

      //! Acquire resources.
      void
      onResourceAcquisition(void)
      {
      }

      //! Initialize resources.
      void
      onResourceInitialization(void)
      {
      
      }

      //! Release resources.
      void
      onResourceRelease(void)
      {
      }


      //! Main loop.
      void
      onMain(void)
      {
        

        if(!m_cap.isOpened()){
          err("Error opening video stream or file");
        }
        // Check if camera opened successfully
        while (!stopping())
        {
          // Create a VideoCapture object and open the input file
          // If the input is the web camera, pass 0 instead of the video file name
            

          // spew("Main Loop");
          m_cap >> m_frame;
          m_frame_detection = m_frame;

          // Capture frame-by-frame
        
          // If the frame is empty, break immediately
          if (m_frame.empty()){
            break;
          }

          cv::imshow("Input Feed", m_frame);
          std::vector<int> ids;
          std::vector<std::vector<cv::Point2f> > corners;
          cv::aruco::detectMarkers(m_frame_detection, m_arUco_dict, corners, ids);          
      
          if (ids.size() > 0) {
            cv::aruco::drawDetectedMarkers(m_frame_detection, corners, ids);
          }
          cv::imshow(m_input_window_name, m_frame);
          cv::imshow(m_detection_window_name, m_frame_detection);


          // Press  ESC on keyboard to exit
          char c=(char)cv::waitKey(25);
          if(c==27) {
            break;
            // stopping = true;
          }
  
     
          // waitForMessages(1.0);
        }
        // // When everything done, release the video capture object
      }
    };
  }
}

DUNE_TASK
