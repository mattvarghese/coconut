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

/*
 * This program simulates a text screen for the coconut mips simulator.
 * It listens on a specified port for connections.
 * Once the coconut simulator makes connection, whatever output that is
 * sent to us by the simulator gets displayed on the console.
 */

# include <iostream>
using std::cin;
using std::cerr;
using std::cout;
using std::flush;

# include <sys/socket.h>

//# include <netinet/in.h>
// Redundant, for struct sockaddr_in.

# include <netdb.h>
// For sockaddr_in, htons(), hostent, gethostbyname().

# include <unistd.h>
// For close()

# include <cstring>
using std::strcpy;
using std::memcpy;

# include <pthread.h>

# define	QUESIZE		2

# define	INPUTPORT	5678
# define	OUTPUTPORT	5680

# include "../include/types.h"

# include "../include/color.h"

void* outputThread ( void * arg )
{
	int port_number = OUTPUTPORT;
	
	// First Open the socket.
	int serv_sock = socket ( AF_INET, SOCK_STREAM, 0 );
	if ( serv_sock == -1 )
	{
		cerr << red << "\nUnrecoverable error, could not create socket !!"
			<< reset << "\n";
		return NULL;
	}
	
	// Then bind the socket to a port number on a particular host.
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port_number);
	hostent *hp = gethostbyname ( "localhost" );
	if ( hp == NULL )
	{
		cerr << red << "\nUnrecoverable error, unable to resolve hostname !!"
			<< reset << "\n";
		close ( serv_sock );
		return NULL;
	}
	memcpy(reinterpret_cast<char*>(&addr.sin_addr),
		reinterpret_cast<char*>(hp->h_addr_list[0]), hp->h_length);
	
	if ( bind ( serv_sock, reinterpret_cast<sockaddr*>(&addr), sizeof(addr) ) != 0 )
	{
		cerr << red << "\nUnrecoverable error, unable to bind to socket !!"
			<< reset << "\n";
		close ( serv_sock );
		return NULL;
	}
	
	// Then listen on the socket for connection requests.
	if ( listen( serv_sock, QUESIZE ) != 0 )
	{
		cerr << red << "\nUnrecoverable error, unable to listen on port !!"
			<< reset "\n";
		close ( serv_sock );
		return NULL;
	}
	
	//do
	//{
		word_32 oneWord;
		// Accept a request when it comes.
		int newsock = accept ( serv_sock, NULL, NULL );
		if ( newsock == -1 )
		{
			cerr << red << "\nUnrecoverable error,"
				<< " unable to accept connection !!" << reset << "\n";
			shutdown ( serv_sock, 2 );
			close ( serv_sock );
			return NULL;
		}
		else cout << "\nOutput Thread made contact !!\n";
		
		while ( true )
		{
			if ( recv ( newsock, &oneWord, 4, 0 ) > 0 )
				cout << static_cast<char>(oneWord) << flush;
			else break;
		}
		
		shutdown ( newsock, 2 );
		close ( newsock );
		
	//}while ( true );
	
	cout << "\n\n";
	
	if ( shutdown( serv_sock,2) != 0 )
		cerr << red << "\nError shutting down server socket"
			<< reset << "\n";
	if ( close ( serv_sock ) != 0 )
		cerr << red << "\nServer socket could not be closed properly"
			<< reset << "\n";

	return NULL;
}

int main ( )
{
	outputThread ( NULL );
	return 0;
}
