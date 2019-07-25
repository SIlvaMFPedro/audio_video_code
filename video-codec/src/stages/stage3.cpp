#include "stages/stage3.hh"

int Stage3::blockSize = 16;
int Stage3::searchAreaSize = 64;
int Stage3::YUVType = -1;
bool Stage3::thisIsFirstUpdate = true;

void Stage3::updateFrame(uchar* givenFrame, int rows, int columns, int type) {
    uchar* buffer;
    height = rows;
    width = columns;
    YUVType = type;

    if (thisIsFirstUpdate) {
        switch (YUVType) {
            case 1:
                referenceFrameY = Mat(Size(width, height), CV_8UC3, 0);
                referenceFrameU = Mat(Size(width, height), CV_8UC3, 0);
                referenceFrameV = Mat(Size(width, height), CV_8UC3, 0);
                break;
            case 2:
                referenceFrameY = Mat(Size(width, height), CV_8UC3, 0);
                referenceFrameU = Mat(Size(width/2, height), CV_8UC3, 0);
                referenceFrameV = Mat(Size(width/2, height), CV_8UC3, 0);
                break;
            case 3:
                referenceFrameY = Mat(Size(width, height), CV_8UC3, 0);
                referenceFrameU = Mat(Size(width/2, height/2), CV_8UC3, 0);
                referenceFrameV = Mat(Size(width/2, height/2), CV_8UC3, 0);
                break;
            default:
                // TODO : handle this case
                break;
        }
        thisIsFirstUpdate = false;
    } else {
        referenceFrameY = frameY;
        referenceFrameU = frameU;
        referenceFrameV = frameV;
    }

    // make Mat from Y plane
    frameY = Mat(Size(width, height), CV_8UC3);
    buffer = (uchar*)frameY.ptr();
    for (int row = 0; row != height; row++) {
        for (int column = 0; column != width; column++) {
            buffer[row*width + column] = givenFrame[row*width + column];
        }
    }

    // make Mat from U plane
    if (type == 1) { /* if YUV 4:4:4 */
        frameU = Mat(Size(width, height), CV_8UC3);
        buffer = (uchar*)frameU.ptr();
        for (int row = 0; row != height; row++) {
            for (int column = 0; column != width; column++) {
                buffer[row*width + column] = givenFrame[height*width + row*width + column];
            }
        }
    } else if (type == 2) { /* if YUV 4:2:2 or YUV 4:2:0 */
        int boundWidth = (width % 2 == 0) ? width/2 : width/2 + 1;
        frameU = Mat(Size(boundWidth, height), CV_8UC3);
        buffer = (uchar*)frameU.ptr();
        for (int row = 0; row != height; row++) {
            for (int column = 0; column != boundWidth; column++) {
                buffer[row*boundWidth + column] = givenFrame[height*width + row*boundWidth + column];
            }
        }
    } else {
        int boundWidth = (width % 2 == 0) ? width/2 : width/2 + 1;
        int boundHeight = (height % 2 == 0) ? height/2 : height/2 + 1;
        frameU = Mat(Size(boundWidth, boundHeight), CV_8UC3);
        buffer = (uchar*)frameU.ptr();
        for (int row = 0; row != boundHeight; row++) {
            for (int column = 0; column != boundWidth; column++) {
                buffer[row*boundWidth + column] = givenFrame[height*width + row*boundWidth + column];
            }
        }
    }

    // make Mat from V plane
    if (type == 1) {
        frameV = Mat(Size(width, height), CV_8UC3);
        buffer = (uchar*)frameV.ptr();
        for (int row = 0; row != height; row++) {
            for (int column = 0; column != width; column++) {
                buffer[row*width + column] = givenFrame[2*height*width + row*width + column];
            }
        }
    } else if (type == 2) {
        int boundWidth = (width % 2 == 0) ? width/2 : width/2 + 1;
        frameV = Mat(Size(boundWidth, height), CV_8UC3);
        buffer = (uchar*)frameV.ptr();
        for (int row = 0; row != height; row++) {
            for (int column = 0; column != boundWidth; column++) {
                buffer[row*boundWidth + column] = givenFrame[height*width + height*boundWidth + row*boundWidth + column];
            }
        }
    } else {
        int boundWidth = (width % 2 == 0) ? width/2 : width/2 + 1;
        int boundHeight = (height % 2 == 0) ? height/2 : height/2 + 1;
        frameV = Mat(Size(boundWidth, boundHeight), CV_8UC3);
        buffer = (uchar*)frameV.ptr();
        for (int row = 0; row != boundHeight; row++) {
            for (int column = 0; column != boundWidth; column++) {
                buffer[row*boundWidth + column] = givenFrame[height*width + boundHeight*boundWidth + row*boundWidth + column];
            }
        }
    }
}

int Stage3::encode(bstream& file) {
    double trustiness;
    Point motionVector;
    int m = 4;
    Golomb golomb = Golomb(m);

    for (int row = 0; row != frameY.rows; row += blockSize) {
        for (int column = 0; column != frameY.cols; column += blockSize) {
            Mat searchAreaMat = getSearchAreaMat(frameY, Point(row, column), searchAreaSize);
            Mat block = getBlock(frameY, row, column, Size(blockSize, blockSize));
            Mat result;
            matchTemplate(searchAreaMat, block, result, TM_SQDIFF);
            minMaxLoc(result, NULL, &trustiness, &motionVector, NULL);
            // TODO : do something when the trustiness level is below MATCHING_THRESHOLD
            motionVector.x = motionVector.x - searchAreaSize;
            motionVector.y = motionVector.y - searchAreaSize;
        }
    }

    return 0;
}

int Stage3::decode(bstream& file) {

}