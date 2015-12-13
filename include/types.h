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

# ifndef __TYPES_H
# define __TYPES_H

typedef int word_32;
typedef unsigned int u_word_32;
typedef long long word_64;

enum ResultStage { NORESULT, RESULT_AT_ID, RESULT_AT_EX, RESULT_AT_MEM };

enum RegisterFetchTarget { ALU_A, ALU_B, IDRES_FT };
enum RegisterWriteSource { IDRES, ALU, ALU_HI, LOAD };

enum PCUpdateType { PC_ABSOLUTE, PC_RELATIVE };

enum PCWritingStage { NOT_WRITTEN, PC_STAGE0, PC_STAGE1, PC_STAGE2 };

# endif
