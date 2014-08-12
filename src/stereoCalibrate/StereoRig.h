#pragma once
#include <opencv2/core.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/core_c.h>
#include <opencv2/features2d.hpp>
#include "cameraInput.h"
#include <string>
using namespace std;
using namespace cv;
class StereoRig
{
public:
	StereoRig(int camIndex0, int camIndex1);
	~StereoRig();
	cameraInput cap0;
	cameraInput cap1;
	Mat E;
	Mat F;
	vector<cv::Mat> R, T;
	void calibrate();
	void saveRigConfig(string fileName);
	void loadRigConfig(string fileName);
};

