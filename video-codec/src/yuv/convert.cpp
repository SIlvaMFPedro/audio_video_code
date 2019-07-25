#include "yuv/convert.hh"

void fromYUV422ToYUV444(uchar input[], const int image_width, const int image_height, uchar output[]) {
    int sizeOfY = image_width * image_height;
    int sizeOfU = (image_width * image_height) / 2;

    // convert Y values
    for (int i = 0; i < sizeOfY; i++) {
        output[i] = input[i];
    }

    // convert U values
    for (int i = 0; i < sizeOfY-1; i++) {
        output[sizeOfY+i] = input[sizeOfY+i/2];
        output[sizeOfY+i+1] = input[sizeOfY+i/2];
    }

    // convert V values
    for (int i = 0; i < sizeOfY-1; i++) {
        output[(2*sizeOfY)+i] = input[sizeOfY+sizeOfU+i/2];
        output[(2*sizeOfY)+i+1] = input[sizeOfY+sizeOfU+i/2];
    }
}

void fromYUV420ToYUV444(uchar input[], const int image_width, const int image_height, uchar output[]) {
    int sizeOfY = image_width * image_height;
    int sizeOfU = (image_width * image_height) / 4;

    // convert Y values
    for (int i = 0; i < sizeOfY; i++) {
        output[i] = input[i];
    }

    // convert U values
    int i = 0;
    int index = 0;
    for (int row = 0; row < image_height; row+=2) {
        for (int column = 0; column < image_width; column+=2) {
            output[sizeOfY + row*image_width/2 + index] = input[sizeOfY+i];
            if (column + 1 < image_width) {
                output[sizeOfY + row*image_width/2 + index + 1] = input[sizeOfY+i];
            }
            if (row + 1 < image_height) {
                output[sizeOfY + row*image_width/2 + index + image_width] = input[sizeOfY+i];
            }
            if (column + 1 < image_width && row + 1 < image_height) {
                output[sizeOfY + row*image_width/2 + index + image_width + 1] = input[sizeOfY+i];
            }
            index += 2;
            i++;
        }
    }

    // convert V values
    i = 0;
    index = 0;
    for (int row = 0; row < image_height; row+=2) {
        for (int column = 0; column < image_width; column+=2) {
            output[2*sizeOfY + row*image_width/2 + index] = input[sizeOfY+sizeOfU+i];
            if (column + 1 < image_width) {
                output[2*sizeOfY + row*image_width/2 + index + 1] = input[sizeOfY+sizeOfU+i];
            }
            if (row + 1 < image_height) {
                output[2*sizeOfY + row*image_width/2 + index + image_width] = input[sizeOfY+sizeOfU+i];
            }
            if (column + 1 < image_width && row + 1 < image_height) {
                output[2*sizeOfY + row*image_width/2 + index + image_width + 1] = input[sizeOfY+sizeOfU+i];
            }
            index += 2;
            i++;
        }
    }
}