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

# include <iostream>
using std::cout;
using std::cin;
using std::flush;

# include <fcntl.h>
# include <semaphore.h>
sem_t * cout_mutex;	// Referred as extern from all
	// other files in 'mips' which uses cout
	// and which might encounter thread switches 
	// while c-ing out...

# include "processor.h"
# include "memory.h"
# include "simple_cache.h"
# include "portmanager.h"

# include "../include/color.h"

# define	INPUTPORT	5678
# define	OUTPUTPORT	5680

// Does it justify having a declaration when the definition is also in the same file?
// Why not move the definition up here?
Cache * pickCache ( Cache * mem, bool noMultilevel, char * type, int level );

int main ( )
{	
	cout_mutex = sem_open ( "coutmutex", O_CREAT | O_EXCL, O_RDWR, 1 );
	if ( cout_mutex == NULL )
	{
		sem_unlink ( "coutmutex" );
		cout_mutex = sem_open ( "coutmutex", O_CREAT, O_RDWR, 1 );
	}
	if ( cout_mutex == NULL )
	{
		cout << red << "\nError, cout_mutex semaphore couldn't be opened." 
			<< "\nTerminating... \n" << reset << flush; 
		return -3;
	}
	
	// TODO this should be a configuration
	MainMemory * mem = new MainMemory ( 4914304 ); // 4 MB
	
	// Here we initialise the memory system
	// so that the processor starting address 
	// contains the bootloader snippet.
	if( ! mem -> Load_MIPS_program ( "a.out" ) )
	{
		cout << red << "\nError, could not load the \"a,out\" program..."
			<< "\nTerminating... \n" << reset << flush;
		return -2;
	}
	
	Cache *dc = NULL, *ic = NULL;
	
	cout << "\nChoose the type of cache you want for " 
		<< green << "Data" << reset << flush;
	dc = pickCache ( mem, false, "DATA", 1 );	// Multilevels are allowed
	
	cout << "\nChoose the type of cache you want for "
		<< green << "Instructions" << reset << flush;
	ic = pickCache ( mem, false, "INSTRUCTION", 1 );	// Multilevels are allowed
	
	if ( dc == NULL ) // No data cache
	{
		cout << red << "\nError, No Data Cache specified... \nTerminating... \n"
			<< reset << flush;
		return -1;
	}

	if ( ic == NULL ) // No data cache
	{
		cout << red << "\nError, No Instruction Cache specified... "
			<< "\nTerminating... \n" << reset << flush;
		return -1;
	}
	
	// We also need to create the port manager system
	// And set up the mapping between ports or device numbers
	// and sockets...
	PortManager * pMan = new PortManager ( );
	pMan -> AddPort ( 1, INPUTPORT );	// A character Input device
	pMan -> AddPort ( 2, OUTPUTPORT );	// A character Output device
	
	Processor proc ( mem, dc,ic, pMan );
	proc.Execute ( );	// Now this thread runs the processor clock function...
	
	return 0;
}

# define MULTILEVEL 3

Cache * pickCache ( Cache * mem, bool noMultilevel, char * type, int level )
{
	cout << "\n" << level << "-level " << type 
		<< " Cache : The choices available are"
		<< "\n 1. No Cache "
		<< "\n 2. Simple single level FIFO writeback cache ";
	if ( noMultilevel == false )
		cout << "\n " << MULTILEVEL << ". Multilevel Cache ";
	cout << "\nPlease enter your choice : " << flush;
	
	int choice;
	cin >> choice;
	while ( choice < 1 || ( noMultilevel == true && choice > (MULTILEVEL-1) ) ||
		( noMultilevel == false && choice > MULTILEVEL ) )
	{
		cout << red << "\nBad choice, Enter again : " << reset << flush;
		cin >> choice;
	}
	
	Cache * c = NULL;
	
	switch ( choice )
	{
	case 1:
		c = dynamic_cast<Cache *>( new NoCache (mem, type, level) );
		break;
	case 2: 
		{
			cout << "\nEnter number of Blocks in cache : ";
			int nob; cin >> nob;
			cout << "\nEnter number of words per block : ";
			int wpb; cin >> wpb;
			cout << "\nEnter associativity : ";
			int assoc; cin >> assoc;
			cout << "\nSelect extent of cache info displayed : "
				<< "\n 1.verbose"
				<< "\n 2.silent"
				<< "\nEnter your choice : ";
			int display; cin >> display;
			while ( display < 1 || display > 2 )
			{
				cout << red << "\nBad choice, Enter again : " 
					<< reset << flush;
				cin >> display;
			}
			bool verbose = ( display == 1 )? true : false ;
			
			c = dynamic_cast<Cache *>( new SimpleCache(mem, nob, wpb,
					assoc, type, level, verbose) );
		}
		break;
	case MULTILEVEL:
		{
			cout << "\nEnter the number of levels in the Cache : ";
			int noLevels; cin >> noLevels;
			Cache * prev_c = mem;
			for ( int i = 0; i < noLevels; i++ )
			{
				cout << "\nChoose the " << noLevels - i << "-level cache : ";
				c = pickCache ( prev_c, true, type, noLevels - i );
					// 2-nd arg = true => no multilevel
				prev_c = c;
			}
		}
		break;
	};
	return c;
}
