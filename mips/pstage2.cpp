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

void Processor :: Stage2 ( )
{
	// Copy inLatch into outLatch... NowForth work with outLatch
	LatchCopy(outLatch[2], inLatch[2]);
	word_64 HiLoBuffer;
	
	// Fisrt check for NOP
	if ( outLatch[2].inst.iV == 0 )
	{
		// Do Nothing 
		outLatch[2].finished = true;
		
		sem_wait ( cout_mutex );
		cout << "\n[ Stage2 ] NOP" << flush;
		sem_post ( cout_mutex );
	}
	else // else perform the switch code
	switch ( outLatch[2].inst.noF.op )
	{
	case OP_ZERO:
		switch ( outLatch[2].inst.rF.funct )
		{
		case FUNCT_ADD:
			outLatch[2].ALUOutput = outLatch[2].A + outLatch[2].B;
			outLatch[2].finished = true;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage2 ] ADD ALUOutput = " << outLatch[2].ALUOutput
				<< flush;
			sem_post ( cout_mutex );
			break;
			
		case FUNCT_AND:
			outLatch[2].ALUOutput = outLatch[2].A & outLatch[2].B;
			outLatch[2].finished = true;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage2 ] AND ALUOutput = " << outLatch[2].ALUOutput
				<< flush;
			sem_post ( cout_mutex );
			break;
			
		case FUNCT_DIV:
			outLatch[2].ALUOutput = outLatch[2].A / outLatch[2].B;	// quotient
			outLatch[2].ALUOutputHi = outLatch[2].A % outLatch[2].B; // remainder
			outLatch[2].finished = true;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage2 ] DIV ALUOutput = " << outLatch[2].ALUOutput
				<< flush;
			cout << "\n[ Stage2 ] DIV ALUOutputHi = " << outLatch[2].ALUOutputHi
				<< flush;
			sem_post ( cout_mutex );
			break;
			
		case FUNCT_MULT:
			HiLoBuffer = static_cast<word_64>(outLatch[2].A)
				* static_cast<word_64>(outLatch[2].B);
			outLatch[2].ALUOutput = static_cast<word_32>
						(HiLoBuffer & 0x00000000ffffffff);
			outLatch[2].ALUOutputHi = 
				static_cast<word_32>((HiLoBuffer >>32) & 0x00000000ffffffff); 
			outLatch[2].finished = true;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage2 ] MULT ALUOutput = " << outLatch[2].ALUOutput
				<< flush;
			cout << "\n[ Stage2 ] MULT ALUOutputHi = " << outLatch[2].ALUOutputHi
				<< flush;
			sem_post ( cout_mutex );
			break;
			
		case FUNCT_NOR:
			outLatch[2].ALUOutput = ~ ( outLatch[2].A | outLatch[2].B );
			outLatch[2].finished = true;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage2 ] NOR ALUOutput = " << outLatch[2].ALUOutput
				<< flush;
			sem_post ( cout_mutex );
			break;
			
		case FUNCT_OR:
			outLatch[2].ALUOutput = outLatch[2].A | outLatch[2].B;
			outLatch[2].finished = true;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage2 ] OR ALUOutput = " << outLatch[2].ALUOutput
				<< flush;
			sem_post ( cout_mutex );
			break;
		
		case FUNCT_SLL:
			outLatch[2].ALUOutput = outLatch[2].B << outLatch[2].A;
			outLatch[2].finished = true;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage2 ] SLL ALUOutput = " << outLatch[2].ALUOutput
				<< flush;
			sem_post ( cout_mutex );
			break;
		
		case FUNCT_SLLV:
			outLatch[2].ALUOutput = outLatch[2].B << outLatch[2].A;
			outLatch[2].finished = true;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage2 ] SLLV ALUOutput = " << outLatch[2].ALUOutput
				<< flush;
			sem_post ( cout_mutex );
			break;
		
		case FUNCT_SRA:
			outLatch[2].ALUOutput = outLatch[2].B >> outLatch[2].A;
			outLatch[2].finished = true;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage2 ] SRA ALUOutput = " << outLatch[2].ALUOutput
				<< flush;
			sem_post ( cout_mutex );
			break;
		
		case FUNCT_SRAV:
			outLatch[2].ALUOutput = outLatch[2].B >> outLatch[2].A;
			outLatch[2].finished = true;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage2 ] SRAV ALUOutput = " << outLatch[2].ALUOutput
				<< flush;
			sem_post ( cout_mutex );
			break;
		
		case FUNCT_SRL:
			//static_cast<u_word_32>(outLatch[2].ALUOutput) = 
			outLatch[2].ALUOutput = 
					static_cast<u_word_32>(outLatch[2].B)
					>> static_cast<u_word_32>(outLatch[2].A);
			outLatch[2].finished = true;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage2 ] SRL ALUOutput = " << outLatch[2].ALUOutput
				<< flush;
			sem_post ( cout_mutex );
			break;
		
		case FUNCT_SRLV:
			//static_cast<u_word_32>(outLatch[2].ALUOutput) = 
			outLatch[2].ALUOutput = 
					static_cast<u_word_32>(outLatch[2].B)
					>> static_cast<u_word_32>(outLatch[2].A);
			outLatch[2].finished = true;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage2 ] SRLV ALUOutput = " << outLatch[2].ALUOutput
				<< flush;
			sem_post ( cout_mutex );
			break;
			
		case FUNCT_SUB:
			outLatch[2].ALUOutput = outLatch[2].A - outLatch[2].B;
			outLatch[2].finished = true;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage2 ] SUB ALUOutput = " << outLatch[2].ALUOutput
				<< flush;
			sem_post ( cout_mutex );
			break;
			
		case FUNCT_XOR:
			outLatch[2].ALUOutput = outLatch[2].A ^ outLatch[2].B;
			outLatch[2].finished = true;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage2 ] XOR ALUOutput = " << outLatch[2].ALUOutput
				<< flush;
			sem_post ( cout_mutex );
			break;
			
		case FUNCT_SLT:
			outLatch[2].ALUOutput = (outLatch[2].A < outLatch[2].B) ? 1 : 0;
			outLatch[2].finished = true;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage2 ] SLT ALUOutput = " << outLatch[2].ALUOutput
				<< flush;
			sem_post ( cout_mutex );
			break;
			
		case FUNCT_JR:
			UpdatePC_Stage2 ( outLatch[2].A, PC_ABSOLUTE);
			outLatch[2].finished = true;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage2 ] JR jumping to absolute address " 
				<< outLatch[2].A << flush;
			sem_post ( cout_mutex );
			break;
		
		case FUNCT_JALR:
			UpdatePC_Stage2 ( outLatch[2].A, PC_ABSOLUTE);
			outLatch[2].finished = true;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage2 ] JALR jumping to absolute address " 
				<< outLatch[2].A << flush;
			sem_post ( cout_mutex );
			break;
			
		case FUNCT_MFHI:
			outLatch[2].finished = true;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage2 ] MFHI idle" << flush;
			sem_post ( cout_mutex );
			break;
			
		case FUNCT_MFLO:
			outLatch[2].finished = true;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage2 ] MFLO idle" << flush;
			sem_post ( cout_mutex );
			break;
			
		case FUNCT_MTHI:
			outLatch[2].finished = true;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage2 ] MTHI idle" << flush;
			sem_post ( cout_mutex );
			break;
			
		case FUNCT_MTLO:
			outLatch[2].finished = true;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage2 ] MTLO idle" << flush;
			sem_post ( cout_mutex );
			break;
			
		case FUNCT_SYSCALL:
			outLatch[2].finished = true;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage2 ] SYSCALL idle" << flush;
			sem_post ( cout_mutex );
			break;
			
		case FUNCT_RDIN:
			outLatch[2].finished = true;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage2 ] RDIN idle" << flush;
			sem_post ( cout_mutex );
			break;
			
		case FUNCT_RDOUT:
			if ( outLatch[2].dataFetchIncomplete == true )
			{
				if ( outLatch[2].FetchFailedFor == ALU_A )
				{
					if ( RegisterFetch_Stage2 ( ALU_A,
							outLatch[2].inst.rF.rs) == true )
						outLatch[2].finished = true;
					else outLatch[2].finished = false;
				}
				else if ( outLatch[2].FetchFailedFor == ALU_B )
				{
					if ( RegisterFetch_Stage2 ( ALU_B,
							outLatch[2].inst.rF.rt ) == true )
						outLatch[2].finished = true;
					else outLatch[2].finished = false;
				}
				else
				{
					sem_wait ( cout_mutex );
					cout << red << "\n[ Stage2 ] inconsistency between "
						<< "dataFetchIncomplete and FetchFailedFor"
						<< reset << flush;
					sem_post ( cout_mutex );
					outLatch[2].finished = false;
				}
			}
			else outLatch[2].finished = true;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage2 ] RDOUT idle" << flush;
			sem_post ( cout_mutex );
			break;
			
		};
		break;
		
	case OP_ONE:
		switch ( outLatch[2].inst.iF.rt )
		{
		case 1:	// The instruction is BGEZ
			outLatch[2].finished = true;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage2 ] BGEZ idle" << flush;
			sem_post ( cout_mutex );
			break;
		case 0:	// The instruction is BLTZ
			outLatch[2].finished = true;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage2 ] BLTZ idle" << flush;
			sem_post ( cout_mutex );
			break;
		};
		break;
		
	case OP_ADDI:
		outLatch[2].ALUOutput = outLatch[2].A + outLatch[2].Imm;
		outLatch[2].finished = true;
		
		sem_wait ( cout_mutex );
		cout << "\n[ Stage2 ] ADDI ALUOutput = " << outLatch[2].ALUOutput
			<< flush;
		sem_post ( cout_mutex );
		break;
		
	case OP_ANDI:
		outLatch[2].ALUOutput = outLatch[2].A & outLatch[2].Imm;
		outLatch[2].finished = true;
		
		sem_wait ( cout_mutex );
		cout << "\n[ Stage2 ] ANDI ALUOutput = " << outLatch[2].ALUOutput
			<< flush;
		sem_post ( cout_mutex );
		break;
		
	case OP_ORI:
		outLatch[2].ALUOutput = outLatch[2].A | outLatch[2].Imm;
		outLatch[2].finished = true;
		
		sem_wait ( cout_mutex );
		cout << "\n[ Stage2 ] ORI ALUOutput = " << outLatch[2].ALUOutput
			<< flush;
		sem_post ( cout_mutex );
		break;
		
	case OP_XORI:
		outLatch[2].ALUOutput = outLatch[2].A ^ outLatch[2].Imm;
		outLatch[2].finished = true;
		
		sem_wait ( cout_mutex );
		cout << "\n[ Stage2 ] XORI ALUOutput = " << outLatch[2].ALUOutput
			<< flush;
		sem_post ( cout_mutex );
		break;
		
	case OP_LUI:
		outLatch[2].ALUOutput = outLatch[2].B & 0x0000ffff;
		outLatch[2].ALUOutput = outLatch[2].ALUOutput | outLatch[2].Imm;
		outLatch[2].finished = true;
		
		sem_wait ( cout_mutex );
		cout << "\n[ Stage2 ] LUI ALUOutput = " << outLatch[2].ALUOutput
			<< flush;
		sem_post ( cout_mutex );
		break;
		
	case OP_SLTI:
		outLatch[2].ALUOutput = (outLatch[2].A < outLatch[2].Imm) ? 1 : 0;
		outLatch[2].finished = true;
		
		sem_wait ( cout_mutex );
		cout << "\n[ Stage2 ] SLTI ALUOutput = " << outLatch[2].ALUOutput
			<< flush;
		sem_post ( cout_mutex );
		break;
	
	case OP_BEQ:
		if ( outLatch[2].A == outLatch[2].B )
		{
			UpdatePC_Stage2 ( outLatch[2].Imm, PC_RELATIVE);
			outLatch[2].finished = true;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage2 ] BEQ jumping to relative address " 
				<< outLatch[2].Imm << flush;
			sem_post ( cout_mutex );
		}
		else
		{
			outLatch[2].finished = true;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage2 ] BEQ branch not taken" << flush;
			sem_post ( cout_mutex );
		}
		break;
		
	case OP_BGTZ:
		outLatch[2].finished = true;
		
		sem_wait ( cout_mutex );
		cout << "\n[ Stage2 ] BGTZ idle" << flush;
		sem_post ( cout_mutex );
		break;
		
	case OP_BLEZ:
		outLatch[2].finished = true;
		
		sem_wait ( cout_mutex );
		cout << "\n[ Stage2 ] BLEZ idle" << flush;
		sem_post ( cout_mutex );
		break;
	
	case OP_BNE:
		if ( outLatch[2].A != outLatch[2].B )
		{
			UpdatePC_Stage2 ( outLatch[2].Imm, PC_RELATIVE);
			outLatch[2].finished = true;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage2 ] BNE jumping to relative address " 
				<< outLatch[2].Imm << flush;
			sem_post ( cout_mutex );
		}
		else
		{
			outLatch[2].finished = true;
			
			sem_wait ( cout_mutex );
			cout << "\n[ Stage2 ] BNE branch not taken" << flush;
			sem_post ( cout_mutex );
		}
		break;
		
	case OP_J:
		outLatch[2].finished = true;
		
		sem_wait ( cout_mutex );
		cout << "\n[ Stage2 ] J idle" << flush;
		sem_post ( cout_mutex );
		break;
		
	case OP_JAL:
		outLatch[2].finished = true;
		
		sem_wait ( cout_mutex );
		cout << "\n[ Stage2 ] JAL idle" << flush;
		sem_post ( cout_mutex );
		break;
		
	case OP_LW:
		outLatch[2].ALUOutput = outLatch[2].A + outLatch[2].Imm;
		outLatch[2].finished = true;
		
		sem_wait ( cout_mutex );
		cout << "\n[ Stage2 ] LW ALUOutput / Memory address = " 
			<< outLatch[2].ALUOutput << flush;
		sem_post ( cout_mutex );
		break;
		
	case OP_SW:
		outLatch[2].ALUOutput = outLatch[2].A + outLatch[2].Imm;
		if ( outLatch[2].dataFetchIncomplete == true )
		{
			if ( RegisterFetch_Stage2 ( ALU_B, outLatch[2].inst.iF.rt )
					== true )
				outLatch[2].finished = true;
			else outLatch[2].finished = false;
		}
		else outLatch[2].finished = true;
		
		sem_wait ( cout_mutex );
		cout << "\n[ Stage2 ] SW ALUOutput / Memory address = "
			<< outLatch[2].ALUOutput << flush;
		sem_post ( cout_mutex );
		break;
		
	case OP_DIN:
		outLatch[2].finished = true;
		
		sem_wait ( cout_mutex );
		cout << "\n[ Stage2 ] DIN idle" << flush;
		sem_post ( cout_mutex );
		break;
		
	case OP_DOUT:
		if ( outLatch[2].dataFetchIncomplete == true )
		{
			if ( RegisterFetch_Stage2 ( ALU_A, outLatch[2].inst.iF.rs )
					== true )
				outLatch[2].finished = true;
			else outLatch[2].finished = false;
		}
		else outLatch[2].finished = true;
		
		sem_wait ( cout_mutex );
		cout << "\n[ Stage2 ] DOUT idle" << flush;
		sem_post ( cout_mutex );
		break;
		
	};
}

bool Processor :: RegisterFetch_Stage2 ( RegisterFetchTarget target, int regNumber )
{
	word_32 fetchResult;
	
	// Please note that many of the cases have been commented out because,
	// as per our design, these will never happen,  as all possible fetches
	// will have been done in the ID stage itself.  These have been
	// retained in comment for use by any subsequent developer who may 
	// redisign the application suite in such a manner that may necessiate
	// their use.  However, such designs are generally not recommended.
	
	/*
	if ( regNumber == 0 )
	{
		sem_wait ( cout_mutex );
		cout << violet << "\n[ Stage2:RegisterFetch ] Register $zero is always 0" 
			<< reset << flush;
		sem_post ( cout_mutex );
		fetchResult = 0;
	}	
	else if ( inLatch[3].targReg == regNumber 
			&& inLatch[3].resultStage == RESULT_AT_ID )
	{
		sem_wait ( cout_mutex );
		cout << violet << "\n[ Stage2:RegisterFetch ]"
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
			cout << violet << "\n[ Stage2:RegisterFetch ]"
				<< " Result from current MEM - ALUOutput"
				<< reset << flush;
			sem_post ( cout_mutex );
			fetchResult = inLatch[3].ALUOutput;
		}
		else
		{
			sem_wait ( cout_mutex );
			cout << violet << "\n[ Stage2:RegisterFetch ]"
				<< " Result from current MEM - ALUOutputHi" 
				<< reset << flush;
			sem_post ( cout_mutex );
			fetchResult = inLatch[3].ALUOutputHi;
		}
	}
	else*/ if ( inLatch[3].targReg == regNumber 
			&& inLatch[3].resultStage == RESULT_AT_MEM )
	{
		// Have to wait till result has been computed
		while ( outLatch[3].finished == false )
			sched_yield ( );	
				// Relinquish processor instead of busywaiting.
		
		sem_wait ( cout_mutex );
		cout << violet << "\n[ Stage2:RegisterFetch ]"
			<< " Result from current MEM - LMD" 
			<< reset << flush;
		sem_post ( cout_mutex );
		fetchResult = outLatch[3].LMD;
	}
	else return false;
	/*
	else	// first wait for write register stage to complete, then read the register
	{
		while ( outLatch[4].finished == false )
			sched_yield ( );	
				// Relinquish processor instead of busywaiting.
		
		sem_wait ( cout_mutex );
		cout << violet << "\n[ Stage2:RegisterFetch ] Result from Register files" 
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
	*/
	
	switch ( target )
	{
	case ALU_A:
		outLatch[2].A = fetchResult;
		sem_wait ( cout_mutex );
		cout << violet << "\n[ Stage2:RegisterFetch ] A = " << outLatch[2].A 
			<< reset << flush;
		sem_post ( cout_mutex );
		break;
	case ALU_B:
		outLatch[2].B = fetchResult;
		sem_wait ( cout_mutex );
		cout << violet << "\n[ Stage2:RegisterFetch ] B = " << outLatch[2].B 
			<< reset << flush;
		sem_post ( cout_mutex );
		break;
	case IDRES_FT:
		// This case should not arise.
		sem_wait ( cout_mutex );
		cout << violet << "\n[ Stage2:RegisterFetch ] "
			<< "Error, IDRES_FT cannot be target in stage2 " 
			<< reset << flush;
		sem_post ( cout_mutex );
		return false;
	};
	return true;
}

void Processor :: UpdatePC_Stage2 ( word_32 value, PCUpdateType updateType )
{
	switch ( updateType )
	{
	case PC_ABSOLUTE:
		if ( inLatch[1].PC == value )
		{
			sem_wait ( cout_mutex );
			cout << skyblue << "\n[ Stage2:UpdatePC ]"
				<< " instruction already in ID stage"
				<< reset << flush;
			sem_post ( cout_mutex );
			return;
		}
		else if ( /*inLatch[0].PC*/PCreg == value )
		{
			flushStage [1] = true;
			
			sem_wait ( cout_mutex );
			cout << skyblue << "\n[ Stage2:UpdatePC ]"
				<< " instruction already in IF stage"
				<< "; requesting flush for ID" 
				<< reset << flush;
			sem_post ( cout_mutex );
		}
		else
		{
			flushStage[1] = true;
			flushStage[0] = true;
			
			PC_update_control ( value, 2 );
			
			sem_wait ( cout_mutex );
			cout << skyblue << "\n[ Stage2:UpdatePC ]"
				<< " updated NPC with absolute address" 
				<< reset << flush;
			sem_post ( cout_mutex );
		}
		break;
	case PC_RELATIVE:
		value = value + PCreg - 4;
		if ( inLatch[1].PC == value )
		{
			sem_wait ( cout_mutex );
			cout << skyblue << "\n[ Stage2:UpdatePC ]"
				<< " instruction already in ID stage"
				<< reset << flush;
			sem_post ( cout_mutex );
			return;
		}
		else if ( /*inLatch[0].PC*/PCreg == value )
		{
			flushStage [1] = true;
			
			sem_wait ( cout_mutex );
			cout << skyblue << "\n[ Stage2:UpdatePC ]"
				<< " instruction already in IF stage"
				<< "; requesting flush for ID" 
				<< reset << flush;
			sem_post ( cout_mutex );
		}
		else
		{
			flushStage[1] = true;
			flushStage[0] = true;
			
			PC_update_control ( value, 2 );
			
			sem_wait ( cout_mutex );
			cout << skyblue << "\n[ Stage2:UpdatePC ]"
				<< " updated NPC with relative address" 
				<< reset << flush;
			sem_post ( cout_mutex );
		}
		break;
	};
}
		
