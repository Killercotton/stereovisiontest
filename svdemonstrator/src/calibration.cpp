// code gets coefficients for camera calibration to perform rectification
#include <iostream>
#include <sstream>
#include <string>
#include <ctime>
#include <cstdio>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/features2d.hpp>


static void calcBoardCornerPositions(cv::Size boardSize, float squareSize, std::vector<cv::Point3f>& corners);

static bool runCalibration( cv::Size patternSize, float squareSize, cv::Size& imageSize, cv::Mat& cameraMatrix, cv::Mat& distCoeffs,
                            std::vector<std::vector<cv::Point2f> > imagePoints, std::vector<cv::Mat>& rvecs, std::vector<cv::Mat>& tvecs,
                            std::vector<float>& reprojErrs,  double& totalAvgErr);
                            
static double computeReprojectionErrors( const std::vector<std::vector<cv::Point3f> >& objectPoints,
                                         const std::vector<std::vector<cv::Point2f> >& imagePoints,
                                         const std::vector<cv::Mat>& rvecs, const std::vector<cv::Mat>& tvecs,
                                         const cv::Mat& cameraMatrix , const cv::Mat& distCoeffs,
                                         std::vector<float>& perViewErrors);
                      
// read configuration file with information about calibration pictures
int main(int , char** ){
	// calibration flags for findChessboardCorners
	//int chessBoardFlags = cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_NORMALIZE_IMAGE | cv::CALIB_CB_FAST_CHECK; // 13 detected pictures
	int chessBoardFlags = cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_NORMALIZE_IMAGE; // 40 detected pictures
	// int chessBoardFlags = cv::CALIB_CB_ADAPTIVE_THRESH; // 37 detected pictures
	// int chessBoardFlags = cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_FILTER_QUADS; // 37 detected pictures
	// int chessBoardFlags = cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_NORMALIZE_IMAGE | cv::CALIB_CB_FILTER_QUADS; // 40 detected pictures
	
	
	// side of one chessboard square in mm
	float squareSize = 10; 
	
	int detectedPictures = 0;
	// put filenames of all calibration pictures in a vector
	std::string folderpath = "/home/wolfgang/code/svdemonstrator/wind_tunnel/calibration/calib_L/*.png";
	std::vector<cv::String> filenames;
	cv::glob(folderpath, filenames);
	
	cv::namedWindow("left", cv::WINDOW_AUTOSIZE);
	
	// define chessboard pattern, pattern has 18x13 inner corners
	cv::Size patternsize;
	patternsize.width = 18;
	patternsize.height = 13;
	
	// vector that saves all detected corner vectors from findchessboardcorners
	std::vector<std::vector<cv::Point2f> > imagePoints;
	
	// loops through all calibration images and saves the detected corners
	for(cv::String name : filenames) {
		//std::cout << name << std::endl;
		
		cv::Mat image = cv::imread(name, cv::IMREAD_GRAYSCALE);
		if(image.empty()) {
		std::cout << "Image " << name << "couldn't be opened!" << std::endl;
		}
		// temporary vector for corners
		std::vector<cv::Point2f> pointBuf;
		
		// detect chessboard corners
		bool cornersExist = cv::findChessboardCorners(image, patternsize, pointBuf, chessBoardFlags);
		if(!cornersExist) {
			//std::cout << "Couldn't find chessboard corners!" << std::endl;
		}
		else {
			// when corners are detected, improve corner coordinates and save point vector, compares with and without
			// improvement 
			
			// debugging code: prints coordinates, draws corner coordinates
			//std::cout << "chessboard corners found.\n" << 
			//"x coordinate " << pointBuf.at(0).x << " y coordinate " << pointBuf.at(0).y << std::endl;
			// cv::Mat imageCopy = image.clone();
			// cv::drawChessboardCorners(imageCopy, patternsize, cv::Mat(pointBuf), cornersExist);
			// cv::imshow("left", imageCopy);
			// cv::waitKey(0);
			
			cv::cornerSubPix(image, pointBuf, cv::Size(5,5), cv::Size(-1,-1), 
			cv::TermCriteria( cv::TermCriteria::EPS+cv::TermCriteria::COUNT, 30, 0.1 ));
			
			//std::cout << "xi coordinate " << pointBuf.at(0).x << " yi coordinate " << pointBuf.at(0).y << std::endl;
			//cv::drawChessboardCorners(image, patternsize, cv::Mat(pointBuf), cornersExist);
			
			//cv::imshow("left", image);
			//cv::waitKey(0);
			imagePoints.push_back(pointBuf);
			detectedPictures++;
		}
	}
	
	
	
// loop through all available calibration pictures
	std::cout << "Number of successfully detected pictures: " << detectedPictures << std::endl;

	// fill vector with coordinates that describe the test pattern, necessary to calibrate camera 
	std::vector<std::vector<cv::Point3f> > objectPoints(1);
    //calcBoardCornerPositions(patternsize, squareSize, objectPoints[0]);
    
    cv::Mat image = cv::imread("/home/wolfgang/code/svdemonstrator/wind_tunnel/calibration/calib_L/frame001.png", cv::IMREAD_GRAYSCALE);
    // calibrate the camera
    cv::Size imageSize = image.size();
    cv::Mat cameraMatrix, distCoeffs;
    
    runCalibrationAndSave( patternsize, squareSize, imageSize, cameraMatrix, distCoeffs, imagePoints);
// calibrate if possible
return 0;
}

static void calcBoardCornerPositions(cv::Size boardSize, float squareSize, std::vector<cv::Point3f>& corners)
{
    corners.clear();

        for( int i = 0; i < boardSize.height; ++i ) {
            for( int j = 0; j < boardSize.width; ++j ) {
                corners.push_back(cv::Point3f(j*squareSize, i*squareSize, 0));
			}}
    
}


static bool runCalibration( cv::Size patternSize, float squareSize, cv::Size& imageSize, cv::Mat& cameraMatrix, cv::Mat& distCoeffs,
                            std::vector<std::vector<cv::Point2f> > imagePoints, std::vector<cv::Mat>& rvecs, std::vector<cv::Mat>& tvecs,
                            std::vector<float>& reprojErrs,  double& totalAvgErr)
{
    //! [fixed_aspect]
    cameraMatrix = cv::Mat::eye(3, 3, CV_64F);
    //if( s.flag & CALIB_FIX_ASPECT_RATIO )
    //    cameraMatrix.at<double>(0,0) = s.aspectRatio;
    //! [fixed_aspect]
    
        distCoeffs = cv::Mat::zeros(8, 1, CV_64F);

    std::vector<std::vector<cv::Point3f> > objectPoints(1);
    calcBoardCornerPositions(patternSize, squareSize, objectPoints[0]);

    objectPoints.resize(imagePoints.size(),objectPoints[0]);
	int calibrationFlags = 0;
	
    //Find intrinsic and extrinsic camera parameters
    double rms;

        rms = calibrateCamera(objectPoints, imagePoints, imageSize, cameraMatrix, distCoeffs, rvecs, tvecs, calibrationFlags);

    std::cout << "Re-projection error reported by calibrateCamera: "<< rms << std::endl;

    bool ok = checkRange(cameraMatrix) && checkRange(distCoeffs);

    totalAvgErr = computeReprojectionErrors(objectPoints, imagePoints, rvecs, tvecs, cameraMatrix,
                                            distCoeffs, reprojErrs);

    return ok;
}

static double computeReprojectionErrors( const std::vector<std::vector<cv::Point3f> >& objectPoints,
                                         const std::vector<std::vector<cv::Point2f> >& imagePoints,
                                         const std::vector<cv::Mat>& rvecs, const std::vector<cv::Mat>& tvecs,
                                         const cv::Mat& cameraMatrix , const cv::Mat& distCoeffs,
                                         std::vector<float>& perViewErrors)
{
    std::vector<cv::Point2f> imagePoints2;
    size_t totalPoints = 0;
    double totalErr = 0, err;
    perViewErrors.resize(objectPoints.size());

    for(size_t i = 0; i < objectPoints.size(); ++i )
    {
        
        cv::projectPoints(objectPoints[i], rvecs[i], tvecs[i], cameraMatrix, distCoeffs, imagePoints2);
        
        err = norm(imagePoints[i], imagePoints2, cv::NORM_L2);

        size_t n = objectPoints[i].size();
        perViewErrors[i] = (float) std::sqrt(err*err/n);
        totalErr        += err*err;
        totalPoints     += n;
    }

    return std::sqrt(totalErr/totalPoints);
}

// Print camera parameters to the output file
static void saveCameraParams( float squareSize, cv::Size& patternsize, cv::Size& imageSize, cv::Mat& cameraMatrix, cv::Mat& distCoeffs,
                              const std::vector<cv::Mat>& rvecs, const std::vector<cv::Mat>& tvecs,
                              const std::vector<float>& reprojErrs, const std::vector<std::vector<cv::Point2f> >& imagePoints,
                              double totalAvgErr )
{
	std::string outputFileName = "camera_parameters"; 
    cv::FileStorage fs( outputFileName, FileStorage::WRITE );

    time_t tm;
    time( &tm );
    struct tm *t2 = localtime( &tm );
    char buf[1024];
    strftime( buf, sizeof(buf), "%c", t2 );

    fs << "calibration_time" << buf;

    if( !rvecs.empty() || !reprojErrs.empty() )
        fs << "nr_of_frames" << (int)std::max(rvecs.size(), reprojErrs.size());
    fs << "image_width" << imageSize.width;
    fs << "image_height" << imageSize.height;
    fs << "board_width" << patternsize.width;
    fs << "board_height" << patternsize.height;
    fs << "square_size" << squareSize;

    //if( s.flag & CALIB_FIX_ASPECT_RATIO ){
    //    fs << "fix_aspect_ratio" << s.aspectRatio;
	//}
    if (flag)
    {
        std::stringstream flagsStringStream;
        if (s.useFisheye)
        {
            flagsStringStream << "flags:"
                << (s.flag & fisheye::CALIB_FIX_SKEW ? " +fix_skew" : "")
                << (s.flag & fisheye::CALIB_FIX_K1 ? " +fix_k1" : "")
                << (s.flag & fisheye::CALIB_FIX_K2 ? " +fix_k2" : "")
                << (s.flag & fisheye::CALIB_FIX_K3 ? " +fix_k3" : "")
                << (s.flag & fisheye::CALIB_FIX_K4 ? " +fix_k4" : "")
                << (s.flag & fisheye::CALIB_RECOMPUTE_EXTRINSIC ? " +recompute_extrinsic" : "");
        }
        else
        {
            flagsStringStream << "flags:"
                << (s.flag & CALIB_USE_INTRINSIC_GUESS ? " +use_intrinsic_guess" : "")
                << (s.flag & CALIB_FIX_ASPECT_RATIO ? " +fix_aspectRatio" : "")
                << (s.flag & CALIB_FIX_PRINCIPAL_POINT ? " +fix_principal_point" : "")
                << (s.flag & CALIB_ZERO_TANGENT_DIST ? " +zero_tangent_dist" : "")
                << (s.flag & CALIB_FIX_K1 ? " +fix_k1" : "")
                << (s.flag & CALIB_FIX_K2 ? " +fix_k2" : "")
                << (s.flag & CALIB_FIX_K3 ? " +fix_k3" : "")
                << (s.flag & CALIB_FIX_K4 ? " +fix_k4" : "")
                << (s.flag & CALIB_FIX_K5 ? " +fix_k5" : "");
        }
        fs.writeComment(flagsStringStream.str());
    }

    fs << "flags" << s.flag;

    fs << "fisheye_model" << s.useFisheye;

    fs << "camera_matrix" << cameraMatrix;
    fs << "distortion_coefficients" << distCoeffs;

    fs << "avg_reprojection_error" << totalAvgErr;
    if (s.writeExtrinsics && !reprojErrs.empty())
        fs << "per_view_reprojection_errors" << Mat(reprojErrs);

    if(s.writeExtrinsics && !rvecs.empty() && !tvecs.empty() )
    {
        CV_Assert(rvecs[0].type() == tvecs[0].type());
        Mat bigmat((int)rvecs.size(), 6, CV_MAKETYPE(rvecs[0].type(), 1));
        bool needReshapeR = rvecs[0].depth() != 1 ? true : false;
        bool needReshapeT = tvecs[0].depth() != 1 ? true : false;

        for( size_t i = 0; i < rvecs.size(); i++ )
        {
            Mat r = bigmat(Range(int(i), int(i+1)), Range(0,3));
            Mat t = bigmat(Range(int(i), int(i+1)), Range(3,6));

            if(needReshapeR)
                rvecs[i].reshape(1, 1).copyTo(r);
            else
            {
                //*.t() is MatExpr (not Mat) so we can use assignment operator
                CV_Assert(rvecs[i].rows == 3 && rvecs[i].cols == 1);
                r = rvecs[i].t();
            }

            if(needReshapeT)
                tvecs[i].reshape(1, 1).copyTo(t);
            else
            {
                CV_Assert(tvecs[i].rows == 3 && tvecs[i].cols == 1);
                t = tvecs[i].t();
            }
        }
        fs.writeComment("a set of 6-tuples (rotation vector + translation vector) for each view");
        fs << "extrinsic_parameters" << bigmat;
    }

    if(s.writePoints && !imagePoints.empty() )
    {
        Mat imagePtMat((int)imagePoints.size(), (int)imagePoints[0].size(), CV_32FC2);
        for( size_t i = 0; i < imagePoints.size(); i++ )
        {
            Mat r = imagePtMat.row(int(i)).reshape(2, imagePtMat.cols);
            Mat imgpti(imagePoints[i]);
            imgpti.copyTo(r);
        }
        fs << "image_points" << imagePtMat;
    }
}

//! [run_and_save]
bool runCalibrationAndSave(cv::Size patternSize, float squareSize, cv::Size imageSize, Mat& cameraMatrix, Mat& distCoeffs,
                           vector<vector<Point2f> > imagePoints)
{
    vector<Mat> rvecs, tvecs;
    vector<float> reprojErrs;
    double totalAvgErr = 0;

    bool ok = runCalibration(patternSize, squareSize, imageSize, cameraMatrix, distCoeffs, imagePoints, rvecs, tvecs,
        reprojErrs, totalAvgErr);
    cout << (ok ? "Calibration succeeded" : "Calibration failed")
         << ". avg re projection error = " << totalAvgErr << endl;

    if (ok)
        saveCameraParams(s, imageSize, cameraMatrix, distCoeffs, rvecs, tvecs, reprojErrs, imagePoints,
                         totalAvgErr);
    return ok;
}
