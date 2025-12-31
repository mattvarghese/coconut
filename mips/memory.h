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

# ifndef __MEMORY_H
# define __MEMORY_H

# include "../include/instruction.h"

# define TYPEFIELDSIZE 16


class Cache
{
public:
	virtual void Statistics ( ) = 0;

	virtual bool Read ( word_32 address, word_32 & result, int noOfBytes ) = 0;
	virtual bool Read_nofetch ( word_32 address, word_32 & result, int noOfBytes ) = 0;
	
	virtual bool Write ( word_32 address, word_32 value, int noOfBytes ) = 0;
	
	// AtExit is the destructor, so there is really no need for the virtual destructor
	virtual void AtExit ( ) = 0;

	virtual ~Cache ( ) { };
};



class MainMemory : public Cache
{
private:
	char * memory;
	int size;
public:
	MainMemory ( int sz );
	~MainMemory ();
	
	// The following two functions are provided just to make 
	// MainMemory confirm to the Cache interface.
	void Statistics ( );
	bool Read_nofetch ( word_32 address, word_32 & result, int noOfBytes );
	
	bool Read ( word_32 address, word_32 & result, int noOfBytes );
	
	bool Write ( word_32 address, word_32 value, int noOfBytes );
	
	bool Load_MIPS_program ( char * filename );
	
	void AtExit ( );
};



class NoCache : public Cache
{
// This is something absolutely unnecessay and does nothing but simply
// acts as an interface between the cache and the memory...
// This is a vestegial class from a previous design, the overhaul
// of which obviated the necessity for this class...
private:
	Cache * mem;
	int accesses;
	
	char type[TYPEFIELDSIZE];
	int level;
public:
	NoCache ( Cache * memory, char * ty, int lev );
	void Statistics ( );
	bool Read ( word_32 address, word_32 & result, int noOfBytes );
	bool Read_nofetch ( word_32 address, word_32 & result, int noOfBytes );
	bool Write ( word_32 address, word_32 value, int noOfBytes );
	void AtExit ( );
};

# endif

