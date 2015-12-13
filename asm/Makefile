 # Copyright 2005-2015 Varghese Mathew (Matt)
 # 
 # This file is part of Coconut (TM).
 # Coconut is a
 #     Multi-threaded simulation of the pipeline of a MIPS-like
 #     Microprocessor (integer instructions only) replete with 
 #     Memory Subsystem, Caches and their performance analysis,
 #     I/O device modules and an assembler.
 # 
 # Coconut is free software: you can redistribute it and/or modify
 # it under the terms of the GNU General Public License as published by
 # the Free Software Foundation, either version 3 of the License, or
 # (at your option) any later version.
 # 
 # Coconut is distributed in the hope that it will be useful,
 # but WITHOUT ANY WARRANTY; without even the implied warranty of
 # MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 # GNU General Public License for more details.
 # 
 # You should have received a copy of the GNU General Public License
 # along with Coconut.  If not, see <http://www.gnu.org/licenses/>.
 # 

CC			= g++
LEX			= flex
YACC			= bison
CFLAGS			= -g -Wsign-promo -Wabi -D__WITH_COLOR
LIBS			= -lfl
RM			= rm
INCLUDEPATH		= ../include/
OUTPUT_ASM		= ../test/asm
OUTPUT_CHECK		= ../test/check

all: $(OUTPUT_ASM) $(OUTPUT_CHECK)

$(OUTPUT_CHECK): check.cpp $(INCLUDEPATH)types.h $(INCLUDEPATH)instruction.h
	$(CC) $(CFLAGS) -o $(OUTPUT_CHECK) check.cpp

$(OUTPUT_ASM): asm.tab.o lexer.o main.o structures.o
	$(CC) $(CFLAGS) -o $(OUTPUT_ASM)\
		asm.tab.o lexer.o main.o structures.o $(LIBS)

main.o: main.cpp structures.h $(INCLUDEPATH)color.h
	$(CC) $(CFLAGS) -c main.cpp

asm.tab.o: asm.tab.hpp asm.tab.cpp $(INCLUDEPATH)color.h
	$(CC) $(CFLAGS) -c asm.tab.cpp

lexer.o: lexer.cpp asm.tab.hpp $(INCLUDEPATH)color.h
	$(CC) $(CFLAGS) -c lexer.cpp

asm.tab.cpp: asm.ypp structures.h $(INCLUDEPATH)types.h\
		$(INCLUDEPATH)instruction.h $(INCLUDEPATH)opcodes.h
	$(YACC) -tv asm.ypp

asm.tab.hpp: asm.ypp structures.h $(INCLUDEPATH)types.h\
		$(INCLUDEPATH)instruction.h $(INCLUDEPATH)opcodes.h
	$(YACC) -dtv asm.ypp

lexer.cpp: lexer.lex
	$(LEX) -olexer.cpp lexer.lex

structures.o: structures.cpp structures.h $(INCLUDEPATH)color.h
	$(CC) $(CFLAGS) -c structures.cpp

distclean:
	$(RM) structures.o
	$(RM) asm.tab.o asm.tab.cpp asm.tab.hpp
	$(RM) asm.output
	$(RM) lexer.o lexer.cpp
	$(RM) main.o
	$(RM) $(OUTPUT_CHECK)
	$(RM) $(OUTPUT_ASM)

