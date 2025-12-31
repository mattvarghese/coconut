# Coconut
**Coconut** is a Multi-threaded simulation of the pipeline of a MIPS Microprocessor (integer instructions only) replete with Memory Subsystem, Caches and their performance analysis, I/O device modules, an assembler, and a Small C compiler.

See accompanying **project-report.pdf** for additional information.

## Copyright
Copyright 2005-2025 **Varghese Mathew (Matt)**

Coconut is distributed under the **GNU General Public License**. See License below.

## Acknowledgements
The author expresses his heartfelt gratitude to **Prof. Dr. Priya Chandran** of the National Institute of Technology, Calicut, under whose guidance, this project first took shape.

## License
Coconut is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 
Coconut is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 
You should have received a copy of the GNU General Public License along with Coconut.  If not, see http://www.gnu.org/licenses/

## Using Coconut
Use of this distribution is subject to the GNU General Public License

You need to have 'c++', 'flex', 'bison' and 'POSIX' libraries installed on your computer to use this distribution.

The distribution has been tested and verified under the following platforms
 1. Redhat / linux 9 x86
 2. Ubuntu / Linux 15.10 x64
 3. Ubuntu MATE / Linux 15.10 armhf on **Raspberry Pi 2**
 4. Ubuntu MATE 24.04 amd64
 5. Ubuntu MATE 24.04 aarch64 on Raspberry Pi 400 & Raspberry Pi 5
 6. Ubuntu MATE 25.10 amd64

To use the distribution, **move into the 'test' directory** and run 'make'. If your terminal doesn't support colors, remove the flag *'-D__WITH_COLOR'* from each of the files 'mips/Makefile', 'asm/Makefile', 'io/Makefile'

Once the compilation completes without errors, Coconut supports running either:
1) Hand-written MIPS assembly via the assembler, or
2) C programs compiled via the SmallC compiler into Coconut MIPS assembly.

---

## Building
From the `test/` directory:

> make

This builds:
- `coconut` (the simulator)
- `asm` (the assembler)
- `dumbterminal` (I/O device)
- `smallc` (the SmallC compiler)

To cleanup the build:

> make distclean

---

## Running MIPS Assembly Programs
**Note:** the assembled output MUST be named `a.out` for Coconut to bootload it.

1. Assemble a `.mips` file:

> ./asm a.out {program.mips}

2. On one terminal, run the I/O device:

> ./dumbterminal

3. On another terminal, run the simulator:

> ./coconut

`./coconut` asks you to first pick the caches. Then it brings you to the prompt:

> mips >

---

## Running C Programs (SmallC Compiler)
C programs intended for SmallC must use the extension:

- `*.smallc.c`

The compiler produces Coconut assembly with extension:

- `*.smallc.mips`

### Example: compile + assemble + run
Given:

- `factorial.smallc.c`

1. Compile C → Coconut assembly:

> ./smallc factorial.smallc.c

This produces:

- `factorial.smallc.mips`

2. Assemble the generated `.smallc.mips` into `a.out`:

> ./asm a.out factorial.smallc.mips

3. Run I/O and simulator as usual:

Terminal 1:
> ./dumbterminal

Terminal 2:
> ./coconut

### Notes / SmallC v1 limitations
SmallC is intentionally small (teaching-focused). Current v1 features include:
- `int` and `char`
- local variables
- `if`, `while`, `for`
- basic arithmetic and comparisons
- arrays (represented as word-addressed storage)
- built-in I/O helpers:
  - `int getc()`   (reads one character)
  - `int putc(int c)` (writes one character)
  - `int puts(char* s)` / `puts("literal")` convenience

SmallC-generated programs typically end by returning from `main`, after which Coconut transfers control to a `HALT` loop in the generated assembly.

---

## Coconut Simulator Commands
The following commands are supported at the `mips >` prompt:
 1. 'h' or '?' shows the list of commands
 2. 'n' execute one clock cycle of the processor.
 3. 'c {number}' execute {number} number of clock cycles.
 4. 'p' print values of registers. (small 'p')
 5. 'P' print values of registers for registers whose value is not zero. (capital 'P')
 6. 'q' quit.
 7. 'm {address}' display the value stored at memory addres {address}.
 8. 'd {address}' display the value stored at 1-level data cache address {address}.
 9. 'i {address}' display the value stored at 1-level instruction cache address {address}.
 10. 's' display stastics for the caches.
 11. 'b {breakpoint no.} {break address}' set one of the 0-15 breakpoints. To unset a breakpoint, set its address as -1.
 12. 'B' view all breakpoints.

When closing the simulator, make sure to first quit out of Coconut by entering 'q' at the 'mips >' prompt, before you close 'dumbterminal' (using Ctrl+D). Otherwise, you may have to wait a bit before the sockets that the terminal binds to are released, before the 'dumbterminal' can bind to them again. 

To cleanup the compile, run 'make distclean' from the 'test/' folder, and remove any 'a.out' file.
Note: An example of virtual terminal mentioned above is an 'xterm' window. However, you can use any terminal.

---

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

### 'compiler' directory
Contains the source for the SmallC compiler, which compiles `*.smallc.c` programs into Coconut-compatible MIPS assembly `*.smallc.mips`.

### 'include' directory
Contains the header files that are made use of in all of the above sources.

### 'test' directory
This is where the programs are run from. Also contains a few `.mips` files which are sample assembly programs and a few  `*.smallc.c` programs which can be compiled to `*.smallc.mips`.  These programs run on the processor and illustrate its workings.

### 'sample_interactions' directory
This directory contains sample outputs procured from test runs of the processor.  This directory is NOT required for the application and may safely be deleted in part or in full.
