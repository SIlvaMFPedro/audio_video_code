#include "jpeg-ls/gradient.hh"

uchar getDelta1(uchar sampleD, uchar sampleB) {
    return sampleD - sampleB;
}

uchar getDelta2(uchar sampleB, uchar sampleC) {
    return sampleB - sampleC;
}

uchar getDelta3(uchar sampleC, uchar sampleA) {
    return sampleC - sampleA;
}