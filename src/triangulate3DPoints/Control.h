#pragma once
#include "Tracker.h"
#include <opencv2/highgui.hpp>
#include "cameraInput.h"
#include <iostream>

using namespace cv;
using namespace std;
class Control
{
public:
	Control(cameraInput videoInput);
	~Control();
	void iterate();
	bool isActive, started;
	char windowName[100];
	Tracker tracker;
private:
	int Smin, Smax, Vmin, Vmax;
	VideoCapture cap;
	
	
	Rect2f trackingWindow;
	static void onMouse(int event, int x, int y, int, void*);
	void onMouseCallBack(int event, int x, int y, int, void*);
	Point2f origin, current;
	Mat frame;
};

