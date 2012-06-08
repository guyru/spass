/*
 * spass - Secure password generator.
 * Copyright (C) 2010  Guy Rutenberg
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

#include <iostream>
#include <string>
#include <boost/program_options.hpp>
#include "grandom.h"
#include "config.h"

using namespace std;
namespace po = boost::program_options;

static void generate_password(size_t length, const string &strip, string &output);
static void expand_strip(const string &in, string &out);

int main(int argc, char **argv)
{
	size_t password_len;
	string strip;
	po::options_description desc("Options");
        desc.add_options()
		("help,h", "produce this help message")
		("version", "prints version string")
		("length,l", po::value<size_t>(&password_len)->default_value(8), "length of password")
		("strip,s", po::value<string>(&strip)->default_value("a-zA-Z0-9!@#$%^&*_-"), "strip for password")
		("verbose,v", "print additional info")
        ;

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);

	if (vm.count("help")) {
		cout<<PACKAGE_STRING<<" - Secure password Generator"<<endl;
		cout<<"Synopsis:"<<endl;
		cout<<"  spass [options]"<<endl<<endl;
		cout<<desc<<endl;
		return 0;
	}
	if (vm.count("version")) {
		cout<<PACKAGE_STRING<<endl;
		cout<<"Copyright (C) 2010-2012 Guy Rutenberg <http://www.guyrutenberg.com/contact-me>"<<endl;
		return 0;
	}

	string exstrip;
	expand_strip(strip, exstrip);
	if (vm.count("verbose")) {
		cout<<"strip: "<<exstrip<<endl;
	}
	string output;
	generate_password(password_len, exstrip, output);

	cout<<output<<endl;

	return 0;
}

/**
 * Generate a password of length 'length' using the strip 'strip'.
 * @param output [out] The generated password.
 */
void generate_password(size_t length, const string &strip, string &output)
{
	Grandom grand;

	output.clear();
	output.reserve(length);

	size_t strip_len = strip.size();
	for (size_t i = 0; i < length; i++) {
		double r = static_cast<double>(grand()) / static_cast<uint32_t>(-1);
		uint32_t transform = static_cast<uint32_t>(r * strip_len) % strip_len;
		output += strip.at(transform);
	}
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
