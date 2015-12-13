# Coconut
**Coconut** is a Multi-threaded simulation of the pipeline of a MIPS-like Microprocessor (integer instructions only) replete with Memory Subsystem, Caches and their performance analysis, I/O device modules and an assembler.

See accompanying **project-report.pdf** for additional information.

## Copyright
Copyright 2005-2015 **Varghese Mathew (Matt)**

Coconut is distributed under the **GNU General Public License**. See License below.

## Acknowledgements
The author expresses his heartfelt gratitude to **Prof. Dr. Priya Chandran** of the National Institute of Technology, Calicut, under whose guidance, this project first took shape.

## License
> Coconut is free software: you can redistribute it and/or modify
> it under the terms of the GNU General Public License as published by
> the Free Software Foundation, either version 3 of the License, or
> (at your option) any later version.
> 
> Coconut is distributed in the hope that it will be useful,
> but WITHOUT ANY WARRANTY; without even the implied warranty of
> MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
> GNU General Public License for more details.
> 
> You should have received a copy of the GNU General Public License
> along with Coconut.  If not, see http://www.gnu.org/licenses/

## Using Coconut
Use of this distribution is subject to the GNU General Public License

You need to have 'c++', 'flex', 'bison' and 'POSIX' libraries installed on your computer to use this distribution.

The distribution has been tested and verified under the following platforms
 1. Redhat / linux 9 x86
 2. Ubuntu / Linux 15.10 x64
 3. Ubuntu MATE / Linux 15.10 armhf on Raspberry Pi 2

To use the distribution, **move into the 'test' directory** and run 'make'. If your terminal doesn't support colors, remove the flag *'-D__WITH_COLOR'* from each of the files 'mips/Makefile', 'asm/Makefile', 'io/Makefile'

Once the compilation completes without errors, run (Note - the file MUST be named a.out for Coconut to bootload it.)
> ./asm a.out {.mips file}

to assemble a mips program for use by the simulatedprocessor.

Then on one virtual terminal (xterm) run 
> ./dumbterminal

and on another run 
> ./coconut

'./coconut' asks you to first pick the caches. Then it brings you to the a prompt 
> mips > 

where you need to issue commands. The following commands are supported
 1. 'n' execute one clock cycle of the processor.
 2. 'c {number}' execute {number} number of clock cycles.
 3. 'p' print values of registers. (small 'p')
 4. 'P' print values of registers for registers whose value is not zero. (capital 'P')
 5. 'q' quit.
 6. 'm {address}' display the value stored at memory addres {address}.
 7. 'd {address}' display the value stored at 1-level data cache address {address}.
 8. 'i {address}' display the value stored at 1-level instruction cache address {address}.
 9. 's' display stastics for the caches.
 10. 'b {breakpoint no.} {break address}' set one of the 0-15 breakpoints. To unset a breakpoint, set its address as -1.
 11. 'B' view all breakpoints.

When closing the simulator, make sure to first quit out of Coconut by entering 'q' at the 'mips >' prompt, before you close 'dumbterminal' (using Ctrl+C). Otherwise, you may have to wait a bit before the sockets that the terminal binds to are released, before the 'dumbterminal' can bind to them again. 

Note: An example of virtual terminal mentioned above is an 'xterm' window. Howevr, you can use any terminal.

## Distribution Structure
### 'asm' directory
Contains the source for a simple assembler for the simulated processor.

### 'mips' directory
Contains the source for the processor simulation, memory subsystem and caches - the main source code of Coconut.

### 'io' directory
Contains the source for three simple i/o devices
 1. A simple keyboard.
 2. A simple screen ( a character terminal ).
 3. A dumbterminal consisting of a simple screen and a simple keyboard.

### 'include' directory
Contains the header files that are made use of in all of the above three sources

### 'test' directory
This is where the programs are run from.  Also it contains a few .mips files which are sample assembly programs which run on the processor and illustrate its working.

### 'sample_interactions' directory
This directory contains sample outputs procured from test runs of the processor.  This directory is NOT required for the application and may safely be deleted in part or in full.
