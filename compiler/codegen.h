/* Copyright 2005-2025 Varghese Mathew (Matt)
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
#include "sym.h"
#include <string>
#include <vector>
#include <unordered_map>

struct AsmOut {
  std::vector<std::string> lines;
  void emit(const std::string& s) { lines.push_back(s); }
};

struct CodeGen {
  // memory layout
  // Coconut examples typically begin/start at 1024.
  int codeBase = 1024;
  int curLabelId = 1;

  SymTab sym;

  // string pool
  struct StrEnt { std::string label; std::string s; };
  std::vector<StrEnt> strings;

  // compile entry
  std::string compile(const Program& p);

private:
  // Passes
  void build_symtabs(const Program& p);
  void gather_strings(const Program& p);

  // Codegen helpers
  std::string newLabel(const std::string& prefix);

  void emit_prologue(AsmOut& o, const std::string& fname, int frameBytes);
  void emit_epilogue(AsmOut& o, const std::string& fname, int frameBytes);

  // Function codegen
  int assign_locals_and_frame(const Func& f, std::unordered_map<std::string, VarInfo>& locals);
  void gen_func(AsmOut& o, const Func& f);

  // Statement / expr
  void gen_stmt(AsmOut& o, const Stmt& s,
                std::unordered_map<std::string, VarInfo>& locals,
                const std::string& funcEndLabel);

  // expr returns result in $t0
  void gen_expr(AsmOut& o, const Expr& e,
                std::unordered_map<std::string, VarInfo>& locals);

  void gen_lvalue_addr(AsmOut& o, const Expr& lhs,
                       std::unordered_map<std::string, VarInfo>& locals);

  // loads/stores
  void emit_load_imm(AsmOut& o, const std::string& rd, int32_t imm);

  // address-of-label (Coconut idiom: addi + lui with label)
  void emit_load_label_addr(AsmOut& o, const std::string& rd, const std::string& label);

  // memory access by address in reg
  void emit_load_word(AsmOut& o, const std::string& rd, const std::string& raddr);
  void emit_store_word(AsmOut& o, const std::string& rs, const std::string& raddr);

  // stack temp
  void push_t0(AsmOut& o);
  void pop_t1(AsmOut& o);

  // builtins
  bool gen_builtin_call(AsmOut& o, const Call& c,
                        std::unordered_map<std::string, VarInfo>& locals);
};
