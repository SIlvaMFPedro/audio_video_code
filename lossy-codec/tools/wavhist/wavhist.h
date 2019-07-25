#ifndef WAVHIST_H
#define WAVHIST_H

#include <vector>
#include <map>
#include <sndfile.hh>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cerrno>

#include "boost/program_options.hpp"
#include "boost/filesystem.hpp"
#include "gnuplot-iostream.h"
#include "utils.h"

class WAVHist {
	private:
		std::vector<std::map<short, size_t>> counts;
		std::vector<short> totalSamples;

	public:
		WAVHist(const SndfileHandle& sfh) {
			counts.resize(sfh.channels()+1);
		}

		void update(const std::vector<short>& samples) {
			size_t n { };
			for (auto s : samples) {
				counts[n++ % counts.size()][s]++;
				totalSamples.push_back(s);
			}
			for (int i = 0; i != totalSamples.size()/2; i+=2) {
				counts[2][(totalSamples.at(i) + totalSamples.at(i+1)) / 2.0]++;
			}
		}

		void dump(const size_t channel) const {
			for (auto [value, counter] : counts[channel]) {
				std::cout << value << '\t' << counter << '\n';
			}
		}

		void save(const size_t channel, std::string filename, bool loud) const {
			if (!boost::filesystem::exists("/tmp/wavhist")) {
				boost::filesystem::create_directory("/tmp/wavhist");
			}
			std::ofstream fout(filename.empty() ? "/tmp/wavhist/histogram.dat" : filename);
			std::stringstream out;
			for (auto [value, counter] : counts[channel]) {
				if (loud) {
					std::cout << value << '\t' << counter << '\n';
				}
				out << value << '\t' << counter << '\n';
			}
			fout << out.str();
			fout.close();
		}
};

#endif

