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
 * This file contains the definition of the symbol table classes
 */

# include "structures.h"

# include <string>
using std::string;
# include <cstring>
using std::strcpy;

# include <iostream>
using std::cerr;
using std::cout;
# include <iomanip>
using std::setw;

# include "../include/color.h"

extern int lineno;	// from lexer.lex
extern int errorcount;	// from asm.ypp
extern bool pass1;	// from main.cpp

SymtabEntry::SymtabEntry ( )
{
	id[0] = '\0';
	address = 0;
	next = 0; // NULL
}

Symtab::Symtab ( )
{
	first = 0;	// NULL
}

Symtab::~Symtab ( )
{
	if ( first != 0 )
	{
		SymtabEntry * temp;
		while ( first != 0 )
		{
			temp = first -> next;
			delete first;
			first = temp;
		};
	}
}

bool Symtab::Insert ( char * id, unsigned int address )
{
	if ( LookUp ( id ) == true )
	{
		cerr << red << "\nLine:" << setw (4) << lineno << ", pass:" << setw(2)
			<< (( pass1 )? 1 : 2)
			<< ", redefinition of symbol : "
			<< id << reset << "\n";
		errorcount ++;
		return false;
	}
	SymtabEntry * se = new SymtabEntry;
	strcpy ( se -> id, id );
	se -> address = address;
	se -> next = first;
	first = se;
	return true;
}

bool Symtab::LookUp ( char * id, unsigned int & address )
{
	SymtabEntry * temp = first;
	while ( temp != 0 )
	{
		if ( string(temp -> id) == string(id) )
		{
			address = temp -> address;
			return true;
		}
		temp = temp -> next;
	};
	
	cerr << red << "\nLine:" << setw(4) << lineno << ", pass:" << setw(2)
		<< (( pass1 )? 1 : 2)
		<< ", unresolved symbol : "
		<< id << reset << "\n";
	errorcount ++;
	return false;
}

bool Symtab::LookUp ( char * id )
{
	SymtabEntry * temp = first;
	while ( temp != 0 )
	{
		if ( string(temp -> id) == string(id) )
		{
			return true;
		}
		temp = temp -> next;
	};
	return false;
}

void Symtab::Display ( )
{
	SymtabEntry * temp = first;
	while ( temp != 0 )
	{
		cout<< temp -> id << " : " << temp -> address << "\n";
		temp = temp -> next;
	};
}
	
