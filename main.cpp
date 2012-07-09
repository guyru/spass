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
#include "audio_random_alsa.h"
#include "audio_random_portaudio.h"
#include "audio_random_oss.h"
#include "config.h"

using namespace std;
namespace po = boost::program_options;

static void generate_password(size_t length, const string& strip, ostream& output);
static void generate_passphrase(size_t length, ostream& output);
static void expand_strip(const string &in, string &out);
static void raw_randomness(size_t length, ostream& out_file);
extern "C" {char* getDiceWd (int n);}

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

	po::variables_map vm;
	try {
		po::store(po::parse_command_line(argc, argv, desc), vm);
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
		cout << "Copyright (C) 2010-2012 Guy Rutenberg "
			"<http://www.guyrutenberg.com/contact-me>" << endl;
		cout << "The passphrase wordlist (C) 2000 Arnold G. Reinhold"
			<< endl;
		return 0;
	}

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

	AudioRandom::getInstance()->setBackend(AudioRandomAlsa::getInstance());

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

/**
 * Generate a password of length 'length' using the strip 'strip'.
 * @param output [out] The generated password.
 */
void generate_password(size_t length, const string& strip, ostream& output)
{
	AudioRandom *arand = AudioRandom::getInstance();

	size_t strip_len = strip.size();
	for (size_t i = 0; i < length; i++) {
		double r = static_cast<double>(arand->getDword()) / static_cast<uint32_t>(-1);
		uint32_t transform = static_cast<uint32_t>(r * strip_len) % strip_len;
		output.put(strip.at(transform));
	}
	output << endl;
}

void generate_passphrase(size_t length, ostream &output)
{
	AudioRandom *arand = AudioRandom::getInstance();

	for (size_t i = 0; i < length; i++) {
		output<<getDiceWd(arand->getDword());
		if (i != length-1)
			output<<' ';
	}
	output << endl;
}

/**
 * Expand String
 */
void expand_strip(const string &in, string &out)
{
	out.clear();
	out.reserve(128); // no reason strip should be longer than that

	size_t in_len = in.size();
	for (size_t i = 0; i < in_len; i++) {
		if (in.at(i) != '-' || i == 0 || i == in_len-1) {
			out += in.at(i);
		}
		else { // we're parsing something like a-b
			while ( *(out.end()-1) < in.at(i+1)-1 ) {
				out += *(out.end()-1) + 1;
			}
		}
	}
}

/**
 * Write at least `length' bytes of random data to `outl_file'.
 */
void raw_randomness(size_t length, ostream& out_file)
{
	size_t entropy_pool_size;
	size_t bytes_written = 0;
	const void* entropy_pool;
	do {
		entropy_pool = AudioRandom::getInstance()->getRaw(&entropy_pool_size);
		out_file.write((const char*)entropy_pool, entropy_pool_size);
		bytes_written += entropy_pool_size;
	} while (!length || length > bytes_written);
}
