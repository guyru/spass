/*
 * spass - Secure password generator.
 * Copyright (C) 2010-2012  Guy Rutenberg
 * http://www.guyrutenberg.com
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

#include <fstream>
#include <iostream>
#include <string>
#include <boost/program_options.hpp>
#include "audio_random.h"
#include "spass_utils.h"
#include "audio_random_portaudio.h"
#include "audio_random_oss.h"
#include "audio_random_alsa.h"
#include "config.h"

using namespace std;
namespace po = boost::program_options;


int main(int argc, char **argv)
{
	size_t password_len = 9;
	string strip = "a-z";
	string file_name;
	ostream *output;
	ofstream out_file;

	po::options_description desc("Options");
        desc.add_options()
		("help,h", "display this help message and exit")
		("version", "output version information and exit")
		("length,l", po::value<size_t>(&password_len)->default_value(8),
			"length of password/passphrase")
		("strip,s",
			po::value<string>(&strip)->default_value("a-zA-Z0-9!@#$%^&*_-"),
			"strip for password")
		("passphrase,p", "generate a passphrase instead of a password")
		("raw", "strip for password")
		("file,f", po::value<string>(&file_name)->default_value("-"),
			"write output to FILE, instead of stdout, unless FILE is -")
		("verbose,v", "print additional info")
        ;

	po::positional_options_description pos_desc;
	pos_desc.add("length", 1);


	po::variables_map vm;
	try {
		po::store(po::command_line_parser(argc, argv).
				options(desc).positional(pos_desc).run(),
			vm);
		po::notify(vm);
	} catch ( const po::error& e ) {
		cerr << PACKAGE ": " << e.what() << endl;
		cerr << "Try `" PACKAGE " --help' for more information. "
			<< endl;
		return 1;
	}

	if (vm.count("help")) {
		cout << PACKAGE_STRING
			" - Secure password/passphrase generator" << endl;
		cout << "Synopsis:" << endl;
		cout << "  " PACKAGE " [options]" << endl << endl;
		cout << desc << endl;
		return 0;
	}
	if (vm.count("version")) {
		cout << PACKAGE_STRING << endl;
		cout << "Copyright (C) 2010-2013 Guy Rutenberg "
			"<http://www.guyrutenberg.com/contact-me>" << endl;
		cout << "The passphrase wordlist (C) 2000 Arnold G. Reinhold"
			<< endl;
		return 0;
	}
	AudioRandom::getInstance()->setBackend(AUDIO_RANDOM_BACKEND::getInstance());

	string exstrip;
	expand_strip(strip, exstrip);
	if (vm.count("verbose")) {
		cout<<"strip: "<<exstrip<<endl;
	}

	output = &cout;
	if (file_name != "-") {
		out_file.open(file_name.c_str(), ofstream::binary | ios::out);
		output = &out_file;
	}


	if (vm.count("raw")) {
		raw_randomness(password_len, *output);
	} else if (vm.count("passphrase")) {
		generate_passphrase(password_len, *output);
	} else {
		generate_password(password_len, exstrip, *output);
	}

	if (out_file.is_open())
		out_file.close();

	return 0;
}
