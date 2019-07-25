#include "jpeg-ls/predictor.hh"

uchar predict(uchar a, uchar b, uchar c) {
    if (c >= std::max(a, b)) {
        return std::min(a, b);
    } else if (c <= std::min(a, b)) {
        return std::max(a, b);
    } else {
        return a + b - c;
    }
}

int computeResidual(int x, int mu_x) {
    return x - mu_x;
}