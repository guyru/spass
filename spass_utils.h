
#ifndef _SPASS_UTILS_H_
#define _SPASS_UTILS_H_

#include <string>

void generate_password(size_t length, const std::string& strip, std::ostream& output);
void generate_passphrase(size_t length, std::ostream &output);
void expand_strip(const std::string &in, std::string &out);
void raw_randomness(size_t length, std::ostream& out_file);

#endif //_SPASS_UTILS_H_
