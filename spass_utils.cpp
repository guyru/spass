
#include <string>
#include <fstream>
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
