#pragma once
#include <opencv2/core.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/core_c.h>
#include <opencv2/features2d.hpp>
#include <string>
using namespace std;
class cameraInput :
	public cv::VideoCapture
{
public:
	cameraInput();
	cameraInput(int num);
	~cameraInput();
	cv::Mat getUndistortedImage();
	bool saveCameraProfile(string fileName);
	bool loadCameraProfile(string fileName);
	void calibrateByCamera();
	void calibrateByFiles(string fileList[], int fileNum);
	int cameraIndex;
	cv::Mat cameraMatrix;
	cv::Mat distCoeffs;
	vector<cv::Mat> R, T;
};

