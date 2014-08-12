#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/nonfree.hpp>
#include <opencv2/calib3d.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

int main()
{
	int cam1 = 1, cam2 = 0;
	string mainWindow1 = "Camera" + (char)(cam1 + '0');
	string mainWindow2 = "Camera" + (char)(cam2 + '0');
	VideoCapture cap1, cap2;
	cap1.open(cam1); cap2.open(cam2);
	if (!(cap2.isOpened() && cap1.isOpened()))
	{
		return 1;
	}
	namedWindow(mainWindow1, WINDOW_AUTOSIZE);
	namedWindow(mainWindow2, WINDOW_AUTOSIZE);
	Mat img1, img2;
	while (true)
	{
		cap1 >> img1;
		cap2 >> img2;
		imshow(mainWindow1, img1);
		imshow(mainWindow2, img2);
		char ch = waitKey(30);
		if (ch == 'c')
		{
			break;
		}
	}
	Mat orig1, orig2;
	orig1 = img1.clone();
	orig2 = img2.clone();
	cvtColor(img1, img1, cv::COLOR_BGR2GRAY);
	cvtColor(img2, img2, cv::COLOR_BGR2GRAY);
	int minHessian = 400;
	SurfFeatureDetector detector(minHessian);
	vector<KeyPoint> kps1, kps2;
	detector.detect(img1, kps1);
	detector.detect(img2, kps2);
	
	SurfDescriptorExtractor extractor;
	Mat desc1, desc2;
	extractor.compute(img1, kps1, desc1);
	extractor.compute(img2, kps2, desc2);

	FlannBasedMatcher matcher;
	vector<DMatch> matches, goodMatches, inMatches;
	matcher.match(desc1, desc2, matches);

	double minDist = 100, maxDist = 0;
	for (int i = 0; i < desc1.rows; i++)
	{
		double dist = matches[i].distance;
		if (dist > maxDist)
		{
			maxDist = dist;
		}
		if (dist < minDist)
		{
			minDist = dist;
		}
	}
	cout << "Statistical Data for Matches: " << endl;
	cout << "-- Min Dist:" << minDist << endl;
	cout << "-- Max Dist:" << maxDist << endl;
	cout << "Will select good matches only, i.e. the ones fall into range(0, " << minDist * 2 << ")" << endl;
	vector<Point2f> gps1, gps2, ips1, ips2;
	for (int i = 0; i < desc1.rows; i++)
	{
		if (matches[i].distance < 2 * max(minDist, 0.01))
		{
			goodMatches.push_back(matches[i]);
			gps1.push_back(kps1[matches[i].queryIdx].pt);
			gps2.push_back(kps2[matches[i].trainIdx].pt);
		}
	}
	Mat matchMap, inMatchMap;
	drawMatches(img1, kps1, img2, kps2, goodMatches, matchMap, Scalar::all(-1), Scalar::all(-1),
		vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
	Mat inMask;
	vector<int> f_mask;
	Mat F = findFundamentalMat(gps1, gps2, inMask, FM_LMEDS );
	for (int i = 0; i < inMask.rows; i++)
	{
		f_mask.push_back(inMask.at<uchar>(i, 0));
	}
	for (int i = 0; i < goodMatches.size(); i++)
	{
		if (f_mask[i] != 0)
		{
			DMatch temp;
			ips1.push_back(gps1[i]);
			ips2.push_back(gps2[i]);
			temp.queryIdx = temp.trainIdx = ips1.size() - 1;
			inMatches.push_back(temp);
		}
	}
	vector<KeyPoint> ikps1, ikps2;
	KeyPoint::convert(ips1, ikps1);
	KeyPoint::convert(ips2, ikps2);
	drawMatches(img1, ikps1, img2, ikps2, inMatches, inMatchMap, Scalar::all(-1), Scalar::all(-1),
		vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
	namedWindow("Matches", WINDOW_AUTOSIZE);
	imshow("Matches", matchMap);
	namedWindow("Inner Matches", WINDOW_AUTOSIZE);
	imshow("Inner Matches", inMatchMap);
	char ch = waitKey(0);
	if (ch == 's')
	{
		FileStorage fs("fundamentalMatrix.yml", FileStorage::WRITE);
		fs << "Fundamental Matrix" << F;
		fs << "Inner Matches" << inMatchMap;
		fs << "Inner Keypoints 1" << ikps1;
		fs << "Inner Keypoints 2" << ikps2;
		fs.release();
	}
	return 0;
}