// this code is written to undistort an image with the obtain calibration coefficients
// opencv3 used

#include <string>
#include <iostream>
 
#include <opencv2/core.hpp>					// Mat, fileStorage
#include <opencv2/imgcodecs.hpp> 			// imread
#include <opencv2/highgui.hpp>				// imshow
#include <opencv2/imgproc.hpp>				// undistort

int main(int, char** ) {
	// load intrinsic camera matrix and distortion coefficients
	// open intrinsic matrix(3x3) and distortion coefficients(5x0) filled with doubles
	std::string filename = "camera_parameters";
	cv::FileStorage fs(filename, cv::FileStorage::READ);
	
	if(!fs.isOpened()) {
		std::cout << "Could not open storage file." << std::endl;
	}
	
	cv::Mat cameraMatrix, distCoeff;
	
	fs["camera_matrix"] >> cameraMatrix;
	fs["distortion_coefficients"] >> distCoeff;
	//std::cout << cameraMatrix.at<double>(0,0) << " " << cameraMatrix.at<double>(0,1) << " " << cameraMatrix.at<double>(0,2) << " " << cameraMatrix.at<double>(1,0)
	//			 << " " << cameraMatrix.at<double>(1,1)  << " " << cameraMatrix.at<double>(1,2);
	//std::cout << distCoeff.at<double>(0,0) << " " << distCoeff.at<double>(1,0) << " " << distCoeff.at<double>(2,0) << " " << distCoeff.at<double>(3,0)
	//			 << " " << distCoeff.at<double>(4,0);
	
	// load distorted image
	std::string imageName = "../wind_tunnel/calibration/calib_L/frame033.png";
	cv::Mat distImage = cv::imread(imageName, cv::IMREAD_GRAYSCALE);
	if(distImage.empty()) {
		std::cout << "Could not open image." << std::endl;
	}
	// undistort
	cv::Mat undistImage;
	// undistort calculates a rectified image with the help of camera matrix and distortion coefficients
	cv::undistort(distImage, undistImage, cameraMatrix, distCoeff);
	// show result
	cv::namedWindow("original", cv::WINDOW_AUTOSIZE);
	cv::namedWindow("undistorted", cv::WINDOW_AUTOSIZE);
	cv::imshow("original", distImage);
	cv::imshow("undistorted", undistImage);
	cv::waitKey(0);
	return 0;
}
