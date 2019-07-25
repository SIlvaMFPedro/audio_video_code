#include "wavhist.h"

using namespace std;

constexpr size_t FRAMES_BUFFER_SIZE = 65536; // Buffer for reading frames
std::string VERSION = "1.1.1"; // Using SemVer

/**
 * @brief Handles the errors, processing its codes and printing on the stderr file, 
 * if wanted to.
 * 
 * @param error {@code ERRNO} error code.
 * @param returnCode Internal error code.
 * @param loud {@code true} to print on {@code stderr} file; otherwise {@code false}.
 * @return int Internal error code return.
 */
int handle_error(int error, int returnCode, bool loud) {
	errno = error;
	if (loud) {
		std::cerr << std::strerror(errno) << std::endl;
	}
	return returnCode;
}

/**
 * @brief Execution of GnuPlot in order to plot an histogram. 
 * 
 * @param inputPath The filesystem path where the data is located. 
 * 					If left empty {@code /tmp/wavhist/histogram.dat} will be considered.
 * @param outputPath The filesystem path where the picture will be left. If left empty
 * 				     {@code ~/.wavhist/histogram.png} will be considered.
 * @return int {@code 0} if execution runs successfully; otherwise the error code.
 */
int plot_histogram(std::string inputPath, std::string outputPath) {
	Gnuplot gnuplot;
	gnuplot << "set terminal png enhanced" << std::endl;
	if (outputPath.empty()) {
		gnuplot << "set output \'~/.wavhist/histogram.png\'" << std::endl; 
	} else {
		gnuplot << "set output \'" + outputPath + "/histogram.png\'" << std::endl;
	}
	if (inputPath.empty()) {
		gnuplot << "DATAFILE = \"/tmp/wavhist/histogram.dat\"" << std::endl;
	} else {
		gnuplot << "DATAFILE = \"" + inputPath + "\"" << std::endl;
	} 
	gnuplot << "set grid" << std::endl;
	gnuplot << "set key below center horizontal noreverse enhanced autotitle box dashtype solid" << std::endl;
	gnuplot << "set tics out nomirror" << std::endl;
	gnuplot << "set border 1 front linetype black linewidth 1.0 dashtype solid" << std::endl;
	gnuplot << "set xtics rotate " << std::endl;
	gnuplot << "set style histogram " << std::endl;
	gnuplot << "set style data histograms" << std::endl;
	gnuplot << "set boxwidth 1.0 absolute" << std::endl;
	gnuplot << "set style fill solid 2.0 border 1" << std::endl;
	gnuplot << "plot DATAFILE using 2:xtic(int($0)\%1000 == 0 ? stringcolumn(1) : '') title \"channel\"" << std::endl;
	return 0; 
}

/**
 * @brief Parse the given arguments by the shell.
 * 
 * @param argc The number of arguments given by the shell.
 * @param argv The value of the arguments given by the shell.
 * @param fileName The WAVE Audio file to process.
 * @param channel The audio channel to process: {@code 0} if LEFT, {@code 1} if RIGHT and {@code 2} if AVERAGE.
 * @param outputFile File where to put the analysis results.
 * @param plot {@code true} if is to produce plot; otherwise {@code false}.
 * @param outputPlot Path to save histogram plot image.
 * @param quiet {@code true} to not presenting data in {@code stdout}; otherwise {@code false}.
 * @return int {@code 0} if execution runs successfully; otherwise the error code.
 */
int parse_arguments(int argc, char *argv[], std::string *fileName, int *channel, std::string *outputFile, bool *plot, std::string *outputPlot, bool *quiet) {
	try {
		std::string appName = boost::filesystem::basename(argv[0]);
		namespace po = boost::program_options;
		po::options_description desc("Options");
		desc.add_options()
			("help,h", "Print this help message")
			("input-file,i", po::value<std::string>(fileName)->required(), "WAVE file to process")
			("channel,c", po::value<int>(channel)->required(), "Audio channel (0: for left; 1: for right)")
			("plot,p", "Plot histogram (GNUPlot needed)")
			("output-file,o", po::value<std::string>(outputFile), "Output results to file")
			("plot-output-path,P", po::value<std::string>(outputPlot), "Path to save PNG histogram (installation of gnuplot needed)")
			("quiet,q", "Quiet mode (no output to stdout)")
			("version,v", "See version");
		po::variables_map vm;
		try {
			po::store(po::command_line_parser(argc, argv).options(desc).run(), vm);
			if (vm.count("help")) {
				std::cout << "This command produces histogram values of a WAVE audio file, as long as it is a common PCM 16-bit WAVE file\n\n" 
						  << "Usage: \n" 
						  << "wavhist [option...] \n\n" 
						  << "  {-c | --channel} CHANNEL\n"
						  << "    sets the processing channel (0 to left channel, 1 to right channel, 2 to average channels)\n"
					      << "  {-h | --help} \n"
						  << "    prints this help message\n"
						  << "  {-i | --input-file} WAVE_FILE_PATH \n"
						  << "    path to WAVE file (must be WAVE audio file in 16-bit PCM) \n"
						  << "  {-o | --output-file} FILE \n"
						  << "    file to export results as output to this program\n"
						  << "  {-p | --plot} \n"
						  << "    plots histogram (installation of gnuplot needed)\n"
						  << "  {-P | --plot-output-path} PATH_WITH_FILENAME\n"
						  << "    path to save PNG histogram (installation of gnuplot needed)\n"
						  << "  {-q | --quiet} \n"
						  << "    quiet mode (no output to stdout)\n"
						  << "  {-v | --version} \n"
						  << "    see version"
						  << std::endl;
				return 3;
			}
			if (vm.count("version")) {
				std::cout << "wavhist (WAVE Audio Histogram) v" + VERSION + "\n" 
						  << "This is free software; see the source for copying conditions.  There is NO\n"
						  << "warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE." << std::endl;
				return 3;
			}
			if (vm.count("channel")) {
				*channel = vm["channel"].as<int>();
			}
			if (vm.count("input-file")) {
				*fileName = vm["input-file"].as<std::string>();
			}
			if (vm.count("output-file")) {
				*outputFile = vm["output-file"].as<std::string>();
			}
			*plot = vm.count("plot");
			if (vm.count("plot-output-path")) {
				*outputPlot = vm["plot-output-path"].as<std::string>();
			}
			*quiet = vm.count("quiet");
			po::notify(vm);
		} catch (boost::program_options::required_option& e) {
			std::cerr << "ERROR: " << e.what() << std::endl;
			return handle_error(EINVAL, 1, false); 
		} catch (boost::program_options::error& e) {
			std::cerr << "ERROR: " << e.what() << std::endl;
			return handle_error(EINVAL, 1, false);
		}
	} catch (std::exception& e) {
		std::cerr << "Unhandled Exception reached the top of main: "
				  << e.what() << ", application will now exit" << std::endl;
		return handle_error(ECANCELED, 2, false); 
	}
	return 0;
}

/**
 * @brief Executes the WAVE Audio Histogram as is.
 * 
 * @param argc The number of given arguments by the shell.
 * @param argv The value of the given arguments by the shell.
 * @return int The execution error code. 
 */
int main(int argc, char *argv[]) {

	std::string fileName;
	std::string outputFile;
	std::string outputPlot;
	int channel;
	bool plot;
	bool quiet;
	int error;

	if ((error = parse_arguments(argc, argv, &fileName, &channel, &outputFile, &plot, &outputPlot, &quiet)) != 0) {
		if (error == 3) {
			errno = EXIT_SUCCESS;
			return 0;
		}
		return handle_error(EINVAL, 1, true); 
	}

	SndfileHandle sndFile { fileName };
	if (sndFile.error()) {
		cerr << "Error: invalid input file" << endl;
		return handle_error(EINVAL, 1, true); 
    }

	if ((sndFile.format() & SF_FORMAT_TYPEMASK) != SF_FORMAT_WAV) {
		cerr << "Error: file is not in WAV format" << endl;
		return handle_error(ENOTSUP, 1, true); 
	}

	if ((sndFile.format() & SF_FORMAT_SUBMASK) != SF_FORMAT_PCM_16) {
		cerr << "Error: file is not in PCM_16 format" << endl;
		return handle_error(ENOTSUP, 1, true); 
	}
	
	if (channel > sndFile.channels()) {
		cerr << "Error: invalid channel requested" << endl;
		return handle_error(EINVAL, 1, true);
	}

	size_t nFrames;
	vector<short> samples(FRAMES_BUFFER_SIZE * sndFile.channels());
	WAVHist hist { sndFile };
	while ((nFrames = sndFile.readf(samples.data(), FRAMES_BUFFER_SIZE))) {
		samples.resize(nFrames * sndFile.channels());
		hist.update(samples);
	}

	hist.save(channel, outputFile, !quiet);

	if (plot) {
		if (!boost::filesystem::is_directory(expand_user("~/.wavhist"))) {
			boost::filesystem::create_directory(expand_user("~/.wavhist"));
		}
		plot_histogram(expand_user(outputFile), expand_user(outputPlot));
	}
	return 0;
}

