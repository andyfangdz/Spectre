#pragma once
#include <opencv2/core.hpp>
#include <vector>
#include <iostream>
using namespace cv;
using namespace std;

class Door
{
public:
	Door(Point3f points[]);
	~Door();
	Point3f pts[8];
	double detWithPlane(int a, int b, int c, Point3f pt);
	bool isInsideBox(Point3f pt);
	bool isBehindPlane(Point3f pt);
};

