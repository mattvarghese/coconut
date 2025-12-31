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
#include "lexer.h"
#include "ast.h"

struct Parser {
  explicit Parser(Lexer lex);

  Program parse_program();

private:
  Lexer _lex;
  Token _tok;

  void advance();
  bool accept(TokKind k);
  void expect(TokKind k, const std::string& msg);

  Type parse_type();
  // decls
  void parse_global_or_func(Program& p);

  // statements
  StmtPtr parse_stmt();
  std::unique_ptr<Block> parse_block();
  StmtPtr parse_vardecl_stmt(Type firstType);

  // expressions (Pratt)
  ExprPtr parse_expr();
  ExprPtr parse_assign();
  ExprPtr parse_or();
  ExprPtr parse_and();
  ExprPtr parse_eq();
  ExprPtr parse_rel();
  ExprPtr parse_add();
  ExprPtr parse_mul();
  ExprPtr parse_unary();
  ExprPtr parse_primary();

  int precedence(TokKind k) const;
};
