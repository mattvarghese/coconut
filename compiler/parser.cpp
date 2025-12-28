#include "parser.h"
#include "util.h"

Parser::Parser(Lexer lex) : _lex(std::move(lex)) {
  _tok = _lex.next();
}

void Parser::advance() { _tok = _lex.next(); }

bool Parser::accept(TokKind k) {
  if (_tok.kind == k) { advance(); return true; }
  return false;
}

void Parser::expect(TokKind k, const std::string& msg) {
  if (_tok.kind != k) {
    throw CompileError(msg + " at line " + std::to_string(_tok.line) + " col " + std::to_string(_tok.col));
  }
  advance();
}

Type Parser::parse_type() {
  Type t;
  if (accept(TokKind::KwInt)) t.base = BaseType::Int;
  else if (accept(TokKind::KwChar)) t.base = BaseType::Char;
  else throw CompileError("Expected type (int/char)");

  return t;
}

Program Parser::parse_program() {
  Program p;
  while (_tok.kind != TokKind::End) {
    parse_global_or_func(p);
  }
  return p;
}

void Parser::parse_global_or_func(Program& p) {
  Type t = parse_type();
  if (_tok.kind != TokKind::Ident) throw CompileError("Expected identifier");
  std::string name = _tok.text;
  advance();

  // function?
  if (accept(TokKind::LParen)) {
    Func f;
    f.ret = t;
    f.name = name;

    if (!accept(TokKind::RParen)) {
      while (true) {
        Type pt = parse_type();
        if (_tok.kind != TokKind::Ident) throw CompileError("Expected parameter name");
        std::string pn = _tok.text;
        advance();

        // arrays as params not supported in v1 (keep simple)
        f.params.push_back({pt, pn});

        if (accept(TokKind::Comma)) continue;
        expect(TokKind::RParen, "Expected ')'");
        break;
      }
    }

    f.body = parse_block();
    p.funcs.push_back(std::move(f));
    return;
  }

  // global var (maybe array)
  GlobalVar gv;
  gv.t = t;
  gv.name = name;

  if (accept(TokKind::LBracket)) {
    if (_tok.kind != TokKind::IntLit) throw CompileError("Array length must be int literal");
    gv.t.isArray = true;
    gv.t.arrayLen = (int)_tok.intVal;
    advance();
    expect(TokKind::RBracket, "Expected ']'");
  }

  if (accept(TokKind::Assign)) {
    // only literal init supported for globals in v1
    ExprPtr e = parse_primary();
    gv.init = std::move(e);
  }

  expect(TokKind::Semi, "Expected ';' after global declaration");
  p.globals.push_back(std::move(gv));
}

std::unique_ptr<Block> Parser::parse_block() {
  expect(TokKind::LBrace, "Expected '{'");
  auto b = std::make_unique<Block>();
  while (_tok.kind != TokKind::RBrace) {
    b->stmts.push_back(parse_stmt());
  }
  expect(TokKind::RBrace, "Expected '}'");
  return b;
}

StmtPtr Parser::parse_vardecl_stmt(Type firstType) {
  // firstType already consumed
  if (_tok.kind != TokKind::Ident) throw CompileError("Expected variable name");
  std::string name = _tok.text;
  advance();

  VarDecl* d = new VarDecl(firstType, name);

  if (accept(TokKind::LBracket)) {
    if (_tok.kind != TokKind::IntLit) throw CompileError("Array length must be int literal");
    d->t.isArray = true;
    d->t.arrayLen = (int)_tok.intVal;
    advance();
    expect(TokKind::RBracket, "Expected ']'");
  }

  if (accept(TokKind::Assign)) {
    ExprPtr e = parse_expr();
    d->init = std::move(e);
  }

  expect(TokKind::Semi, "Expected ';' after declaration");
  return StmtPtr(d);
}

StmtPtr Parser::parse_stmt() {
  // block
  if (_tok.kind == TokKind::LBrace) return parse_block();

  // declarations
  if (_tok.kind == TokKind::KwInt || _tok.kind == TokKind::KwChar) {
    Type t = parse_type();
    return parse_vardecl_stmt(t);
  }

  // return
  if (accept(TokKind::KwReturn)) {
    if (accept(TokKind::Semi)) return StmtPtr(new Return());
    ExprPtr e = parse_expr();
    expect(TokKind::Semi, "Expected ';' after return");
    return StmtPtr(new Return(std::move(e)));
  }

  // if
  if (accept(TokKind::KwIf)) {
    expect(TokKind::LParen, "Expected '(' after if");
    ExprPtr c = parse_expr();
    expect(TokKind::RParen, "Expected ')' after if condition");
    StmtPtr thenS = parse_stmt();
    StmtPtr elseS;
    if (accept(TokKind::KwElse)) elseS = parse_stmt();
    return StmtPtr(new If(std::move(c), std::move(thenS), std::move(elseS)));
  }

  // while
  if (accept(TokKind::KwWhile)) {
    expect(TokKind::LParen, "Expected '(' after while");
    ExprPtr c = parse_expr();
    expect(TokKind::RParen, "Expected ')' after while condition");
    StmtPtr body = parse_stmt();
    return StmtPtr(new While(std::move(c), std::move(body)));
  }

  // for
  if (accept(TokKind::KwFor)) {
    expect(TokKind::LParen, "Expected '(' after for");

    StmtPtr init;
    if (!accept(TokKind::Semi)) {
      if (_tok.kind == TokKind::KwInt || _tok.kind == TokKind::KwChar) {
        Type t = parse_type();
        init = parse_vardecl_stmt(t);
      } else {
        ExprPtr e = parse_expr();
        expect(TokKind::Semi, "Expected ';' in for");
        init = StmtPtr(new ExprStmt(std::move(e)));
      }
    }

    ExprPtr cond;
    if (!accept(TokKind::Semi)) {
      cond = parse_expr();
      expect(TokKind::Semi, "Expected ';' in for");
    }

    ExprPtr step;
    if (!accept(TokKind::RParen)) {
      step = parse_expr();
      expect(TokKind::RParen, "Expected ')' after for");
    }

    StmtPtr body = parse_stmt();
    return StmtPtr(new For(std::move(init), std::move(cond), std::move(step), std::move(body)));
  }

  // expression statement
  ExprPtr e = parse_expr();
  expect(TokKind::Semi, "Expected ';' after expression");
  return StmtPtr(new ExprStmt(std::move(e)));
}

// expression parsing
ExprPtr Parser::parse_expr() { return parse_assign(); }

ExprPtr Parser::parse_assign() {
  ExprPtr lhs = parse_or();
  if (accept(TokKind::Assign)) {
    ExprPtr rhs = parse_assign();
    return ExprPtr(new Assign(std::move(lhs), std::move(rhs)));
  }
  return lhs;
}

ExprPtr Parser::parse_or() {
  ExprPtr e = parse_and();
  while (accept(TokKind::OrOr)) {
    ExprPtr r = parse_and();
    e = ExprPtr(new Binary(BinOp::Or, std::move(e), std::move(r)));
  }
  return e;
}

ExprPtr Parser::parse_and() {
  ExprPtr e = parse_eq();
  while (accept(TokKind::AndAnd)) {
    ExprPtr r = parse_eq();
    e = ExprPtr(new Binary(BinOp::And, std::move(e), std::move(r)));
  }
  return e;
}

ExprPtr Parser::parse_eq() {
  ExprPtr e = parse_rel();
  while (true) {
    if (accept(TokKind::EqEq)) {
      ExprPtr r = parse_rel();
      e = ExprPtr(new Binary(BinOp::Eq, std::move(e), std::move(r)));
    } else if (accept(TokKind::NotEq)) {
      ExprPtr r = parse_rel();
      e = ExprPtr(new Binary(BinOp::Ne, std::move(e), std::move(r)));
    } else break;
  }
  return e;
}

ExprPtr Parser::parse_rel() {
  ExprPtr e = parse_add();
  while (true) {
    if (accept(TokKind::Lt)) { ExprPtr r = parse_add(); e = ExprPtr(new Binary(BinOp::Lt, std::move(e), std::move(r))); }
    else if (accept(TokKind::Le)) { ExprPtr r = parse_add(); e = ExprPtr(new Binary(BinOp::Le, std::move(e), std::move(r))); }
    else if (accept(TokKind::Gt)) { ExprPtr r = parse_add(); e = ExprPtr(new Binary(BinOp::Gt, std::move(e), std::move(r))); }
    else if (accept(TokKind::Ge)) { ExprPtr r = parse_add(); e = ExprPtr(new Binary(BinOp::Ge, std::move(e), std::move(r))); }
    else break;
  }
  return e;
}

ExprPtr Parser::parse_add() {
  ExprPtr e = parse_mul();
  while (true) {
    if (accept(TokKind::Plus)) { ExprPtr r = parse_mul(); e = ExprPtr(new Binary(BinOp::Add, std::move(e), std::move(r))); }
    else if (accept(TokKind::Minus)) { ExprPtr r = parse_mul(); e = ExprPtr(new Binary(BinOp::Sub, std::move(e), std::move(r))); }
    else break;
  }
  return e;
}

ExprPtr Parser::parse_mul() {
  ExprPtr e = parse_unary();
  while (true) {
    if (accept(TokKind::Star)) { ExprPtr r = parse_unary(); e = ExprPtr(new Binary(BinOp::Mul, std::move(e), std::move(r))); }
    else if (accept(TokKind::Slash)) { ExprPtr r = parse_unary(); e = ExprPtr(new Binary(BinOp::Div, std::move(e), std::move(r))); }
    else if (accept(TokKind::Percent)) { ExprPtr r = parse_unary(); e = ExprPtr(new Binary(BinOp::Mod, std::move(e), std::move(r))); }
    else break;
  }
  return e;
}

ExprPtr Parser::parse_unary() {
  if (accept(TokKind::Minus)) return ExprPtr(new Unary(UnOp::Neg, parse_unary()));
  if (accept(TokKind::Bang)) return ExprPtr(new Unary(UnOp::Not, parse_unary()));
  return parse_primary();
}

ExprPtr Parser::parse_primary() {
  if (_tok.kind == TokKind::IntLit) {
    int v = (int)_tok.intVal;
    advance();
    return ExprPtr(new IntLit(v));
  }
  if (_tok.kind == TokKind::CharLit) {
    char v = _tok.charVal;
    advance();
    return ExprPtr(new CharLitE(v));
  }
  if (_tok.kind == TokKind::StrLit) {
    std::string s = _tok.text; // lexer stores decoded string in text
    advance();
    return ExprPtr(new StrLitE(std::move(s)));
  }

  if (_tok.kind == TokKind::Ident) {
    std::string name = _tok.text;
    advance();

    // call?
    if (accept(TokKind::LParen)) {
      auto c = std::make_unique<Call>(name);
      if (!accept(TokKind::RParen)) {
        while (true) {
          c->args.push_back(parse_expr());
          if (accept(TokKind::Comma)) continue;
          expect(TokKind::RParen, "Expected ')'");
          break;
        }
      }
      return c;
    }

    // array indexing?
    if (accept(TokKind::LBracket)) {
      ExprPtr idx = parse_expr();
      expect(TokKind::RBracket, "Expected ']'");
      return ExprPtr(new IndexRef(name, std::move(idx)));
    }

    return ExprPtr(new VarRef(name));
  }

  if (accept(TokKind::LParen)) {
    ExprPtr e = parse_expr();
    expect(TokKind::RParen, "Expected ')'");
    return e;
  }

  throw CompileError("Unexpected token in expression");
}
