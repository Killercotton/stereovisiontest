// loads two images, scales them to 640x480(VGA) and performs stereo matching

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/calib3d.hpp>

#include <iostream>

int main(int argc, char** argv) {
cv::Mat image1, image2;
image1 = cv::imread("../svtest/im0.png", CV_LOAD_IMAGE_GRAYSCALE);
image2 = cv::imread("../svtest/im1.png", CV_LOAD_IMAGE_GRAYSCALE);

if(!image1.data || !image2.data) {
std::cout << "Could not open or find image." << std::endl;
return -1;
}
// shrink both images to 640x480
// horizontal factor 0.2286
// vertical factor 0.2516
cv::Size2i size;
size.height = 0;
size.width = 0;
cv::resize(image1, image1, size, 0.2286, 0.2516, 3);
cv::resize(image2, image2, size, 0.2286, 0.2516, 3);
// create windows that display images
cv::namedWindow("left", CV_WINDOW_AUTOSIZE);
cv::namedWindow("right", CV_WINDOW_AUTOSIZE);
cv::namedWindow("disp", CV_WINDOW_AUTOSIZE);
//create object that does stereo matching and configure algorithm
int disparities = 64;
int blocksize = 9;
cv::Ptr<cv::StereoBM> bm = cv::StereoBM::create(disparities, blocksize);
cv::Mat disp, disp8;
cv::Rect roi1, roi2;
bm->setROI1(roi1);
bm->setROI2(roi2);
bm->setPreFilterCap(31);
bm->setMinDisparity(0);
bm->setTextureThreshold(10);
bm->setUniquenessRatio(15);
bm->setSpeckleWindowSize(100);
bm->setSpeckleRange(32);
bm->setDisp12MaxDiff(1);
// measure time necessary for stereo matching
int64 t = cv::getTickCount();
// stereo matching
bm->compute(image1, image2, disp);
t = cv::getTickCount() - t;
printf("Time elapsed: %fms\n", t*1000/cv::getTickFrequency());
// convert to visible output
disp.convertTo(disp8, CV_8U, 255/(disparities*16.));
cv::imshow("left", image1);
cv::imshow("right", image2);
cv::imshow("disp", disp8);
cv::waitKey(0);

return 0;
}
