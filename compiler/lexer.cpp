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
 
#include "lexer.h"
#include "util.h"

Lexer::Lexer(std::string input) : _in(std::move(input)) {}

char Lexer::cur() const {
  if (_i >= _in.size()) return '\0';
  return _in[_i];
}

bool Lexer::eof() const { return _i >= _in.size(); }

char Lexer::get() {
  if (eof()) return '\0';
  char c = _in[_i++];
  if (c == '\n') { _line++; _col = 1; }
  else { _col++; }
  return c;
}

Token Lexer::make(TokKind k, std::string t, int line, int col) {
  Token tok;
  tok.kind = k;
  tok.text = std::move(t);
  tok.line = line;
  tok.col = col;
  return tok;
}

void Lexer::skip_ws_and_comments() {
  while (!eof()) {
    char c = cur();
    // whitespace
    if (c == ' ' || c == '\t' || c == '\r' || c == '\n') {
      get();
      continue;
    }
    // // comment
    if (c == '/' && _i + 1 < _in.size() && _in[_i + 1] == '/') {
      while (!eof() && get() != '\n') {}
      continue;
    }
    // /* comment */
    if (c == '/' && _i + 1 < _in.size() && _in[_i + 1] == '*') {
      get(); get();
      while (!eof()) {
        if (cur() == '*' && _i + 1 < _in.size() && _in[_i + 1] == '/') {
          get(); get();
          break;
        }
        get();
      }
      continue;
    }
    break;
  }
}

Token Lexer::lex_ident_or_kw() {
  int line = _line, col = _col;
  std::string s;
  while (!eof() && is_alnum(cur())) s.push_back(get());

  if (s == "int") return make(TokKind::KwInt, s, line, col);
  if (s == "char") return make(TokKind::KwChar, s, line, col);
  if (s == "return") return make(TokKind::KwReturn, s, line, col);
  if (s == "if") return make(TokKind::KwIf, s, line, col);
  if (s == "else") return make(TokKind::KwElse, s, line, col);
  if (s == "while") return make(TokKind::KwWhile, s, line, col);
  if (s == "for") return make(TokKind::KwFor, s, line, col);

  return make(TokKind::Ident, s, line, col);
}

Token Lexer::lex_number() {
  int line = _line, col = _col;
  std::string s;
  while (!eof() && is_digit(cur())) s.push_back(get());
  Token t = make(TokKind::IntLit, s, line, col);
  t.intVal = std::stoll(s);
  return t;
}

Token Lexer::lex_char() {
  int line = _line, col = _col;
  std::string raw;
  raw.push_back(get()); // '

  if (eof()) throw CompileError("Unterminated char literal");
  char c = get();
  char val = 0;

  if (c == '\\') {
    if (eof()) throw CompileError("Bad escape in char literal");
    char e = get();
    switch (e) {
      case 'n': val = '\n'; break;
      case 't': val = '\t'; break;
      case 'r': val = '\r'; break;
      case '\\': val = '\\'; break;
      case '\'': val = '\''; break;
      case '0': val = '\0'; break;
      default: throw CompileError("Unknown escape in char literal");
    }
  } else {
    val = c;
  }

  if (eof() || get() != '\'') throw CompileError("Unterminated char literal");

  raw.push_back('?'); raw.push_back('\''); // minimal text
  Token t = make(TokKind::CharLit, raw, line, col);
  t.charVal = val;
  t.intVal = (unsigned char)val;
  return t;
}

Token Lexer::lex_string() {
  int line = _line, col = _col;
  get(); // "
  std::string out;
  while (!eof()) {
    char c = get();
    if (c == '"') break;
    if (c == '\\') {
      if (eof()) throw CompileError("Bad escape in string literal");
      char e = get();
      switch (e) {
        case 'n': out.push_back('\n'); break;
        case 't': out.push_back('\t'); break;
        case 'r': out.push_back('\r'); break;
        case '\\': out.push_back('\\'); break;
        case '"': out.push_back('"'); break;
        case '0': out.push_back('\0'); break;
        default: throw CompileError("Unknown escape in string literal");
      }
    } else {
      out.push_back(c);
    }
  }
  Token t = make(TokKind::StrLit, out, line, col);
  return t;
}

Token Lexer::next() {
  if (_hasPeeked) { _hasPeeked = false; return _peeked; }
  skip_ws_and_comments();
  if (eof()) return make(TokKind::End, "", _line, _col);

  int line = _line, col = _col;
  char c = cur();

  // identifiers / keywords
  if (is_alpha(c)) return lex_ident_or_kw();
  // numbers
  if (is_digit(c)) return lex_number();
  // char / string
  if (c == '\'') return lex_char();
  if (c == '"') return lex_string();

  // punctuation / operators
  auto two = [&](char a, char b) {
    return (!eof() && cur() == a && _i + 1 < _in.size() && _in[_i + 1] == b);
  };

  if (two('&','&')) { get(); get(); return make(TokKind::AndAnd, "&&", line, col); }
  if (two('|','|')) { get(); get(); return make(TokKind::OrOr, "||", line, col); }
  if (two('=','=')) { get(); get(); return make(TokKind::EqEq, "==", line, col); }
  if (two('!','=')) { get(); get(); return make(TokKind::NotEq, "!=", line, col); }
  if (two('<','=')) { get(); get(); return make(TokKind::Le, "<=", line, col); }
  if (two('>','=')) { get(); get(); return make(TokKind::Ge, ">=", line, col); }

  switch (c) {
    case '(': get(); return make(TokKind::LParen, "(", line, col);
    case ')': get(); return make(TokKind::RParen, ")", line, col);
    case '{': get(); return make(TokKind::LBrace, "{", line, col);
    case '}': get(); return make(TokKind::RBrace, "}", line, col);
    case '[': get(); return make(TokKind::LBracket, "[", line, col);
    case ']': get(); return make(TokKind::RBracket, "]", line, col);
    case ',': get(); return make(TokKind::Comma, ",", line, col);
    case ';': get(); return make(TokKind::Semi, ";", line, col);

    case '=': get(); return make(TokKind::Assign, "=", line, col);
    case '+': get(); return make(TokKind::Plus, "+", line, col);
    case '-': get(); return make(TokKind::Minus, "-", line, col);
    case '*': get(); return make(TokKind::Star, "*", line, col);
    case '/': get(); return make(TokKind::Slash, "/", line, col);
    case '%': get(); return make(TokKind::Percent, "%", line, col);
    case '!': get(); return make(TokKind::Bang, "!", line, col);
    case '<': get(); return make(TokKind::Lt, "<", line, col);
    case '>': get(); return make(TokKind::Gt, ">", line, col);
  }

  throw CompileError("Unexpected character: " + std::string(1, c));
}

Token Lexer::peek() {
  if (!_hasPeeked) {
    _peeked = next();
    _hasPeeked = true;
  }
  return _peeked;
}
