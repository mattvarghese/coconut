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

# include "portmanager.h"

# include <sys/socket.h>

# include <netinet/in.h>
// Redundant, for struct sockaddr_in. But in FreeBSD 14.3 required.

# include <netdb.h>
// For sockaddr_in, htons(), hostent, gethostbyname().

# include <unistd.h>
// For close()

# include <cstring>
using std::strcpy;
using std::memcpy;

# include <iostream>
using std::cout;
using std::flush;

# include "../include/color.h"

# include <semaphore.h>
extern sem_t * cout_mutex;	// Defined in main.cpp
// Only Read and Write functions need mutual exclusion for 
// cout statements.

PortManager::PortManager ( )
{
	strcpy ( hostname, "localhost" );
	for ( int i = 0; i < MAX_PORTS ; i++ )
	{
		portMap [i] = -1;
	}
}

PortManager :: ~PortManager ( )
{
	AtExit ( );
}

void PortManager :: AtExit ( )
{
	for ( int i = 0; i < MAX_PORTS ; i++ )
	{
		if ( portMap [i] != -1 )
			RemovePort ( i );
	}
}

int PortManager :: AddPort ( int portNo, int mapsTo )
{
	if ( portMap [portNo] != -1 ) return -1;
	portMap [portNo] = socket ( AF_INET, SOCK_STREAM, 0 );
	if ( portMap [portNo] == -1 ) return -2;
	
	sockaddr_in deviceManager;
	deviceManager.sin_family = AF_INET;
	deviceManager.sin_port = htons(mapsTo);
	
	hostent * hp = gethostbyname ( hostname );
	if ( hp == NULL )
	{
		close ( portMap [portNo] );
		portMap [portNo] = -1;
		return -3;
	}
	
	memcpy(reinterpret_cast<char*>(&deviceManager.sin_addr),
			reinterpret_cast<char*>(hp->h_addr_list[0]), hp->h_length);
	
	if ( connect( portMap[portNo], reinterpret_cast<sockaddr*>(&deviceManager),
				sizeof(deviceManager)) != 0 )
	{
		cout << red << "\n[ PortManager :: AddPort ] Attempt to connect Device "
			<< portNo << " to socket " << mapsTo << " falied" 
			<< reset << flush;
		close ( portMap [portNo] );
		portMap [portNo] = -1;
		return -4;
	}
	else cout << green << "\n[ PortManager :: AddPort ] Connected Device " << portNo
		<< " to socket " << mapsTo << " successfully" << reset << flush;
	return 0;
}

int PortManager :: RemovePort ( int portNo )
{
	if ( portMap [ portNo ] == -1 )
	{
		cout << red << "\n[ PortManager :: RemovePort ] User attempted to"
			<< " release unallocated Device No "
			<< portNo << reset << flush;
		return -1;
	}
	shutdown ( portMap [ portNo ], 2 );
	close ( portMap [ portNo ] );
	portMap [ portNo ] = -1 ;
	cout << green << "\n[ PortManager :: RemovePort ] Successfully released Device " 
		<< portNo << reset << flush;
	return 0;
}

int PortManager :: Write ( int portNo, word_32 oneWord )
{
	if ( portMap [ portNo ] == -1 )
	{
		sem_wait ( cout_mutex );
		cout << red << "\n[ PortManager :: Write ] User attempted to write"
			<< " to unallocated Device "
			<< portNo << reset << flush;
		sem_post ( cout_mutex );
		return -1;
	}
	if ( send ( portMap [ portNo ], &oneWord, 4, 0 ) == -1 )
	{
		sem_wait ( cout_mutex );
		cout << red << "\n[ PortManager :: Write ] Error while writing to Device " 
			<< portNo << reset << flush;
		sem_post ( cout_mutex );
		return -2;
	}
	sem_wait ( cout_mutex );
	cout << green << "\n[ PortManager :: Write ] Wrote word to Device " 
		<< portNo << reset << flush;
	sem_post ( cout_mutex );
	return 0;
}

int PortManager :: Read ( int portNo, word_32 & oneWord )
{
	if ( portMap [ portNo ] == -1 )
	{
		sem_wait ( cout_mutex );
		cout << red << "\n[ PortManager :: Read ] User attempted to read"
			<< " from unallocated Device "
			<< portNo << reset << flush;
		sem_post ( cout_mutex );
		return -1;
	}
	if ( recv ( portMap [ portNo ], &oneWord, 4, 0 ) == -1 )
	{
		sem_wait ( cout_mutex );
		cout << red << "\n[ PortManager :: Read ] Error while reading from Device " 
			<< portNo << reset << flush;
		sem_post ( cout_mutex );
		return -2;
	}
	sem_wait ( cout_mutex );
	cout << green << "\n[ PortManager :: Read ] Read word from Device " 
		<< portNo << reset << flush;
	sem_post ( cout_mutex );
	return 0;
}
