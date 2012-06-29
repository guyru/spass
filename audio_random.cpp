/*
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

#include <stdexcept>
#include <cstring>
#include <cstdio>
#include <cerrno>
#include "md5.h"
#include "audio_random.h"

using namespace std;

const size_t NUM_SAMPLES = 2048 + 512;

AudioRandom::AudioRandom() :
	m_index(0)
{
	md5_init_ctx(&m_md5_ctx);
}

AudioRandom* AudioRandom::getInstance()
{
	static AudioRandom instance;
	return &instance;
}

void AudioRandom::setBackend(AudioRandomSource* source)
{
	m_source = source;
}

uint32_t AudioRandom::getDword()
{
	if (!m_index) {
		// we don't have any prepared random dwords
		get_block();
	}
	return m_buffer.v[--m_index];
}

void AudioRandom::gather_entropy()
{
	int16_t sample_buffer[NUM_SAMPLES];
	uint32_t noise;
	unsigned int bits_needed = 512;

	while (bits_needed) {
		m_source->getSamples(sample_buffer, NUM_SAMPLES);

		// fill 512/32 dwords with random bits from the sample buffer
		for (int i = 0; i<NUM_SAMPLES && bits_needed; i+=2) {
			// apply a von-neuman correction
			if (!((sample_buffer[i] ^ sample_buffer[i+1]) & 0x0001))
				continue;

			// xoring more bits only adds entropy
			noise = (sample_buffer[i]) << ((bits_needed-1) % 32);
			m_block[(bits_needed - 1)/32] ^= noise;
			--bits_needed;
		}
	}
}

const void* AudioRandom::getRaw(size_t *entropy_pool_size)
{
	if (!entropy_pool_size)
		return NULL;

	gather_entropy();
	*entropy_pool_size = sizeof(m_block);
	return m_block;
}
void AudioRandom::get_block()
{
	gather_entropy();
	md5_process_block(m_block, 64, &m_md5_ctx);
	md5_read_ctx(&m_md5_ctx,m_buffer.digest);
	m_index = 4;
}
