#pragma once
#include "Tracker.h"
#include "cameraInput.h"
#include <opencv2/highgui.hpp>
#include <iostream>
#include <opencv2/videoio.hpp>

using namespace cv;
using namespace std;
class Control
{
public:
	Control(cameraInput videoSource);
	~Control();
	void iterate();
	bool isActive, started;
private:
	int Smin, Smax, Vmin, Vmax;
	String windowName;
	Tracker tracker;
	Rect2f trackingWindow;
	static void onMouse(int event, int x, int y, int, void*);
	void onMouseCallBack(int event, int x, int y, int, void*);
	Point2f origin, current;
	Mat frame;
};

