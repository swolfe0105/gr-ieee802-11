/*
 * Copyright (C) 2016 Bastian Bloessl <bloessl@ccs-labs.org>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "ls.h"
#include <cstring>
#include <iostream>
#include <fstream>
#include <ctime>
#include <ratio>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>


using namespace gr::ieee802_11::equalizer;

void ls::equalize(gr_complex *in, int n, gr_complex *symbols, uint8_t *bits, boost::shared_ptr<gr::digital::constellation> mod) {

	if(n == 0) {
		std::memcpy(d_H, in, 64 * sizeof(gr_complex));

	} else if(n == 1) {
		double signal = 0;
		double noise = 0;
		for(int i = 0; i < 64; i++) {
			noise += std::pow(std::abs(d_H[i] - in[i]), 2);
			signal += std::pow(std::abs(d_H[i] + in[i]), 2);
		}

		d_snr = 10 * std::log10(signal / noise / 2);

		for(int i = 0; i < 64; i++) {
			d_H[i] += in[i];
			d_H[i] /= LONG[i] * gr_complex(2, 0);
		}

		log_csi();

	} else {

		int c = 0;
		for(int i = 0; i < 64; i++) {
			if( (i == 11) || (i == 25) || (i == 32) || (i == 39) || (i == 53) || (i < 6) || ( i > 58)) {
				continue;
			} else {
				symbols[c] = in[i] / d_H[i];
				bits[c] = mod->decision_maker(&symbols[c]);
				c++;
			}
		}
	}
}

double ls::get_snr() {
	return d_snr;
}

void ls::log_csi() {
    std::ofstream logfile;

	// open log file
	logfile.open(logfilename, std::ios::app | std::ios::ate | std::ios::out);
	if (!logfile.is_open()) {
		std::cout << "unable to open file";
	};

	// timestamp
	std::chrono::high_resolution_clock::time_point end_time
                             = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> timespan = std::chrono::duration_cast<std::chrono::duration<double>>(end_time - start_time);
	logfile << timespan.count();

	// SNR
	logfile << "," << get_snr();

	// CSI
	for(int i = 0; i < 64; i++) {
		if( (i == 11) || (i == 25) || (i == 32) || (i == 39) ||
            (i == 53) || (i < 6) || ( i > 58)) {
			continue;
		} else {
			logfile << "," << d_H[i].real() << "," << d_H[i].imag();
		}
	}

	logfile << "\n";
	logfile.close();
}
