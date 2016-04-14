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

MAKE		= make
CD		= cd
TESTDIR		= test/

all: 
	$(CD) $(TESTDIR); echo ; $(MAKE)
	@echo
	@echo "Ideally cd to test and do everything within that folder"

clean:
	$(CD) $(TESTDIR); echo ; $(MAKE) distclean
	@echo
	@echo "Ideally cd to test and do everything within that folder"