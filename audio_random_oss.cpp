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
#include "audio_random_oss.h"
#include "md5.h"

using namespace std;

AudioRandomOSS::AudioRandomOSS()
{
	m_dsp_fd = fopen("/dev/dsp","rb");
	if (!m_dsp_fd) {
		throw runtime_error(strerror(errno));
	}
	
	int format = AFMT_S16_NE;
	if (ioctl(fileno(m_dsp_fd), SNDCTL_DSP_SETFMT, &format) == -1) {
		throw runtime_error(strerror(errno));
	}
	if (format != AFMT_S16_NE) {
		throw runtime_error(strerror(errno));
	}

	int speed = 44100; // cd speed, should be supported anywhere
	if (ioctl(fileno(m_dsp_fd), SNDCTL_DSP_SPEED, &speed) == -1) {
		throw runtime_error(strerror(errno));
	}
}

AudioRandomOSS::~AudioRandomOSS()
{
	if (m_dsp_fd)
		fclose(m_dsp_fd);
}

AudioRandomOSS* AudioRandomOSS::getInstance()
{
	static AudioRandomOSS instance;
	return &instance;
}

void AudioRandomOSS::getSamples(void *samples, size_t num_samples)
{
	if (fread(samples, sizeof(int16_t), num_samples, m_dsp_fd) != num_samples) {
		throw runtime_error("AudioRandomOSS: failed reading");
	}
}
