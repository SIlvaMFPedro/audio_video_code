#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

Mat getBlock(Mat& from, int row, int column, Size size);

int getResidual(Mat& reference, Mat& current);

int putBlock(Mat& block, int row, int column, Mat& image);
