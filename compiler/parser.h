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
