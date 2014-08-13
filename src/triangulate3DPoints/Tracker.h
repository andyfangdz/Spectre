#pragma once
#include <opencv2/video/tracking.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>
#define TRACKING 1
#define MISSING 2
#define PENGING 0
#define CAM_W 640
#define CAM_H 480

using namespace cv;
using namespace std;

class Tracker
{
public:
	Tracker();
	~Tracker();
	void setState(int state);
	int getState();
	Rect2f initialize(int selection[4], Mat frame, Scalar lowBound, Scalar highBound);
	float windowSize();
	RotatedRect iterate(Mat frame, Scalar lowBound, Scalar highBound);
	RotatedRect iterate(Mat frame, Mat additionalMask);
	Mat bkp;
	void process(int selection[4], Mat frame, Scalar lowBound, Scalar highBound);
	void process(Mat frame, Scalar lowBound, Scalar highBound);
	int status;
	Mat hsv, mask, hsv_roi, mask_roi;
	RotatedRect trackBox;
	Rect trackingWindow;
	Mat histogram;
	vector<Mat> hsv_planes;
	
};

