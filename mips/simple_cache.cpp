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

# include "simple_cache.h"

# include <iostream>
using std::cout;
using std::cin;
using std::flush;
# include <cstring>
using std::strcpy;

# include "../include/color.h"

# include <semaphore.h>
extern sem_t * cout_mutex;	// Defined in main.cpp

SimpleCache_TagRecord :: SimpleCache_TagRecord ( )
{
	tag = 0;
	valid = false;
	modified = false;
}



SimpleCache :: SimpleCache ( Cache * memory, int nob, int wpb, int assoc, char * ty,
	int lev, bool verbos ) 
{
	mem = memory;
	
	strcpy ( type, ty );
	level = lev;
	verbose = verbos;
	
	noOfBlocks = nob;
	wordsPerBlock = wpb;
	associativity = assoc;
	noOfSets = noOfBlocks / associativity;
	noOfBlocks = noOfSets * associativity;		// remove fractions
	
	cache = new word_32 ** [noOfSets];
	if ( cache == 0 )
	{
		sem_wait ( cout_mutex );
		cout << red << "\n[ SimpleCache " << type << " "
			<< level << "-level ] Error allocating memory (1)" 
			<< reset << flush;
		sem_post ( cout_mutex );
	}
	tagArray = new SimpleCache_TagRecord * [noOfSets];
	fifoIndex = new int [noOfSets];
	for ( int i = 0; i < noOfSets; i++ )
	{
		fifoIndex[i] = 0;
		cache[i] = new word_32 * [associativity];
		if ( cache[i] == 0 )
		{
			sem_wait ( cout_mutex );
			cout << red << "\n[ SimpleCache " << type << " "
				<< level << "-level ] Error allocating memory (2)"
				<< reset << flush;
			sem_post ( cout_mutex );
		}
		tagArray[i] = new SimpleCache_TagRecord [associativity];
		
		for ( int j = 0; j < associativity; j++ )
		{
			cache[i][j] = new word_32 [wordsPerBlock];
			if ( cache[i][j] == 0 )
			{
				sem_wait ( cout_mutex );
				cout << red << "\n[ SimpleCache " << type << " "
					<< level << "-level ]"
					<< " Error allocating memory (3)"
					<< reset << flush;
				sem_post ( cout_mutex );
			}
		}
	}
	
	readCount = readHitCount = 0;
	writeCount = writeHitCount = 0;
}

bool SimpleCache :: Read ( word_32 address, word_32 & result, int noOfBytes )
{
	if ( noOfBytes != 4 )
	{
		// Feature not supported right now
		sem_wait ( cout_mutex );
		cout << red << "\n[ SimpleCache::Read " << type << " "
			<< level << "-level ] no of bytes != 4" 
			<< reset << flush;
		sem_post ( cout_mutex );
		return false;
	}
	if ( address % 4 != 0 )
	{
		// Alignment error
		sem_wait ( cout_mutex );
		cout << red << "\n[ SimpleCache::Read " << type << " "
			<< level << "-level ] Alignment error" 
			<< reset << flush;
		sem_post ( cout_mutex );
		return false;
	}
	
	int blockTag = ( address / 4 ) / ( wordsPerBlock );
	int blockOffset = ( address / 4 ) % ( wordsPerBlock );
	int setNo = blockTag % noOfSets;
	
	int indexInSet = -1;
	for ( int i = 0; i < associativity; i++ )
	{
		if ( tagArray[setNo][i].valid == true &&
			tagArray[setNo][i].tag == blockTag )
		{
			// Hit.
			indexInSet = i;
			break;
		}
	}
	
	if ( indexInSet != -1 )
	{
		// Hit.
		if ( verbose == true )
		{
			sem_wait ( cout_mutex );
			cout << green << "\n[ SimpleCache::Read " << type << " "
				<< level << "-level ] Hit, SetNo = "
				<< setNo << ", indexInSet = " << indexInSet 
				<< ", blockOffset = " << blockOffset << reset << flush;
			sem_post ( cout_mutex );
		}
		readCount ++;
		readHitCount ++;
		
		result = cache[setNo][indexInSet][blockOffset];
		return true;
	}
	// We have a miss.
	int index = fifoIndex[setNo];
	if ( verbose == true )
	{
		sem_wait ( cout_mutex );
		cout << green << "\n[ SimpleCache::Read " << type << " "
			<< level << "-level ] Miss, SetNo = "
			<< setNo << ", replaceIntoIndex = " << index
			<< ", blockOffset = " << blockOffset << reset << flush;
		sem_post ( cout_mutex );
	}
	if ( tagArray[setNo][index].valid == true &&
		tagArray[setNo][index].modified == true )
	{
		// We need to write back.
		if ( verbose == true )
		{
			sem_wait ( cout_mutex );
			cout << green << "\n[ SimpleCache::Read " << type << " "
				<< level << "-level ]"
				<< " Previous contents require writing back..."
				<< reset << flush;
			sem_post ( cout_mutex );
		}
		int writeBaseAddress = tagArray[setNo][index].tag * wordsPerBlock * 4;
		for ( int i = 0; i < wordsPerBlock; i++ )
		{
			if ( mem -> Write ( writeBaseAddress + (4*i), 
				cache[setNo][index][i], 4) == false )
			{
				sem_wait ( cout_mutex );
				cout << red << "\n[ SimpleCache::Read " << type << " "
					<< level << "-level ] Write Back failed" 
					<< reset << flush;
				sem_post ( cout_mutex );
			}
		}
	}
	
	tagArray[setNo][index].tag = blockTag;
	tagArray[setNo][index].valid = true;
	tagArray[setNo][index].modified = false;
	
	if ( verbose == true )
	{
		sem_wait ( cout_mutex );
		cout << green << "\n[ SimpleCache::Read " << type << " "
			<< level << "-level ] Fetching new block..."
			<< reset << flush;
		sem_post ( cout_mutex );
	}

	int readBaseAddress = blockTag * wordsPerBlock * 4;
	for ( int i = 0; i < wordsPerBlock; i++ )
	{
		if ( mem -> Read ( readBaseAddress + (4*i),
			cache[setNo][index][i], 4 ) == false )
		{
			sem_wait ( cout_mutex );
			cout << red << "\n[ SimpleCache::Read " << type << " "
				<< level << "-level ] Read from from lower level failed" 
				<< reset << flush;
			sem_post ( cout_mutex );
		}
	}
	
	readCount ++;
	result = cache[setNo][index][blockOffset];
	fifoIndex[setNo] = ( fifoIndex[setNo] + 1) % associativity;
	return true;
}

bool SimpleCache :: Read_nofetch ( word_32 address, word_32 & result, int noOfBytes )
{
	if ( noOfBytes != 4 )
	{
		// Feature not supported right now
		sem_wait ( cout_mutex );
		cout << red << "\n[ SimpleCache::Read_nofetch " << type << " "
			<< level << "-level ] no of bytes != 4" 
			<< reset << flush;
		sem_post ( cout_mutex );
		return false;
	}
	if ( address % 4 != 0 )
	{
		// Alignment error
		sem_wait ( cout_mutex );
		cout << red << "\n[ SimpleCache::Read_nofetch " << type << " "
			<< level << "-level ] Alignment error" 
			<< reset << flush;
		sem_post ( cout_mutex );
		return false;
	}
	
	int blockTag = ( address / 4 ) / ( wordsPerBlock );
	int blockOffset = ( address / 4 ) % ( wordsPerBlock );
	int setNo = blockTag % noOfSets;
	
	int indexInSet = -1;
	for ( int i = 0; i < associativity; i++ )
	{
		if ( tagArray[setNo][i].valid == true &&
			tagArray[setNo][i].tag == blockTag )
		{
			// Hit.
			indexInSet = i;
			break;
		}
	}
	
	if ( indexInSet != -1 )
	{
		result = cache[setNo][indexInSet][blockOffset];
		return true;
	}
	
	return false;
}

bool SimpleCache :: Write ( word_32 address, word_32 value, int noOfBytes )
{
	if ( noOfBytes != 4 )
	{
		// Feature not supported right now
		sem_wait ( cout_mutex );
		cout << red << "\n[ SimpleCache::Write " << type << " "
			<< level << "-level ] no of bytes != 4" 
			<< reset << flush;
		sem_post ( cout_mutex );
		return false;
	}
	if ( address % 4 != 0 )
	{
		// Alignment error
		sem_wait ( cout_mutex );
		cout << red << "\n[ SimpleCache::Write " << type << " "
			<< level << "-level ] Alignment error" 
			<< reset << flush;
		sem_post ( cout_mutex );
		return false;
	}
	
	int blockTag = ( address / 4 ) / ( wordsPerBlock );
	int blockOffset = ( address / 4 ) % ( wordsPerBlock );
	int setNo = blockTag % noOfSets;
	
	int indexInSet = -1;
	for ( int i = 0; i < associativity; i++ )
	{
		if ( tagArray[setNo][i].valid == true &&
			tagArray[setNo][i].tag == blockTag )
		{
			// Hit.
			indexInSet = i;
			break;
		}
	}
	
	if ( indexInSet != -1 )
	{
		// Hit.
		if ( verbose == true )
		{
			sem_wait ( cout_mutex );
			cout << green << "\n[ SimpleCache::Write " << type << " "
				<< level << "-level ] Hit, SetNo = "
				<< setNo << ", indexInSet = " << indexInSet 
				<< ", blockOffset = " << blockOffset << reset << flush;
			sem_post ( cout_mutex );
		}
		writeCount ++;
		writeHitCount ++;
		
		cache[setNo][indexInSet][blockOffset] = value;
		tagArray[setNo][indexInSet].modified = true;
		return true;
	}
	// We have a miss.
	int index = fifoIndex[setNo];
	if ( verbose == true )
	{
		sem_wait ( cout_mutex );
		cout << green << "\n[ SimpleCache::Write " << type << " "
			<< level << "-level ] Miss, SetNo = "
			<< setNo << ", replaceIntoIndex = " << index
			<< ", blockOffset = " << blockOffset << reset << flush;
		sem_post ( cout_mutex );
	}
	if ( tagArray[setNo][index].valid == true &&
		tagArray[setNo][index].modified == true )
	{
		// We need to write back.
		if ( verbose == true )
		{
			sem_wait ( cout_mutex );
			cout << green << "\n[ SimpleCache::Write " << type << " "
				<< level << "-level ]"
				<< " Previous contents require writing back..."
				<< reset << flush;
			sem_post ( cout_mutex );
		}
		int writeBaseAddress = tagArray[setNo][index].tag * wordsPerBlock * 4;
		for ( int i = 0; i < wordsPerBlock; i++ )
		{
			if ( mem -> Write ( writeBaseAddress + (4*i), 
				cache[setNo][index][i], 4) == false )
			{
				sem_wait ( cout_mutex );
				cout << red << "\n[ SimpleCache::Write " << type << " "
					<< level << "-level ]"
					<< " Writeback failed" 
					<< reset << flush;
				sem_post ( cout_mutex );
			}
		}
	}
	
	tagArray[setNo][index].tag = blockTag;
	tagArray[setNo][index].valid = true;
	tagArray[setNo][index].modified = true;
	
	if ( verbose == true )
	{
		sem_wait ( cout_mutex );
		cout << green << "\n[ SimpleCache::Write " << type << " "
			<< level << "-level ] Fetching new block..."
			<< reset << flush;
		sem_post ( cout_mutex );
	}
	
	int readBaseAddress = blockTag * wordsPerBlock * 4;
	for ( int i = 0; i < wordsPerBlock; i++ )
	{
		if ( mem -> Read ( readBaseAddress + (4*i),
			cache[setNo][index][i], 4 ) == false )
		{
			sem_wait ( cout_mutex );
			cout << red << "\n[ SimpleCache::Write " << type << " "
				<< level << "-level ] Read from lower level failed" 
				<< reset << flush;
			sem_post ( cout_mutex );
		}
	}
	
	writeCount ++;
	cache[setNo][index][blockOffset] = value;
	fifoIndex[setNo] = ( fifoIndex[setNo] + 1) % associativity;
	return true;
}

void SimpleCache :: AtExit ( )
{
	for ( int i = 0; i < noOfSets; i++ )
	{
		for ( int j = 0; j < associativity; j++ )
			delete[] cache[i][j];
		delete[] cache[i];
		delete[] tagArray[i];
	}
	delete[] cache;
	delete[] tagArray;
	delete[] fifoIndex;
}

void SimpleCache :: Statistics ( )
{
	cout << green << "\n[ SimpleCache::Statistics ] Statistics for the "
		<< level << "-level \n\t" << type << " cache are displayed"
		<< reset << flush;
	cout << "\nNumber of Blocks : " << noOfBlocks
		<< "\nWords per block : " << wordsPerBlock
		<< "\nAssociativity : " << associativity
		<< "\nNumber of sets : " << noOfSets
		<< "\nTotal Cache Size in Bytes : " << noOfBlocks * wordsPerBlock * 4
		
		<< "\n\nTotal Number of reads : " << readCount
		<< "\nTotal Number of read Hits : " << readHitCount
		<< "\nTotal Number of read Misses : " << readCount - readHitCount
		<< "\nRead Hit Ratio : " 
		<< ((readCount != 0 ) ? static_cast<double>(readHitCount) / readCount : 0)
		
		<< "\n\nTotal Number of writes : " << writeCount
		<< "\nTotal Number of write Hits : " << writeHitCount
		<< "\nTotal Number of write Misses : " << writeCount - writeHitCount
		<< "\nWrite Hit Ratio : " 
		<< ((writeCount != 0 ) ? static_cast<double>(writeHitCount) / writeCount : 0)
		
		<< "\n\nTotal Number of accesses : " << readCount + writeCount
		<< "\nTotal Number of Hits : " << readHitCount + writeHitCount
		<< "\nTotal Number of Misses : " 
		<< ( readCount + writeCount ) - ( readHitCount + writeHitCount )
		<< "\nOverall Hit Ratio : " 
		<< (( (readCount + writeCount) != 0 ) ?  static_cast<double>
			( readHitCount + writeHitCount) / ( readCount + writeCount ) : 0)
		<< flush;
	
	mem -> Statistics ( );
}
