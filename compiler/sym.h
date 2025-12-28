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

#pragma once
#include "ast.h"
#include <string>
#include <unordered_map>

struct VarInfo {
  Type t;
  bool isGlobal = false;
  int addr = 0;      // absolute address for globals
  int fpOffset = 0;  // negative offsets for locals (e.g., -4, -8)
};

struct FuncInfo {
  Type ret;
  std::vector<Param> params;
};

struct SymTab {
  std::unordered_map<std::string, VarInfo> globals;
  std::unordered_map<std::string, FuncInfo> funcs;
};
