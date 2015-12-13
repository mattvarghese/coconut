
%{
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

 
	# include "asm.tab.hpp"
	
	# include <iostream>
	using std::cout;
	using std::cerr;
	# include <iomanip>
	using std::setw;
	# include <cstring>
	using std::strcpy;
	
	# include "../include/color.h"
	
	extern int lineno;	// from main.cpp
	extern int errorcount;	// from asm.ypp
	extern bool pass1;	// from main.cpp
	
	char stringbuffer[100];
	int stringindex;
%}

%x string

%%

"begin"		cout << " " << yytext ; return BEG;
"start"		cout << " " << yytext ; return START;
"end"		cout << " " << yytext ; return END;

"add"		cout << " " << yytext ; return ADD;
"addi"		cout << " " << yytext ; return ADDI;

"and"		cout << " " << yytext ; return AND;
"andi"		cout << " " << yytext ; return ANDI;
"nor"		cout << " " << yytext ; return NOR;
"or"		cout << " " << yytext ; return OR;
"ori"		cout << " " << yytext ; return ORI;

"div"		cout << " " << yytext ; return DIV;
"mult"		cout << " " << yytext ; return MULT;

"sll"		cout << " " << yytext ; return SLL;
"sllv"		cout << " " << yytext ; return SLLV;
"sra"		cout << " " << yytext ; return SRA;
"srav"		cout << " " << yytext ; return SRAV;
"srl"		cout << " " << yytext ; return SRL;
"srlv"		cout << " " << yytext ; return SRLV;

"sub"		cout << " " << yytext ; return SUB;

"xor"		cout << " " << yytext ; return XOR;
"xori"		cout << " " << yytext ; return XORI;

"lui"		cout << " " << yytext ; return LUI;

"slt"		cout << " " << yytext ; return SLT;
"slti"		cout << " " << yytext ; return SLTI;

"beq"		cout << " " << yytext ; return BEQ;
"bgez"		cout << " " << yytext ; return BGEZ;
"bgtz"		cout << " " << yytext ; return BGTZ;
"blez"		cout << " " << yytext ; return BLEZ;
"bltz"		cout << " " << yytext ; return BLTZ;
"bne"		cout << " " << yytext ; return BNE;

"j"		cout << " " << yytext ; return J;
"jal"		cout << " " << yytext ; return JAL;
"jalr"		cout << " " << yytext ; return JALR;
"jr"		cout << " " << yytext ; return JR;

"lw"		cout << " " << yytext ; return LW;
"sw"		cout << " " << yytext ; return SW;

"mfhi"		cout << " " << yytext ; return MFHI;
"mflo"		cout << " " << yytext ; return MFLO;
"mthi"		cout << " " << yytext ; return MTHI;
"mtlo"		cout << " " << yytext ; return MTLO;

"syscall"	cout << " " << yytext ; return SYSCALL;
"nop"		cout << " " << yytext ; return NOP;

"din"		cout << " " << yytext ; return DIN;
"dout"		cout << " " << yytext ; return DOUT;
"rdin"		cout << " " << yytext ; return RDIN;
"rdout"		cout << " " << yytext ; return RDOUT;

"dw"		cout << " " << yytext ; return DW;

	/* Registers */
	
"$zero"	|
"$0"	yylval.value = 0; cout << " " << yytext ; return REGISTER;

"$at"	|
"$1"	yylval.value = 1; cout << " " << yytext ; return REGISTER;

"$v0"	|
"$2"	yylval.value = 2; cout << " " << yytext ; return REGISTER;

"$v1"	|
"$3"	yylval.value = 3; cout << " " << yytext ; return REGISTER;

"$a0"	|
"$4"	yylval.value = 4; cout << " " << yytext ; return REGISTER;

"$a1"	|
"$5"	yylval.value = 5; cout << " " << yytext ; return REGISTER;

"$a2"	|
"$6"	yylval.value = 6; cout << " " << yytext ; return REGISTER;

"$a3"	|
"$7"	yylval.value = 7; cout << " " << yytext ; return REGISTER;

"$t0"	|
"$8"	yylval.value = 8; cout << " " << yytext ; return REGISTER;

"$t1"	|
"$9"	yylval.value = 9; cout << " " << yytext ; return REGISTER;

"$t2"	|
"$10"	yylval.value = 10; cout << " " << yytext ; return REGISTER;

"$t3"	|
"$11"	yylval.value = 11; cout << " " << yytext ; return REGISTER;

"$t4"	|
"$12"	yylval.value = 12; cout << " " << yytext ; return REGISTER;

"$t5"	|
"$13"	yylval.value = 13; cout << " " << yytext ; return REGISTER;

"$t6"	|
"$14"	yylval.value = 14; cout << " " << yytext ; return REGISTER;

"$t7"	|
"$15"	yylval.value = 15; cout << " " << yytext ; return REGISTER;

"$s0"	|
"$16"	yylval.value = 16; cout << " " << yytext ; return REGISTER;

"$s1"	|
"$17"	yylval.value = 17; cout << " " << yytext ; return REGISTER;

"$s2"	|
"$18"	yylval.value = 18; cout << " " << yytext ; return REGISTER;

"$s3"	|
"$19"	yylval.value = 19; cout << " " << yytext ; return REGISTER;

"$s4"	|
"$20"	yylval.value = 20; cout << " " << yytext ; return REGISTER;

"$s5"	|
"$21"	yylval.value = 21; cout << " " << yytext ; return REGISTER;

"$s6"	|
"$22"	yylval.value = 22; cout << " " << yytext ; return REGISTER;

"$s7"	|
"$23"	yylval.value = 23; cout << " " << yytext ; return REGISTER;

"$t8"	|
"$24"	yylval.value = 24; cout << " " << yytext ; return REGISTER;

"$t9"	|
"$25"	yylval.value = 25; cout << " " << yytext ; return REGISTER;

"$k0"	|
"$26"	yylval.value = 26; cout << " " << yytext ; return REGISTER;

"$k1"	|
"$27"	yylval.value = 27; cout << " " << yytext ; return REGISTER;

"$gp"	|
"$28"	yylval.value = 28; cout << " " << yytext ; return REGISTER;

"$sp"	|
"$29"	yylval.value = 29; cout << " " << yytext ; return REGISTER;

"$fp"	|
"$30"	yylval.value = 30; cout << " " << yytext ; return REGISTER;

"$ra"	|
"$31"	yylval.value = 31; cout << " " << yytext ; return REGISTER;

	/* Identifiers */
([a-zA-Z_])([a-zA-Z_0-9])*	{
	strcpy(yylval.id,yytext); cout << " " << yytext ; return IDENTIFIER;
	}

	/* Integer constants */
([0-9])*	yylval.value = atoi(yytext); cout << " " << yytext ; return INTCONSTANT;
\-([0-9])*	yylval.value = atoi(yytext); cout << " " << yytext ; return INTCONSTANT;
\'[0-9a-zA-Z\-]\'	{
		yylval.value = static_cast<int>(yytext[1]);
		cout << " " << yytext ; return INTCONSTANT;
	}
\'\\n\'		{
		yylval.value = static_cast<int>('\n');
		cout << " " << yytext ; return INTCONSTANT;
	}
\'\\r\'		{
		yylval.value = static_cast<int>('\r');
		cout << " " << yytext ; return INTCONSTANT;
	}
\'\\t\'		{
		yylval.value = static_cast<int>('\t');
		cout << " " << yytext ; return INTCONSTANT;
	}
\'\\\"\'		{
		yylval.value = static_cast<int>('\"');
		cout << " " << yytext ; return INTCONSTANT;
	}
\'\\\'\'		{
		yylval.value = static_cast<int>('\'');
		cout << " " << yytext ; return INTCONSTANT;
	}

	/* Punctuators */
\:	|
\[	|
\]	|
\(	|
\)	|
\,		cout << " " << yytext ; return yytext[0];

\n	lineno ++; cout << " " << yytext ; return yytext[0];

([\t ])+	//For the moment we are ignoring white spaces;

	/* Single line comment */
\#[^\n]*	cout << " " << yytext ; lineno ++; /* Ignore comments. */


\"	stringindex = 0; BEGIN(string);
<string>\"	{ 
		// saw the closing quote, all done
		BEGIN(INITIAL);
		stringbuffer[stringindex] = '\0';
		cout << " \"" << stringbuffer << "\"";
		strcpy ( yylval.id, stringbuffer );
		return STRING;
	}
<string>\n	{
		cerr << red << "\nLine:" << setw(4) << lineno << ", pass:" << setw(2)
			<< (( pass1 )? 1 : 2)
			<< ", Unterminated string constant" << reset << "\n";
		errorcount ++;
		BEGIN(INITIAL);
	}
<string>\\n	stringbuffer[stringindex++] = '\n';
<string>\\r	stringbuffer[stringindex++] = '\r';
<string>\\t	stringbuffer[stringindex++] = '\t';
<string>\\\\	stringbuffer[stringindex++] = '\\';
<string>\\\"	stringbuffer[stringindex++] = '\"';
<string>\\'	stringbuffer[stringindex++] = '\'';
<string>[^\\\n\"]	{
		char * yptr = yytext;
		while ( *yptr )
			stringbuffer[stringindex++] = *yptr++;
	}

	

	/* Anything not matched so far results in an error */
.	{
		cerr << red << "\nLine:" << setw(4) << lineno << ", pass:" << setw(2)
			<< (( pass1 )? 1 : 2)
			<< ", Invalid lexicon : " << yytext << reset << "\n";
		errorcount ++;
	}
