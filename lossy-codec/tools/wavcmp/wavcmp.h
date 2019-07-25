#ifndef WAVCMP_H
#define WAVCMP_H

#include <cerrno>
#include <vector>
#include <fstream>
#include <iostream>
#include <cmath>
#include <cstring>
#include <vector>
#include <math.h>
#include <sndfile.hh>

#include "boost/program_options.hpp"
#include "boost/filesystem.hpp"

using namespace std;

constexpr size_t FRAMES_BUFFER_SIZE = 65536; 	
std::string VERSION = "1.0.0"; // Using SemVer

vector<short> open_audio(SndfileHandle audio);
double mean(vector<short> samples);
double var(vector<short> samples, double meanValue);
double get_snr(vector<short> original, vector<short> modified);
short get_max_abs_error_per_sample(vector<short> original, vector<short> modified);
int main(int argc, char* argv[]);

#endif