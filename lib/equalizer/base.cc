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

#include "base.h"
#include <cstring>
#include <iostream>
#include <fstream>

using namespace gr::ieee802_11::equalizer;

const gr_complex base::LONG[] = {
	 0,  0,  0,  0,  0,  0,  1,  1, -1, -1,
	 1,  1, -1,  1, -1,  1,  1,  1,  1,  1,
	 1, -1, -1,  1,  1, -1,  1, -1,  1,  1,
	 1,  1,  0,  1, -1, -1,  1,  1, -1,  1,
	-1,  1, -1, -1, -1, -1, -1,  1,  1, -1,
	-1,  1, -1,  1, -1,  1,  1,  1,  1,  0,
	 0,  0,  0,  0
};

const gr_complex base::POLARITY[127] = {
	 1, 1, 1, 1,-1,-1,-1, 1,-1,-1,-1,-1, 1, 1,-1, 1,
	-1,-1, 1, 1,-1, 1, 1,-1, 1, 1, 1, 1, 1, 1,-1, 1,
	 1, 1,-1, 1, 1,-1,-1, 1, 1, 1,-1, 1,-1,-1,-1, 1,
	-1, 1,-1,-1, 1,-1,-1, 1, 1, 1, 1, 1,-1,-1, 1, 1,
	-1,-1, 1,-1, 1,-1, 1, 1,-1,-1,-1, 1, 1,-1,-1,-1,
	-1, 1,-1,-1, 1,-1, 1, 1, 1, 1,-1, 1,-1, 1,-1, 1,
	-1,-1,-1,-1,-1, 1,-1, 1, 1,-1, 1,-1, 1, 1, 1,-1,
	-1, 1,-1,-1,-1, 1, 1, 1,-1,-1,-1,-1,-1,-1,-1 };

base::base() {

	const char *homedir;
	std::ofstream logfile;

	/* Initializatize CSI Logging */
	start_time = std::chrono::high_resolution_clock::now();

	// Place logfile in user's home directory
	if ((homedir = std::getenv("HOME")) == NULL) {
		logfilename = "";
	} else {
		std::string s(homedir);
		logfilename = s;

#ifdef _WIN32
		logfilename.append("\\");
#else
		logfilename.append("/");
#endif
		logfilename.append("csi.log");

	// Add start tag to log file
		logfile.open(logfilename.c_str());
		logfile << "LOGGING START\n";
		logfile.close();
	}
}
