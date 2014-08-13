#include <opencv2/core.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "cameraInput.h"
#include "StereoRig.h"

using namespace cv;
using namespace std;

string fileList[] = { "calib1.jpg", "calib2.jpg", "calib3.jpg", "calib4.jpg", "calib5.jpg", "calib6.jpg" };

int main()
{
	StereoRig cap(0, 1);
	cap.cap0.calibrateByFiles(fileList, 6);
	cap.cap1.calibrateByFiles(fileList, 6);
	cap.calibrate();
	cap.saveRigConfig("config.yaml");
	return 0;
}