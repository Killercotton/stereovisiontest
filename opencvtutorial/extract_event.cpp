// objective of this code is to compare two consecutive frames of a video and extract
// pixels with intensity changes
// uses c++11, opencv

#include <iostream> // for standard I/O
#include <string>   // for strings
//#include <iomanip>  // for controlling float print precision
//#include <sstream>  // string to number conversion

#include <opencv2/core.hpp>     // Basic OpenCV structures (cv::Mat, Scalar)
#include <opencv2/imgproc.hpp>  // Gaussian Blur
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>  // OpenCV window I/O

int main(int argc, char *argv[]){
	// load a video
	// load two frames in mat object
	// iterate over all pixels and save pixel position with changing intensity 
	// in vector 
	//cv::VideoCapture testVideo("/home/wolfgang/Downloads/opencvtutorial/coastguard_cif.yuv");
	cv::Mat testImage;
	testImage = cv::imread("/home/wolfgang/Downloads/opencvtutorial?coastguard_cif.y4m");
	std::cout << cv::getBuildInformation() << std::endl;
	if( testImage.empty()){
		std::cout << "Could not open video." << std::endl;
		std::cout <<  "~/Downloads/opencvtutorial/coastguard_cif.yuv" << std::endl;
	}
	return 0;
}
