#include <opencv2/core/utility.hpp>
#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include "Control.h"

#include <iostream>
#include <ctype.h>

using namespace cv;
using namespace std;

int main()
{
	Control mainControl(0);
	while (true)
	{
		mainControl.iterate();
		char ch = waitKey(30);
		if (ch == 27)
		{
			break;
		}
	}
	return 0;
}