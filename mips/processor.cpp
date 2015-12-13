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

# include "processor.h"

# include <iostream>
using std::cout;
using std::flush;

# include <cstdlib>

# include <fcntl.h>
# include <signal.h>

# include "../include/color.h"

/**********************************************************************************
**********************Constructor and Destructor functions************************/

Processor :: Processor ( MainMemory * m, Cache * dc, Cache * ic, PortManager * pm )
{
	mem = m;
	dataCache = dc;
	instrCache = ic;
	pman = pm;
	requestProgramTermination = false;
	blockUpdate = false;
	
	for ( int i = 0; i < 32 ; i++ )
		reg[i] = 0;
	Hi = Lo = 0;
	
	for ( int i = 0; i < 5; i++ )
	{
		inLatch[i].Initialise ( );
		outLatch[i].Initialise ( );
		flushStage[i] = false;
	}
	PCreg = SYSTEM_START_ADDRESS;
	
	continueCount = 0;
	for ( int i = 0; i < BREAKPOINTARRAYSIZE; i++ )
		breakPointArray[i] = -1;
}

Processor :: ~Processor ( )
{
	AtExit ( );
}

void Processor :: AtExit ( )
{
	for ( int i = 0; i < 5; i++ )
		pthread_kill ( stagethread[i], SIGKILL );
	
	// Kill all the threads before closing semaphores.
	
	for ( int i = 0; i < 5; i++ )
	{
		int stagesem_value;
		if ( sem_getvalue ( stagesem[i], &stagesem_value ) == -1 )
			cout << red << "\nError polling the value of stagesem["
				<< i << "]" << reset << flush;
		else
			cout << gray << "\nThe value of stagesem[" << i << "] at closing = "
				<< stagesem_value << reset << flush;
			
		sem_close ( stagesem[i] );
	}
	
	int clocksem_value;
	if ( sem_getvalue ( clocksem, &clocksem_value ) == -1 )
		cout << red << "\nError polling the value of clocksem"
			<< reset << flush;
	else
		cout << gray << "\nThe value of clocksem at closing = "
			<< clocksem_value << reset << flush;
	sem_close ( clocksem );
	
	int pc_mutex_value;
	if ( sem_getvalue ( pc_mutex, &pc_mutex_value ) == -1 )
		cout << red << "\nError polling the value of pc_mutex"
			<< reset << flush;
	else
		cout << gray << "\nThe value of pc_mutex at closing = "
			<< pc_mutex_value << reset << flush;
	sem_close ( pc_mutex );
	
	sem_unlink ( "clocksem" );
	sem_unlink ( "stagesem1" );
	sem_unlink ( "stagesem2" );
	sem_unlink ( "stagesem3" );
	sem_unlink ( "stagesem4" );
	sem_unlink ( "stagesem5" );
	sem_unlink ( "pcmutex" );
}

void Processor :: Terminate ( )
{
	requestProgramTermination = true;
}



/*******************************************************************************
******************Thread Entry Points******************************************/
// These are friend functions which call a corresponding Stage function in the 
// processor... This approach is necessary because we cannot take the address of
// a class member function to provide it to pthread_create API

void* stage0 ( void * pobj )
{
	Processor * p = reinterpret_cast<Processor*> (pobj);
	
	do
	{
		sem_wait ( p -> stagesem[0] );
		
		p -> Stage0 ( );
		
		sem_post ( p -> clocksem );
		
	} while ( true );
	
	return NULL;
}

void* stage1 ( void * pobj )
{
	Processor * p = reinterpret_cast<Processor*> (pobj);
	
	do
	{
		sem_wait ( p -> stagesem[1] );
		
		p -> Stage1 ( );
		
		sem_post ( p -> clocksem );
		
	} while ( true );
	
	return NULL;
}

void* stage2 ( void * pobj )
{
	Processor * p = reinterpret_cast<Processor*> (pobj);
	
	do
	{
		sem_wait ( p -> stagesem[2] );
		
		p -> Stage2 ( );
		
		sem_post ( p -> clocksem );
		
	} while ( true);
	
	return NULL;
}

void* stage3 ( void * pobj )
{
	Processor * p = reinterpret_cast<Processor*> (pobj);
	
	do
	{
		sem_wait ( p -> stagesem[3] );
		
		p -> Stage3 ( );
		
		sem_post ( p -> clocksem );
		
	} while ( true );
	
	return NULL;
}

void* stage4 ( void * pobj )
{
	Processor * p = reinterpret_cast<Processor*> (pobj);
	
	do
	{
		sem_wait ( p -> stagesem[4] );
		
		p -> Stage4 ( );
		
		sem_post ( p -> clocksem );
		
	} while ( true );
	
	return NULL;
}


/*********************************************************************************
*******************Threading initialisation**************************************/

void Processor :: Execute ( )
{
	clocksem = sem_open ( "clocksem", O_CREAT | O_EXCL, O_RDWR, 5 );
	if ( clocksem == NULL )
	{
		sem_unlink ( "clocksem" );
		clocksem = sem_open ( "clocksem", O_CREAT, O_RDWR, 5 );
	}
	
	pc_mutex = sem_open ( "pcmutex", O_CREAT | O_EXCL, O_RDWR, 1 );
	if ( pc_mutex == NULL )
	{
		sem_unlink ( "pcmutex" );
		pc_mutex = sem_open ( "pcmutex", O_CREAT, O_RDWR, 1 );
	}
	
	stagesem[0] = sem_open ( "stagesem1", O_CREAT | O_EXCL, O_RDWR, 0 );
	if ( stagesem[0] == NULL )
	{
		sem_unlink ( "stagesem1" );
		stagesem[0] = sem_open ( "stagesem1", O_CREAT, O_RDWR, 0 );
	}
	
	stagesem[1] = sem_open ( "stagesem2", O_CREAT | O_EXCL, O_RDWR, 0 );
	if ( stagesem[1] == NULL )
	{
		sem_unlink ( "stagesem2" );
		stagesem[1] = sem_open ( "stagesem2", O_CREAT, O_RDWR, 0 );
	}
	
	stagesem[2] = sem_open ( "stagesem3", O_CREAT | O_EXCL, O_RDWR, 0 );
	if ( stagesem[2] == NULL )
	{
		sem_unlink ( "stagesem3" );
		stagesem[2] = sem_open ( "stagesem3", O_CREAT, O_RDWR, 0 );
	}
	
	stagesem[3] = sem_open ( "stagesem4", O_CREAT | O_EXCL, O_RDWR, 0 );
	if ( stagesem[3] == NULL )
	{
		sem_unlink ( "stagesem4" );
		stagesem[3] = sem_open ( "stagesem4", O_CREAT, O_RDWR, 0 );
	}
	
	stagesem[4] = sem_open ( "stagesem5", O_CREAT | O_EXCL, O_RDWR, 0 );
	if ( stagesem[4] == NULL )
	{
		sem_unlink ( "stagesem5" );
		stagesem[4] = sem_open ( "stagesem5", O_CREAT, O_RDWR, 0 );
	}
	
	if ( clocksem == NULL || pc_mutex == NULL || stagesem[0] == NULL ||
		 stagesem[1] == NULL || stagesem[2] == NULL ||
		 stagesem[3] == NULL || stagesem[4] == NULL)
	{
		cout << red << "\nError, Some of the semaphores couldn't be opened." 
			<< "\nTerminating...\n" << red << flush;
		std::exit (0);
	}
	
	pthread_create ( &stagethread[0], NULL, &::stage0, this );
	pthread_create ( &stagethread[1], NULL, &::stage1, this );
	pthread_create ( &stagethread[2], NULL, &::stage2, this );
	pthread_create ( &stagethread[3], NULL, &::stage3, this );
	pthread_create ( &stagethread[4], NULL, &::stage4, this );
	
	ExecutionThread ( );	// This thread now becomes the clockmanager...
}

void Processor :: ExecutionThread ( )
{
	int clock_count = 0;
	do
	{
		for ( int i = 0; i < 5; i++ )
			sem_wait ( clocksem );
		
		Clock( clock_count ++ );
		// Note that clock count is incremented
		
		for ( int i = 0; i < 5; i++ )
			sem_post ( stagesem[i] );
	
	} while ( true );
}
