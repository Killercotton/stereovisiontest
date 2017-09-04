//! [includes]

// compiles with: g++ display_image.cpp -o display_image -I/home/wolfgang/libs/opencvnormal/include -L/home/wolfgang/libs/opencvnormal/lib -lopencv_core -lopencv_highgui -lopencv_imgcodecs

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>
#include <string>
//! [includes]

//! [namespace]
using namespace cv;
//! [namespace]

using namespace std;

// functions written for debugging
void printType(Mat &mat) {
         if(mat.depth() == CV_8U)  printf("unsigned char(%d)", mat.channels());
    else if(mat.depth() == CV_8S)  printf("signed char(%d)", mat.channels());
    else if(mat.depth() == CV_16U) printf("unsigned short(%d)", mat.channels());
    else if(mat.depth() == CV_16S) printf("signed short(%d)", mat.channels());
    else if(mat.depth() == CV_32S) printf("signed int(%d)", mat.channels());
    else if(mat.depth() == CV_32F) printf("float(%d)", mat.channels());
    else if(mat.depth() == CV_64F) printf("double(%d)", mat.channels());
    else                           printf("unknown(%d)", mat.channels());
}

void printInfo(const char *prefix, Mat &mat) {
    printf("%s: ", prefix);
    printf("dim(%d, %d)", mat.rows, mat.cols);
    printType(mat);
    printf("\n");
}

void printInfo(Mat &mat) {
    printf("dim(%d, %d)", mat.rows, mat.cols);
    printType(mat);
    printf("\n");
}

int main( int argc, char** argv )
{
    //! [load]
    String imageName( "/home/wolfgang/Downloads/Public/fbecker/City/without_rectification/0200_0.png" ); // by default
    if( argc > 1)
    {
        imageName = argv[1];
    }
    //! [load]
	//cout << imageName << endl;
    //! [mat]
    Mat image;
    //! [mat]

    //! [imread]
    image = imread( imageName, IMREAD_ANYDEPTH ); // Read the file
    //! [imread]

    if( image.empty() )                      // Check for invalid input
    {
        cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }

    //! [window]
    namedWindow( "Display window", WINDOW_AUTOSIZE ); // Create a window for display.
    //! [window]
    
    printInfo(image);
	image.convertTo(image, CV_16U, 20); //255/(16*16.));
    //! [imshow]
    imshow( "Display window", image );                // Show our image inside it.
    //! [imshow]

    //! [wait]
    waitKey(0); // Wait for a keystroke in the window
    //! [wait]
    return 0;
}
