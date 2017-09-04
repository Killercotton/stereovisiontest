// loads two images
 
 #include <opencv2/core/core.hpp>
 #include <opencv2/highgui/highgui.hpp>
 
 #include <iostream>
 
 int main(int argc, char** argv) {
	 cv::Mat image1, image2;
	 image1 = cv::imread("../opencvtest/im0.png", CV_LOAD_IMAGE_GRAYSCALE);
	 image2 = cv::imread("../opencvtest/im1.png", CV_LOAD_IMAGE_GRAYSCALE);
	 
	 if(!image1.data || !image2.data) {
		 std::cout << "Could not open or find image." << std::endl;
		 return -1;
	 }
	 
	 cv::namedWindow("left", CV_WINDOW_AUTOSIZE);
	 cv::namedWindow("right", CV_WINDOW_AUTOSIZE);
	 cv::imshow("left", image1);
	 cv::imshow("right", image2);
	 cv::waitKey(0);
	 
	 return 0;
 }
