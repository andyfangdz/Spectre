#pragma once
#include "Tracker.h"
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;
class Control
{
public:
	Control(int videoSource);
	~Control();
	void iterate();
	bool isActive, started;
private:
	int Smin, Smax, Vmin, Vmax;
	VideoCapture cap;
	String windowName;
	Tracker tracker;
	Rect2f trackingWindow;
	static void onMouse(int event, int x, int y, int, void*);
	void onMouseCallBack(int event, int x, int y, int, void*);
	Point2f origin, current;
	Mat frame;
};

