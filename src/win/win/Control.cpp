#include "Control.h"

using namespace std;
Control::Control(cameraInput videoSource)
{
	Vmin = 10, Vmax = 256, Smin = 30, Smax = 256;
	isActive = false;
	tracker.setState(PENGING);
	windowName = "Camshift DEMO " + (char)(videoSource + '0');
	cap.open(videoSource);
	namedWindow(windowName, WINDOW_AUTOSIZE);
	setMouseCallback(windowName, Control::onMouse, this);
	started = false;
	createTrackbar("Vmin", windowName, &Vmin, 256, 0);
	createTrackbar("Vmax", windowName, &Vmax, 256, 0);
	createTrackbar("Smin", windowName, &Smin, 256, 0);
	createTrackbar("Smax", windowName, &Smax, 256, 0);

}


Control::~Control()
{
}

void Control::onMouse(int event, int x, int y, int flags, void* param)
{
	auto* ptr = static_cast<Control*>(param);
	ptr->onMouseCallBack(event, x, y, flags, param);
}

void Control::onMouseCallBack(int event, int x, int y, int flags, void* param)
{
	switch (event)
	{
	case EVENT_LBUTTONDOWN:
		origin = Point2f(x, y);
		isActive = true;
		tracker.setState(PENGING);
		break;

	case EVENT_MOUSEMOVE:
		if (isActive)
		{
			current = Point2f(x, y);
		}
		break;

	case EVENT_LBUTTONUP:
		if (isActive)
		{
			int selection[4];
			selection[0] = origin.x; selection[1] = origin.y;
			selection[2] = current.x; selection[3] = current.y;
			if (Rect2f(origin, current).size().area() >= 0)
			{
				//cout << "Did reach initialization." << endl;
				trackingWindow = tracker.initialize(selection, frame, Scalar(0, Smin, Vmin), Scalar(180, Smax, Vmax));
				isActive = false;
				started = true;
			}
			
		}
	}
}

void Control::iterate()
{
	cap >> frame;
	Mat vis = frame.clone();
	if (started && !isActive)
	{
		//cout << "here" << endl;
		RotatedRect CoM = tracker.iterate(frame.clone(), Scalar(0, Smin, Vmin), Scalar(180, Smax, Vmax));
		vis = tracker.bkp.clone();
		if (tracker.getState() == TRACKING)
		{
			ellipse(vis, CoM, Scalar(0, 0, 255), 3, LINE_AA);
		}
		rectangle(vis, trackingWindow, Scalar(0, 0, 255), 3, LINE_AA);
		//cout << isActive << endl;
	}
	if (isActive)
	{
		
		Mat roi(vis, Rect2d(origin, current));
		roi = ~roi;
		
	}
	imshow(windowName, vis);
}