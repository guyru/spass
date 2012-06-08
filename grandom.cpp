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
#include <portaudio.h>
#include "grandom.h"
#include "md5.h"

using namespace std;

const size_t NUM_SAMPLES = 2048;

Grandom::Grandom() :
	m_index(-1)
{
	PaError err;

	err = Pa_Initialize();
	m_pa_initialization_failed = (err != paNoError);
	if (err != paNoError )
		throw runtime_error("Pa_Initizlize() failed");

	err = Pa_OpenDefaultStream(
		&m_stream,
		1,
		0,
		paInt16, // sample format
		44100, // sample rate
		NUM_SAMPLES,
		(PaStreamCallback *)NULL, // stream callback
		NULL // user data to pass to stream callback
	);
	
	if (err != paNoError )
		throw runtime_error("Pa_OpenDefaultStream() failed");

	md5_init_ctx(&m_md5_ctx);
}

Grandom::~Grandom()
{
	if (!m_pa_initialization_failed)
		Pa_CloseStream( m_stream );
}

Grandom& Grandom::getInstance() {
	static Grandom instance;
	return instance;
}

uint32_t Grandom::operator()()
{
	if (m_index) {
		// we don't have any prepared random dwords
		get_block();
	}
	return m_buffer.v[--m_index];
}

void Grandom::gather_entropy()
{
	int16_t sample_buffer[NUM_SAMPLES];
	unsigned int noise;
	unsigned int bits_needed = 512;
	PaError err;

	err = Pa_StartStream(m_stream);
	if (err != paNoError)
		throw runtime_error("Pa_StartStream() failed");

	while (bits_needed) {
		err = Pa_ReadStream(m_stream, sample_buffer, NUM_SAMPLES);
		if (err != paNoError)
			throw runtime_error("Pa_ReadStream() failed");


		// fill 512/32 dwords with random bits from the sample buffer
		for (int i = 0; i<NUM_SAMPLES && bits_needed; i+=2) {
			// apply a von-neuman correction
			if (!((sample_buffer[i] ^ sample_buffer[i+1]) & 0x0001))
				continue;

			// xoring more bits only adds entropy
			noise = (sample_buffer[i]) << ((bits_needed-1) % 32);
			m_block[(bits_needed - 1)/16] ^= noise;
			--bits_needed;
		}
	}

	err = Pa_StopStream(m_stream);
	if (err != paNoError)
		throw runtime_error("Pa_StopStream() failed");
}

void Grandom::get_block()
{
	gather_entropy();
	md5_process_block(m_block, 64, &m_md5_ctx);
	md5_read_ctx(&m_md5_ctx,m_buffer.digest);
	m_index = 4;
}
