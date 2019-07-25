#include <ctime>
#include <stdio.h>
#include <opencv2/opencv.hpp>

#include "bstream.hh"
#include "progress.hh"

#include "coding/golomb.hh"

#include "jpeg-ls/gradient.hh"
#include "jpeg-ls/mode.hh"
#include "jpeg-ls/matrix.hh"
#include "jpeg-ls/predictor.hh"
#include "jpeg-ls/rle.hh"

#include "yuv/info.hh"

using namespace cv;
using namespace std;

class Stage2 {
    public: 
        /**
         *
         * @param file
         * @return
         */
        static int encode(bstream& file);

        /**
         *
         * @return
         */
        static vector<vector<uchar>> decode();

        /**
         *
         * @param givenFrame
         * @param rows
         * @param columns
         * @param type
         */
        static void updateFrame(uchar* givenFrame, int rows, int columns, int type, int level);

        /**
         *
         */
        static void endEncode();

        /**
         *
         * @param name
         */
        static void setFilename(string name);

        /**
         *
         * @param file
         * @param originalHeader
         * @param numberOfFrames
         * @return
         */
        static int writeHeader(bstream& file, string originalHeader, int numberOfFrames, int quantizationLevel);
private:
        /**
         *
         * @return
         */
        static bool noFrameHasBeenGiven();

        /**
         *
         */
        static uchar* frame;

        /**
         *
         */
        static int height;

        /**
         *
         */
        static int width;

        /**
         *
         */
        static string filename;

        /**
         *
         */
        static int frames;

        /**
         *
         */
        static int YUVType;

        /**
         *
         */
        static int quantization;
};