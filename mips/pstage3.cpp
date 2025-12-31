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

void Processor :: Stage3 ( )
{
	// Copy inLatch into outLatch... NowForth work with outLatch
	LatchCopy ( outLatch[3], inLatch[3] );
	
	// Fisrt check for NOP
	if ( outLatch[3].inst.iV == 0 )
	{
		// Do Nothing 
		outLatch[3].finished = true;
		
		sem_wait ( cout_mutex );
		cout << "\n[ Stage3 ] NOP" << flush;
		sem_post ( cout_mutex );
	}
	else // else perform the switch code
	switch ( outLatch[3].inst.noF.op )
	{
	case OP_ZERO:
		switch ( outLatch[3].inst.rF.funct )
		{
		case FUNCT_ADD:
			outLatch[3].finished = true;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage3 ] ADD idle" << flush;
			sem_post ( cout_mutex );
			break;
			
		case FUNCT_AND:
			outLatch[3].finished = true;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage3 ] AND idle" << flush;
			sem_post ( cout_mutex );
			break;
			
		case FUNCT_DIV:
			outLatch[3].finished = true;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage3 ] DIV idle" << flush;
			sem_post ( cout_mutex );
			break;
			
		case FUNCT_MULT:
			outLatch[3].finished = true;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage3 ] MULT idle" << flush;
			sem_post ( cout_mutex );
			break;
			
		case FUNCT_NOR:
			outLatch[3].finished = true;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage3 ] NOR idle" << flush;
			sem_post ( cout_mutex );
			break;
			
		case FUNCT_OR:
			outLatch[3].finished = true;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage3 ] OR idle" << flush;
			sem_post ( cout_mutex );
			break;
			
		case FUNCT_SLL:
			outLatch[3].finished = true;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage3 ] SLL idle" << flush;
			sem_post ( cout_mutex );
			break;
			
		case FUNCT_SLLV:
			outLatch[3].finished = true;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage3 ] SLLV idle" << flush;
			sem_post ( cout_mutex );
			break;
			
		case FUNCT_SRA:
			outLatch[3].finished = true;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage3 ] SRA idle" << flush;
			sem_post ( cout_mutex );
			break;
			
		case FUNCT_SRAV:
			outLatch[3].finished = true;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage3 ] SRAV idle" << flush;
			sem_post ( cout_mutex );
			break;
			
		case FUNCT_SRL:
			outLatch[3].finished = true;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage3 ] SRL idle" << flush;
			sem_post ( cout_mutex );
			break;
			
		case FUNCT_SRLV:
			outLatch[3].finished = true;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage3 ] SRLV idle" << flush;
			sem_post ( cout_mutex );
			break;
			
		case FUNCT_SUB:
			outLatch[3].finished = true;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage3 ] SUB idle" << flush;
			sem_post ( cout_mutex );
			break;
			
		case FUNCT_XOR:
			outLatch[3].finished = true;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage3 ] XOR idle" << flush;
			sem_post ( cout_mutex );
			break;
			
		case FUNCT_SLT:
			outLatch[3].finished = true;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage3 ] SLT idle" << flush;
			sem_post ( cout_mutex );
			break;
			
		case FUNCT_JR:
			outLatch[3].finished = true;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage3 ] JR idle" << flush;
			sem_post ( cout_mutex );
			break;
			
		case FUNCT_JALR:
			outLatch[3].finished = true;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage3 ] JALR idle" << flush;
			sem_post ( cout_mutex );
			break;
			
		case FUNCT_MFHI:
			outLatch[3].finished = true;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage3 ] MFHI idle" << flush;
			sem_post ( cout_mutex );
			break;
			
		case FUNCT_MFLO:
			outLatch[3].finished = true;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage3 ] MFLO idle" << flush;
			sem_post ( cout_mutex );
			break;
			
		case FUNCT_MTHI:
			outLatch[3].finished = true;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage3 ] MTHI idle" << flush;
			sem_post ( cout_mutex );
			break;
			
		case FUNCT_MTLO:
			outLatch[3].finished = true;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage3 ] MTLO idle" << flush;
			sem_post ( cout_mutex );
			break;
			
		case FUNCT_SYSCALL:
			outLatch[3].finished = true;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage3 ] SYSCALL idle" << flush;
			sem_post ( cout_mutex );
			break;
			
		case FUNCT_RDIN:
			pman -> Read ( outLatch[3].B, outLatch[3].LMD );
			outLatch[3].finished = true;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage3 ] RDIN read value = " 
				<< outLatch[3].LMD << " from device "
				<< outLatch[3].B << flush;
			sem_post ( cout_mutex );
			break;
			
		case FUNCT_RDOUT: 
			pman -> Write ( outLatch[3].B, outLatch[3].A );
			outLatch[3].finished = true;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage3 ] RDOUT wrote value = " 
				<< outLatch[3].A << " to device "
				<< outLatch[3].B << flush;
			sem_post ( cout_mutex );
			break;
			
		};
		break;
		
	case OP_ONE:
		switch ( outLatch[3].inst.iF.rt )
		{
		case 1:	// The instruction is BGEZ
			outLatch[3].finished = true;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage3 ] BGEZ idle" << flush;
			sem_post ( cout_mutex );
			break;
		case 0:	// The instruction is BLTZ
			outLatch[3].finished = true;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage3 ] BLTZ idle" << flush;
			sem_post ( cout_mutex );
			break;
		};
		break;
		
	case OP_ADDI:
		outLatch[3].finished = true;
		
		sem_wait ( cout_mutex );
		cout << "\n[ Stage3 ] ADDI idle" << flush;
		sem_post ( cout_mutex );
		break;
		
	case OP_ANDI:
		outLatch[3].finished = true;
		
		sem_wait ( cout_mutex );
		cout << "\n[ Stage3 ] ANDI idle" << flush;
		sem_post ( cout_mutex );
		break;
		
	case OP_ORI:
		outLatch[3].finished = true;
		
		sem_wait ( cout_mutex );
		cout << "\n[ Stage3 ] ORI idle" << flush;
		sem_post ( cout_mutex );
		break;
		
	case OP_XORI:
		outLatch[3].finished = true;
		
		sem_wait ( cout_mutex );
		cout << "\n[ Stage3 ] XORI idle" << flush;
		sem_post ( cout_mutex );
		break;
		
	case OP_LUI:
		outLatch[3].finished = true;
		
		sem_wait ( cout_mutex );
		cout << "\n[ Stage3 ] LUI idle" << flush;
		sem_post ( cout_mutex );
		break;
		
	case OP_SLTI:
		outLatch[3].finished = true;
		
		sem_wait ( cout_mutex );
		cout << "\n[ Stage3 ] SLTI idle" << flush;
		sem_post ( cout_mutex );
		break;
		
	case OP_BEQ:
		outLatch[3].finished = true;
		
		sem_wait ( cout_mutex );
		cout << "\n[ Stage3 ] BEQ idle" << flush;
		sem_post ( cout_mutex );
		break;
		
	case OP_BGTZ:
		outLatch[3].finished = true;
		
		sem_wait ( cout_mutex );
		cout << "\n[ Stage3 ] BGTZ idle" << flush;
		sem_post ( cout_mutex );
		break;
		
	case OP_BLEZ:
		outLatch[3].finished = true;
		
		sem_wait ( cout_mutex );
		cout << "\n[ Stage3 ] BLEZ idle" << flush;
		sem_post ( cout_mutex );
		break;
		
	case OP_BNE:
		outLatch[3].finished = true;
		
		sem_wait ( cout_mutex );
		cout << "\n[ Stage3 ] BNE idle" << flush;
		sem_post ( cout_mutex );
		break;
		
	case OP_JAL:
		outLatch[3].finished = true;
		
		sem_wait ( cout_mutex );
		cout << "\n[ Stage3 ] JAL idle" << flush;
		sem_post ( cout_mutex );
		break;
		
	case OP_J:
		outLatch[3].finished = true;
		
		sem_wait ( cout_mutex );
		cout << "\n[ Stage3 ] J idle" << flush;
		sem_post ( cout_mutex );
		break;
	
	case OP_LW:
		if ( ReadMem( outLatch[3].ALUOutput, outLatch[3].LMD, 4 ) == true )
		{
			outLatch[3].finished = true;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage3 ] LW read value = " << outLatch[3].LMD 
				<< " from address " << outLatch[3].ALUOutput << flush;
			sem_post ( cout_mutex );
		}
		else 
		{
			outLatch[3].finished = false;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage3 ] LW read failed" << flush;
			sem_post ( cout_mutex );
		}
		break;
		
	case OP_SW:
		if ( WriteMem ( outLatch[3].ALUOutput, outLatch[3].B, 4 ) == true )
		{
			outLatch[3].finished = true;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage3 ] SW wrote value = " << outLatch[3].B 
				<< " to address " << outLatch[3].ALUOutput << flush;
			sem_post ( cout_mutex );
		}
		else
		{
			outLatch[3].finished = false;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage3 ] SW write failed" << flush;
			sem_post ( cout_mutex );
		}
		break;
		
	case OP_DIN:
		pman -> Read ( outLatch[3].Imm, outLatch[3].LMD );
		outLatch[3].finished = true;
		
		sem_wait ( cout_mutex );
		cout << "\n[ Stage3 ] DIN read value = " 
			<< outLatch[3].LMD << " from device "
			<< outLatch[3].Imm << flush;
		sem_post ( cout_mutex );
		break;
		
	case OP_DOUT:
		pman -> Write ( outLatch[3].Imm, outLatch[3].A );
		outLatch[3].finished = true;
		
		sem_wait ( cout_mutex );
		cout << "\n[ Stage3 ] DOUT wrote value = " 
			<< outLatch[3].A << " to device "
			<< outLatch[3].Imm << flush;
		sem_post ( cout_mutex );
		break;
		
	};
}


bool Processor :: ReadMem ( word_32 address, word_32 & result, int noOfBytes )
{
	return dataCache -> Read ( address, result, noOfBytes );
}

bool Processor :: WriteMem ( word_32 address, word_32 value, int noOfBytes )
{
	if ( blockUpdate == true ) return true;
	// Return telling the processor that memory was updated but
	// actually do not update memory if blockUpdate is true;
	return dataCache -> Write ( address, value, noOfBytes );
}
