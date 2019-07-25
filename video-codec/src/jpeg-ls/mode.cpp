#include "jpeg-ls/mode.hh"

bool enableRunLengthCoding(uchar delta1, uchar delta2, uchar delta3) {
    return ((delta1 | delta2 | delta3) == 0);
}