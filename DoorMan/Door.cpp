#include "Door.h"


Door::Door(Point3f points[])
{
	for (int i = 0; i < 8; i++)
	{
		pts[i] = points[i];
	}
}

double Door::detWithPlane(int a, int b, int c, Point3f pt)
{
	vector<Point3f> tmp;
	tmp.push_back(pt - pts[a]);
	tmp.push_back(pts[b] - pts[a]);
	tmp.push_back(pts[c] - pts[a]);
	Mat mart(tmp);
	mart = mart.reshape(1, 3);
	return determinant(mart);
}

Door::~Door()
{
}


bool Door::isInsideBox(Point3f pt)
{
	if (detWithPlane(0, 1, 2, pt) * detWithPlane(4, 5, 6, pt) <= 0)
	{
		if (detWithPlane(0, 1, 5, pt) * detWithPlane(3, 2, 6, pt) <= 0)
		{
			if (detWithPlane(0, 3, 7, pt) * detWithPlane(1, 2, 6, pt) <= 0)
			{
				return true;
			}
		}
	}
	return false;
}


bool Door::isBehindPlane(Point3f pt)
{
	return detWithPlane(5, 1, 4, pt) > 0;
}
