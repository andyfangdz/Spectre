#include "cameraInput.h"
#include <opencv2/core.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/core_c.h>
#include <opencv2/features2d.hpp>
#include <string>
#include <iostream>
using namespace std;
using namespace cv;

cameraInput::cameraInput(int num)
{
	cameraIndex = num;
	open(cameraIndex);
}

cameraInput::cameraInput()
{
	cameraInput(0);
}

cameraInput::~cameraInput()
{
}


cv::Mat cameraInput::getUndistortedImage()
{
	cv::Mat img, proc;
	read(img);
	undistort(img, proc, cameraMatrix, distCoeffs);
	return proc;
}


bool cameraInput::saveCameraProfile(string fileName)
{
	FileStorage fs(fileName, FileStorage::WRITE);
	fs << "CameraMatrix" << cameraMatrix;
	fs << "distCoeffs" << distCoeffs;
	fs << "R" << R;
	fs << "T" << T;
	return true;
}


bool cameraInput::loadCameraProfile(string fileName)
{
	FileStorage fs(fileName, FileStorage::READ);
	fs["CameraMatrix"] >> cameraMatrix;
	fs["distCoeffs"] >> distCoeffs;
	fs["R"] >> R;
	fs["T"] >> T;
	return true;
}
void cameraInput::calibrateByFiles(string fileList[], int fileNum)
{
	cv::Size chessboardSize(9, 6);
	double length = 27.0;
	vector<vector<cv::Point2f>> img_points;
	vector<vector<cv::Point3f>> obj_points;

	//cv::Mat corners[10];
	vector<cv::Point3f> objPoints;
	string oimg = "Original Image", pimg = "Processed Image";
	cv::namedWindow(oimg, cv::WINDOW_AUTOSIZE);
	cv::namedWindow(pimg, cv::WINDOW_AUTOSIZE);
	cv::moveWindow(oimg, 10, 10);
	cv::moveWindow(pimg, 660, 10);
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			cv::Point3f cur(j*length, i*length, 0);
			objPoints.push_back(cur);
		}
	}
	for (int i = 0; i < fileNum; i++)
	{
		
		cv::Mat img = cv::imread(fileList[i], 0);
		//cv::cvtColor(img, img, cv::COLOR_BGR2GRAY);
		vector<cv::Point2f> corners;
		corners.resize(54);
		bool patternFound = cv::findChessboardCorners(img, chessboardSize, cv::Mat(corners));
		if (patternFound)
		{
			cv::Size winSize(5, 5);
			cv::Size zeroZone(-1, -1);
			cv::TermCriteria criteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.001);

			/// Calculate the refined corner locations
			cornerSubPix(img, corners, winSize, zeroZone, criteria);
			cv::imshow(oimg, img);
			cv::cvtColor(img, img, cv::COLOR_GRAY2BGR);
			cv::drawChessboardCorners(img, chessboardSize, cv::Mat(corners), true);
			cv::imshow(pimg, img);
			img_points.push_back(corners);
			obj_points.push_back(objPoints);
		}
		cv::waitKey(300);
	}
	cv::calibrateCamera(obj_points, img_points, cv::Size(640, 480), cameraMatrix, distCoeffs, R, T);
	cout << "Camera Matrix:" << endl;
	cout << cameraMatrix << endl;
	cout << "Distortion:" << endl;
	cout << distCoeffs << endl;
	cout << "Rotation:" << endl;
	cout << R[0] << endl;
	cout << "Translation:" << endl;
	cout << T[0] << endl;
}

void cameraInput::calibrateByCamera()
{
	cv::Size chessboardSize(9, 6);
	double length = 1;
	vector<vector<cv::Point2f>> img_points;
	vector<vector<cv::Point3f>> obj_points;

	//cv::Mat corners[10];
	vector<cv::Point3f> objPoints;
	string oimg = "Original Image", pimg = "Processed Image";
	cv::namedWindow(oimg, cv::WINDOW_AUTOSIZE);
	cv::namedWindow(pimg, cv::WINDOW_AUTOSIZE);
	cv::moveWindow(oimg, 10, 10);
	cv::moveWindow(pimg, 660, 10);
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			cv::Point3f cur(j*length, i*length, 0);
			objPoints.push_back(cur);
		}
	}
	while (true)
	{
		cv::Mat img;
		read(img);
		cv::imshow("Input", img);
		char ch = cv::waitKey(30);
		if (ch == ' ')
		{
			cv::cvtColor(img, img, cv::COLOR_BGR2GRAY);
			vector<cv::Point2f> corners;
			corners.resize(54);
			bool patternFound = cv::findChessboardCorners(img, chessboardSize, cv::Mat(corners));
			if (patternFound)
			{
				cv::Size winSize(5, 5);
				cv::Size zeroZone(-1, -1);
				cv::TermCriteria criteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.001);

				/// Calculate the refined corner locations
				cornerSubPix(img, corners, winSize, zeroZone, criteria);
				cv::imshow(oimg, img);
				cv::cvtColor(img, img, cv::COLOR_GRAY2BGR);
				cv::drawChessboardCorners(img, chessboardSize, cv::Mat(corners), true);
				cv::imshow(pimg, img);
				img_points.push_back(corners);
				obj_points.push_back(objPoints);
			}
		}
		if (ch == 27)
		{
			break;
		}
	}
	cv::calibrateCamera(obj_points, img_points, cv::Size(640, 480), cameraMatrix, distCoeffs, R, T);
	cout << "Camera Matrix:" << endl;
	cout << cameraMatrix << endl;
	cout << "Distortion:" << endl;
	cout << distCoeffs << endl;
	cout << "Rotation:" << endl;
	cout << R[0] << endl;
	cout << "Translation:" << endl;
	cout << T[0] << endl;
}
