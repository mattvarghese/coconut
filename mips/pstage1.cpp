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

# include <pthread.h>
# include <iostream>
using std::cout;
using std::flush;

# include "../include/color.h"

# include <semaphore.h>
extern sem_t * cout_mutex;	// Defined in main.cpp

void Processor :: Stage1 ( )
{
	// Copy inLatch into outLatch... NowForth work with outLatch
	LatchCopy ( outLatch[1], inLatch[1] );
	
	// Fisrt check for NOP
	if ( outLatch[1].inst.iV == 0 )
	{
		// Do Nothing 
		outLatch[1].finished = true;
		
		sem_wait ( cout_mutex );
		cout << "\n[ Stage1 ] NOP" << flush;
		sem_post ( cout_mutex );
	}
	else // else perform the switch code
	switch ( outLatch[1].inst.noF.op )
	{
	case OP_ZERO:
		switch ( outLatch[1].inst.rF.funct )
		{
		case FUNCT_ADD:
			outLatch[1].resultStage = RESULT_AT_EX;
			
			if ( RegisterFetch ( ALU_A, outLatch[1].inst.rF.rs )
					&& RegisterFetch ( ALU_B, outLatch[1].inst.rF.rt ) )
				outLatch[1].finished = true;
			else outLatch[1].finished = false;	// Not necessary.
			
			outLatch[1].targReg = outLatch[1].inst.rF.rd;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage1 ] ADD r" << outLatch[1].inst.rF.rs << " + r"
				<< outLatch[1].inst.rF.rt << " -> r"
				<< outLatch[1].inst.rF.rd << flush;
			sem_post ( cout_mutex );
			break;
			
		case FUNCT_AND:
			outLatch[1].resultStage = RESULT_AT_EX;
			
			if ( RegisterFetch ( ALU_A, outLatch[1].inst.rF.rs )
					&& RegisterFetch ( ALU_B, outLatch[1].inst.rF.rt ) )
				outLatch[1].finished = true;
			else outLatch[1].finished = false;	// Not necessary.
			
			outLatch[1].targReg = outLatch[1].inst.rF.rd;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage1 ] AND r" << outLatch[1].inst.rF.rs << " & r"
				<< outLatch[1].inst.rF.rt << " -> r"
				<< outLatch[1].inst.rF.rd << flush;
			sem_post ( cout_mutex );
			break;
			
		case FUNCT_DIV:
			outLatch[1].resultStage = RESULT_AT_EX;
			
			if ( RegisterFetch ( ALU_A, outLatch[1].inst.rF.rs )
					&& RegisterFetch ( ALU_B, outLatch[1].inst.rF.rt ) )
				outLatch[1].finished = true;
			else outLatch[1].finished = false;	// Not necessary.
			
			outLatch[1].targReg = REG_LO;
			outLatch[1].targReg2 = REG_HI;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage1 ] DIV r" << outLatch[1].inst.rF.rs << " / r"
				<< outLatch[1].inst.rF.rt << " -> Hi-Lo" << flush;
			sem_post ( cout_mutex );
			break;
			
		case FUNCT_MULT:
			outLatch[1].resultStage = RESULT_AT_EX;
			
			if ( RegisterFetch ( ALU_A, outLatch[1].inst.rF.rs )
					&& RegisterFetch ( ALU_B, outLatch[1].inst.rF.rt ) )
				outLatch[1].finished = true;
			else outLatch[1].finished = false;	// Not necessary.
			
			outLatch[1].targReg = REG_LO;
			outLatch[1].targReg2 = REG_HI;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage1 ] MULT r" << outLatch[1].inst.rF.rs << " * r"
				<< outLatch[1].inst.rF.rt << " -> Hi-Lo" << flush;
			sem_post ( cout_mutex );
			break;
			
		case FUNCT_NOR:
			outLatch[1].resultStage = RESULT_AT_EX;
			
			if ( RegisterFetch ( ALU_A, outLatch[1].inst.rF.rs )
					&& RegisterFetch ( ALU_B, outLatch[1].inst.rF.rt ) )
				outLatch[1].finished = true;
			else outLatch[1].finished = false;	// Not necessary.
			
			outLatch[1].targReg = outLatch[1].inst.rF.rd;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage1 ] NOR r" << outLatch[1].inst.rF.rs << " nor r"
				<< outLatch[1].inst.rF.rt << " -> r"
				<< outLatch[1].inst.rF.rd << flush;
			sem_post ( cout_mutex );
			break;
			
		case FUNCT_OR:
			outLatch[1].resultStage = RESULT_AT_EX;
			
			if ( RegisterFetch ( ALU_A, outLatch[1].inst.rF.rs )
					&& RegisterFetch ( ALU_B, outLatch[1].inst.rF.rt ) )
				outLatch[1].finished = true;
			else outLatch[1].finished = false;	// Not necessary.
			
			outLatch[1].targReg = outLatch[1].inst.rF.rd;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage1 ] OR r" << outLatch[1].inst.rF.rs << " or r"
				<< outLatch[1].inst.rF.rt << " -> r"
				<< outLatch[1].inst.rF.rd << flush;
			sem_post ( cout_mutex );
			break;
			
		case FUNCT_SLL:
			outLatch[1].resultStage = RESULT_AT_EX;
			
			outLatch[1].A = outLatch[1].inst.rF.shamt;
			if ( RegisterFetch ( ALU_B, outLatch[1].inst.rF.rt ) == true )
				outLatch[1].finished = true;
			else outLatch[1].finished = false;	// Not necessary.
			
			outLatch[1].targReg = outLatch[1].inst.rF.rd;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage1 ] SLL r" << outLatch[1].inst.rF.rt << " << shamt"
				<< outLatch[1].inst.rF.shamt << " -> r"
				<< outLatch[1].inst.rF.rd << flush;
			sem_post ( cout_mutex );
			break;
			
		case FUNCT_SLLV:
			outLatch[1].resultStage = RESULT_AT_EX;
			
			if ( RegisterFetch ( ALU_B, outLatch[1].inst.rF.rt )
					&& RegisterFetch ( ALU_A, outLatch[1].inst.rF.rs ) )
				outLatch[1].finished = true;
			else outLatch[1].finished = false;	// Not necessary.
			
			outLatch[1].targReg = outLatch[1].inst.rF.rd;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage1 ] SLLV r" << outLatch[1].inst.rF.rt << " << r"
				<< outLatch[1].inst.rF.rs << " -> r"
				<< outLatch[1].inst.rF.rd << flush;
			sem_post ( cout_mutex );
			break;
			
		case FUNCT_SRA:
			outLatch[1].resultStage = RESULT_AT_EX;
			
			outLatch[1].A = outLatch[1].inst.rF.shamt;
			if ( RegisterFetch ( ALU_B, outLatch[1].inst.rF.rt ) == true )
				outLatch[1].finished = true;
			else outLatch[1].finished = false;	// Not necessary.
			
			outLatch[1].targReg = outLatch[1].inst.rF.rd;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage1 ] SRA r" << outLatch[1].inst.rF.rt << " >> shamt"
				<< outLatch[1].inst.rF.shamt << " -> r"
				<< outLatch[1].inst.rF.rd << flush;
			sem_post ( cout_mutex );
			break;
			
		case FUNCT_SRAV:
			outLatch[1].resultStage = RESULT_AT_EX;
			
			if ( RegisterFetch ( ALU_B, outLatch[1].inst.rF.rt )
					&& RegisterFetch ( ALU_A, outLatch[1].inst.rF.rs ) )
				outLatch[1].finished = true;
			else outLatch[1].finished = false;	// Not necessary.
			
			outLatch[1].targReg = outLatch[1].inst.rF.rd;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage1 ] SRAV r" << outLatch[1].inst.rF.rt << " >> r"
				<< outLatch[1].inst.rF.rs << " -> r"
				<< outLatch[1].inst.rF.rd << flush;
			sem_post ( cout_mutex );
			break;
			
		case FUNCT_SRL:
			outLatch[1].resultStage = RESULT_AT_EX;
			
			outLatch[1].A = outLatch[1].inst.rF.shamt;
			if ( RegisterFetch ( ALU_B, outLatch[1].inst.rF.rt ) == true )
				outLatch[1].finished = true;
			else outLatch[1].finished = false;	// Not necessary.
			
			outLatch[1].targReg = outLatch[1].inst.rF.rd;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage1 ] SRL r" << outLatch[1].inst.rF.rt << " >> shamt"
				<< outLatch[1].inst.rF.shamt << " -> r"
				<< outLatch[1].inst.rF.rd << flush;
			sem_post ( cout_mutex );
			break;
			
		case FUNCT_SRLV:
			outLatch[1].resultStage = RESULT_AT_EX;
			
			if ( RegisterFetch ( ALU_B, outLatch[1].inst.rF.rt )
					&& RegisterFetch ( ALU_A, outLatch[1].inst.rF.rs ) )
				outLatch[1].finished = true;
			else outLatch[1].finished = false;	// Not necessary.
			
			outLatch[1].targReg = outLatch[1].inst.rF.rd;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage1 ] SRLV r" << outLatch[1].inst.rF.rt << " >> r"
				<< outLatch[1].inst.rF.rs << " -> r"
				<< outLatch[1].inst.rF.rd << flush;
			sem_post ( cout_mutex );
			break;
			
		case FUNCT_SUB:
			outLatch[1].resultStage = RESULT_AT_EX;
			
			if ( RegisterFetch ( ALU_A, outLatch[1].inst.rF.rs )
					&& RegisterFetch ( ALU_B, outLatch[1].inst.rF.rt ) )
				outLatch[1].finished = true;
			else outLatch[1].finished = false;	// Not necessary.
			
			outLatch[1].targReg = outLatch[1].inst.rF.rd;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage1 ] SUB r" << outLatch[1].inst.rF.rs << " - r"
				<< outLatch[1].inst.rF.rt << " -> r"
				<< outLatch[1].inst.rF.rd << flush;
			sem_post ( cout_mutex );
			break;
			
		case FUNCT_XOR:
			outLatch[1].resultStage = RESULT_AT_EX;
			
			if ( RegisterFetch ( ALU_A, outLatch[1].inst.rF.rs )
					&& RegisterFetch ( ALU_B, outLatch[1].inst.rF.rt ) )
				outLatch[1].finished = true;
			else outLatch[1].finished = false;	// Not necessary.
			
			outLatch[1].targReg = outLatch[1].inst.rF.rd;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage1 ] XOR r" << outLatch[1].inst.rF.rs << " xor r"
				<< outLatch[1].inst.rF.rt << " -> r"
				<< outLatch[1].inst.rF.rd << flush;
			sem_post ( cout_mutex );
			break;
			
		case FUNCT_SLT:
			outLatch[1].resultStage = RESULT_AT_EX;
			
			if ( RegisterFetch ( ALU_A, outLatch[1].inst.rF.rs )
					&& RegisterFetch ( ALU_B, outLatch[1].inst.rF.rt ) )
				outLatch[1].finished = true;
			else outLatch[1].finished = false;	// Not necessary.
			
			outLatch[1].targReg = outLatch[1].inst.rF.rd;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage1 ] SLT r" << outLatch[1].inst.rF.rs << " < r"
				<< outLatch[1].inst.rF.rt << " ? 1 : 0  -> r"
				<< outLatch[1].inst.rF.rd << flush;
			sem_post ( cout_mutex );
			break;
			
		case FUNCT_JR:
			outLatch[1].resultStage = NORESULT;
			
			if ( RegisterFetch ( ALU_A, outLatch[1].inst.rF.rs ) )
				outLatch[1].finished = true;
			else outLatch[1].finished = false;	// Not necessary.
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage1 ] JR to address in r" 
				<< outLatch[1].inst.rF.rs << flush;
			sem_post ( cout_mutex );
			break;
			
		case FUNCT_JALR:
			outLatch[1].resultStage = RESULT_AT_ID;
			
			outLatch[1].IDRes = outLatch[1].PC + 4;
			
			if ( RegisterFetch ( ALU_A, outLatch[1].inst.rF.rs ) )
				outLatch[1].finished = true;
			else outLatch[1].finished = false;	// Not necessary.
			
			outLatch[1].targReg = outLatch[1].inst.rF.rd;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage1 ] JALR to address in r" 
				<< outLatch[1].inst.rF.rs 
				<< ", return address in r"
				<< outLatch[1].inst.rF.rd << flush;
			sem_post ( cout_mutex );
			break;
			
		case FUNCT_MFHI:
			outLatch[1].resultStage = RESULT_AT_ID;
			
			if ( RegisterFetch ( IDRES_FT, REG_HI ) )
				outLatch[1].finished = true;
			else outLatch[1].finished = false;	// Not necessary.
			
			outLatch[1].targReg = outLatch[1].inst.rF.rd;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage1 ] MFHI Hi -> r" 
				<< outLatch[1].inst.rF.rd << flush;
			sem_post ( cout_mutex );
			break;
			
		case FUNCT_MFLO:
			outLatch[1].resultStage = RESULT_AT_ID;
			
			if ( RegisterFetch ( IDRES_FT, REG_LO ) )
				outLatch[1].finished = true;
			else outLatch[1].finished = false;	// Not necessary.
			
			outLatch[1].targReg = outLatch[1].inst.rF.rd;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage1 ] MFLO Lo -> r" 
				<< outLatch[1].inst.rF.rd << flush;
			sem_post ( cout_mutex );
			break;
			
		case FUNCT_MTHI:
			outLatch[1].resultStage = RESULT_AT_ID;
			
			if ( RegisterFetch ( IDRES_FT, outLatch[1].inst.rF.rs ) )
				outLatch[1].finished = true;
			else outLatch[1].finished = false;	// Not necessary.
			
			outLatch[1].targReg = REG_HI;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage1 ] MTHI r" << outLatch[1].inst.rF.rs
				<< " -> Hi" << flush;
			sem_post ( cout_mutex );
			break;
			
		case FUNCT_MTLO:
			outLatch[1].resultStage = RESULT_AT_ID;
			
			if ( RegisterFetch ( IDRES_FT, outLatch[1].inst.rF.rs ) )
				outLatch[1].finished = true;
			else outLatch[1].finished = false;	// Not necessary.
			
			outLatch[1].targReg = REG_LO;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage1 ] MTLO r" << outLatch[1].inst.rF.rs
				<< " -> Lo" << flush;
			sem_post ( cout_mutex );
			break;
			
		case FUNCT_SYSCALL:
			outLatch[1].resultStage = RESULT_AT_ID;
			
			outLatch[1].IDRes = outLatch[1].PC + 4;
			
			UpdatePC_Stage1 ( 0, PC_ABSOLUTE );
			
			outLatch[1].finished = true;
			
			outLatch[1].targReg = 31;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage1 ] SYSCALL jumping to address 0" 
				<< ", return address in r31" << flush;
			sem_post ( cout_mutex );
			break;
			
		case FUNCT_RDIN:
			outLatch[1].resultStage = RESULT_AT_MEM;
			
			if ( RegisterFetch ( ALU_B, outLatch[1].inst.rF.rt ) )
				outLatch[1].finished = true;
			else outLatch[1].finished = false;	// Not necessary.

			outLatch[1].targReg = outLatch[1].inst.rF.rd;

			sem_wait ( cout_mutex );
			cout << "\n[ Stage1 ] RDIN read from device in r"
				<< outLatch[1].inst.rF.rt << " to r"
				<< outLatch[1].inst.rF.rd << flush;
			sem_post ( cout_mutex );
			break;
		
		case FUNCT_RDOUT:
			outLatch[1].resultStage = NORESULT;
			
			// Even though both fetches are allowed to fail as noFail == true
			// for the RegisterFetch(), max one of them only will fail at any 
			// time as Stage3 produces atmost one result.
			if ( RegisterFetch ( ALU_B, outLatch[1].inst.rF.rt, true ) &&
				RegisterFetch ( ALU_A, outLatch[1].inst.rF.rs, true ) )
				outLatch[1].finished = true;
			else outLatch[1].finished = false;	// Not necessary.
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage1 ] RDOUT write to device in r"
				<< outLatch[1].inst.rF.rt << " from r"
				<< outLatch[1].inst.rF.rs << flush;
			sem_post ( cout_mutex );
			break;
			
		};
		break;
		
	case OP_ONE:
		switch ( outLatch[1].inst.iF.rt )
		{
		case 1:	// The instruction is BGEZ
			outLatch[1].resultStage = NORESULT;

			outLatch[1].Imm = outLatch[1].inst.iF.imm * 4;
			if ( RegisterFetch ( ALU_A, outLatch[1].inst.iF.rs ) )
			{
				if ( outLatch[1].A >= 0 )
				{
					UpdatePC_Stage1( outLatch[1].Imm, PC_RELATIVE);
					
					sem_wait ( cout_mutex );
					cout << "\n[ Stage1 ] BGEZ r" 
						<< outLatch[1].inst.iF.rs 
						<< " jumped to relative address " 
						<< outLatch[1].Imm << flush;
					sem_post ( cout_mutex );
				}
				else
				{
					sem_wait ( cout_mutex );
					cout << "\n[ Stage1 ] BGEZ r" 
						<< outLatch[1].inst.iF.rs 
						<< " did not jump to relative address " 
						<< outLatch[1].Imm << flush;
					sem_post ( cout_mutex );
				}

				outLatch[1].finished = true;

			}
			else
				outLatch[1].finished = false;	// Not necessary.
			break;
			
		case 0:	// The instruction is BLTZ
			outLatch[1].resultStage = NORESULT;

			outLatch[1].Imm = outLatch[1].inst.iF.imm * 4;
			if ( RegisterFetch ( ALU_A, outLatch[1].inst.iF.rs ) )
			{
				if ( outLatch[1].A < 0 )
				{
					UpdatePC_Stage1( outLatch[1].Imm, PC_RELATIVE);
					
					sem_wait ( cout_mutex );
					cout << "\n[ Stage1 ] BLTZ r" 
						<< outLatch[1].inst.iF.rs 
						<< " jumped to relative address " 
						<< outLatch[1].Imm << flush;
					sem_post ( cout_mutex );
				}
				else
				{
					sem_wait ( cout_mutex );
					cout << "\n[ Stage1 ] BLTZ r" 
						<< outLatch[1].inst.iF.rs 
						<< " did not jump to relative address " 
						<< outLatch[1].Imm << flush;
					sem_post ( cout_mutex );
				}

				outLatch[1].finished = true;

			}
			else
				outLatch[1].finished = false;	// Not necessary.
			break;
		};
		break;
		
	case OP_ADDI:
		outLatch[1].resultStage = RESULT_AT_EX;

		outLatch[1].Imm = outLatch[1].inst.iF.imm;
		if ( RegisterFetch ( ALU_A, outLatch[1].inst.iF.rs ) )
			outLatch[1].finished = true;
		else outLatch[1].finished = false;	// Not necessary.

		outLatch[1].targReg = outLatch[1].inst.iF.rt;

		sem_wait ( cout_mutex );
		cout << "\n[ Stage1 ] ADDI r" << outLatch[1].inst.iF.rs << " + imm "
			<< outLatch[1].inst.iF.imm << " -> r"
			<< outLatch[1].inst.iF.rt << flush;
		sem_post ( cout_mutex );
		break;
		
	case OP_ANDI:
		outLatch[1].resultStage = RESULT_AT_EX;

		outLatch[1].Imm = outLatch[1].inst.iF.imm;
		if ( RegisterFetch ( ALU_A, outLatch[1].inst.iF.rs ) )
			outLatch[1].finished = true;
		else outLatch[1].finished = false;	// Not necessary.

		outLatch[1].targReg = outLatch[1].inst.iF.rt;

		sem_wait ( cout_mutex );
		cout << "\n[ Stage1 ] ANDI r" << outLatch[1].inst.iF.rs << " & imm "
			<< outLatch[1].inst.iF.imm << " -> r"
			<< outLatch[1].inst.iF.rt << flush;
		sem_post ( cout_mutex );
		break;
		
	case OP_ORI:
		outLatch[1].resultStage = RESULT_AT_EX;

		outLatch[1].Imm = outLatch[1].inst.iF.imm;
		if ( RegisterFetch ( ALU_A, outLatch[1].inst.iF.rs ) )
			outLatch[1].finished = true;
		else outLatch[1].finished = false;	// Not necessary.

		outLatch[1].targReg = outLatch[1].inst.iF.rt;

		sem_wait ( cout_mutex );
		cout << "\n[ Stage1 ] ORI r" << outLatch[1].inst.iF.rs << " | imm "
			<< outLatch[1].inst.iF.imm << " -> r"
			<< outLatch[1].inst.iF.rt << flush;
		sem_post ( cout_mutex );
		break;
		
	case OP_XORI:
		outLatch[1].resultStage = RESULT_AT_EX;

		outLatch[1].Imm = outLatch[1].inst.iF.imm;
		if ( RegisterFetch ( ALU_A, outLatch[1].inst.iF.rs ) )
			outLatch[1].finished = true;
		else outLatch[1].finished = false;	// Not necessary.

		outLatch[1].targReg = outLatch[1].inst.iF.rt;

		sem_wait ( cout_mutex );
		cout << "\n[ Stage1 ] XORI r" << outLatch[1].inst.iF.rs << " xor imm "
			<< outLatch[1].inst.iF.imm << " -> r"
			<< outLatch[1].inst.iF.rt << flush;
		sem_post ( cout_mutex );
		break;
		
	case OP_LUI:
		outLatch[1].resultStage = RESULT_AT_EX;
		
		outLatch[1].Imm = outLatch[1].inst.iF.imm << 16;
		if ( RegisterFetch ( ALU_B, outLatch[1].inst.iF.rt ) )
			outLatch[1].finished = true;
		else outLatch[1].finished = false;	// Not necessary.
		
		outLatch[1].targReg = outLatch[1].inst.iF.rt;
		
		sem_wait ( cout_mutex );
		cout << "\n[ Stage1 ] LUI imm" << outLatch[1].inst.iF.imm
			<< " -> upper_16( r" << outLatch[1].inst.iF.rt
			<< " ) " << flush;
		sem_post ( cout_mutex );
			
		break;
	
	case OP_SLTI:
		outLatch[1].resultStage = RESULT_AT_EX;

		outLatch[1].Imm = outLatch[1].inst.iF.imm;
		if ( RegisterFetch ( ALU_A, outLatch[1].inst.iF.rs ) )
			outLatch[1].finished = true;
		else outLatch[1].finished = false;	// Not necessary.

		outLatch[1].targReg = outLatch[1].inst.iF.rt;

		sem_wait ( cout_mutex );
		cout << "\n[ Stage1 ] SLTI r" << outLatch[1].inst.iF.rs << " < imm "
			<< outLatch[1].inst.iF.imm << " ? 1 : 0  -> r"
			<< outLatch[1].inst.iF.rt << flush;
		sem_post ( cout_mutex );
		break;
	
	case OP_BEQ:
		outLatch[1].resultStage = NORESULT;

		outLatch[1].Imm = outLatch[1].inst.iF.imm * 4;
		if ( RegisterFetch ( ALU_A, outLatch[1].inst.iF.rs )
				&& RegisterFetch ( ALU_B, outLatch[1].inst.iF.rt ) )
			outLatch[1].finished = true;
		else outLatch[1].finished = false;	// Not necessary.

		sem_wait ( cout_mutex );
		cout << "\n[ Stage1 ] BEQ r" << outLatch[1].inst.iF.rs << ", r"
			<< outLatch[1].inst.iF.rt << " to relative address "
			<< outLatch[1].Imm << flush;
		sem_post ( cout_mutex );
		break;
	
	case OP_BGTZ:
		outLatch[1].resultStage = NORESULT;

		outLatch[1].Imm = outLatch[1].inst.iF.imm * 4;
		if ( RegisterFetch ( ALU_A, outLatch[1].inst.iF.rs ) )
		{
			if ( outLatch[1].A > 0 )
			{
				UpdatePC_Stage1( outLatch[1].Imm, PC_RELATIVE );
				
				sem_wait ( cout_mutex );
				cout << "\n[ Stage1 ] BGTZ r" << outLatch[1].inst.iF.rs 
					<< " jumped to relative address " 
					<< outLatch[1].Imm << flush;
				sem_post ( cout_mutex );
			}
			else
			{
				sem_wait ( cout_mutex );
				cout << "\n[ Stage1 ] BGTZ r" << outLatch[1].inst.iF.rs 
					<< " did not jump to relative address " 
					<< outLatch[1].Imm << flush;
				sem_post ( cout_mutex );
			}
		
			outLatch[1].finished = true;

		}
		else
			outLatch[1].finished = false;	// Not necessary.
		break;
	
	case OP_BLEZ:
		outLatch[1].resultStage = NORESULT;

		outLatch[1].Imm = outLatch[1].inst.iF.imm * 4;
		if ( RegisterFetch ( ALU_A, outLatch[1].inst.iF.rs ) )
		{
			if ( outLatch[1].A <= 0 )
			{
				UpdatePC_Stage1( outLatch[1].Imm, PC_RELATIVE );
				
				sem_wait ( cout_mutex );
				cout << "\n[ Stage1 ] BLEZ r" << outLatch[1].inst.iF.rs 
					<< " jumped to relative address " 
					<< outLatch[1].Imm << flush;
				sem_post ( cout_mutex );
			}
			else
			{
				sem_wait ( cout_mutex );
				cout << "\n[ Stage1 ] BLEZ r" << outLatch[1].inst.iF.rs 
					<< " did not jump to relative address " 
					<< outLatch[1].Imm << flush;
				sem_post ( cout_mutex );
			}
		
			outLatch[1].finished = true;

		}
		else
			outLatch[1].finished = false;	// Not necessary.
		break;
	
	case OP_BNE:
		outLatch[1].resultStage = NORESULT;

		outLatch[1].Imm = outLatch[1].inst.iF.imm * 4;
		if ( RegisterFetch ( ALU_A, outLatch[1].inst.iF.rs )
				&& RegisterFetch ( ALU_B, outLatch[1].inst.iF.rt ) )
			outLatch[1].finished = true;
		else outLatch[1].finished = false;	// Not necessary.

		sem_wait ( cout_mutex );
		cout << "\n[ Stage1 ] BNE r" << outLatch[1].inst.iF.rs << ", r"
			<< outLatch[1].inst.iF.rt << " to relative address "
			<< outLatch[1].Imm << flush;
		sem_post ( cout_mutex );
		break;
	
	case OP_J:
		outLatch[1].resultStage = NORESULT;
		
		outLatch[1].Imm = outLatch[1].inst.jF.tAddr * 4;
		UpdatePC_Stage1( outLatch[1].Imm, PC_ABSOLUTE );
		
		outLatch[1].finished = true;
		
		sem_wait ( cout_mutex );
		cout << "\n[ Stage1 ] J (jump) to absolute address "
			<< outLatch[1].Imm << flush;
		sem_post ( cout_mutex );
		break;
	
	case OP_JAL:
		outLatch[1].resultStage = RESULT_AT_ID;
		
		outLatch[1].Imm = outLatch[1].inst.jF.tAddr * 4;
		UpdatePC_Stage1( outLatch[1].Imm, PC_ABSOLUTE );
		
		outLatch[1].targReg = 31;
		outLatch[1].IDRes = outLatch[1].PC + 4;
		
		outLatch[1].finished = true;
		
		sem_wait ( cout_mutex );
		cout << "\n[ Stage1 ] JAL (jump) to absolute address "
			<< outLatch[1].Imm << flush;
		sem_post ( cout_mutex );
		break;
		
	case OP_LW:
		outLatch[1].resultStage = RESULT_AT_MEM;

		outLatch[1].Imm = outLatch[1].inst.iF.imm;
		if ( RegisterFetch ( ALU_A, outLatch[1].inst.iF.rs ) )
			outLatch[1].finished = true;
		else outLatch[1].finished = false;	// Not necessary.

		outLatch[1].targReg = outLatch[1].inst.iF.rt;

		sem_wait ( cout_mutex );
		cout << "\n[ Stage1 ] LW Memory[" << outLatch[1].inst.iF.imm
			<< " + (r"<< outLatch[1].inst.iF.rs << ")] -> r"
			<< outLatch[1].inst.iF.rt << flush;
		sem_post ( cout_mutex );
		break;
	
	case OP_SW:
		outLatch[1].resultStage = NORESULT;
		
		// Note that here rt contains the source and rs the destination
		// address to confirm to the same format as LW
		outLatch[1].Imm = outLatch[1].inst.iF.imm;
		
		// The Register fetch for rt may fail because we need the data only in 
		// Stage3, and so the data need only be forwarded in Stage2
		if ( RegisterFetch ( ALU_A, outLatch[1].inst.iF.rs ) &&
			RegisterFetch ( ALU_B, outLatch[1].inst.iF.rt, true ) )
			outLatch[1].finished = true;
		else outLatch[1].finished = false;	// Not necessary.
		
		sem_wait ( cout_mutex );
		cout << "\n[ Stage1 ] SW Memory[" << outLatch[1].inst.iF.imm
			<< " + (r"<< outLatch[1].inst.iF.rs << ")] <- r"
			<< outLatch[1].inst.iF.rt << flush;
		sem_post ( cout_mutex );
		break;
		
	case OP_DIN:
		outLatch[1].resultStage = RESULT_AT_MEM;
		
		outLatch[1].Imm = outLatch[1].inst.iF.imm;
		
		outLatch[1].finished = true;
		
		outLatch[1].targReg = outLatch[1].inst.iF.rt;
		
		sem_wait ( cout_mutex );
		cout << "\n[ Stage1 ] DIN read from device " 
			<< outLatch[1].inst.iF.imm << " to r"
			<< outLatch[1].inst.iF.rt << flush;
		sem_post ( cout_mutex );
		break;
		
	case OP_DOUT:
		outLatch[1].resultStage = NORESULT;
		
		outLatch[1].Imm = outLatch[1].inst.iF.imm;
		if ( RegisterFetch ( ALU_A, outLatch[1].inst.iF.rs, true ) )
			outLatch[1].finished = true;
		else outLatch[1].finished = false;	// Not necessary.
		
		sem_wait ( cout_mutex );
		cout << "\n[ Stage1 ] DOUT write to device "
			<< outLatch[1].inst.iF.imm << " contents of r"
			<< outLatch[1].inst.iF.rs << flush;
		sem_post ( cout_mutex );
		break;
		
	};
}

bool Processor :: RegisterFetch ( RegisterFetchTarget target, int regNumber, bool noFail )
{
	word_32 fetchResult;
	if ( regNumber == 0 )
	{
		sem_wait ( cout_mutex );
		cout << violet << "\n[ Stage1:RegisterFetch ] Register $zero is always 0" 
			<< reset << flush;
		sem_post ( cout_mutex );
		fetchResult = 0;
	}
	else if ( inLatch[2].targReg == regNumber && inLatch[2].resultStage == RESULT_AT_ID)
	{
		sem_wait ( cout_mutex );
		cout << violet << "\n[ Stage1:RegisterFetch ] Result from current EX-IDRes" 
			<< reset << flush;
		sem_post ( cout_mutex );
		fetchResult = inLatch[2].IDRes;
	}
	else if ( ( inLatch[2].targReg == regNumber || inLatch[2].targReg2 == regNumber )
			&& inLatch[2].resultStage == RESULT_AT_MEM )
	{
		sem_wait ( cout_mutex );
		cout << violet << "\n[ Stage1:RegisterFetch ] Result unavailable" 
			<< reset << flush;
		sem_post ( cout_mutex );
		if ( noFail == false )
			return false;	// Will only get result in next clock
		else 
		{
			sem_wait ( cout_mutex );
			cout << violet << "\n[ Stage1:RegisterFetch ] " 
				<< "will forward while in Stage2"
				<< reset << flush;
			sem_post ( cout_mutex );
			outLatch[1].dataFetchIncomplete = true;
			outLatch[1].FetchFailedFor = target;
			return true;
		}
	}
	else if ( ( inLatch[2].targReg == regNumber || inLatch[2].targReg2 == regNumber )
			&& inLatch[2].resultStage == RESULT_AT_EX )
	{
		// Have to wait till result has been computed
		while ( outLatch[2].finished == false )
			sched_yield ( );	
				// Relinquish processor instead of busyWaiting.
				
		if ( regNumber == inLatch[2].targReg )
		{
			sem_wait ( cout_mutex );
			cout << violet << "\n[ Stage1:RegisterFetch ]"
				<< " Result from current EX - ALUOutput" 
				<< reset << flush;
			sem_post ( cout_mutex );
			fetchResult = outLatch[2].ALUOutput;
		}
		else 
		{
			sem_wait ( cout_mutex );
			cout << violet << "\n[ Stage1:RegisterFetch ]"
				<< " Result from current EX - ALUOutputHi"
				<< reset << flush;
			sem_post ( cout_mutex );
			fetchResult = outLatch[2].ALUOutputHi;
		}
	}
	else if ( inLatch[3].targReg == regNumber 
			&& inLatch[3].resultStage == RESULT_AT_ID )
	{
		sem_wait ( cout_mutex );
		cout << violet << "\n[ Stage1:RegisterFetch ]"
			<< " Result from current MEM - IDRes" 
			<< reset << flush;
		sem_post ( cout_mutex );
		fetchResult = inLatch[3].IDRes;
	}
	else if ( ( inLatch[3].targReg == regNumber || inLatch[3].targReg2 == regNumber )
			&& inLatch[3].resultStage == RESULT_AT_EX )
	{
		// result has already been computed in the previous clock
		if ( regNumber == inLatch[3].targReg )
		{
			sem_wait ( cout_mutex );
			cout << violet << "\n[ Stage1:RegisterFetch ]"
				<< " Result from current MEM - ALUOutput"
				<< reset << flush;
			sem_post ( cout_mutex );
			fetchResult = inLatch[3].ALUOutput;
		}
		else
		{
			sem_wait ( cout_mutex );
			cout << violet << "\n[ Stage1:RegisterFetch ]"
				<< " Result from current MEM - ALUOutputHi" 
				<< reset << flush;
			sem_post ( cout_mutex );
			fetchResult = inLatch[3].ALUOutputHi;
		}
	}
	else if ( inLatch[3].targReg == regNumber 
			&& inLatch[3].resultStage == RESULT_AT_MEM )
	{
		// Have to wait till result has been computed
		while ( outLatch[3].finished == false )
			sched_yield ( );	
				// Relinquish processor instead of busywaiting.
		
		sem_wait ( cout_mutex );
		cout << violet << "\n[ Stage1:RegisterFetch ]"
			<< " Result from current MEM - LMD" 
			<< reset << flush;
		sem_post ( cout_mutex );
		fetchResult = outLatch[3].LMD;
	}
	else	// first wait for write register stage to complete, then read the register
	{
		while ( outLatch[4].finished == false )
			sched_yield ( );	
				// Relinquish processor instead of busywaiting.
		
		sem_wait ( cout_mutex );
		cout << violet << "\n[ Stage1:RegisterFetch ] Result from Register files" 
			<< reset << flush;
		sem_post ( cout_mutex );
		
		switch ( regNumber )
		{
		case REG_LO:
			fetchResult = Lo;
			break;
		case REG_HI:
			fetchResult = Hi;
			break;
		default:
			fetchResult = reg[regNumber];
			break;
		};
	}
	
	switch ( target )
	{
	case ALU_A:
		outLatch[1].A = fetchResult;
		sem_wait ( cout_mutex );
		cout << violet << "\n[ Stage1:RegisterFetch ] A = " << outLatch[1].A 
			<< reset << flush;
		sem_post ( cout_mutex );
		break;
	case ALU_B:
		outLatch[1].B = fetchResult;
		sem_wait ( cout_mutex );
		cout << violet << "\n[ Stage1:RegisterFetch ] B = " << outLatch[1].B 
			<< reset << flush;
		sem_post ( cout_mutex );
		break;
	case IDRES_FT:
		outLatch[1].IDRes = fetchResult;
		sem_wait ( cout_mutex );
		cout << violet << "\n[ Stage1:RegisterFetch ] IDRes = "
			<< outLatch[1].IDRes << reset << flush;
		sem_post ( cout_mutex );
		break;
	};
	return true;
}

void Processor :: UpdatePC_Stage1 ( word_32 value, PCUpdateType updateType )
{
	switch ( updateType )
	{
	case PC_ABSOLUTE:
		if ( PCreg == value )
		{
			sem_wait ( cout_mutex );
			cout << skyblue << "\n[ Stage1:UpdatePC ]"
				<< " instruction already in IF stage"
				<< reset << flush;
			sem_post ( cout_mutex );
			return;
		}
		else
		{
			while ( outLatch[2].finished == false )
				sched_yield ( );
			sem_wait ( pc_mutex );
			if ( flushStage[1] == true )
			{
				sem_wait ( cout_mutex );
				cout << skyblue << "\n[ Stage1:UpdatePC ]"
					<< " this stage was flushed by"
					<< " stage2, disallowing execution" 
					<< reset << flush;
				sem_post ( cout_mutex );
				
				sem_post ( pc_mutex );
				return;
			}
			sem_post ( pc_mutex );
			flushStage[0] = true;
			PC_update_control ( value, 1 );
			
			sem_wait ( cout_mutex );
			cout << skyblue << "\n[ Stage1:UpdatePC ]"
				<< " updated NPC with absolute address"
				<< reset << flush;
			sem_post ( cout_mutex );
		}
		break;
	case PC_RELATIVE:
		value = value + PCreg;
		if ( PCreg == value )
		{
			sem_wait ( cout_mutex );
			cout << skyblue << "\n[ Stage1:UpdatePC ]"
				<< " instruction already in IF stage"
				<< reset << flush;
			sem_post ( cout_mutex );
			return;
		}
		else
		{
			while ( outLatch[2].finished == false )
				sched_yield ( );
			sem_wait ( pc_mutex );
			if ( flushStage[1] == true )
			{
				sem_wait ( cout_mutex );
				cout << skyblue << "\n[ Stage1:UpdatePC ]"
					<< " this stage was flushed by"
					<< " stage2, disallowing execution" 
					<< reset << flush;
				sem_post ( cout_mutex );
				
				sem_post ( pc_mutex );
				return;
			}
			sem_post ( pc_mutex );
			flushStage[0] = true;
			PC_update_control ( value, 1 );
			
			sem_wait ( cout_mutex );
			cout << skyblue << "\n[ Stage1:UpdatePC ]"
				<< " updated NPC with relative address"
				<< reset << flush;
			sem_post ( cout_mutex );
		}
		break;
	};
}
