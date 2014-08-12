#include <opencv2/core.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "cameraInput.h"

using namespace cv;
using namespace std;

string fileList[] = { "calib1.jpg", "calib2.jpg", "calib3.jpg", "calib4.jpg", "calib5.jpg", "calib6.jpg" };

int main()
{
	cameraInput cap(0);
	cap.calibrateByFiles(fileList, 6);
	//cap.calibrateByCamera();
	waitKey(0);
	while (true)
	{
		Mat img, img1;
		img = cap.getUndistortedImage();
		cap >> img1;
		imshow("Undistorted Image", img);
		imshow("Distorted Image", img1);
		char ch = waitKey(30);
		if (ch == 27)
		{
			break;
		}
	}
	return 0;
}