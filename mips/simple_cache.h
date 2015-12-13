/* Copyright 2005-2015 Varghese Mathew (Matt)
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

# ifndef __SIMPLE_CACHE
# define __SIMPLE_CACHE

# include "memory.h"

class SimpleCache_TagRecord
{
public:
	int tag;
	bool valid;
	bool modified;
	
	SimpleCache_TagRecord ( );
};

class SimpleCache : public Cache
{
private:
	// Memory is also a type of cache to 
	// make multilevel cache's easier to implement
	Cache * mem;
	
	char type[TYPEFIELDSIZE];
	int level;
	bool verbose;
	
	int noOfBlocks;
	int wordsPerBlock;
	int associativity;
	int noOfSets;

	word_32 *** cache;
	SimpleCache_TagRecord ** tagArray;
	int * fifoIndex;
	
	int readCount;
	int readHitCount;
	int writeCount;
	int writeHitCount;
public:
	SimpleCache ( Cache * memory, int nob, int wpb, int assoc, char * ty, int lev,
		bool verbos );
	void Statistics ( );
	bool Read ( word_32 address, word_32 & result, int noOfBytes );
	bool Read_nofetch ( word_32 address, word_32 & result, int noOfBytes );
	bool Write ( word_32 address, word_32 value, int noOfBytes );
	void AtExit ( );
};

# endif
