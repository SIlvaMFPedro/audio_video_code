#include "progress.hh"

void printProgress(double percentage, string message) {
    int val = (int) (percentage * 100);
    int lpad = (int) (percentage * PBWIDTH);
    int rpad = PBWIDTH - lpad;
    printf ("\r%10s: %3d%% [\033[32;1m%.*s%*s\033[0m]", message.data(), val, lpad, PBSTR, rpad, "");
    fflush (stdout);
    if (percentage == 1) {
        printf("\n");
    }
}