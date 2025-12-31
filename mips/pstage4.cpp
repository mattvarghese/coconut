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
using std::cout;
using std::flush;

# include "../include/color.h"

# include <semaphore.h>
extern sem_t * cout_mutex;	// Defined in main.cpp

void Processor :: Stage4 ( )
{
	// Copy inLatch into outLatch... NowForth work with outLatch
	LatchCopy (outLatch[4], inLatch[4]);
	
	// Fisrt check for NOP
	if ( outLatch[4].inst.iV == 0 )
	{
		// Do Nothing 
		outLatch[4].finished = true;
		
		sem_wait ( cout_mutex );
		cout << "\n[ Stage4 ] NOP" << flush;
		sem_post ( cout_mutex );
	}
	else // else perform the switch code
	switch ( outLatch[4].inst.noF.op )
	{
	case OP_ZERO:
		switch ( outLatch[4].inst.rF.funct )
		{
		case FUNCT_ADD:
			RegisterWrite ( outLatch[4].targReg, ALU );
			outLatch[4].finished = true;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage4 ] ADD stored " << outLatch[4].ALUOutput
				<< " into r" << outLatch[4].targReg << flush;
			sem_post ( cout_mutex );
			break;
			
		case FUNCT_AND:
			RegisterWrite ( outLatch[4].targReg, ALU );
			outLatch[4].finished = true;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage4 ] AND stored " << outLatch[4].ALUOutput
				<< " into r" << outLatch[4].targReg << flush;
			sem_post ( cout_mutex );
			break;
			
		case FUNCT_DIV:
			RegisterWrite ( outLatch[4].targReg, ALU );
			RegisterWrite ( outLatch[4].targReg2, ALU_HI );
			outLatch[4].finished = true;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage4 ] DIV stored " << outLatch[4].ALUOutput
				<< " into Lo" << flush;
			cout << "\n[ Stage4 ] DIV stored " << outLatch[4].ALUOutputHi
				<< " into Hi" << flush;
			sem_post ( cout_mutex );
			break;
			
		case FUNCT_MULT:
			RegisterWrite ( outLatch[4].targReg, ALU );
			RegisterWrite ( outLatch[4].targReg2, ALU_HI );
			outLatch[4].finished = true;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage4 ] MULT stored " << outLatch[4].ALUOutput
				<< " into Lo" << flush;
			cout << "\n[ Stage4 ] MULT stored " << outLatch[4].ALUOutputHi
				<< " into Hi" << flush;
			sem_post ( cout_mutex );
			break;
			
		case FUNCT_NOR:
			RegisterWrite ( outLatch[4].targReg, ALU );
			outLatch[4].finished = true;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage4 ] NOR stored " << outLatch[4].ALUOutput
				<< " into r" << outLatch[4].targReg << flush;
			sem_post ( cout_mutex );
			break;
			
		case FUNCT_OR:
			RegisterWrite ( outLatch[4].targReg, ALU );
			outLatch[4].finished = true;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage4 ] OR stored " << outLatch[4].ALUOutput
				<< " into r" << outLatch[4].targReg << flush;
			sem_post ( cout_mutex );
			break;
			
		case FUNCT_SLL:
			RegisterWrite ( outLatch[4].targReg, ALU );
			outLatch[4].finished = true;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage4 ] SLL stored " << outLatch[4].ALUOutput
				<< " into r" << outLatch[4].targReg << flush;
			sem_post ( cout_mutex );
			break;
			
		case FUNCT_SLLV:
			RegisterWrite ( outLatch[4].targReg, ALU );
			outLatch[4].finished = true;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage4 ] SLLV stored " << outLatch[4].ALUOutput
				<< " into r" << outLatch[4].targReg << flush;
			sem_post ( cout_mutex );
			break;
			
		case FUNCT_SRA:
			RegisterWrite ( outLatch[4].targReg, ALU );
			outLatch[4].finished = true;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage4 ] SRA stored " << outLatch[4].ALUOutput
				<< " into r" << outLatch[4].targReg << flush;
			sem_post ( cout_mutex );
			break;
			
		case FUNCT_SRAV:
			RegisterWrite ( outLatch[4].targReg, ALU );
			outLatch[4].finished = true;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage4 ] SRAV stored " << outLatch[4].ALUOutput
				<< " into r" << outLatch[4].targReg << flush;
			sem_post ( cout_mutex );
			break;
			
		case FUNCT_SRL:
			RegisterWrite ( outLatch[4].targReg, ALU );
			outLatch[4].finished = true;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage4 ] SRL stored " << outLatch[4].ALUOutput
				<< " into r" << outLatch[4].targReg << flush;
			sem_post ( cout_mutex );
			break;
			
		case FUNCT_SRLV:
			RegisterWrite ( outLatch[4].targReg, ALU );
			outLatch[4].finished = true;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage4 ] SRLV stored " << outLatch[4].ALUOutput
				<< " into r" << outLatch[4].targReg << flush;
			sem_post ( cout_mutex );
			break;
			
		case FUNCT_SUB:
			RegisterWrite ( outLatch[4].targReg, ALU );
			outLatch[4].finished = true;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage4 ] SUB stored " << outLatch[4].ALUOutput
				<< " into r" << outLatch[4].targReg << flush;
			sem_post ( cout_mutex );
			break;
			
		case FUNCT_XOR:
			RegisterWrite ( outLatch[4].targReg, ALU );
			outLatch[4].finished = true;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage4 ] XOR stored " << outLatch[4].ALUOutput
				<< " into r" << outLatch[4].targReg << flush;
			sem_post ( cout_mutex );
			break;
			
		case FUNCT_SLT:
			RegisterWrite ( outLatch[4].targReg, ALU );
			outLatch[4].finished = true;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage4 ] SLT stored " << outLatch[4].ALUOutput
				<< " into r" << outLatch[4].targReg << flush;
			sem_post ( cout_mutex );
			break;
		
		case FUNCT_JR:
			outLatch[4].finished = true;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage4 ] JR idle" << flush;
			sem_post ( cout_mutex );
			break;
			
		case FUNCT_JALR:
			RegisterWrite ( outLatch[4].targReg, IDRES );
			outLatch[4].finished = true;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage4 ] JALR stored " << outLatch[4].IDRes
				<< " into r" << outLatch[4].targReg << flush;
			sem_post ( cout_mutex );
			break;
			
		case FUNCT_MFHI:
			RegisterWrite ( outLatch[4].targReg, IDRES );
			outLatch[4].finished = true;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage4 ] MFHI stored " << outLatch[4].IDRes
				<< " into r" << outLatch[4].targReg << flush;
			sem_post ( cout_mutex );
			break;
			
		case FUNCT_MFLO:
			RegisterWrite ( outLatch[4].targReg, IDRES );
			outLatch[4].finished = true;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage4 ] MFLO stored " << outLatch[4].IDRes
				<< " into r" << outLatch[4].targReg << flush;
			sem_post ( cout_mutex );
			break;
			
		case FUNCT_MTHI:
			RegisterWrite ( outLatch[4].targReg, IDRES );
			outLatch[4].finished = true;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage4 ] MTHI stored " << outLatch[4].IDRes
				<< " into r" << outLatch[4].targReg << flush;
			sem_post ( cout_mutex );
			break;
			
		case FUNCT_MTLO:
			RegisterWrite ( outLatch[4].targReg, IDRES );
			outLatch[4].finished = true;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage4 ] MTLO stored " << outLatch[4].IDRes
				<< " into r" << outLatch[4].targReg << flush;
			sem_post ( cout_mutex );
			break;
			
		case FUNCT_SYSCALL:
			RegisterWrite ( outLatch[4].targReg, IDRES );
			outLatch[4].finished = true;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage4 ] SYSCALL stored " << outLatch[4].IDRes
				<< " into r" << outLatch[4].targReg << flush;
			sem_post ( cout_mutex );
			break;
		
		case FUNCT_RDIN:
			RegisterWrite ( outLatch[4].targReg, LOAD );
			outLatch[4].finished = true;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage4 ] RDIN stored " << outLatch[4].LMD 
				<< " into r" << outLatch[4].targReg << flush;
			sem_post ( cout_mutex );
			break;
			
		case FUNCT_RDOUT:
			outLatch[4].finished = true;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage4 ] RDOUT idle" << flush;
			sem_post ( cout_mutex );
			break;
			
		};
		break;
		
	case OP_ONE:
		switch ( outLatch[4].inst.iF.rt )
		{
		case 1:	// The instruction is BGEZ
			outLatch[4].finished = true;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage4 ] BGEZ idle" << flush;
			sem_post ( cout_mutex );
			break;
		case 0:	// The instruction is BLTZ
			outLatch[4].finished = true;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage4 ] BLTZ idle" << flush;
			sem_post ( cout_mutex );
			break;
		};
		break;
		
	case OP_ADDI:
		RegisterWrite ( outLatch[4].targReg, ALU );
		outLatch[4].finished = true;
		
		sem_wait ( cout_mutex );
		cout << "\n[ Stage4 ] ADDI stored " << outLatch[4].ALUOutput
			<< " into r" << outLatch[4].targReg << flush;
		sem_post ( cout_mutex );
		break;
		
	case OP_ANDI:
		RegisterWrite ( outLatch[4].targReg, ALU );
		outLatch[4].finished = true;
		
		sem_wait ( cout_mutex );
		cout << "\n[ Stage4 ] ANDI stored " << outLatch[4].ALUOutput
			<< " into r" << outLatch[4].targReg << flush;
		sem_post ( cout_mutex );
		break;
		
	case OP_ORI:
		RegisterWrite ( outLatch[4].targReg, ALU );
		outLatch[4].finished = true;
		
		sem_wait ( cout_mutex );
		cout << "\n[ Stage4 ] ORI stored " << outLatch[4].ALUOutput
			<< " into r" << outLatch[4].targReg << flush;
		sem_post ( cout_mutex );
		break;
		
	case OP_XORI:
		RegisterWrite ( outLatch[4].targReg, ALU );
		outLatch[4].finished = true;
		
		sem_wait ( cout_mutex );
		cout << "\n[ Stage4 ] XORI stored " << outLatch[4].ALUOutput
			<< " into r" << outLatch[4].targReg << flush;
		sem_post ( cout_mutex );
		break;
		
	case OP_LUI:
		RegisterWrite ( outLatch[4].targReg, ALU );
		outLatch[4].finished = true;
		
		sem_wait ( cout_mutex );
		cout << "\n[ Stage4 ] LUI stored " << outLatch[4].ALUOutput
			<< " into r" << outLatch[4].targReg << flush;
		sem_post ( cout_mutex );
		break;
		
	case OP_SLTI:
		RegisterWrite ( outLatch[4].targReg, ALU );
		outLatch[4].finished = true;
		
		sem_wait ( cout_mutex );
		cout << "\n[ Stage4 ] SLTI stored " << outLatch[4].ALUOutput
			<< " into r" << outLatch[4].targReg << flush;
		sem_post ( cout_mutex );
		break;
		
	case OP_BEQ:
		outLatch[4].finished = true;
		
		sem_wait ( cout_mutex );
		cout << "\n[ Stage4 ] BEQ idle" << flush;
		sem_post ( cout_mutex );
		break;
		
	case OP_BGTZ:
		outLatch[4].finished = true;
		
		sem_wait ( cout_mutex );
		cout << "\n[ Stage4 ] BGTZ idle" << flush;
		sem_post ( cout_mutex );
		break;
		
	case OP_BLEZ:
		outLatch[4].finished = true;
		
		sem_wait ( cout_mutex );
		cout << "\n[ Stage4 ] BLEZ idle" << flush;
		sem_post ( cout_mutex );
		break;
		
	case OP_BNE:
		outLatch[4].finished = true;
		
		sem_wait ( cout_mutex );
		cout << "\n[ Stage4 ] BNE idle" << flush;
		sem_post ( cout_mutex );
		break;
		
	case OP_J:
		outLatch[4].finished = true;
		
		sem_wait ( cout_mutex );
		cout << "\n[ Stage4 ] J idle" << flush;
		sem_post ( cout_mutex );
		break;
		
	case OP_JAL:
		RegisterWrite ( outLatch[4].targReg, IDRES );
		outLatch[4].finished = true;
		
		sem_wait ( cout_mutex );
		cout << "\n[ Stage4 ] JAL stored " << outLatch[4].IDRes
			<< " into r" << outLatch[4].targReg << flush;
		sem_post ( cout_mutex );
		break;
		
	case OP_LW:
		RegisterWrite ( outLatch[4].targReg, LOAD );
		outLatch[4].finished = true;
		
		sem_wait ( cout_mutex );
		cout << "\n[ Stage4 ] LW stored " << outLatch[4].LMD 
			<< " into r" << outLatch[4].targReg << flush;
		sem_post ( cout_mutex );
		break;
		
	case OP_SW:
		outLatch[4].finished = true;
		
		sem_wait ( cout_mutex );
		cout << "\n[ Stage4 ] SW idle" << flush;
		sem_post ( cout_mutex );
		break;
		
	case OP_DIN:
		RegisterWrite ( outLatch[4].targReg, LOAD );
		outLatch[4].finished = true;
		
		sem_wait ( cout_mutex );
		cout << "\n[ Stage4 ] DIN stored " << outLatch[4].LMD 
			<< " into r" << outLatch[4].targReg << flush;
		sem_post ( cout_mutex );
		break;
		
	case OP_DOUT:
		outLatch[4].finished = true;
		
		sem_wait ( cout_mutex );
		cout << "\n[ Stage4 ] DOUT idle" << flush;
		sem_post ( cout_mutex );
		break;
		
	};
}

bool Processor :: RegisterWrite ( int regNumber, RegisterWriteSource source )
{
	if ( blockUpdate == true ) return false;
	
	word_32 writeValue;
	switch ( source )
	{
	case IDRES:
		writeValue = inLatch[4].IDRes;
		break;
	case ALU:
		writeValue = inLatch[4].ALUOutput;
		break;
	case ALU_HI:
		writeValue = inLatch[4].ALUOutputHi;
		break;
	case LOAD:
		writeValue = inLatch[4].LMD;
		break;
	};
	
	switch ( regNumber )
	{
	case 0: // $zero
		sem_wait ( cout_mutex );
		cout << red << "\n[ Stage4:RegisterWrite ] instruction attempting"
			<< " to modify $zero, ignoring..." << reset << flush;
		sem_post ( cout_mutex );
		return false;		// This register cannot be modified.
	case REG_LO:
		sem_wait ( cout_mutex );
		cout << violet << "\n[ Stage4:RegisterWrite ] writing value "
			<< writeValue << " to register Lo" << reset << flush;
		sem_post ( cout_mutex );
		Lo = writeValue;
		break;
	case REG_HI:
		sem_wait ( cout_mutex );
		cout << violet << "\n[ Stage4:RegisterWrite ] writing value "
			<< writeValue << " to register Hi" << reset << flush;
		sem_post ( cout_mutex );
		Hi = writeValue;
		break;
	default:
		sem_wait ( cout_mutex );
		cout << violet << "\n[ Stage4:RegisterWrite ] writing value "
			<< writeValue << " to register r"
			<< regNumber << reset << flush;
		sem_post ( cout_mutex );
		reg[regNumber] = writeValue;
		break;
	}
	return true;
}

