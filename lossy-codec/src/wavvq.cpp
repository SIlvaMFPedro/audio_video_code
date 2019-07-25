#include "wavvq.h"

using namespace std;

int parse_arguments(int argc, char *argv[], std::string *encode, std::string *decode, std::string *codebook, bool *generate, int *booksize, int *overlap, bool *quiet) {
	try {
		std::string appName = boost::filesystem::basename(argv[0]);
		namespace po = boost::program_options;
		po::options_description desc("Options");
		desc.add_options()
			("help,h", "Print this help message")
			("encode,e", po::value<std::string>(encode), "Encode WAV audio file")
			("decode,d", po::value<std::string>(decode), "Decode WAV audio file")
            ("codebook,c", po::value<std::string>(codebook), "Codebook file to help on coding actions")
            ("generate-codebook,g", "Generate Codebook")
            ("booksize,b", po::value<int>(booksize), "Codebook size (number of codewords)")
            ("overlap,o", po::value<int>(overlap), "Codebook overlap factor")
            ("quiet,q", "Quiet mode")
			("version,v", "See version");
		po::variables_map vm;
		try {
			po::store(po::command_line_parser(argc, argv).options(desc).run(), vm);
			if (vm.count("help")) { // TODO: complete this section
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
			if (vm.count("encode")) {
				*encode = vm["encode"].as<std::string>();
			}
			if (vm.count("decode")) {
				*decode = vm["decode"].as<std::string>();
			}
            if (vm.count("codebook")) {
				*codebook = vm["codebook"].as<std::string>();
			}
            *generate = vm.count("generate-codebook");
            *quiet = vm.count("quiet");
            if (vm.count("booksize")) {
				*booksize = vm["booksize"].as<int>();
			}
            if (vm.count("overlap")) {
				*overlap = vm["overlap"].as<int>();
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

/**
 * @brief the main function
 * 
 * @return int 0 if it runs successfully; otherwise, an error code.
 */
int main(int argc, char* argv[]) {
    string codebookFileName;
    string encodeFileName;
    string decodeFileName;
    bool generateCodebook;
    int overlapFactor;
    int bookSize;
    bool quiet; 
    int error;

	/*if ((error = parse_arguments(argc, argv, &encodeFileName, &decodeFileName, &codebookFileName, &generateCodebook, &bookSize, &overlapFactor, &quiet)) != 0) {
		if (error == 3) {
			errno = EXIT_SUCCESS;
			return 0;
		}
		return 1; 
	}

    SndfileHandle audio;
    Codebook codebook;

    if (!encodeFileName.empty()) {
        audio = SndfileHandle(encodeFileName + ".wav");
        Quantizer quantizer = Quantizer(audio, 10); // TODO : parameterize this value
        quantizer.encode();
        quantizer.saveWAVFile(encodeFileName + ".wavq.wav");
        if (!codebookFileName.empty()) {
            codebook = Codebook().load(codebookFileName); // without format (no *.cb)
        } else {
            codebook = Codebook(bookSize, overlapFactor);
			codebook.create_codebook(encodeFileName + "wavq.wav");
			codebook.save(encodeFileName);
        }
        vector<string> encodedFileName;
        boost::split(encodedFileName, encodeFileName, boost::is_any_of("."));
        Encoder encoder = Encoder(codebook, audio, encodedFileName.at(0));
        encoder.encode();
    } else if (!decodeFileName.empty()) {
        Decoder decoder = Decoder(decodeFileName, codebookFileName);
        decoder.decode();
        decoder.saveWAVFile(decodeFileName);
    }*/

	bstream file{"test", ios::out|ios::binary};

	//file.writeBit(0);
	//file.writeBit(0);
	//file.writeBit(0);
	file.writeBit(0);
	file.writeBit(1);

	file.writeBit(0);
	
	file.writeBit(1);

	file.grantWrite();
	/*
	file.writeBit(0);

	file.writeBit(0);

	file.writeBit(0);
	
	
	file.writeBit(1);

	file.writeBit(0);
	file.writeBit(1);

	file.writeBit(1);
	
	file.writeBit(1);

	file.writeBit(0);

	file.writeBit(0);

	file.writeBit(0);
	
	
	file.writeBit(0);

	file.writeBit(0);
	file.writeBit(0);

	file.writeBit(0);
	
	file.writeBit(0);

	file.writeBit(0);

	file.writeBit(0);

	file.writeBit(0);
	
	
	file.writeBit(0);



	//file.writeBit(1);



	bstream output{"test", ios::in|ios::binary};

	/*
	for(int i = 0; i < 7; i++){
		cout << output.readBit();
	}
	*/


	return 0;
}