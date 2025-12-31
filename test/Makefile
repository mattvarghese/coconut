 # Copyright 2005-2025 Varghese Mathew (Matt)
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

MAKE		= make
CD		= cd
RM		= rm
MIPSDIR		= ../mips/
ASMDIR		= ../asm/
IODIR		= ../io/

all: 
	@echo 
	@echo Making \'mips\'...
	$(CD) $(MIPSDIR); echo ; $(MAKE)
	@echo 
	@echo Making \'asm\'...
	$(CD) $(ASMDIR); echo ; $(MAKE)
	@echo 
	@echo Making \'io\'...
	$(CD) $(IODIR); echo ; $(MAKE)
	@echo

distclean:
	@echo 
	@echo Cleaning \'mips\'...
	$(CD) $(MIPSDIR); echo ; $(MAKE) distclean
	@echo 
	@echo cleaning \'asm\'...
	$(CD) $(ASMDIR); echo ; $(MAKE) distclean
	@echo 
	@echo Cleaning \'io\'...
	$(CD) $(IODIR); echo ; $(MAKE) distclean
	@echo

