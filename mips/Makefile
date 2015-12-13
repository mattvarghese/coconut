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

CC		= g++
CFLAGS		= -g -Wsign-promo -Wold-style-cast -Wabi -D__WITH_COLOR
RM		= rm
LIBS		= -lpthread
INCLUDEPATH	= ../include/
OUTPUT_MIPS	= ../test/coconut

all: $(OUTPUT_MIPS)

$(OUTPUT_MIPS): main.o memory.o simple_cache.o portmanager.o processor.o pclock.o\
		pstage0.o pstage1.o pstage2.o pstage3.o pstage4.o latch.o
	$(CC) $(CFLAGS) -o $(OUTPUT_MIPS)\
		main.o memory.o simple_cache.o portmanager.o processor.o pclock.o\
		pstage0.o pstage1.o pstage2.o pstage3.o pstage4.o latch.o $(LIBS)

main.o: main.cpp processor.h memory.h portmanager.h simple_cache.h $(INCLUDEPATH)color.h
	$(CC) $(CFLAGS) -c main.cpp
	
simple_cache.o: simple_cache.h memory.h simple_cache.cpp $(INCLUDEPATH)instruction.h\
		$(INCLUDEPATH)color.h
	$(CC) $(CFLAGS) -c simple_cache.cpp	

memory.o: memory.h memory.cpp $(INCLUDEPATH)types.h $(INCLUDEPATH)instruction.h\
		$(INCLUDEPATH)opcodes.h $(INCLUDEPATH)color.h
	$(CC) $(CFLAGS) -c memory.cpp

portmanager.o: portmanager.h portmanager.cpp $(INCLUDEPATH)types.h $(INCLUDEPATH)color.h
	$(CC) $(CFLAGS) -c portmanager.cpp

latch.o : latch.h latch.cpp
	$(CC) $(CFLAGS) -c latch.cpp

processor.o: processor.h processor.cpp memory.h portmanager.h latch.h\
		$(INCLUDEPATH)types.h $(INCLUDEPATH)instruction.h\
		$(INCLUDEPATH)opcodes.h $(INCLUDEPATH)color.h
	$(CC) $(CFLAGS) -c processor.cpp
	
pclock.o: processor.h pclock.cpp memory.h portmanager.h latch.h\
		$(INCLUDEPATH)types.h $(INCLUDEPATH)instruction.h\
		$(INCLUDEPATH)opcodes.h $(INCLUDEPATH)color.h
	$(CC) $(CFLAGS) -c pclock.cpp

pstage0.o: processor.h pstage0.cpp memory.h portmanager.h latch.h\
		$(INCLUDEPATH)types.h $(INCLUDEPATH)instruction.h\
		$(INCLUDEPATH)opcodes.h $(INCLUDEPATH)color.h
	$(CC) $(CFLAGS) -c pstage0.cpp

pstage1.o: processor.h pstage1.cpp memory.h portmanager.h latch.h\
		$(INCLUDEPATH)types.h $(INCLUDEPATH)instruction.h\
		$(INCLUDEPATH)opcodes.h $(INCLUDEPATH)color.h
	$(CC) $(CFLAGS) -c pstage1.cpp

pstage2.o: processor.h pstage2.cpp memory.h portmanager.h latch.h\
		$(INCLUDEPATH)types.h $(INCLUDEPATH)instruction.h\
		$(INCLUDEPATH)opcodes.h $(INCLUDEPATH)color.h
	$(CC) $(CFLAGS) -c pstage2.cpp

pstage3.o: processor.h pstage3.cpp memory.h portmanager.h latch.h\
		$(INCLUDEPATH)types.h $(INCLUDEPATH)instruction.h\
		$(INCLUDEPATH)opcodes.h $(INCLUDEPATH)color.h
	$(CC) $(CFLAGS) -c pstage3.cpp

pstage4.o: processor.h pstage4.cpp memory.h portmanager.h latch.h\
		$(INCLUDEPATH)types.h $(INCLUDEPATH)instruction.h\
		$(INCLUDEPATH)opcodes.h $(INCLUDEPATH)color.h
	$(CC) $(CFLAGS) -c pstage4.cpp

distclean:
	$(RM) $(OUTPUT_MIPS)
	$(RM) main.o 
	$(RM) memory.o 
	$(RM) portmanager.o  
	$(RM) processor.o 
	$(RM) latch.o 
	$(RM) pclock.o 
	$(RM) pstage0.o 
	$(RM) pstage1.o 
	$(RM) pstage2.o 
	$(RM) pstage3.o 
	$(RM) pstage4.o
	$(RM) simple_cache.o

