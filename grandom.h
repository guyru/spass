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

#include <cstdio>
#include <portaudio.h>
#include "md5.h"

class Grandom
{
public:
	static Grandom& getInstance();
	virtual ~Grandom();
	/**
	 * Generate a random dword
	 */
	uint32_t operator()();
private:
	Grandom();
	void gather_entropy();
	void get_block();
	
	PaStream *m_stream;
	uint32_t m_index;
	union {
		char digest[16];
		uint32_t v[4];
	} m_buffer;

	md5_ctx m_md5_ctx;
	uint32_t m_block[512/32];

	bool m_pa_initialization_failed;
	Grandom(Grandom const&); //no implementation
	void operator=(Grandom const&); //no implementation
};
