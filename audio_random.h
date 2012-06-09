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

#ifndef _AUDIO_RANDOM_H_
#define _AUDIO_RANDOM_H_

#include "md5.h"

class AudioRandomSource;

class AudioRandom {
public:
	static AudioRandom* getInstance();
	virtual uint32_t getDword();

	/**
	 * Return a pointer to the raw entropy pool.
	 */
	virtual const void* getRaw(size_t *entropy_pool_size);
	virtual void setBackend(AudioRandomSource* source);
	virtual ~AudioRandom(){};

private:
	AudioRandom();
	void gather_entropy();
	void get_block();

	AudioRandomSource* m_source;
	uint32_t m_index;
	union {
		char digest[16];
		uint32_t v[4];
	} m_buffer;

	md5_ctx m_md5_ctx;
	uint32_t m_block[512/32];
};

class AudioRandomSource {
public:
	static AudioRandomSource* getInstance();
	virtual void getSamples(void *samples, size_t num_samples) = 0;
protected:
	AudioRandomSource(){};
	AudioRandomSource(AudioRandomSource const&); //no implementation
	void operator=(AudioRandomSource const&); //no implementation
};

#endif // _AUDIO_RANDOM_H_
