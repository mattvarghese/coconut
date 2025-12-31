/* Copyright 2005-2025 Varghese Mathew (Matt)
 *
 * This file is part of Coconut (TM).
 * Coconut is a
 *     Multi-threaded simulation of the pipeline of a MIPS-like
 *     Microprocessor (integer instructions only) replete with 
 *     Memory Subsystem, Caches and their performance analysis,
 *     I/O device modules and an assembler.
 * 
 * Coconut is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Coconut is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Coconut.  If not, see <http://www.gnu.org/licenses/>.
 */

# ifndef __PORTMANAGER_H
# define __PORTMANAGER_H

# include "../include/types.h"

# define MAX_PORTS 32

class PortManager
{
private:
	char hostname[32];
	int portMap [MAX_PORTS];
public:
	PortManager ( );
	~PortManager ( );
	void AtExit ( );
	int AddPort ( int portNo, int mapsTo );
	int RemovePort ( int portNo );
	int Write ( int portNo, word_32 oneWord );
	int Read ( int portNo, word_32 & oneWord );
};

# endif
