// first test to detect some features with opencv3.2
// compiled with: 
// g++ fastfeat.cpp -o featureApp -std=c++11 -Wall -Wextra -pedantic -I/home/wolfgang/libs/opencvlinux/include 
// -L/home/wolfgang/libs/opencvlinux/lib -lopencv_core -lopencv_imgcodecs -lopencv_features2d -lopencv_highgui

#include <opencv2/features2d.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <vector>
#include <iostream>

int main(int, char** ) {
	std::string filename = "bla.jpg";
	cv::Mat image = cv::imread(filename, cv::IMREAD_GRAYSCALE);
	std::vector<cv::KeyPoint> kpts;
	cv::Ptr<cv::FastFeatureDetector> fastfeat = cv::FastFeatureDetector::create();
	fastfeat->detect(image, kpts);
	cv::Mat imageKey;
	cv::drawKeypoints(image, kpts, imageKey);
	cv::namedWindow("Display Image", cv::WINDOW_AUTOSIZE );
    cv::imshow("Display Image", imageKey);
    cv::waitKey(0);
	return 0;
}
