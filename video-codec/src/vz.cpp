#include <ctime>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <fstream>

#include "progress.hh"

#include "stages/stage2.hh"

#include "yuv/convert.hh"
#include "yuv/info.hh"

using namespace cv;
using namespace std;

int getNumberOfFrames(const string filename) {
    ifstream file(filename);
    string line;
    int numberOfFrames = 0;

    while (!file.eof()) {
        getline(file, line);
        if (line.find("FRAME") != string::npos) {
            numberOfFrames++;
        }
    }
    cout << numberOfFrames << endl;

    return numberOfFrames;
}

int main(int argc, char** argv) {
    string line; 								/* store the header */
    int yCols, yRows; 							/* frame dimension */
    unsigned char *imgData; 					/* file data buffer*/
    int end = 0; 		                        /* control variables */
    int YUVType = 0;							/* code for YUV type: 1 for 4:4:4, 2 for 4:2:2 and 3 for 4:2:0 */
    int quantizationLevel = 0;

    /* check for the mandatory arguments */
    if (argc < 2) {
        cerr << "Usage: vz <filename> [<quantization_level=0>]" << endl;
        return 1;
    }

    /* Opening video file */
    ifstream myfile (argv[1]);
    if (argc == 3) {
        quantizationLevel = atoi(argv[2]);
    }

    /* Processing header */
    getline (myfile,line);
    cout << line << endl;
    cout << line.substr(line.find(" W") + 2, line.find(" H") - line.find(" W") - 2) << endl;
    yCols = getWidth(line);
    yRows = getHeight(line);
    YUVType = getType(line);
    cout << yCols << ", " << yRows << endl;

    /* header writing */
    bstream encoded { (string(argv[1]) + "z").data(), ios::binary|ios::out };
    Stage2::setFilename(string(argv[1]) + "z");
    int numberOfFrames = getNumberOfFrames(argv[1]);
    Stage2::writeHeader(encoded, line, numberOfFrames, quantizationLevel);

    int count = 0;

    clock_t begin = clock();
    while (!end) {
        /* load a new frame, if possible */
        getline (myfile,line); // Skipping word FRAME
        switch (YUVType) {
            case 1:
                imgData = new unsigned char[yCols * yRows * 3];
                myfile.read((char *)imgData, yCols * yRows * 3);
                break;
            case 2:
                imgData = new unsigned char[yCols * yRows + (yCols * yRows / 2) + (yCols * yRows / 2)];
                myfile.read((char *)imgData, yCols * yRows + (yCols * yRows / 2) + (yCols * yRows / 2));
                break;
            case 3:
                imgData = new unsigned char[yCols * yRows + (yCols * yRows / 4) + (yCols * yRows / 4)];
                myfile.read((char *)imgData, yCols * yRows + (yCols * yRows / 4) + (yCols * yRows / 4));
                break;
            default:
                // TODO : handle this case
                break;

        }
        if (myfile.gcount() == 0) {
            end = 1;
            break;
        }

        Stage2::updateFrame(imgData, yRows, yCols, YUVType, quantizationLevel);
        if (int output = Stage2::encode(encoded) != 0) {
        	exit(output);
        }

        printProgress(++count/(double)numberOfFrames, "Encoding");

    }
    clock_t endTime = clock();
    double elapsedSeconds = double(endTime - begin) / CLOCKS_PER_SEC;
    cout << "Encoding done in " << elapsedSeconds << " seconds." << endl;
    encoded.close();
    Stage2::decode();

    return 0;
}