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
#include <string>
#include <cstdint>
#include <vector>

enum class TokKind {
  End,

  Ident,
  IntLit,
  CharLit,
  StrLit,

  // keywords
  KwInt,
  KwChar,
  KwReturn,
  KwIf,
  KwElse,
  KwWhile,
  KwFor,

  // punctuation
  LParen, RParen,
  LBrace, RBrace,
  LBracket, RBracket,
  Comma,
  Semi,

  // operators
  Assign,        // =
  Plus, Minus, Star, Slash, Percent,
  Bang,          // !
  AndAnd, OrOr,  // && ||
  EqEq, NotEq,
  Lt, Gt, Le, Ge,
};

struct Token {
  TokKind kind{};
  std::string text;
  int line = 1;
  int col = 1;

  // For literals
  int64_t intVal = 0;
  char charVal = 0;
};

struct Lexer {
  explicit Lexer(std::string input);

  Token next();
  Token peek();

private:
  std::string _in;
  size_t _i = 0;
  int _line = 1;
  int _col = 1;

  Token _peeked;
  bool _hasPeeked = false;

  char cur() const;
  char get();
  bool eof() const;

  void skip_ws_and_comments();

  Token lex_ident_or_kw();
  Token lex_number();
  Token lex_char();
  Token lex_string();

  Token make(TokKind k, std::string t, int line, int col);
};
