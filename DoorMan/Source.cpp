#include <iostream>
#include <cstdlib>
#include "Door.h"
using namespace std;

int main()
{
	Point3f tmp[8];
	tmp[0] = Point3f(0, 0, 0);
	tmp[1] = Point3f(1, 0, 0);
	tmp[2] = Point3f(1, 1, 0);
	tmp[3] = Point3f(0, 1, 0);
	tmp[4] = Point3f(0, 0, 1);
	tmp[5] = Point3f(1, 0, 1);
	tmp[6] = Point3f(1, 1, 1);
	tmp[7] = Point3f(0, 1, 1);
	Door door(tmp);
	int n;
	cin >> n;
	for (int i = 0; i < n; i++)
	{
		Point3f tmp;
		cin >> tmp.x >> tmp.y >> tmp.z;
		cout << door.isInsideBox(tmp) << endl;
	}
	system("pause");
	return 0;

}