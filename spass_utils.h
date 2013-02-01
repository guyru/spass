
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
