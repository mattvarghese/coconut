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

# ifndef __COLOR_H
# define __COLOR_H

// This should be defined if you want colors
// Note that this is better defined in the mips Makefile
// as a flag to the compier
//# define __WITH_COLOR

# ifdef __WITH_COLOR

	# define reset  "\033[0m" 
	# define bold  "\033[1m" 
	# define uline  "\033[4m" 
	# define blink  "\033[5m" 
	# define invert  "\033[7m" 

	# define black  "\033[30m" 	// usual messages
	# define red  "\033[31m" 	// ALL ERROR messages
	# define green  "\033[32m" 	// memory / device / cache messages
	# define yellow  "\033[33m" 
	# define blue  "\033[34m" 	// Clock ( )
	# define violet  "\033[35m" 	// registerfetch / register write messages
	# define skyblue  "\033[36m"	// updatePC messages
	# define gray  "\033[37m" 	// information messages

	# define bgblack  "\033[40m" 
	# define bgred  "\033[41m" 
	# define bggreen  "\033[42m" 
	# define bgyellow  "\033[43m" 
	# define bgblue  "\033[44m" 
	# define bgviolet  "\033[45m" 
	# define bgskyblue  "\033[46m" 
	# define bggray  "\033[47m" 

	# define lblack  "\033[90m" 
	# define lred  "\033[91m"
	# define lgreen  "\033[92m" 
	# define lyellow  "\033[93m" 
	# define lblue  "\033[94m" 
	# define lviolet  "\033[95m" 
	# define lskyblue  "\033[96m" 
	# define lgray  "\033[97m" 

	# define lbgblack  "\033[100m" 
	# define lbgred  "\033[101m" 
	# define lbggreen  "\033[102m" 
	# define lbgyellow  "\033[103m" 
	# define lbgblue  "\033[104m" 
	# define lbgviolet  "\033[105m" 
	# define lbgskyblue  "\033[106m" 
	# define lbggray  "\033[107m" 

	# define white  "\033[97m" 
	# define bgwhite  "\033[107m"
	
//# elifdef __NO_COLOR
# else

	# define reset  "" 
	# define bold  "" 
	# define uline  "" 
	# define blink  "" 
	# define invert  "" 

	# define black  "" 
	# define red  "" 
	# define green  "" 
	# define yellow  "" 
	# define blue  "" 
	# define violet  "" 
	# define skyblue  "" 
	# define gray  "" 

	# define bgblack  "" 
	# define bgred  "" 
	# define bggreen  "" 
	# define bgyellow  "" 
	# define bgblue  "" 
	# define bgviolet  "" 
	# define bgskyblue  "" 
	# define bggray  "" 

	# define lblack  "" 
	# define lred  ""
	# define lgreen  "" 
	# define lyellow  "" 
	# define lblue  "" 
	# define lviolet  "" 
	# define lskyblue  "" 
	# define lgray  "" 

	# define lbgblack  "" 
	# define lbgred  "" 
	# define lbggreen  "" 
	# define lbgyellow  "" 
	# define lbgblue  "" 
	# define lbgviolet  "" 
	# define lbgskyblue  "" 
	# define lbggray  "" 

	# define white  "" 
	# define bgwhite  "" 
	
# endif
	
# endif
