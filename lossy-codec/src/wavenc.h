#ifndef WAVENC_H
#define WAVENC_H

#include "wavcb.h"
#include "bstream.h"

using namespace std;

class Encoder {
    public:
        Encoder(Codebook codebook, SndfileHandle audio, string filename);
        int encode();
    private:
        int apply_codebook();
        string write_header();
        int write_file();
        Codebook codebook;
        SndfileHandle audio;
        string filename;
        vector<short> encodedAudio;
};

#endif