#include <opencv2/opencv.hpp>
#include <stdio.h>

using namespace cv;
using namespace std;

Mat getSearchAreaMat(Mat& frame, Point point, int areaLength);