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
