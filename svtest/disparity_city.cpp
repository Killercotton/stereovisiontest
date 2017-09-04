// get left and right frame from the video and perform stereo matching on them. the disparity map (video) is saved in a file
// there is a video in /Downloads/Public that is saved as a bunch of *.pngs

#include <opencv2/core.hpp> // opencv core functions
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>  // OpenCV window I/O
#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>
#include <string>

int main(int, char**) {
	// initialize left and right frame, load from file system
	cv::Mat fLeft, fRight, showLeft, showRight;
	std::string filename = "/home/wolfgang/Downloads/Public/fbecker/City/with_rectification/";
	//create object that does stereo matching and configure algorithm
	int disparities = 64;
	int minDisparity = 0;
	int blocksize = 5;
	//cv::Ptr<cv::StereoBM> bm = cv::StereoBM::create(disparities, blocksize);
	cv::Ptr<cv::StereoSGBM> sgbm = cv::StereoSGBM::create(minDisparity, disparities, blocksize);
	cv::Mat disp, disp8;
	//cv::Rect roi1, roi2;
	//bm->setROI1(roi1);
	//bm->setROI2(roi2);
	//sgbm->setPreFilterCap(31);
	//sgbm->setMinDisparity(1);
	//bm->setTextureThreshold(10);
	//bm->setUniquenessRatio(15);
	//bm->setSpeckleWindowSize(100);
	//bm->setSpeckleRange(32);
	//bm->setDisp12MaxDiff(1);
	sgbm->setPreFilterCap(63);
    int sgbmWinSize = blocksize;
    sgbm->setBlockSize(sgbmWinSize);
	fLeft = cv::imread(filename + "0000_0.png", cv::IMREAD_GRAYSCALE);
    int cn = fLeft.channels();

    sgbm->setP1(8*cn*sgbmWinSize*sgbmWinSize);
    sgbm->setP2(32*cn*sgbmWinSize*sgbmWinSize);
    sgbm->setMinDisparity(0);
    sgbm->setNumDisparities(disparities);
    sgbm->setUniquenessRatio(10);
    sgbm->setSpeckleWindowSize(100);
    sgbm->setSpeckleRange(32);
    sgbm->setDisp12MaxDiff(1);
	// loops through all .png images
	for(int i = 0; i < 255; i++){
		//std::cout << filename + "000" + std::to_string(i) + "_0" << std::endl;
		if(i >= 100){
			fLeft = cv::imread(filename + "0" + std::to_string(i) + "_0.png", cv::IMREAD_GRAYSCALE);
			fRight = cv::imread(filename + "0" + std::to_string(i) + "_1.png", cv::IMREAD_GRAYSCALE);
		}
		else if(i >= 10){
			fLeft = cv::imread(filename + "00" + std::to_string(i) + "_0.png", cv::IMREAD_GRAYSCALE);
			fRight = cv::imread(filename + "00" + std::to_string(i) + "_1.png", cv::IMREAD_GRAYSCALE);
		}
		else{
			fLeft = cv::imread(filename + "000" + std::to_string(i) + "_0.png", cv::IMREAD_GRAYSCALE);
			fRight = cv::imread(filename + "000" + std::to_string(i) + "_1.png", cv::IMREAD_GRAYSCALE);
		}
		cv::namedWindow("left", cv::WINDOW_AUTOSIZE);
		cv::namedWindow("right", cv::WINDOW_AUTOSIZE);
		cv::namedWindow("disparity", cv::WINDOW_AUTOSIZE);
		//cv::cvtColor(fLeft, fLeft, cv::COLOR_BGR2GRAY);
		//cv::cvtColor(fRight, fRight, cv::COLOR_BGR2GRAY);
		fLeft.convertTo(showLeft, CV_8U, 20);
		fRight.convertTo(showRight, CV_8U, 20);
		cv::imshow("left", showLeft);
		cv::imshow("right", showRight);
		//cv::cvtColor(fLeft, fLeft, cv::COLOR_BGR2GRAY);
		//cv::cvtColor(fRight, fRight, cv::COLOR_BGR2GRAY);
		//fLeft.convertTo(fLeft, CV_8UC1);
		//fRight.convertTo(fRight, CV_8UC1);
		sgbm->compute(fLeft, fRight, disp);
		// convert images to output that makes sense
		//fLeft.convertTo(fLeft, CV_16U, 20);
		//fRight.convertTo(fRight, CV_16U, 20);
		// make windows and show both frames
		disp.convertTo(disp8, CV_8U, 255/(disparities*16.));
		cv::imshow("disparity", disp8);
		
		cv::waitKey(0);
	}
	if(fLeft.empty()){
		std::cout << "Could not open image." << std::endl;
	}
	cv::waitKey(0);
	// create stereo matching object, maybe block matching
	
	// configure block matching object
	
	// compute disparity map
	
	// save disparity map in a file
	
	// display time elapsed

}
