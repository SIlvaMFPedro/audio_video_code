#ifndef WAVVQ_H
#define WAVVQ_H

#include <boost/algorithm/string.hpp>

#include "boost/filesystem.hpp"
#include "boost/program_options.hpp"
#include "wavcb.h"
#include "wavenc.h"
#include "wavdec.h"


using namespace std;

std::string VERSION = "1.0.0"; // Using SemVer
//extern constexpr size_t FRAMES_BUFFER_SIZE = 65536;

int main(int argc, char* argv[]);

#endif