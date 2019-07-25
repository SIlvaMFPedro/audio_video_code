#ifndef WAVCB_H
#define WAVCB_H

#include <map>
#include <bitset>
#include <random>
#include <vector>
#include <climits>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <sndfile.hh>

#include "bitmath.h"

using namespace std;

class Codebook {
    public: 
        Codebook(size_t bookSize = 512, int overlap = 1);
        int create_codebook(std::string filename);
        int save(std::string filename);
        int load(std::string filename);
        short closest_vector(bitset<16> sample);
        int size();
        vector<bitset<16> > getCodebook();

    private:
        int generate_centroids(SndfileHandle file);
        int produce_codebook(SndfileHandle file);
        int iterate_centroids(SndfileHandle file);
        double evaluate_distortion(int numberOfSamples);
        int evaluate_centroids();
        bool hasCodebookConverged();
        vector<bitset<16> > samples;
        vector<bitset<16> > centroids;
        vector<bitset<16> > codebook;
        vector<vector<tuple<bitset<16>, int> > > iterationSamples;
        size_t codebookSize;
        int overlapFactor;
        double distortion;
        bool codebookHasConverged = false;
};

#endif