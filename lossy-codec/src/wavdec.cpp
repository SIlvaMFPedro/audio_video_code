#include "wavdec.h"

Decoder::Decoder(string filename, string codebookName) {
    Decoder::filename = filename;
    Decoder::codebookName = codebookName;
    codebook = Codebook();
    Decoder::read_codebook();
}

int Decoder::read_codebook() {
    codebook.load(Decoder::codebookName);
    return 0;
}

int Decoder::read_compressed_audio() {
    string format;
    string channels;
    string samplerate;
    bstream file((Decoder::filename + ".wavz").data(), ios::in|ios::binary);
    file.read((char*)format.data(), 4);
    file.read((char*)channels.data(), 4);
    file.read((char*)samplerate.data(), 4);
    Decoder::format = bitset<32>(format).to_ulong();
    Decoder::channels = bitset<32>(channels).to_ulong();
    Decoder::samplerate = bitset<32>(samplerate).to_ulong();
    long filesize = get_file_size(Decoder::filename + ".wavz") - 12;
    double fileBytesRead = 0.0;
    while (fileBytesRead <= filesize) { // TODO : get the end of the file
        short value = 0;
        for (int i = 8; i >= 0; i--) {
            value |= ((file.readBit() & 0x1) << i);
            fileBytesRead += 1.0/9.0;
        }
        Decoder::decodedAudio.push_back(value);
    }
    return 0;
}

int Decoder::decode() {
    short index = 0;
    Decoder::read_compressed_audio();
    for (auto sample : Decoder::decodedAudio) {
        index = codebook.closest_vector(bitset<16>(sample));
        Decoder::audioSamples.push_back(codebook.getCodebook().at(index).to_ulong());
    }
    return 0;
}

int Decoder::saveWAVFile(std::string filename) {
	if (Decoder::audioSamples.empty()) {																	// if there are no decoded samples, then
		return 1; 																							//     do nothing and exit with error 1 (TODO)
	} 																										//
	SndfileHandle file { (filename+"decoded.wav").data() , SFM_WRITE, Decoder::format, Decoder::channels, Decoder::samplerate };       // create audio file with precise characteristics
	file.write(Decoder::audioSamples.data(), Decoder::audioSamples.size());								    // write data into the WAV file
	return 0;																								// exit successfully
}



