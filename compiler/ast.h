#pragma once
#include <string>
#include <vector>
#include <memory>
#include <optional>

enum class BaseType { Int, Char, Void };

struct Type {
  BaseType base = BaseType::Int;
  bool isArray = false;
  int arrayLen = 0; // if isArray
  int sizeBytes() const {
    int elem = (base == BaseType::Char) ? 1 : 4;
    if (isArray) return elem * arrayLen;
    return elem;
  }
};

struct Expr {
  virtual ~Expr() = default;
};
using ExprPtr = std::unique_ptr<Expr>;

struct IntLit : Expr { int v; explicit IntLit(int x): v(x){} };
struct CharLitE : Expr { char v; explicit CharLitE(char x): v(x){} };
struct StrLitE : Expr { std::string s; explicit StrLitE(std::string x): s(std::move(x)){} };
struct VarRef : Expr { std::string name; explicit VarRef(std::string n): name(std::move(n)){} };
struct IndexRef : Expr { std::string name; ExprPtr idx; IndexRef(std::string n, ExprPtr i): name(std::move(n)), idx(std::move(i)){} };

enum class BinOp {
  Add, Sub, Mul, Div, Mod,
  Eq, Ne, Lt, Le, Gt, Ge,
  And, Or
};
struct Binary : Expr { BinOp op; ExprPtr a,b; Binary(BinOp o, ExprPtr x, ExprPtr y): op(o), a(std::move(x)), b(std::move(y)){} };

enum class UnOp { Neg, Not };
struct Unary : Expr { UnOp op; ExprPtr e; Unary(UnOp o, ExprPtr x): op(o), e(std::move(x)){} };

struct Assign : Expr {
  // lhs: VarRef or IndexRef only
  ExprPtr lhs;
  ExprPtr rhs;
  Assign(ExprPtr l, ExprPtr r): lhs(std::move(l)), rhs(std::move(r)){}
};

struct Call : Expr {
  std::string callee;
  std::vector<ExprPtr> args;
  explicit Call(std::string c): callee(std::move(c)){}
};

struct Stmt { virtual ~Stmt() = default; };
using StmtPtr = std::unique_ptr<Stmt>;

struct Block : Stmt { std::vector<StmtPtr> stmts; };

struct VarDecl : Stmt {
  Type t;
  std::string name;
  std::optional<ExprPtr> init; // optional initializer
  VarDecl(Type ty, std::string n): t(ty), name(std::move(n)){}
};

struct ExprStmt : Stmt { ExprPtr e; explicit ExprStmt(ExprPtr x): e(std::move(x)){} };
struct Return : Stmt { std::optional<ExprPtr> e; Return() = default; explicit Return(ExprPtr x): e(std::move(x)){} };

struct If : Stmt {
  ExprPtr cond;
  StmtPtr thenS;
  StmtPtr elseS; // may be null
  If(ExprPtr c, StmtPtr t, StmtPtr e): cond(std::move(c)), thenS(std::move(t)), elseS(std::move(e)){}
};

struct While : Stmt {
  ExprPtr cond;
  StmtPtr body;
  While(ExprPtr c, StmtPtr b): cond(std::move(c)), body(std::move(b)){}
};

struct For : Stmt {
  StmtPtr init;        // may be null
  ExprPtr cond;        // may be null (treated as true)
  ExprPtr step;        // may be null
  StmtPtr body;
  For(StmtPtr i, ExprPtr c, ExprPtr s, StmtPtr b): init(std::move(i)), cond(std::move(c)), step(std::move(s)), body(std::move(b)){}
};

struct Param { Type t; std::string name; };

struct Func {
  Type ret;
  std::string name;
  std::vector<Param> params;
  std::unique_ptr<Block> body;
};

struct GlobalVar {
  Type t;
  std::string name;
  std::optional<ExprPtr> init; // int/char only for now
};

struct Program {
  std::vector<GlobalVar> globals;
  std::vector<Func> funcs;
};
