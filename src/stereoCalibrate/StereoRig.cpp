#include "StereoRig.h"
#include <opencv2/core.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include "cameraInput.h"
#include <string>
#define True true


StereoRig::StereoRig(int camIndex0, int camIndex1)
{
	cap0 = cameraInput(camIndex0);
	cap1 = cameraInput(camIndex1);

}


StereoRig::~StereoRig()
{
}


void StereoRig::calibrate()
{
	string name0 = "Image 0", name1 = "Image 1";
	cv::namedWindow(name0, cv::WINDOW_AUTOSIZE);
	cv::namedWindow(name1, cv::WINDOW_AUTOSIZE);
	cv::moveWindow(name0, 10, 10);
	cv::moveWindow(name1, 660, 10);
	vector<vector<cv::Point2f>> img_points0, img_points1;
	vector<vector<cv::Point3f>> obj_points;
	
	while (true)
	{
		Mat img0, img1;
		cap0.read(img0); cap1.read(img1);
		imshow(name0, img0);
		imshow(name1, img1);
		char ch = waitKey(30);
		if (ch == 'c')
		{
			cv::Size chessboardSize(9, 6);
			double length = 27.0;
			vector<cv::Point3f> objPoints;

			//cv::Mat corners[10];
			
			for (int i = 0; i < 6; i++)
			{
				for (int j = 0; j < 9; j++)
				{
					cv::Point3f cur(j*length, i*length, 0);
					objPoints.push_back(cur);
				}
			}
			cvtColor(img0, img0, cv::COLOR_BGR2GRAY);
			cvtColor(img1, img1, cv::COLOR_BGR2GRAY);
			vector<cv::Point2f> corners0, corners1;
			corners0.resize(54); corners1.resize(54);
			bool patternFound0 = cv::findChessboardCorners(img0, chessboardSize, cv::Mat(corners0));
			bool patternFound1 = cv::findChessboardCorners(img1, chessboardSize, cv::Mat(corners1));
			if (patternFound0 && patternFound1)
			{
				cv::Size winSize(5, 5);
				cv::Size zeroZone(-1, -1);
				cv::TermCriteria criteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.001);
				cornerSubPix(img0, corners0, winSize, zeroZone, criteria);
				cornerSubPix(img1, corners1, winSize, zeroZone, criteria);
				cv::cvtColor(img0, img0, cv::COLOR_GRAY2BGR);
				cv::cvtColor(img1, img1, cv::COLOR_GRAY2BGR);
				cv::drawChessboardCorners(img0, chessboardSize, cv::Mat(corners0), true);
				cv::drawChessboardCorners(img1, chessboardSize, cv::Mat(corners0), true);
				img_points0.push_back(corners0);
				img_points1.push_back(corners1);
				obj_points.push_back(objPoints);
				imshow(name0, img0);
				imshow(name1, img1);
				waitKey(0);
			}
		}
		if (ch == 27)
		{
			break;
		}
	}
	stereoCalibrate(obj_points, img_points0, img_points1, cap0.cameraMatrix, cap0.distCoeffs, cap1.cameraMatrix, cap1.distCoeffs, cv::Size(640, 480), R, T, E, F);
}


void StereoRig::saveRigConfig(string fileName)
{
	FileStorage fs(fileName, FileStorage::WRITE);
	fs["CameraMatrix0"]>>cap0.cameraMatrix;
	fs["distCoeffs0"]>>cap0.distCoeffs;
	fs["CameraMatrix1"]>>cap1.cameraMatrix;
	fs["distCoeffs1"]>>cap1.distCoeffs;
	fs["R"]>>R;
	fs["T"]>>T;
	fs["E"]>>E;
	fs["F"]>>F;
}


void StereoRig::loadRigConfig(string fileName)
{
	FileStorage fs(fileName, FileStorage::READ);
	fs << "CameraMatrix0" << cap0.cameraMatrix;
	fs << "distCoeffs0" << cap0.distCoeffs;
	fs << "CameraMatrix1" << cap1.cameraMatrix;
	fs << "distCoeffs1" << cap1.distCoeffs;
	fs << "R" << R;
	fs << "T" << T;
	fs << "E" << E;
	fs << "F" << F;
}
