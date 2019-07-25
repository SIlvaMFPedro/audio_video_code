#include "motion/blocks.hh"

Mat getBlock(Mat& from, int row, int column, Size size) {
    Rect blockRect = Rect(row, column, size.width, size.height);
    return from(blockRect);
}

int getResidual(Mat& reference, Mat& current) {
    Mat result;
    subtract(reference, current, result);
    return 0;
}

int putBlock(Mat& block, int row, int column, Mat& image) {

    return 0;
}