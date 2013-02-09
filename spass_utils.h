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

#ifndef _SPASS_UTILS_H_
#define _SPASS_UTILS_H_

#include <string>
#include <vector>

void generate_password(size_t length, const std::string& strip, std::ostream& output);
void generate_passphrase(size_t length, std::ostream &output);
void expand_strip(const std::string &in, std::string &out);
void raw_randomness(size_t length, std::ostream& out_file);

class SpassStrip
{
public:
	void generatePassword(size_t length, std::string &password);
	std::vector<std::string> strip;
	/**
	 * Separator inserted between symbols in the password
	 */
	std::string separator;
};

#endif //_SPASS_UTILS_H_
