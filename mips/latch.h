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

# ifndef __LATCH_H
# define __LATCH_H

# include "../include/instruction.h"

class Latch
{
public:
	u_word_32 PC;
	Inst inst;
	
	int targReg;
	int targReg2;	// This comes useful in MULT and DIV instructions.
	
	ResultStage resultStage;
	
	word_32 A;
	word_32 B;
	bool dataFetchIncomplete;	// this flag is set if the instruction needs
			// the data only in stage3 and the data will become availabe
			// only in stage2
	RegisterFetchTarget FetchFailedFor;	// contains the destination needing fetch 
			// in stage2 when dataFetchIncomplete == 2
			// This is useful only for the RDOUT instruction.
	word_32 Imm;
	
	word_32 IDRes;
	
	word_32 ALUOutput;
	word_32 ALUOutputHi;
	
	word_32 LMD;
	
	bool finished;
	
	void Initialise ( );
};

void LatchCopy ( Latch &ldest, Latch &lsource );

# endif
