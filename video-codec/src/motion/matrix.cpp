#include "motion/matrix.hh"

Mat getSearchAreaMat(Mat& frame, Point point, int areaLength) {
    Mat searchAreaMat;
    Rect searchAreaRect;
    Point topLeftPoint;
    int w_s = areaLength*2 + 1;
    int h_s = areaLength*2 + 1;

    if (point.x < areaLength) {
        topLeftPoint.x = 0;
        w_s = w_s - (areaLength - point.x);
    } else {
        topLeftPoint.x = point.x - areaLength;
    }

    if (point.x + areaLength > frame.cols) {
        w_s = w_s - (point.x + areaLength - frame.cols);
    }

    if (point.y < areaLength) {
        topLeftPoint.y = 0;
        h_s = h_s - (areaLength - point.y);
    } else {
        topLeftPoint.y = point.y - areaLength;
    }

    if (point.y + areaLength > frame.rows) {
        h_s = h_s - (point.y + areaLength - frame.rows);
    }

    searchAreaRect = Rect(topLeftPoint.x, topLeftPoint.y, w_s, h_s);
    searchAreaMat = frame(searchAreaRect);

    return searchAreaMat;
}