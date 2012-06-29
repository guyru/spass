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
#include <boost/format.hpp>
#include <alsa/asoundlib.h>
#include "audio_random_alsa.h"

using namespace std;

const size_t NUM_SAMPLES = 2048;

AudioRandomAlsa::AudioRandomAlsa() :
	m_is_device_opened(0)
{
	int err;
	snd_pcm_hw_params_t *hw_params;
	const char * pcm_name = "default";

	err = snd_pcm_open (&m_capture_handle, pcm_name, SND_PCM_STREAM_CAPTURE, 0);
	if (err < 0) {
		boost::format err_msg("Cannot open audio device %1 (%2)");
		err_msg % pcm_name % snd_strerror(err);
		throw runtime_error(err_msg.str());
	}
	m_is_device_opened = true;

	if ((err = snd_pcm_hw_params_malloc (&hw_params)) < 0) {
		hw_params = NULL;
		boost::format err_msg("Cannot allocate hardware parameter structure (%s)");
		err_msg % snd_strerror(err);
		throw runtime_error(err_msg.str());
	}

	if ((err = snd_pcm_hw_params_any (m_capture_handle, hw_params)) < 0) {
		boost::format err_msg("Cannot initialize hardware parameter structure (%s)");
		err_msg % snd_strerror(err);
		throw runtime_error(err_msg.str());
	}

	if ((err = snd_pcm_hw_params_set_access (m_capture_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
		boost::format err_msg("Cannot set access type (%s)");
		err_msg % snd_strerror(err);
		throw runtime_error(err_msg.str());
	}

	if ((err = snd_pcm_hw_params_set_format (m_capture_handle, hw_params, SND_PCM_FORMAT_S16_LE)) < 0) {
		boost::format err_msg("Cannot set sample format (%s)");
		err_msg % snd_strerror(err);
		throw runtime_error(err_msg.str());
	}

	unsigned int caputre_rate = 44100;
	if ((err = snd_pcm_hw_params_set_rate_near (m_capture_handle, hw_params, &caputre_rate, 0)) < 0) {
		boost::format err_msg("Cannot set sample rate (%s)");
		err_msg % snd_strerror(err);
		throw runtime_error(err_msg.str());
	}

	if ((err = snd_pcm_hw_params_set_channels (m_capture_handle, hw_params, 1)) < 0) {
		boost::format err_msg("Cannot set channel count (%s)");
		err_msg % snd_strerror(err);
		throw runtime_error(err_msg.str());
	}

	if ((err = snd_pcm_hw_params (m_capture_handle, hw_params)) < 0) {
		boost::format err_msg("Cannot set parameters (%s)");
		err_msg % snd_strerror(err);
		throw runtime_error(err_msg.str());
	}
	snd_pcm_hw_params_free (hw_params);
	
	if ((err = snd_pcm_prepare (m_capture_handle)) < 0) {
		boost::format err_msg("Cannot prepare audio interface for use (%s)");
		err_msg % snd_strerror(err);
		throw runtime_error(err_msg.str());
	}

}

AudioRandomAlsa::~AudioRandomAlsa()
{
	if (m_is_device_opened)
		snd_pcm_close(m_capture_handle);
}

AudioRandomAlsa* AudioRandomAlsa::getInstance()
{
	static AudioRandomAlsa instance;
	return &instance;
}

void AudioRandomAlsa::getSamples(void *samples, size_t num_samples)
{
	int err;

	err = snd_pcm_readi(m_capture_handle, samples, num_samples);
	if (err != num_samples) {
		boost::format err_msg("Read from audio interface failed (%s)");
		err_msg % snd_strerror(err);
		throw runtime_error(err_msg.str());
	}

}
