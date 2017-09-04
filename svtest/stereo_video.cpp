// extract two VGA frames from a video and perform stereo matching algorithm. do this for the whole video
// this is to be performed on the videos in the Downloads folder. These contain the left and the right frame

#include <opencv2/core.hpp> // opencv core functions
#include <opencv2/imgcodecs.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>  // OpenCV window I/O

int main(int, char**) {
	// initialize left and right frame
	cv::Mat fLeft, fRight;
	// extract frames from the video
	cv::VideoCapture 
	// create stereo matching object, maybe block matching
	
	// configure block matching object
	
	// compute disparity map
	
	// save disparity map in a file
	
	// display time elapsed

}
