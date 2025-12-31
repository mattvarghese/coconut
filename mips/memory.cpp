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

# include "memory.h"

# include <iostream>
using std::cout;
using std::flush;
# include <fstream>
using std::ifstream;
# include <cstring>
using std::strcpy;
# include <cstdlib>

# include "../include/color.h"

# include <semaphore.h>
extern sem_t * cout_mutex;	// Defined in main.cpp

MainMemory :: MainMemory ( int sz )
{
	size = sz;
	memory = new char [ size ];
	if (memory == NULL)
	{
		cout << red << "\nError: could not allocate memory...\n" 
			<< reset << flush;
		std::exit ( 10 );
	}
}

MainMemory :: ~MainMemory ()
{
	AtExit ( );
}

bool MainMemory :: Read ( word_32 address, word_32 & result, int noOfBytes )
{
	word_32 retVal = 0;
	char * ref = reinterpret_cast<char *> (&retVal);
	if ( address >= size || address < 0 )
	{
		sem_wait ( cout_mutex );
		cout << red << "\n[ Memory::Read ] Error, Address out of bounds" 
			<< reset << flush;
		sem_post ( cout_mutex );
		return false;
	}
	
	switch ( noOfBytes )
	{
	case 1:
		ref[0] = memory[address];
		break;
	case 2:
		if (address % 2 != 0 )
		{
			sem_wait ( cout_mutex );
			cout << red << "\n[ Memory::Read ] Alignment Error" 
				<< reset << flush;
			sem_post ( cout_mutex );
			return false;
		}
		ref[0] = memory[address];
		ref[1] = memory[address+1];
		break;
	case 4:
		if (address % 4 != 0 )
		{
			sem_wait ( cout_mutex );
			cout << red << "\n[ Memory::Read ] Alignment Error" 
				<< reset << flush;
			sem_post ( cout_mutex );
			return false;
		}
		ref[0] = memory[address];
		ref[1] = memory[address+1];
		ref[2] = memory[address+2];
		ref[3] = memory[address+3];
		break;
	default:
		sem_wait ( cout_mutex );
		cout << red << "\n[ Memory::Read ] Error, Unacceptable noOfBytes" 
			<< reset << flush;
		sem_post ( cout_mutex );
		return false;
		break;
	};
	result = retVal;
	return true;
}

bool MainMemory :: Write ( word_32 address, word_32 value, int noOfBytes )
{
	char * ref = reinterpret_cast<char *> (&value);
	if ( address >= size || address < 0 )
	{
		sem_wait ( cout_mutex );
		cout << red << "\n[ Memory::Write ] Error, Address out of bounds" 
			<< reset << flush;
		sem_post ( cout_mutex );
		return false;
	}
	
	switch ( noOfBytes )
	{
	case 1:
		memory[address] = ref[0];
		break;
	case 2:
		if (address % 2 != 0 )
		{
			sem_wait ( cout_mutex );
			cout << red << "\n[ Memory::Write ] Alignment Error" 
				<< reset << flush;
			sem_post ( cout_mutex );
			return false;
		}
		memory[address] = ref[0];
		memory[address+1] = ref[1];
		break;
	case 4:
		if (address % 4 != 0 )
		{
			sem_wait ( cout_mutex );
			cout << red << "\n[ Memory::Write ] Alignment Error" 
				<< reset << flush;
			sem_post ( cout_mutex );
			return false;
		}
		memory[address] = ref[0];
		memory[address+1] = ref[1];
		memory[address+2] = ref[2];
		memory[address+3] = ref[3];
		break;
	default:
		sem_wait ( cout_mutex );
		cout << red << "\n[ Memory::Write ] Error, Unacceptable noOfBytes" 
			<< reset << flush;
		sem_post ( cout_mutex );
		return false;
		break;
	};
	return true;
}

bool MainMemory :: Load_MIPS_program ( char * filename )
{
	ifstream progFile( filename );
	if ( !progFile ) return false;	// failed to open the file.
	
	// The first record in the file is the record giving the
	// Starting address of the program...
	// This record can be ignored in this function as the
	// boot time fetch address is fixed...
	// Note that this function is used only for BOOTLOADING.
	
	OneRecord rec;
	int size = sizeof (rec);
	progFile.read ( reinterpret_cast<char*>(&rec), size);
	if ( !progFile ) return false;
	
	// Reading the first significant record.
	progFile.read ( reinterpret_cast<char*>(&rec), size);
	
	while ( progFile )
	{
		*( reinterpret_cast<int*> (memory + rec.address) ) = rec.inst.iV;
		progFile.read ( reinterpret_cast<char*>(&rec), size);
	}
	progFile.close();
	return true;
}

void MainMemory :: AtExit ( )
{
	if ( memory != NULL )
		delete [] memory;
	size = 0;
}

void MainMemory :: Statistics ( ) 
{
}

bool MainMemory :: Read_nofetch ( word_32 address, word_32 & result, int noOfBytes )
{
	return false;
}

/********************************************************************/


NoCache :: NoCache ( Cache * memory, char * ty, int lev )
{
	mem = memory;
	accesses = 0;
	
	strcpy ( type, ty );
	level = lev;
}

void NoCache :: Statistics ( )
{
	cout << green << "\n[ NoCache::Statistics] Statistics for the "
		<< level << "-level " << type << " cache." << reset
		<< "\nNo of accesses = " << accesses << flush;
	mem -> Statistics ( );
}

bool NoCache :: Read ( word_32 address, word_32 & result, int noOfBytes )
{
	accesses ++;
	return mem -> Read ( address, result, noOfBytes );
}

bool NoCache :: Read_nofetch ( word_32 address, word_32 & result, int noOfBytes )
{
	return false;
}

bool NoCache :: Write ( word_32 address, word_32 value, int noOfBytes )
{
	accesses ++;
	return mem -> Write ( address, value, noOfBytes );
}

void NoCache :: AtExit ( )
{	// Does nothing
}

