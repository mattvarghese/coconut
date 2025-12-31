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
 * This file contains the declaration of the symbol table classes
 */

# ifndef __STRUCTURES_H
# define __STRUCTURES_H

class SymtabEntry
{
	/* This class is essentially a single
	 * node on a linked list
	 */
public:
	char id[100];
	unsigned int address;
	SymtabEntry * next;
	
	SymtabEntry ( );
};

class Symtab
{
	/* TODO
	 * This is a really bad implementation of the symbol table
	 * Ideally this should be converted to something like a
	 * hash table for efficiency.
	 * But the interface can be the same.
	 */
private:
	SymtabEntry * first;

public:
	Symtab ( );
	~Symtab ( );
	
	// Add an entry into the symbol table
	bool Insert ( char * id, unsigned int address );
	
	// Does the entry exist in the symbol table
	bool LookUp ( char * id );

	// Does the entry exist in the symbol table
	// And if it does, retrieve its address
	bool LookUp ( char * id, unsigned int & address );
	
	// Display the contents of the symbol table
	void Display ( );
};

# endif
