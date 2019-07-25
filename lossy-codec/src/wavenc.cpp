#include "wavenc.h"

using namespace std;

constexpr size_t FRAMES_BUFFER_SIZE = 65536; 							// Buffer for reading frames

Encoder::Encoder(Codebook codebook, SndfileHandle audio, string filename) : codebook(codebook) {
    Encoder::audio = audio;
    Encoder::filename = filename;
}

int Encoder::encode() {
    Encoder::apply_codebook();
    Encoder::write_file();
    codebook.save(Encoder::filename);
}

int Encoder::apply_codebook() {
    size_t numberOfFrames;
    vector<short> chunk(FRAMES_BUFFER_SIZE * audio.channels());
    while ((numberOfFrames = audio.readf(chunk.data(), FRAMES_BUFFER_SIZE))) {
        for (auto sample : chunk) {
            Encoder::encodedAudio.push_back(codebook.closest_vector(bitset<16>(sample)));
        }
        chunk.resize(numberOfFrames * audio.channels());
    }
    return 0;
} 

int Encoder::write_file() {
    bstream file { (Encoder::filename + ".wavz").data(), ios::out|ios::binary };
    file.write(Encoder::write_header().data(), 12);
    for (auto index : Encoder::encodedAudio) {
        for (int i = 8; i >= 0; i--) {
            file.writeBit(index >> i);
        }
    }
    file.grantWrite();
    return 0;
}

string Encoder::write_header() {
    return bitset<32>(Encoder::audio.format()).to_string() + bitset<32>(Encoder::audio.channels()).to_string() + bitset<32>(Encoder::audio.samplerate()).to_string();
}