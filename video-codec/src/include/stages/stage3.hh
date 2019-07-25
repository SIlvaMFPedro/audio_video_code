#include <stdio.h>
#include <opencv2/opencv.hpp>

#include "bstream.hh"

#include "coding/golomb.hh"

#include "motion/blocks.hh"
#include "motion/matrix.hh"

using namespace cv;
using namespace std;

constexpr int MATCHING_THRESHOLD = 0.5;

class Stage3 {
    public:
        static void updateFrame(uchar* givenFrame, int rows, int columns, int type);
        static int encode(bstream& file);
        static int decode(bstream& file);
    private:
        static int blockSize;
        static int searchAreaSize;
        static Mat frameY;
        static Mat frameU;
        static Mat frameV;
        static Mat referenceFrameY;
        static Mat referenceFrameU;
        static Mat referenceFrameV;
        static int YUVType;
        static int height;
        static int width;
        static bool thisIsFirstUpdate;
};