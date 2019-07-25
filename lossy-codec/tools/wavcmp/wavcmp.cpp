#include "wavcmp.h"

using namespace std;

vector<short> open_audio(SndfileHandle audio) {
	vector<short> samples;
	size_t nFrames;																	
	std::vector<short> tmp(FRAMES_BUFFER_SIZE * audio.channels());		
	while ((nFrames = audio.readf(tmp.data(), FRAMES_BUFFER_SIZE))) {	
		for (auto sample : tmp) {													
			samples.push_back(sample);
		}																			
		tmp.resize(nFrames * audio.channels());
	}
	return samples;
}

double mean(vector<short> samples) {
    double sum = 0;
    for (auto sample : samples) {
        sum += sample;
    }
    return sum/samples.size();
}

double var(vector<short> samples, double meanValue) {
    double var = 0;
    for (auto sample : samples) {
        var += (sample - meanValue) * (sample - meanValue);
    }
    return var/samples.size();
}

double get_snr(vector<short> original, vector<short> modified) {
    double meanOriginal = mean(original);
    double meanModified = mean(modified);
    double rawSNR = var(original, meanOriginal)/var(modified, meanModified);
    return 10 * log10(rawSNR);
}

double get_snr2(vector<short> original, vector<short> modified) {
	double originalValuesSum = 0;
	for (auto value : original) {
		originalValuesSum += value*value;
	}
	double modifiedValuesSum = 0;
	for (int i = 0; i != original.size(); i++) {
		modifiedValuesSum += (original.at(i) - modified.at(i)) * (original.at(i) - modified.at(i));
	}
	return 10 * log10(originalValuesSum/modifiedValuesSum);
}

short get_max_abs_error_per_sample(vector<short> original, vector<short> modified) {
	short maxValue = 0;
	short tempMax = 0;
	if (original.size() <= modified.size()) {
		for (int i = 0; i != original.size(); i++) {
			if ((tempMax = abs(original.at(i) - modified.at(i))) > maxValue) {
				maxValue = tempMax;
			}
		}
	} else {
		for (int i = 0; i != modified.size(); i++) {
			if ((tempMax = abs(original.at(i) - modified.at(i))) > maxValue) {
				maxValue = tempMax;
			}
		}
	}
	return maxValue;
}

int parse_arguments(int argc, char *argv[], std::string *original, std::string *modified) {
	try {
		std::string appName = boost::filesystem::basename(argv[0]);
		namespace po = boost::program_options;
		po::options_description desc("Options");
		desc.add_options()
			("help,h", "Print this help message")
			("original,o", po::value<std::string>(original)->required(), "Original WAV audio file")
			("modified,m", po::value<std::string>(modified)->required(), "Modified (or noisy) WAV audio file")
			("version,v", "See version");
		po::variables_map vm;
		try {
			po::store(po::command_line_parser(argc, argv).options(desc).run(), vm);
			if (vm.count("help")) {
				std::cout << "This command evaluates the Signal-to-Noise Ratio and the Maximum per sample absolute error\n" << 
							 "between two WAVE audio files, as long as it is a common PCM 16-bit WAVE file\n\n" 
						  << "Usage: \n" 
						  << "wavcmp [option...] \n\n" 
						  << "  {-o | --original} FILE\n"
						  << "    original WAV audio file\n"
						  << "  {-m | --modified} FILE\n"
						  << "    modified (or noisy) WAV audio file\n"
					      << "  {-h | --help} \n"
						  << "    prints this help message\n"
						  << "  {-v | --version} \n"
						  << "    see version"
						  << std::endl;
				return 3;
			}
			if (vm.count("version")) {
				std::cout << "wavcmp (WAVE Audio SNR and MAE) v" + VERSION + "\n" 
						  << "This is free software; see the source for copying conditions.  There is NO\n"
						  << "warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE." << std::endl;
				return 3;
			}
			if (vm.count("original")) {
				*original = vm["original"].as<std::string>();
			}
			if (vm.count("modified")) {
				*modified = vm["modified"].as<std::string>();
			}
			po::notify(vm);
		} catch (boost::program_options::required_option& e) {
			std::cerr << "ERROR: " << e.what() << std::endl;
			return 1; 
		} catch (boost::program_options::error& e) {
			std::cerr << "ERROR: " << e.what() << std::endl;
			return 1;
		}
	} catch (std::exception& e) {
		std::cerr << "Unhandled Exception reached the top of main: "
				  << e.what() << ", application will now exit" << std::endl;
		return 2; 
	}
	return 0;
}

int main(int argc, char* argv[]) {
	string originalFileName;
	string modifiedFileName;
	int error;

	if ((error = parse_arguments(argc, argv, &originalFileName, &modifiedFileName)) != 0) {
		if (error == 3) {
			errno = EXIT_SUCCESS;
			return 0;
		}
		return 1; 
	}

	SndfileHandle originalAudio { originalFileName };
	SndfileHandle modifiedAudio { modifiedFileName };

	if (originalAudio.error() || modifiedAudio.error()) {
		cerr << "Error: invalid input file" << endl;
		return 1; 
    }

	if ((originalAudio.format() & SF_FORMAT_TYPEMASK) != SF_FORMAT_WAV || (modifiedAudio.format() & SF_FORMAT_TYPEMASK) != SF_FORMAT_WAV) {
		cerr << "Error: file is not in WAV format" << endl;
		return 1; 
	}

	if ((originalAudio.format() & SF_FORMAT_SUBMASK) != SF_FORMAT_PCM_16 || (modifiedAudio.format() & SF_FORMAT_SUBMASK) != SF_FORMAT_PCM_16) {
		cerr << "Error: file is not in PCM_16 format" << endl;
		return 1; 
	}

	vector<short> originalSamples = open_audio(originalAudio);
	vector<short> modifiedSamples = open_audio(modifiedAudio);

	cout << "The Signal-to-Noise ratio is " << get_snr2(originalSamples, modifiedSamples) << " dB" << endl;
	cout << "The Maximum per sample Absolute Error is " << get_max_abs_error_per_sample(originalSamples, modifiedSamples) << endl;

	return 0;
}
