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

#ifndef _AUDIO_RANDOM_OSS_H_
#define _AUDIO_RANDOM_OSS_H_

#include "audio_random.h"

class AudioRandomOSS
: public AudioRandomSource {
public:
	static AudioRandomOSS* getInstance();
	virtual ~AudioRandomOSS();
	virtual void getSamples(void *samples, size_t num_samples);
private:
	AudioRandomOSS();
	AudioRandomOSS(AudioRandomOSS const&); //no implementation
	void operator=(AudioRandomOSS const&); //no implementation

	FILE* m_dsp_fd;
};

#endif // _AUDIO_RANDOM_OSS_H_
