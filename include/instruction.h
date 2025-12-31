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
 * This file declares the various instruction formats
 * and structures associated with mips instructions
 */

# ifndef __INSTRUCTION_H
# define __INSTRUCTION_H

# include "types.h"
 
class Instruction
{
public :
	u_word_32 op : 6;
	u_word_32 rest : 26;
};

/* different types of instructions being modeled
 * as derived types of Instruction will cause inconsistencies
 * in instruction sizes.  So we have to explicitly cast and 
 * convert between them.
 */

class RFormat
{
public : 
	u_word_32 op : 6;
	u_word_32 rs : 5;
	u_word_32 rt : 5;
	u_word_32 rd : 5;
	u_word_32 shamt : 5;
	u_word_32 funct : 6;
};

class IFormat
{
public :
	u_word_32 op : 6;
	u_word_32 rs : 5;
	u_word_32 rt : 5;
	word_32 imm : 16;	// Signed
};

class JFormat
{ /* This is structurally same as Instruction */
public :
	u_word_32 op : 6;
	u_word_32 tAddr : 26;
};

/*
 * This union object means that we can interpret
 * the instruction as any of the above formats
 */
union Inst
{ // This can be optimally be used everywhere.
	word_32 iV;		// Signed
	Instruction noF;
	IFormat iF;
	RFormat rF;
	JFormat jF;
};

struct OneRecord
{ // This structure is used in the assembler and in the
  // Bootloading code in memory.cpp
	unsigned int address;
	Inst inst;
};

# endif
