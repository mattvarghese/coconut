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

# ifndef __PROCESSOR_H
# define __PROCESSOR_H

# include "memory.h"
# include "portmanager.h"
# include "latch.h"
# include "../include/opcodes.h"

# include <pthread.h>
# include <semaphore.h>

# define SYSCALL_HANDLER_ADDRESS 0
# define SYSTEM_START_ADDRESS 1024
 
# define REG_HI 32
# define REG_LO 33

# define BREAKPOINTARRAYSIZE 16

class Processor
{
private:
	word_32 reg[32];
	word_32 Hi, Lo;		// numbers REG_HI & REG_LO will be used to 
				// address these registers
	
	u_word_32 PCreg;
	u_word_32 NPCreg;
	PCWritingStage NPCfrom;
	
	bool flushStage[5];
	
	MainMemory * mem;
	Cache * dataCache;
	Cache * instrCache;
	
	PortManager * pman;
	
	Latch inLatch [5];
	Latch outLatch [5];
	/** 
	 * Note that inLatch[0] and 
	 * outLatch[4] have rudimentary
	 * use.
	**/
	
	// NOTE: The following booleans are flags...
	bool blockUpdate;	// This is set when mem/reg updates should no
				// longer be allowed to happen.
	bool requestProgramTermination;

	/********************************************************
	 * Following are Threading related variables
	********************************************************/
	pthread_t stagethread[5];
	
	sem_t * clocksem;
	sem_t * stagesem[5];
	sem_t * pc_mutex;
	
	// The following variables are used for the stepping 
	// and controlling the execution of the user program
	// on our simulated processor.
	int continueCount;
	word_32 breakPointArray[BREAKPOINTARRAYSIZE];
public:
	//bool SingleStep;  // TODO
	//bool Pause;       // TODO

	Processor ( MainMemory * m, Cache * dc, Cache * ic, PortManager * pm );
	~Processor ( );  // calls AtExit (); note that this destructor is never invoked.
	void AtExit ( ); // Destroys the threads.
	
	void Terminate ( ); // Oversees Termination of program in case of error.
	
	void Execute ( ); // Creates the threads and starts ExecutionThread
	void ExecutionThread ( );
		// Manages the clock for the processor.
		// Executes the oneClock ( ) function within an infinite loop.
	void Clock ( int clk );
		// Manages transfering and setting up
		// the pipelineLatch objects
		// and gives the user control over the execution of the
		// user program on the simulated processor.
		
	bool RegisterFetch ( RegisterFetchTarget target, int regNumber, 
		bool noFail = false ); // if noFail = true, will not fail even
		// if the data will become available only in the next cycle,
		// because the instruction needs that data only in stage3,
		// i.e., the instruction is a sw/rdout/dout
	bool RegisterFetch_Stage2 ( RegisterFetchTarget target, int regNumber );
	// Abstracts away the details of data forwarding etc.
	bool RegisterWrite ( int regNumber, RegisterWriteSource source );
	// controls the writing of registers, prevents the modification of $zero
	
	// The foll two functions provide a simple abstraction for memory access.
	// also can be used to aid in preventing memory write when 
	// implementing hardware exceptions...
	bool ReadMem ( word_32 address, word_32 & result, int noOfBytes );
	bool WriteMem ( word_32 address, word_32 value, int noOfBytes );
	
	// Abstracts away the process of updating the NPC register.
	void UpdatePC_Stage1 ( word_32 value, PCUpdateType updateType );
	void UpdatePC_Stage2 ( word_32 value, PCUpdateType updateType );
	
	// Critical section in the above two functions...
	// Also used by stage 0 to update pc.
	void PC_update_control ( word_32 value, int stage );
	
	// Each of the following is spawned as different
	// threads by the constructor of this class.
	// The friend function is the entry point which calls the 
	// corresponding member function.
	friend void* stage0 ( void * );
	void Stage0 ( );
	
	friend void* stage1 ( void * );
	void Stage1 ( );
	
	friend void* stage2 ( void * );
	void Stage2 ( );
	
	friend void* stage3 ( void * );
	void Stage3 ( );
	
	friend void* stage4 ( void * );
	void Stage4 ( );
};

# endif
