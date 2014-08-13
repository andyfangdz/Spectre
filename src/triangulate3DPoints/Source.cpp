#include <opencv2/core.hpp>
#include "cameraInput.h"
#include "Control.h"
#include "StereoRig.h"
#include "Tracker.h"

using namespace cv;
using namespace std;

int main()
{
	StereoRig rig(0, 1);
	rig.loadRigConfig("new.yaml");
	Control ctl0(rig.cap0), ctl1(rig.cap1);
	while (true)
	{
		vector<Point2f> imgPts0, imgPts1;
		Mat pnts3D(1, 1, CV_64FC4);
		ctl0.iterate(); ctl1.iterate();
		if (ctl0.tracker.getState() == TRACKING && ctl1.tracker.getState() == TRACKING)
		{
			imgPts0.push_back(Point2f(ctl0.tracker.trackingWindow.x, ctl0.tracker.trackingWindow.y));
			imgPts1.push_back(Point2f(ctl1.tracker.trackingWindow.x, ctl1.tracker.trackingWindow.y));
			triangulatePoints(rig.projectionMatrix0, rig.projectionMatrix1, Mat(imgPts0), Mat(imgPts1), pnts3D);
			cout << pnts3D << endl;
		}
		char ch = waitKey(30);
		if (ch == 27)
		{
			break;
		}
	}
}