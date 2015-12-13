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
CFLAGS			= -g -Wold-style-cast -Wsign-promo -Wabi -D__WITH_COLOR
RM			= rm
LIBS			= -lpthread
INCLUDEPATH		= ../include/
OUTPUT_INOUTTEST	= ../test/dumbterminal
OUTPUT_OUTPUTDEV	= ../test/simplescreen
OUTPUT_INPUTDEV		= ../test/simplekeyboard

all: $(OUTPUT_INOUTTEST) $(OUTPUT_OUTPUTDEV) $(OUTPUT_INPUTDEV)

$(OUTPUT_INOUTTEST): dumbterminal.cpp $(INCLUDEPATH)types.h $(INCLUDEPATH)color.h
	$(CC) $(CFLAGS)\
		-o $(OUTPUT_INOUTTEST) dumbterminal.cpp $(LIBS)

$(OUTPUT_OUTPUTDEV): simplescreen.cpp $(INCLUDEPATH)types.h $(INCLUDEPATH)color.h
	$(CC) $(CFLAGS)\
		-o $(OUTPUT_OUTPUTDEV) simplescreen.cpp $(LIBS)

$(OUTPUT_INPUTDEV): simplekeyboard.cpp $(INCLUDEPATH)types.h $(INCLUDEPATH)color.h
	$(CC) $(CFLAGS)\
		-o $(OUTPUT_INPUTDEV) simplekeyboard.cpp $(LIBS)

distclean:
	$(RM) $(OUTPUT_INOUTTEST)
	$(RM) $(OUTPUT_OUTPUTDEV)
	$(RM) $(OUTPUT_INPUTDEV)
