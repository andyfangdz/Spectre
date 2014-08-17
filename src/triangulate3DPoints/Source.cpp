#include <opencv2/core.hpp>
#include <opencv2/viz.hpp>
#include "cameraInput.h"
#include "Control.h"
#include "StereoRig.h"
#include "Tracker.h"

using namespace cv;
using namespace std;

bool first = true;
viz::Viz3d myWindow("Coordinate Frame");
Point3f origin;

Point3f recent[5];
double weight[6] = {0.2, 0.2, 0.2, 0.2, 0.2, 0.2};
int top = 0;

Point3f preprocess(Point3f a)
{
	if (top < 4)
	{
		recent[top++] = a;
		return a;
	}
	else
	{
		Point3f tmp(0, 0, 0);
		for (int i = 0; i < 4; i++)
		{
			tmp += recent[i] * weight[i];
		}
		tmp += a * weight[4];
		for (int i = 0; i < 3; i++)
		{
			recent[i] = recent[i + 1];
		}
		recent[4] = tmp;
		return tmp;
	}
}

void update(Point3f pos)
{
	pos = preprocess(pos);
	if (first)
	{
		origin = pos + Point3f(-0.25, -0.25, 0.25);
		first = false;
	}
	if (!myWindow.wasStopped())
	{
		Point3f delta = pos - origin;
		Mat rot = Mat::eye(Size2d(3, 3), CV_32F);
		Affine3f pose = Affine3f(rot, Vec3f(delta) / 100);
		myWindow.setWidgetPose("Cube Widget", pose);
		myWindow.spinOnce(1, true);
	}

}

int main()
{
	StereoRig rig(0, 1);
	rig.loadRigConfig("new.yaml");
	//rig.calibrate();
	Control ctl0(rig.cap0), ctl1(rig.cap1);
	
	myWindow.showWidget("Coordinate Widget", viz::WCoordinateSystem());
	viz::WLine axis(Point3f(-1.0f, -1.0f, -1.0f), Point3f(1.0f, 1.0f, 1.0f));
	axis.setRenderingProperty(viz::LINE_WIDTH, 4.0);
	myWindow.showWidget("Line Widget", axis);
	viz::WCube cube_widget(Point3f(0.5, 0.5, 0.0), Point3f(0.0, 0.0, -0.5), true, viz::Color::blue());
	cube_widget.setRenderingProperty(viz::LINE_WIDTH, 4.0);
	myWindow.showWidget("Cube Widget", cube_widget);
	
	while (true)
	{
		vector<Point2f> imgPts0, imgPts1;
		Point3f pos;
		Mat pnts3D(1, 1, CV_64FC4);
		ctl0.iterate(); ctl1.iterate();
		if (ctl0.tracker.getState() == TRACKING && ctl1.tracker.getState() == TRACKING)
		{
			//imgPts0.push_back(Point2f(ctl0.tracker.trackingWindow.x, ctl0.tracker.trackingWindow.y));
			//imgPts1.push_back(Point2f(ctl1.tracker.trackingWindow.x, ctl1.tracker.trackingWindow.y));
			//triangulatePoints(rig.projectionMatrix0, rig.projectionMatrix1, Mat(imgPts0), Mat(imgPts1), pnts3D);
			double x0 = ctl0.tracker.trackingWindow.x, x1 = ctl1.tracker.trackingWindow.x;
			double y0 = ctl0.tracker.trackingWindow.y, y1 = ctl1.tracker.trackingWindow.y;
			double X, Y, Z, W;
			X = x0 * rig.Q.at<double>(0, 0) + rig.Q.at<double>(0, 3);
			Y = y0 * rig.Q.at<double>(1, 1) + rig.Q.at<double>(1, 3);
			Z = rig.Q.at<double>(2, 3);
			W = (x0 - x1) * rig.Q.at<double>(3, 2) + rig.Q.at<double>(3, 3);
			pos = Point3f(X / W, Y / W, Z / W);
			//pos = Point3f(pnts3D.at<float>(0) * (1 / pnts3D.at<float>(3)), pnts3D.at<float>(1) * (1 / pnts3D.at<float>(3)), pnts3D.at<float>(2) * (1 / pnts3D.at<float>(3)));
			//cout << pos << endl;
			update(pos);
		}
		char ch = waitKey(30);
		if (ch == 27)
		{
			break;
		}
		if (ch == 'c')
		{
			origin = pos + Point3f(-0.25, -0.25, 0.25);
		}
	}
}