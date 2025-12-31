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
using std::flush;

# include "../include/color.h"

# include <semaphore.h>
extern sem_t * cout_mutex;	// Defined in main.cpp

void Processor :: Stage0 ( )
{
	LatchCopy ( outLatch[0], inLatch[0] ); 
	// Copy inLatch into outLatch... NowForth work with outLatch
	// Note the invariant that inLatch[0] is a constant for all practical 
	// purposes.
	
	if ( instrCache -> Read ( PCreg, outLatch[0].inst.iV, 4 ) == true )
	{
		outLatch[0].PC = PCreg;
	
		PC_update_control ( PCreg + 4, 0 );
	
		outLatch[0].finished = true;
		sem_wait ( cout_mutex );
		cout << "\n[ Stage0 ] PC to fetch = " << PCreg << ", Instruction = " 
			<< outLatch[0].inst.iV << flush;
		sem_post ( cout_mutex );
	}
	else
	{
		outLatch[0].finished = false;
		sem_wait ( cout_mutex );
		cout << red << "\n[ Stage0 ] PC fetch failed, will try again in next clock"
			<< reset << flush;
		sem_post ( cout_mutex );
	}
}

void Processor :: PC_update_control ( word_32 value, int stage )
{
	sem_wait ( pc_mutex );
	
	switch ( stage )
	{
	case 2:
		NPCreg = value;
		NPCfrom = PC_STAGE2;
		sem_wait ( cout_mutex );
		cout << skyblue << "\n[__ PC_update_control __] NPC updated by stage 2 to "
			<< reset << NPCreg << flush;
		sem_post ( cout_mutex );
		break;
	case 1:
		if ( NPCfrom == PC_STAGE2 )
		{
			sem_wait ( cout_mutex );
			cout << skyblue << "\n[__ PC_update_control __]"
				<< " NPC update by stage 1 aborted"
				<< reset << flush;
			sem_post ( cout_mutex );
			break;
		}
		NPCreg = value;
		NPCfrom = PC_STAGE1;
		sem_wait ( cout_mutex );
		cout << skyblue << "\n[__ PC_update_control __] NPC updated by stage 1 to "
			<< reset << NPCreg << flush;
		sem_post ( cout_mutex );
		break;
	case 0:
		if ( NPCfrom == PC_STAGE2 || NPCfrom == PC_STAGE1 )
		{
			sem_wait ( cout_mutex );
			cout << skyblue << "\n[__ PC_update_control __]"
				<< " NPC update by stage 0 aborted"
				<< reset << flush;
			sem_post ( cout_mutex );
			break;
		}
		NPCreg = value;
		NPCfrom = PC_STAGE0;
		sem_wait ( cout_mutex );
		cout << skyblue << "\n[__ PC_update_control __] NPC updated by stage 0 to "
			<< reset << NPCreg << flush;
		sem_post ( cout_mutex );
		break;
	};
	
	sem_post ( pc_mutex );
}
