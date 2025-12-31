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
 * This file contains the entry point of the assembler
 */

# include <iostream>
using std::cerr;
using std::cout;
using std::flush;

# include <fstream>
using std::ofstream;
using std::ios;

# include "structures.h"

# include "../include/color.h"

ofstream ofile;

extern FILE *yyin;	// from lexer.lex
extern int yyparse ( );	// from asm.ypp
extern Symtab symtab;	// from asm.ypp
extern int errorcount;	// from asm.ypp

int lineno;
bool pass1 = true;

int main(int argc, char **argv)
{
	// Check for correct usage
	if ( argc < 3 )
	{
		cerr << red << "\nusage : asm <output_file> <input_file>"
			<< reset << "\n\n";
		return -1;
	}
	
	// Check input file
	yyin = fopen( argv[2], "r" );
	if ( yyin == NULL )
	{
		cerr << red << "\nInput file not found." << reset << "\n\n";
		return -2;
	}
	
	// Validate output file
	ofile.open ( argv[1], ios::out | ios::trunc | ios::binary );
	if ( !ofile )
	{
		cerr << red << "\nCould not open output file for writing." 
			<< reset << "\n\n";
		return -3;
	}
	
	// Pass 1
	cout << flush << "\nStarting pass 1 ... \n";
	lineno = 1;
	pass1 = true;
	yyparse ();
	fclose ( yyin );

	// if Pass 1 worked, Pass 2
	if ( errorcount == 0 )
	{
		cout << flush << "\nStarting pass 2 ... \n";
		yyin = fopen( argv[2], "r" );
		pass1 = false;
		lineno = 1;
		yyparse ();
		fclose ( yyin );
	}
	
	// All done !
	cout << flush << "\nDisplaying the symbol table ... \n";
	symtab.Display ( );
	
	ofile.close ();
	
	return 0;
}
