#ifndef WAVDEC_H
#define WAVDEC_H

#include "wavcb.h"
#include "bstream.h"
#include "utils.h"

using namespace std;

class Decoder {
    public:
        Decoder(string filename, string codebookName);
        int read_codebook();
        int read_compressed_audio();
        int decode();
        int saveWAVFile(std::string filename);
    private:
        int format;
        int channels;
        int samplerate;
        string filename;
        string codebookName;
        Codebook codebook;
        vector<short> decodedAudio;
        vector<short> audioSamples;
};

#endif
