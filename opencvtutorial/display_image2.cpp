#include <opencv2/opencv.hpp>
using namespace cv;
int main( int argc, char** argv )
{
 char* imageName = argv[1];
 Mat image;
 image = imread( imageName, 1 );
 if( argc != 2 || !image.data )
 {
   printf( " No image data \n " );
   return -1;
 }
 Mat gray_image;
 cvtColor( image, gray_image, COLOR_BGR2GRAY );
 imwrite( "/home/wolfgang/Pictures/Gray_Image.jpg", gray_image );
 namedWindow( imageName, WINDOW_AUTOSIZE );
 namedWindow( "Gray image", WINDOW_AUTOSIZE );
 imshow( imageName, image );
 imshow( "Gray image", gray_image );
 std::cout << gray_image << std::endl;
 waitKey(0);
 return 0;
}
