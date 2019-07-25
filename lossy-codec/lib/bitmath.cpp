#include "bitmath.h"

int euclidean_distance16(bitset<16> p, bitset<16> q) {
    int squaredDistance = 0;
    for (int i = 0; i != 16; i++) {
        squaredDistance += pow((q[i] ? 1 : 0) - (p[i] ? 1 : 0), 2);
    }
    return sqrt(squaredDistance);
}