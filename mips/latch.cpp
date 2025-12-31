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

# include "latch.h"

void Latch :: Initialise ( )
{
	PC = 0;
	inst.iV = 0;
	
	targReg = -1;
	targReg2 = -1;
	
	resultStage = NORESULT;
	
	dataFetchIncomplete = false;	// indicates fetch did not fail.
	FetchFailedFor = IDRES_FT;	// indicates fetch did not fail.
	finished = false;
}

void LatchCopy ( Latch &ldest, Latch &lsource )
{
	ldest.PC = lsource.PC;
	ldest.inst.iV = lsource.inst.iV;
	
	ldest.targReg = lsource.targReg;
	ldest.targReg2 = lsource.targReg2;
	
	ldest.resultStage = lsource.resultStage;
	
	ldest.A = lsource.A;
	ldest.B = lsource.B;
	
	ldest.dataFetchIncomplete = lsource.dataFetchIncomplete;
	ldest.FetchFailedFor = lsource.FetchFailedFor;
	
	ldest.Imm = lsource.Imm;
	ldest.IDRes = lsource.IDRes;
	ldest.ALUOutput = lsource.ALUOutput;
	ldest.ALUOutputHi = lsource.ALUOutputHi;
	ldest.LMD = lsource.LMD;
	
	ldest.finished = false;
}
