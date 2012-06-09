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
#include <portaudio.h>
#include "audio_random_portaudio.h"

using namespace std;

const size_t NUM_SAMPLES = 2048;

AudioRandomPortAudio::AudioRandomPortAudio()
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
}

AudioRandomPortAudio::~AudioRandomPortAudio()
{
	if (!m_pa_initialization_failed)
		Pa_CloseStream( m_stream );
}

AudioRandomPortAudio* AudioRandomPortAudio::getInstance()
{
	static AudioRandomPortAudio instance;
	return &instance;
}

void AudioRandomPortAudio::getSamples(void *samples, size_t num_samples)
{
	PaError err;
	err = Pa_StartStream(m_stream);
	if (err != paNoError)
		throw runtime_error("Pa_StartStream() failed");

	err = Pa_ReadStream(m_stream, samples, num_samples);
	if (err != paNoError)
		throw runtime_error("Pa_ReadStream() failed");

	err = Pa_StopStream(m_stream);
	if (err != paNoError)
		throw runtime_error("Pa_StopStream() failed");
}
