#include "jpeg-ls/matrix.hh"

/* type is: 1 - YUV 4:4:4; 2 - YUV 4:2:2; 3 - YUV 4:2:0 */

/* ENCODE-TIME ACCESS METHODS */

void putYSample(uchar matrix[], Size size, int row, int column, int type, uchar sample) {
    matrix[row*size.width + column] = sample;
}

uchar getYSample(uchar matrix[], Size size, int row, int column, int type) {
    return matrix[row*size.width + column];
}

uchar getYSampleA(uchar matrix[], Size size, int row, int column, int type) {
    if (column != 0) {
        return matrix[row*size.width + column - 1];
    }
    return 0;
}

uchar getYSampleB(uchar matrix[], Size size, int row, int column, int type) {
    if (row != 0) {
        return matrix[(row-1)*size.width + column];
    }
    return 0;
}

uchar getYSampleC(uchar matrix[], Size size, int row, int column, int type) {
    if (row != 0 && column != 0) {
        return matrix[(row-1)*size.width + column - 1];
    }
    return 0;
}

void putUSample(uchar matrix[], Size size, int row, int column, int type, uchar sample) {
    int boundWidth = (size.width % 2 == 0) ? size.width/2 : size.width/2 + 1;
    switch (type) {
        case 1:
            matrix[(size.height * size.width) + row*size.width + column] = sample;
        case 2:
            matrix[(size.height * size.width) + row*boundWidth + column] = sample;
        case 3:
            matrix[(size.height * size.width) + row*boundWidth + column] = sample;
        default:
            // TODO : handle this case
            return ;
    }
}

uchar getUSample(uchar matrix[], Size size, int row, int column, int type) {
    int boundWidth = (size.width % 2 == 0) ? size.width/2 : size.width/2 + 1;
    switch (type) {
        case 1:
            return matrix[(size.height * size.width) + row*size.width + column];
        case 2:
            return matrix[(size.height * size.width) + row*boundWidth + column];
        case 3:
            return matrix[(size.height * size.width) + row*boundWidth + column];
        default:
            // TODO : handle this case
            return 0;
    }
}

uchar getUSampleA(uchar matrix[], Size size, int row, int column, int type) {
    if (column != 0) {
        int boundWidth = (size.width % 2 == 0) ? size.width/2 : size.width/2 + 1;
        switch (type) {
            case 1:
                return matrix[(size.height * size.width) + row*size.width + column - 1];
            case 2:
                return matrix[(size.height * size.width) + row*boundWidth + column - 1];
            case 3:
                return matrix[(size.height * size.width) + row*boundWidth + column - 1];
            default:
                // TODO : handle this case
                return 0;
        }
    }
    return 0;
}

uchar getUSampleB(uchar matrix[], Size size, int row, int column, int type) {
    if (row != 0) {
        int boundWidth = (size.width % 2 == 0) ? size.width/2 : size.width/2 + 1;
        switch (type) {
            case 1:
                return matrix[(size.height * size.width) + (row-1)*size.width + column];
            case 2:
                return matrix[(size.height * size.width) + (row-1)*boundWidth + column];
            case 3:
                return matrix[(size.height * size.width) + (row-1)*boundWidth + column];
            default:
                // TODO : handle this case
                return 0;
        }
    }
    return 0;
}

uchar getUSampleC(uchar matrix[], Size size, int row, int column, int type) {
    if (row != 0 && column != 0) {
        int boundWidth = (size.width % 2 == 0) ? size.width/2 : size.width/2 + 1;
        switch (type) {
            case 1:
                return matrix[(size.height * size.width) + (row-1)*size.width + column - 1];
            case 2:
                return matrix[(size.height * size.width) + (row-1)*boundWidth + column - 1];
            case 3:
                return matrix[(size.height * size.width) + (row-1)*boundWidth + column - 1];
            default:
                // TODO : handle this case
                return 0;
        }
    }
    return 0;
}

void putVSample(uchar matrix[], Size size, int row, int column, int type, uchar sample) {
    int boundWidth = (size.width % 2 == 0) ? size.width/2 : size.width/2 + 1;
    int boundHeight = (size.height % 2 == 0) ? size.height/2 : size.height/2 + 1;
    switch (type) {
        case 1:
            matrix[(2 * size.height * size.width) + row*size.width + column] = sample;
        case 2:
            matrix[(size.height * size.width) + (size.height * boundWidth) + row*boundWidth + column] = sample;
        case 3:
            matrix[(size.height * size.width) + (boundHeight * boundWidth) + row*boundWidth + column] = sample;
        default:
            // TODO : handle this case
            return ;
    }
}

uchar getVSample(uchar matrix[], Size size, int row, int column, int type) {
    int boundWidth = (size.width % 2 == 0) ? size.width/2 : size.width/2 + 1;
    int boundHeight = (size.height % 2 == 0) ? size.height/2 : size.height/2 + 1;
    switch (type) {
        case 1:
            return matrix[(2 * size.height * size.width) + row*size.width + column];
        case 2:
            return matrix[(size.height * size.width) + (size.height * boundWidth) + row*boundWidth + column];
        case 3:
            return matrix[(size.height * size.width) + (boundHeight * boundWidth) + row*boundWidth + column];
        default:
            // TODO : handle this case
            return 0;
    }
}

uchar getVSampleA(uchar matrix[], Size size, int row, int column, int type) {
    if (column != 0) {
        int boundWidth = (size.width % 2 == 0) ? size.width/2 : size.width/2 + 1;
        int boundHeight = (size.height % 2 == 0) ? size.height/2 : size.height/2 + 1;
        switch (type) {
            case 1:
                return matrix[(2 * size.height * size.width) + row*size.width + column - 1];
            case 2:
                return matrix[(size.height * size.width) + (size.height * boundWidth) + row*boundWidth + column - 1];
            case 3:
                return matrix[(size.height * size.width) + (boundHeight * boundWidth) + row*boundWidth + column - 1];
            default:
                // TODO : handle this case
                return 0;
        }
    }
    return 0;
}

uchar getVSampleB(uchar matrix[], Size size, int row, int column, int type) {
    if (row != 0) {
        int boundWidth = (size.width % 2 == 0) ? size.width/2 : size.width/2 + 1;
        int boundHeight = (size.height % 2 == 0) ? size.height/2 : size.height/2 + 1;
        switch (type) {
            case 1:
                return matrix[(2 * size.height * size.width) + (row-1)*size.width + column];
            case 2:
                return matrix[(size.height * size.width) + (size.height * boundWidth) + (row-1)*boundWidth + column];
            case 3:
                return matrix[(size.height * size.width) + (boundHeight * boundWidth) + (row-1)*boundWidth + column];
            default:
                // TODO : handle this case
                return 0;
        }
    }
    return 0;
}

uchar getVSampleC(uchar matrix[], Size size, int row, int column, int type) {
    if (row != 0 && column != 0) {
        int boundWidth = (size.width % 2 == 0) ? size.width/2 : size.width/2 + 1;
        int boundHeight = (size.height % 2 == 0) ? size.height/2 : size.height/2 + 1;
        switch (type) {
            case 1:
                return matrix[(2 * size.height * size.width) + (row-1)*size.width + column - 1];
            case 2:
                return matrix[(size.height * size.width) + (size.height * boundWidth) + (row-1)*boundWidth + column - 1];
            case 3:
                return matrix[(size.height * size.width) + (boundHeight * boundWidth) + (row-1)*boundWidth + column - 1];
            default:
                // TODO : handle this case
                return 0;
        }
    }
    return 0;
}

/* DECODE-TIME ACCESS METHODS */

uchar getDecodedYSampleA(std::vector<uchar>* matrix, Size size, int row, int column, int type) {
    if (column != 0) {
        return matrix->at(row*size.width + column - 1);
    }
    return 0;
}

uchar getDecodedYSampleB(std::vector<uchar>* matrix, Size size, int row, int column, int type) {
    if (row != 0) {
        return matrix->at((row-1)*size.width + column);
    }
    return 0;
}

uchar getDecodedYSampleC(std::vector<uchar>* matrix, Size size, int row, int column, int type) {
    if (row != 0 && column != 0) {
        return matrix->at((row-1)*size.width + column - 1);
    }
    return 0;
}

uchar getDecodedUSampleA(std::vector<uchar>* matrix, Size size, int row, int column, int type) {
    if (column != 0) {
        int boundWidth = (size.width % 2 == 0) ? size.width/2 : size.width/2 + 1;
        switch (type) {
            case 1:
                return matrix->at((size.height * size.width) + row*size.width + column - 1);
            case 2:
                return matrix->at((size.height * size.width) + row*boundWidth + column - 1);
            case 3:
                return matrix->at((size.height * size.width) + row*boundWidth + column - 1);
            default:
                // TODO : handle this case
                return 0;
        }
    }
    return 0;
}

uchar getDecodedUSampleB(std::vector<uchar>* matrix, Size size, int row, int column, int type) {
    if (row != 0) {
        int boundWidth = (size.width % 2 == 0) ? size.width/2 : size.width/2 + 1;
        switch (type) {
            case 1:
                return matrix->at((size.height * size.width) + (row-1)*size.width + column);
            case 2:
                return matrix->at((size.height * size.width) + (row-1)*boundWidth + column);
            case 3:
                return matrix->at((size.height * size.width) + (row-1)*boundWidth + column);
            default:
                // TODO : handle this case
                return 0;
        }
    }
    return 0;
}

uchar getDecodedUSampleC(std::vector<uchar>* matrix, Size size, int row, int column, int type) {
    if (row != 0 && column != 0) {
        int boundWidth = (size.width % 2 == 0) ? size.width/2 : size.width/2 + 1;
        switch (type) {
            case 1:
                return matrix->at((size.height * size.width) + (row-1)*size.width + column - 1);
            case 2:
                return matrix->at((size.height * size.width) + (row-1)*boundWidth + column - 1);
            case 3:
                return matrix->at((size.height * size.width) + (row-1)*boundWidth + column - 1);
            default:
                // TODO : handle this case
                return 0;
        }
    }
    return 0;
}

uchar getDecodedVSampleA(std::vector<uchar>* matrix, Size size, int row, int column, int type) {
    if (column != 0) {
        int boundWidth = (size.width % 2 == 0) ? size.width/2 : size.width/2 + 1;
        int boundHeight = (size.height % 2 == 0) ? size.height/2 : size.height/2 + 1;
        switch (type) {
            case 1:
                return matrix->at((2 * size.height * size.width) + row*size.width + column - 1);
            case 2:
                return matrix->at((size.height * size.width) + (size.height * boundWidth) + row*boundWidth + column - 1);
            case 3:
                return matrix->at((size.height * size.width) + (boundHeight * boundWidth) + row*boundWidth + column - 1);
            default:
                // TODO : handle this case
                return 0;
        }
    }
    return 0;
}

uchar getDecodedVSampleB(std::vector<uchar>* matrix, Size size, int row, int column, int type) {
    if (row != 0) {
        int boundWidth = (size.width % 2 == 0) ? size.width/2 : size.width/2 + 1;
        int boundHeight = (size.height % 2 == 0) ? size.height/2 : size.height/2 + 1;
        switch (type) {
            case 1:
                return matrix->at((2 * size.height * size.width) + (row-1)*size.width + column);
            case 2:
                return matrix->at((size.height * size.width) + (size.height * boundWidth) + (row-1)*boundWidth + column);
            case 3:
                return matrix->at((size.height * size.width) + (boundHeight * boundWidth) + (row-1)*boundWidth + column);
            default:
                // TODO : handle this case
                return 0;
        }
    }
    return 0;
}

uchar getDecodedVSampleC(std::vector<uchar>* matrix, Size size, int row, int column, int type) {
    if (row != 0 && column != 0) {
        int boundWidth = (size.width % 2 == 0) ? size.width/2 : size.width/2 + 1;
        int boundHeight = (size.height % 2 == 0) ? size.height/2 : size.height/2 + 1;
        switch (type) {
            case 1:
                return matrix->at((2 * size.height * size.width) + (row-1)*size.width + column - 1);
            case 2:
                return matrix->at((size.height * size.width) + (size.height * boundWidth) + (row-1)*boundWidth + column - 1);
            case 3:
                return matrix->at((size.height * size.width) + (boundHeight * boundWidth) + (row-1)*boundWidth + column - 1);
            default:
                // TODO : handle this case
                return 0;
        }
    }
    return 0;
}
