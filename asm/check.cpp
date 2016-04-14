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

/*
 * This file displays the contents of an assembled mips program in a
 * tabular fashion for validating the assembly
 */

# include <iostream>
# include <fstream>
# include <iomanip>
using std::cout;
using std::setw;
using std::ios;
using std::ifstream;

# include "../include/instruction.h"

union UN
{
	int x;
	Instruction i;
	IFormat iF;
	JFormat jF;
	RFormat rF;
};

int main ( int argc, char ** argv )
{
	if ( argc < 2 )
	{
		cout << "usage : check <filename>\n";
		return  -1;
	}
	
	int address; UN instr;
	ifstream ifile ( argv[1], ios::in | ios::binary );
	ifile.read ( (char*)& address, sizeof (int) );
	ifile.read ( (char*)& instr, sizeof (UN) );
	cout << setw(16) << "address" << setw(4) << "op"
		<< setw(4) << "rs" << setw(4) << "rt"
		<< setw(4) << "rd" << setw(5) << "sht"
		<< setw(4) << "fc" << setw(12) << "imm"
		<< setw(16) << "tAddr" << setw(16) << "value" << "\n";
	while ( ifile )
	{
		cout << setw (16) << address << setw(4) << instr.i.op 
			<< setw(4) << instr.rF.rs << setw(4) << instr.rF.rt 
			<< setw(4) << instr.rF.rd << setw(5) << instr.rF.shamt
			<< setw(4) << instr.rF.funct << setw(12) << instr.iF.imm
			<< setw(16) << instr.jF.tAddr << setw(16) << instr.x << "\n";
		ifile.read ( (char*)& address, sizeof (int) );
		ifile.read ( (char*)& instr, sizeof (UN) );
	};
	
	return 0;
}
