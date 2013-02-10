/*
 * Copyright (C) 2010-2013  Guy Rutenberg
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

#ifndef _AUDIO_RANDOM_PORTAUDIO_H_
#define _AUDIO_RANDOM_PORTAUDIO_H_

#include "audio_random.h"

typedef void PaStream;

class AudioRandomPortAudio
: public AudioRandomSource {
public:
	static AudioRandomPortAudio* getInstance();
	virtual ~AudioRandomPortAudio();
	virtual void getSamples(void *samples, size_t num_samples);
private:
	AudioRandomPortAudio();
	AudioRandomPortAudio(AudioRandomPortAudio const&); //no implementation
	void operator=(AudioRandomPortAudio const&); //no implementation

	PaStream *m_stream;
	bool m_pa_initialization_failed;
};

#endif // _AUDIO_RANDOM_PORTAUDIO_H_
