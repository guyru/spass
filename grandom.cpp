/*
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

#include <sys/soundcard.h>
#include <sys/ioctl.h>
#include <stdexcept>
#include <cstring>
#include <cstdio>
#include <cerrno>
#include "grandom.h"
#include "md5.h"

using namespace std;

Grandom::Grandom() :
	m_index(-1)
{
	m_dsp_fd = fopen("/dev/dsp","rb");
	if (!m_dsp_fd) {
		throw runtime_error(strerror(errno));
	}
	
	int format = AFMT_S16_NE;
	if (ioctl(fileno(m_dsp_fd), SNDCTL_DSP_SETFMT, &format)==-1) {
		throw runtime_error("Grandom: SNDCTL_DSP_SETFMT failed");
	}
	if (format != AFMT_S16_NE) {
		throw runtime_error("Grandom: failed to configure sample size");
	}

	int speed = 44100; // cd speed, should be supported anywhere
	if (ioctl(fileno(m_dsp_fd), SNDCTL_DSP_SPEED, &speed)==-1) {
		throw runtime_error("Grandom: SNDCTL_DSP_SPEED failed");
	}

	md5_init_ctx(&m_md5_ctx);
}

Grandom::~Grandom()
{
	fclose(m_dsp_fd);
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
	int16_t sample_buffer[512];

	if (fread(&sample_buffer, sizeof(int16_t), 512, m_dsp_fd) != 512) {
		throw runtime_error("Grandom: failed reading");
	}

	// fill 512/32 dwords with random bits from the sample buffer
	for (int i = 0; i < 512/32; i++) {
		for (int j = 0; j < 32; j++) {
			m_block[i] <<= 1;
			m_block[i] ^= (sample_buffer[32*i+j] & 1);
		}
	}
}

void Grandom::get_block()
{
	gather_entropy();
	md5_process_block(m_block, 64, &m_md5_ctx);
	md5_read_ctx(&m_md5_ctx,m_buffer.digest);
	m_index = 4;
}
