/*
 * Copyright (C) 2013  Guy Rutenberg
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

#include <string>
#include <fstream>
#include <cmath>
#include "audio_random.h"
#include "spass_utils.h"

extern "C" {char* getDiceWd (int n);}

using namespace std;


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
		} else { // we're parsing something like a-b
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

void SpassStrip::generatePassword(size_t length, string &out)
{
	AudioRandom *arand = AudioRandom::getInstance();
	size_t len_strip = strip.size();
	if (len_strip <= 0)
		return;
	unsigned strip_log = ceil(log2(len_strip));

	for (size_t i = 0; i < length; i++) {
		size_t rand;
		do {
			rand = arand->getDword() & ((1 << strip_log) - 1);
		} while (rand >= len_strip);

		if (i != 0)
			out += separator;
		out += strip.at(rand);
	}
}
