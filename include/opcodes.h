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
 * This file defines all the supported opcodes
 */

# ifndef __OPCODES_H
# define __OPCODES_H

/*Opcode definitions go here
 */

# define	OP_ZERO		0
// Further demuxed using the 'funct' field
// as ...

# define	FUNCT_ADD	0x20
# define	FUNCT_AND	0x24
# define	FUNCT_DIV	0x1a
# define	FUNCT_MULT	0x18
# define	FUNCT_NOR	0x27
# define	FUNCT_OR	0x25
# define	FUNCT_SLL	0
# define	FUNCT_SLLV	4
# define	FUNCT_SRA	3
# define	FUNCT_SRAV	7
# define	FUNCT_SRL	2
# define	FUNCT_SRLV	6
# define	FUNCT_SUB	0x22
# define	FUNCT_XOR	0x26
# define	FUNCT_SLT	0x2a
# define	FUNCT_JALR	9
# define	FUNCT_JR	0x8
# define	FUNCT_MFHI	0x10
# define	FUNCT_MFLO	0x12
# define	FUNCT_MTHI	0x11
# define	FUNCT_MTLO	0x13
# define	FUNCT_SYSCALL	0xc
# define	FUNCT_RDIN	0x3f		// Addition to MIPS
# define	FUNCT_RDOUT	0x3e		// Addition to MIPS



# define	OP_ADDI		8
# define	OP_ANDI		0xc
# define	OP_ORI		0xd
# define	OP_XORI		0xe
# define	OP_LUI		0xf
# define	OP_SLTI		0xa
# define	OP_BEQ		4

# define	OP_ONE		1		
// This need further demux through 'rt' field
// to identify the instruction as BGEZ or BLTZ

# define	OP_BGTZ		7
# define	OP_BLEZ		6
# define	OP_BNE		5
# define	OP_J		2
# define	OP_JAL		3
# define	OP_LW		0x23
# define	OP_SW		0x2b

# define	OP_DIN		0x3f		// Addition to MIPS
# define	OP_DOUT		0x3e		// Addition to MIPS

# endif

