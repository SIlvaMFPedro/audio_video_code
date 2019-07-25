#include <iostream>
#include <vector>
#include <sndfile.hh>

using namespace std;

constexpr size_t FRAMES_BUFFER_SIZE = 65536; // Buffer for reading/writing frames

/**
 * @brief Executes the WAVE Audio copier as is.
 * 
 * @param argc The number of given arguments by the shell.
 * @param argv The value of the given arguments by the shell.
 * @return int The execution error code.
 */
int main(int argc, char *argv[]) {

	// Argument count validation
	if (argc < 3) {
		cerr << "Usage: wavcp <input file> <output file>" << endl;
		return 1;
	}

	// Creation of an Sound File Handler based on the penultimate given argument
	SndfileHandle sndFileIn { argv[argc-2] };

	// Verification of file handler creation.
	if (sndFileIn.error()) {
		cerr << "Error: invalid input file" << endl;
		return 1;
    }

	// Verification of file type (it must be a WAVE file)
	if ((sndFileIn.format() & SF_FORMAT_TYPEMASK) != SF_FORMAT_WAV) {
		cerr << "Error: file is not in WAV format" << endl;
		return 1;
	}

	// Verification of file format (it must be PCM 16-bits)
	if ((sndFileIn.format() & SF_FORMAT_SUBMASK) != SF_FORMAT_PCM_16) {
		cerr << "Error: file is not in PCM_16 format" << endl;
		return 1;
	}

	// Definition of proper execution
	cout << "Input file has:" << endl;
	cout << '\t' << sndFileIn.frames() << " frames" << endl;
	cout << '\t' << sndFileIn.samplerate() << " samples per second" << endl;
	cout << '\t' << sndFileIn.channels() << " channels" << endl;

	// Creation of a Sound File Handler based on the last given argument
	SndfileHandle sndFileOut { argv[argc-1], SFM_WRITE, sndFileIn.format(), sndFileIn.channels(), sndFileIn.samplerate() };

	// Verification of file handler creation.
	if (sndFileOut.error()) {
		cerr << "Error: invalid output file" << endl;
		return 1;
    }

	// Copy the file from the beginning to the end
	size_t nFrames;
	vector<short> samples(FRAMES_BUFFER_SIZE * sndFileIn.channels());
	while ((nFrames = sndFileIn.readf(samples.data(), FRAMES_BUFFER_SIZE))) {
		sndFileOut.writef(samples.data(), nFrames);
	}

	return 0;
}

