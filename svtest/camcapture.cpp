// captures frames from my webcam

#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>

int main( int argc, char** argv) {
	cv::Mat frame;
    //--- INITIALIZE VIDEOCAPTURE
    cv::VideoCapture cap;
    // open the default camera using default API
    cap.open(0);
    // OR advance usage: select any API backend
    int deviceID = 0;             // 0 = open default camera
    int apiID = cv::CAP_ANY;      // 0 = autodetect default API
    // open selected camera using selected API
    cap.open(deviceID + apiID);
    // check if we succeeded
    if (!cap.isOpened()) {
        std::cerr << "ERROR! Unable to open camera\n";
        return -1;
    }
    //--- GRAB AND WRITE LOOP
    std::cout << "Start grabbing" << std::endl
        << "Press any key to terminate" << std::endl;
    for (;;)
    {
        // wait for a new frame from camera and store it into 'frame'
        cap.read(frame);
        // check if we succeeded
        if (frame.empty()) {
            std::cerr << "ERROR! blank frame grabbed\n";
            //break;
        }
        // show live and wait for a key with timeout long enough to show images
        imshow("Live", frame);
        if (cv::waitKey(50) == 97){ // 97 equals 'a'
			std::cout << "shutdown" << std::endl;
            break;
		}
    }
	return 0;
}
