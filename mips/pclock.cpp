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

# include "processor.h"

# include <iostream>
using std::cout;
using std::cin;
using std::flush;

# include <cstdlib>

# include <iomanip>
using std::setw;

# include <signal.h>

# include "../include/color.h"

# include <semaphore.h>
extern sem_t * cout_mutex;	// Defined in main.cpp
// Clock runs when other threads are blocked,
// Therefore there is no need to control couts by 
// mutual exclusion in the clocks...
// But the clock function has the responsibility to 
// close the semaphore when exiting the program...

void Processor :: Clock ( int clk )
{	
	if ( requestProgramTermination == true )
	{
		for ( int i = 0; i < 5; i++ )
			pthread_kill (stagethread[i],SIGTERM);
			
		int cout_mutex_value;
		if ( sem_getvalue ( cout_mutex, &cout_mutex_value ) == -1 )
			cout << red << "\nError polling the value of cout_mutex"
				<< reset << flush;
		else
			cout << gray << "\nThe value of cout_mutex at closing = "
				<< cout_mutex_value << gray << flush;
			
		sem_close ( cout_mutex );
		sem_unlink ( "coutmutex" );
				
		// As we are using the 'exit()' to exit from the program,
		// the various destructors don't get invoked...
		// Therefore, instead of a destructor, we have provided
		// an 'AtExit()' functions wherever applicable
		dataCache -> AtExit ( );
		instrCache -> AtExit ( );
		pman -> AtExit ( );
		AtExit ( );
		
		cout << "\n\n" << flush;
		std::exit ( -99 );
	}
	
	cout << blue << "\n[** Clock: " << clk << " **] Executed..." << reset << flush;
	
	for ( int i = 0; i < 5; i++ )
		if ( flushStage[i] == true )
		{
			cout << blue << "\n[** Clock: " << clk << " **] Flushing stage "
				<< i << reset << flush;
			inLatch[i].Initialise ( );
			inLatch[i].finished = true;
			outLatch[i].Initialise ( );
			outLatch[i].finished = true;
		}
	
	if ( outLatch[4].finished == true )
	{
		if ( outLatch[3].finished == true )
		{
			//cout << "\n[** Clock: " << clk 
			//	<< " **] inLatch[4] <- outLatch[3]"
			//	<< flush;
			LatchCopy ( inLatch[4], outLatch[3] );
			
			if ( outLatch[2].finished == true )
			{
				//cout << "\n[** Clock: " << clk 
				//	<< " **] inLatch[3] <- outLatch[2]"
				//	<< flush;
				LatchCopy ( inLatch[3], outLatch[2] );
				
				if ( outLatch[1].finished == true )
				{
					//cout << "\n[** Clock: " << clk 
					//	<< " **] inLatch[2] <- outLatch[1]"
					//	<< flush;
					LatchCopy ( inLatch[2], outLatch[1] );
					
					if ( outLatch[0].finished == true )
					{
						//cout << "\n[** Clock: " << clk 
						//	<< " **] inLatch[1] <- outLatch[0]"
						//	<< flush;
						LatchCopy ( inLatch[1], outLatch[0] );
						inLatch[0].Initialise ( );
						
						NPCfrom = NOT_WRITTEN;
		// If pipeline was stalled at ID, but a branch in EX had completed, then
		// the NPCreg value should be preserved.  NPCfrom is updated only if 
		// there were no stalls.
						PCreg = NPCreg;
					}
					else
					{
						cout << blue << "\n[** Clock: " << clk 
							<< " **] inLatch[1].Initialise ( )"
							<< reset << flush;
						inLatch[1].Initialise ( );
					}
				}
				else
				{
					cout << blue << "\n[** Clock: " << clk 
						<< " **] inLatch[2].Initialise ( )"
						<< reset << flush;
					inLatch[2].Initialise ( );
				}
			}
			else
			{
				cout << blue << "\n[** Clock: " << clk 
					<< " **] inLatch[3].Initialise ( )"
					<< reset << flush;
				inLatch[3].Initialise ( );
			}
		}
		else
		{
			cout << blue << "\n[** Clock: " << clk 
				<< " **] inLatch[4].Initialise ( )"
				<< reset << flush;
			inLatch[4].Initialise ( );
		}
	}
	
	for ( int i = 0; i < 5; i++ )
	{
		outLatch[i].Initialise ( );
		flushStage[i] = false;
	}
	// The above is required because latchcopy from inlatch to outlatch
	// happens only once the stage thread got a chance to run
	
	if ( continueCount > 0 )
	{
		continueCount --;
		for ( int i = 0; i < BREAKPOINTARRAYSIZE; i++ )
		{
			if ( PCreg == breakPointArray[i] )
			{
				continueCount = 0;
				break;
			}
		}
	}
	else if ( continueCount < 0 )
	{
		cout << red << "\n[** Clock: " << clk 
			<< " **] negative continue count, resetting" 
			<< reset << flush;
		continueCount = 0;
	}
	
	cout << "\n" << flush;
	
	if ( continueCount == 0 )
	{
		char ch;
		do
		{
			cout << blue << "\nmips > " << reset << flush;
			cin >> ch;
			switch ( ch )
			{
			case 'p':	// small p
				cout << blue << "\nRegister Values are printed below." 
					<< reset << flush;
				for ( int i = 0; i < 32 ; i++ )
				{
					cout << "\n  r" << setw (2) << i << ": value = " 
						<< reg[i] << flush;
				}
				cout << "\n  Lo : value = " << Lo << flush;
				cout << "\n  Hi : value = " << Hi << flush;
				cout << "\n  PC : value = " << PCreg << flush;
				break;

			case 'P':	// capital P
				cout << blue << "\nNon-zero Register Values"
					<< " are printed below."
					<< reset << flush;
				for ( int i = 0; i < 32 ; i++ )
				{
					if ( reg[i]!= 0 )
						cout << "\n  r" << setw (2) << i 
							<< ": value = " 
							<< reg[i] << flush;
				}
				if ( Lo != 0 ) cout << "\n  Lo : value = " << Lo << flush;
				if ( Hi != 0 ) cout << "\n  Hi : value = " << Hi << flush;
				cout << "\n  PC : value = " << PCreg << flush;	
						// PC is always printed
				break;

			case 'q':
				Terminate ( );
				//requestProgramTermination = true;

			case 'h':
			case '?':
				cout << blue
					<< "\nCommands:"
					<< "\n  h or ?       show this help"
					<< "\n  n            execute one clock cycle"
					<< "\n  c <number>   execute <number> clock cycles"
					<< "\n  p            print all registers"
					<< "\n  P            print non-zero registers"
					<< "\n  m <addr>     display memory word at <addr>"
					<< "\n  d <addr>     display data cache word at <addr>"
					<< "\n  i <addr>     display instruction cache word at <addr>"
					<< "\n  s            display cache statistics"
					<< "\n  b <i> <addr> set breakpoint i (0-15) to <addr> (use -1 to clear)"
					<< "\n  B            list breakpoints"
					<< "\n  q            quit"
					<< reset << flush;
				break;

			case 'n':
				break;	// Do nothing... Avoid going into "default"

			case 'm': {
				int address;
				cin >> address;

				word_32 result;
				if ( mem -> Read ( address, result, 4 ) == true )
					cout << blue << "\nMemory[" << address << "] = " 
						<< result << " " 
						<< static_cast<char>(result) 
						<< reset << flush;
				else cout << red << "\nMemory read failed" << reset << flush;

				break;
				}

			case 'd': {
				int address;
				cin >> address;

				word_32 result;
				if ( dataCache -> Read_nofetch ( address, result, 4 ) 
						== true )
					cout << blue << "\ndataCache[" << address << "] = " 
						<< result << reset << flush;
				else cout << red << "\ndataCache read missed" 
					<< reset << flush;

				break;
				}

			case 'i': {
				int address;
				cin >> address;

				word_32 result;
				if ( instrCache -> Read_nofetch ( address, result, 4 ) 
						== true )
					cout << blue << "\ninstrCache[" << address << "] = " 
						<< result << reset << flush;
				else cout << red << "\ninstrCache read missed" 
					<< reset << flush;

				break;
				}

			case 's':
				cout << blue << "\ndataCache Statistics : " 
					<< reset << flush;
				dataCache -> Statistics ( );
				cout << blue << "\ninstrCache Statistics : " 
					<< reset << flush;
				instrCache -> Statistics ( );
				break;
				
			case 'c': cin >> continueCount;
				break;
				
			case 'b':{
				int breakPointIndex;
				cin >> breakPointIndex;
				cin >> breakPointArray[breakPointIndex];
				break;
				}
				
			case 'B':
				cout << blue << "\n[** Clock: " << clk 
					<< " **] The break points are listed below\n"
					<< reset << flush;
				for ( int i = 0; i < BREAKPOINTARRAYSIZE; i ++ )
					if ( breakPointArray[i] != -1 )
						cout << "\t" << i << "\t" 
							<< breakPointArray[i] << "\n";
				cout << flush;
				break;

			default:
				cout << red << "\n[** Clock: " << clk 
					<< " **] Unrecognised command,"
					<< " Ignoring... " << reset << flush;
				continueCount = 0;
				break;
			};
		}
		while ( ch != 'q' && ch != 'n' && ch != 'c' );
	}
}
