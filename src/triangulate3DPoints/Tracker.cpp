#include "Tracker.h"
#include <iostream>
using namespace std;

Tracker::Tracker()
{
	setState(PENGING);
}


Tracker::~Tracker()
{
}

int Tracker::getState()
{
	return status;
}

void Tracker::setState(int state)
{
	if (state >= PENGING && state <= MISSING)
	{
		status = state;
	}
}

Rect2f Tracker::initialize(int selection[4], Mat frame, Scalar lowBound, Scalar highBound)
{
	trackingWindow = Rect(Point2f(selection[0], selection[1]), Point2f(selection[2], selection[3]));
	Mat initialHist;
	process(selection, frame, lowBound, highBound);
	int histsize = 16;
	int channels[] = { 0 };
	float range[] = { 0, 180 };
	const float* histRange = { range };
	//calcHist(&hsv_roi, 1, 0, mask_roi, initialHist, 1 ,  &histsize, &histRange);
	cv::calcHist(&hsv_roi, 1 , 0, mask_roi, initialHist, 1, &histsize, &histRange );
	normalize(initialHist, initialHist, 0, 255, NORM_MINMAX);
	histogram = initialHist.clone();
	cout << histogram;
	setState(TRACKING);
	return trackingWindow;
}

float Tracker::windowSize()
{
	return trackingWindow.width * trackingWindow.height;
}

void Tracker::process(int selection[4], Mat frame, Scalar lowBound, Scalar highBound)
{
	Rect2d roi(Point2d(selection[0], selection[1]), Point2d(selection[2], selection[3]));
	cvtColor(frame, hsv, COLOR_BGR2HSV);
	split(hsv, hsv_planes);
	inRange(hsv, lowBound, highBound, mask);
	hsv_roi = Mat(hsv_planes[0], roi);
	mask_roi = Mat(mask, roi);
}
void Tracker::process(Mat frame, Scalar lowBound, Scalar highBound)
{
	//ect2d roi(Point2d(selection[0], selection[1]), Point2d(selection[2], selection[3]));
	
	cvtColor(frame, hsv, COLOR_BGR2HSV);
	split(hsv, hsv_planes);
	inRange(hsv, lowBound, highBound, mask);
	//hsv_roi = Mat(hsv, roi);
	//mask_roi = Mat(mask, roi);
}


RotatedRect Tracker::iterate(Mat frame, Scalar lowBound, Scalar highBound)
{
	process(frame, lowBound, highBound);
	float range[] = { 0, 180 };
	const float* histRange = { range };
	calcBackProject(&hsv_planes[0], 1, 0, histogram, bkp, &histRange);
	bkp &= mask;
	int element_shape = MORPH_RECT;
	int an = 2;
	Mat element = getStructuringElement(element_shape, Size(an * 2 + 1, an * 2 + 1), Point(an, an));
	morphologyEx(bkp, bkp, MORPH_OPEN, element);
	morphologyEx(bkp, bkp, MORPH_CLOSE, element);
	trackBox = CamShift(bkp, trackingWindow,
		TermCriteria(TermCriteria::EPS | TermCriteria::COUNT, 10, 1));
	if (trackingWindow.size().area() <= 1)
	{
		int cols = bkp.cols, rows = bkp.rows, r = (MIN(cols, rows) + 5) / 6;
		trackingWindow = Rect(trackingWindow.x - r, trackingWindow.y - r,
			trackingWindow.x + r, trackingWindow.y + r) &
			Rect(0, 0, cols, rows);
		setState(MISSING);
	}
	setState(TRACKING);
	return trackBox;
}