#include "codegen.h"
#include "util.h"
#include <sstream>
#include <functional>
#include <cstdint>

static inline int align4(int x) { return (x + 3) & ~3; }

std::string CodeGen::newLabel(const std::string& prefix) {
  return prefix + "_" + std::to_string(curLabelId++);
}

void CodeGen::build_symtabs(const Program& p) {
  sym.funcs.clear();
  sym.globals.clear();

  // functions
  for (const auto& f : p.funcs) {
    FuncInfo fi;
    fi.ret = f.ret;
    fi.params = f.params;
    sym.funcs[f.name] = fi;
  }
  // globals (addresses not needed for label-based formatting; still track type)
  for (const auto& g : p.globals) {
    VarInfo vi;
    vi.t = g.t;
    vi.isGlobal = true;
    sym.globals[g.name] = vi;
  }
}

void CodeGen::gather_strings(const Program& p) {
  strings.clear();
  int sid = 1;

  auto noteStr = [&](const std::string& s) {
    for (auto& e : strings) if (e.s == s) return; // dedupe
    StrEnt ent;
    ent.label = "STR_" + std::to_string(sid++);
    ent.s = s;
    strings.push_back(std::move(ent));
  };

  std::function<void(const Expr&)> scanE = [&](const Expr& e) {
    if (auto* s = dynamic_cast<const StrLitE*>(&e)) { noteStr(s->s); return; }
    if (auto* b = dynamic_cast<const Binary*>(&e)) { scanE(*b->a); scanE(*b->b); return; }
    if (auto* u = dynamic_cast<const Unary*>(&e)) { scanE(*u->e); return; }
    if (auto* a = dynamic_cast<const Assign*>(&e)) { scanE(*a->lhs); scanE(*a->rhs); return; }
    if (auto* c = dynamic_cast<const Call*>(&e)) { for (auto& arg : c->args) scanE(*arg); return; }
    if (auto* i = dynamic_cast<const IndexRef*>(&e)) { scanE(*i->idx); return; }
  };

  std::function<void(const Stmt&)> scanS = [&](const Stmt& s) {
    if (auto* b = dynamic_cast<const Block*>(&s)) { for (auto& st : b->stmts) scanS(*st); return; }
    if (auto* v = dynamic_cast<const VarDecl*>(&s)) { if (v->init) scanE(*(*v->init)); return; }
    if (auto* e = dynamic_cast<const ExprStmt*>(&s)) { scanE(*e->e); return; }
    if (auto* r = dynamic_cast<const Return*>(&s)) { if (r->e) scanE(*(*r->e)); return; }
    if (auto* i = dynamic_cast<const If*>(&s)) { scanE(*i->cond); scanS(*i->thenS); if (i->elseS) scanS(*i->elseS); return; }
    if (auto* w = dynamic_cast<const While*>(&s)) { scanE(*w->cond); scanS(*w->body); return; }
    if (auto* f = dynamic_cast<const For*>(&s)) {
      if (f->init) scanS(*f->init);
      if (f->cond) scanE(*f->cond);
      if (f->step) scanE(*f->step);
      scanS(*f->body);
      return;
    }
  };

  for (const auto& fn : p.funcs) scanS(*fn.body);
}

void CodeGen::emit_load_imm(AsmOut& o, const std::string& rd, int32_t imm) {
  if (imm >= -32768 && imm <= 32767) {
    o.emit("\taddi\t" + rd + ", $zero, " + std::to_string(imm));
  } else {
    int32_t hi = (imm >> 16) & 0xFFFF;
    int32_t lo = imm & 0xFFFF;
    o.emit("\tlui\t" + rd + ", " + std::to_string(hi));
    o.emit("\tori\t" + rd + ", " + rd + ", " + std::to_string(lo));
  }
}

void CodeGen::emit_load_label_addr(AsmOut& o, const std::string& rd, const std::string& label) {
  // Coconut idiom seen in your sample:
  //   addi rd, $zero, LABEL
  //   lui  rd, LABEL
  o.emit("\taddi\t" + rd + ", $zero, " + label);
  o.emit("\tlui\t" + rd + ", " + label);
}

void CodeGen::emit_load_word(AsmOut& o, const std::string& rd, const std::string& raddr) {
  o.emit("\tlw\t" + rd + ", 0(" + raddr + ")");
}
void CodeGen::emit_store_word(AsmOut& o, const std::string& rs, const std::string& raddr) {
  o.emit("\tsw\t" + rs + ", 0(" + raddr + ")");
}

void CodeGen::push_t0(AsmOut& o) {
  o.emit("\taddi\t$sp, $sp, -4");
  o.emit("\tsw\t$t0, 0($sp)");
}
void CodeGen::pop_t1(AsmOut& o) {
  o.emit("\tlw\t$t1, 0($sp)");
  o.emit("\taddi\t$sp, $sp, 4");
}

int CodeGen::assign_locals_and_frame(const Func& f, std::unordered_map<std::string, VarInfo>& locals) {
  int offset = 0;

  // Params: allocate stack slots so we can treat them like locals.
  for (const auto& p : f.params) {
    VarInfo vi;
    vi.t = p.t;
    vi.isGlobal = false;
    offset += align4(vi.t.sizeBytes());
    vi.fpOffset = -offset;
    locals[p.name] = vi;
  }

  std::function<void(const Stmt&)> scan = [&](const Stmt& s) {
    if (auto* b = dynamic_cast<const Block*>(&s)) { for (auto& st : b->stmts) scan(*st); return; }
    if (auto* v = dynamic_cast<const VarDecl*>(&s)) {
      VarInfo vi;
      vi.t = v->t;
      vi.isGlobal = false;
      offset += align4(vi.t.sizeBytes());
      vi.fpOffset = -offset;
      locals[v->name] = vi;
      return;
    }
    if (auto* i = dynamic_cast<const If*>(&s)) { scan(*i->thenS); if (i->elseS) scan(*i->elseS); return; }
    if (auto* w = dynamic_cast<const While*>(&s)) { scan(*w->body); return; }
    if (auto* f2 = dynamic_cast<const For*>(&s)) { if (f2->init) scan(*f2->init); scan(*f2->body); return; }
  };

  scan(*f.body);

  int localsBytes = align4(offset);
  int saveBytes = 8; // ra, fp
  int frame = align4(localsBytes + saveBytes);
  if (frame < 16) frame = 16;
  return frame;
}

void CodeGen::emit_prologue(AsmOut& o, const std::string& fname, int frameBytes) {
  (void)fname;
  o.emit("\taddi\t$sp, $sp, -" + std::to_string(frameBytes));
  o.emit("\tsw\t$ra, " + std::to_string(frameBytes - 4) + "($sp)");
  o.emit("\tsw\t$fp, " + std::to_string(frameBytes - 8) + "($sp)");
  o.emit("\taddi\t$fp, $sp, " + std::to_string(frameBytes));
}

void CodeGen::emit_epilogue(AsmOut& o, const std::string& fname, int frameBytes) {
  (void)fname;
  o.emit("\tlw\t$ra, " + std::to_string(frameBytes - 4) + "($sp)");
  o.emit("\tlw\t$fp, " + std::to_string(frameBytes - 8) + "($sp)");
  o.emit("\taddi\t$sp, $sp, " + std::to_string(frameBytes));
  o.emit("\tjr\t$ra");
}

bool CodeGen::gen_builtin_call(AsmOut& o, const Call& c,
                               std::unordered_map<std::string, VarInfo>& locals) {
  // Builtins:
  //   int getc()         => din  $v0, 1
  //   void putc(int x)   => eval x -> $t0 ; dout $t0, 2
  //   void puts("lit")   => loop load words from string label; dout each; until 0 word
  //
  // Note: These match Coconut style: dout reg, 2 ; din reg, 1

  if (c.callee == "getc") {
    o.emit("\tdin\t$v0, 1");
    o.emit("\tadd\t$t0, $v0, $zero"); // expression result in t0
    return true;
  }

  if (c.callee == "putc") {
    if (c.args.size() != 1) throw CompileError("putc expects 1 arg");
    gen_expr(o, *c.args[0], locals);
    o.emit("\tdout\t$t0, 2");
    return true;
  }

  if (c.callee == "puts") {
    if (c.args.size() != 1) throw CompileError("puts expects 1 arg");
    auto* s = dynamic_cast<const StrLitE*>(c.args[0].get());
    if (!s) throw CompileError("puts(v1) only supports string literal argument");

    std::string label;
    bool found = false;
    for (auto& e : strings) {
      if (e.s == s->s) { label = e.label; found = true; break; }
    }
    if (!found) throw CompileError("Internal: string not found");

    std::string loop = newLabel("LOOP");
    std::string done = newLabel("DONE");

    emit_load_label_addr(o, "$s0", label);

    o.emit(loop);
    emit_load_word(o, "$t1", "$s0");
    o.emit("\tbeq\t$t1, $zero, " + done);
    o.emit("\tdout\t$t1, 2");
    o.emit("\taddi\t$s0, $s0, 4");
    o.emit("\tj\t" + loop);

    o.emit(done);
    o.emit("\tadd\t$t0, $zero, $zero"); // result = 0
    return true;
  }

  return false;
}

void CodeGen::gen_lvalue_addr(AsmOut& o, const Expr& lhs,
                              std::unordered_map<std::string, VarInfo>& locals) {
  // result address in $t2
  if (auto* v = dynamic_cast<const VarRef*>(&lhs)) {
    auto itL = locals.find(v->name);
    if (itL != locals.end()) {
      int off = itL->second.fpOffset;
      o.emit("\taddi\t$t2, $fp, " + std::to_string(off));
      return;
    }
    auto itG = sym.globals.find(v->name);
    if (itG != sym.globals.end()) {
      // For v1: globals are labels, so we can load address by label.
      emit_load_label_addr(o, "$t2", v->name + "_addr");
      return;
    }
    throw CompileError("Unknown variable: " + v->name);
  }

  if (auto* a = dynamic_cast<const IndexRef*>(&lhs)) {
    auto itL = locals.find(a->name);
    bool isLocal = false;
    VarInfo vi{};
    if (itL != locals.end()) { isLocal = true; vi = itL->second; }
    else {
      auto itG = sym.globals.find(a->name);
      if (itG == sym.globals.end()) throw CompileError("Unknown array: " + a->name);
      vi = itG->second;
    }
    if (!vi.t.isArray) throw CompileError("Indexing non-array: " + a->name);

    if (isLocal) o.emit("\taddi\t$t2, $fp, " + std::to_string(vi.fpOffset));
    else emit_load_label_addr(o, "$t2", a->name + "_addr");

    gen_expr(o, *a->idx, locals); // idx in t0
    int elem = (vi.t.base == BaseType::Char) ? 4 : 4; // store char as word in v1
    emit_load_imm(o, "$t1", elem);

    o.emit("\tmult\t$t0, $t1");
    o.emit("\tmflo\t$t0");
    o.emit("\tadd\t$t2, $t2, $t0");
    return;
  }

  throw CompileError("Invalid assignment target");
}

void CodeGen::gen_expr(AsmOut& o, const Expr& e,
                       std::unordered_map<std::string, VarInfo>& locals) {
  if (auto* i = dynamic_cast<const IntLit*>(&e)) {
    emit_load_imm(o, "$t0", i->v);
    return;
  }
  if (auto* c = dynamic_cast<const CharLitE*>(&e)) {
    emit_load_imm(o, "$t0", (unsigned char)c->v);
    return;
  }
  if (auto* s = dynamic_cast<const StrLitE*>(&e)) {
    // expression value = address of string label
    for (auto& ent : strings) {
      if (ent.s == s->s) {
        emit_load_label_addr(o, "$t0", ent.label);
        return;
      }
    }
    throw CompileError("Internal: string not found");
  }
  if (auto* v = dynamic_cast<const VarRef*>(&e)) {
    auto itL = locals.find(v->name);
    if (itL != locals.end()) {
      o.emit("\tlw\t$t0, " + std::to_string(itL->second.fpOffset) + "($fp)");
      return;
    }
    auto itG = sym.globals.find(v->name);
    if (itG != sym.globals.end()) {
      emit_load_label_addr(o, "$t2", v->name + "_addr");
      emit_load_word(o, "$t0", "$t2");
      return;
    }
    throw CompileError("Unknown variable: " + v->name);
  }

  if (dynamic_cast<const IndexRef*>(&e)) {
    gen_lvalue_addr(o, e, locals); // addr -> t2
    emit_load_word(o, "$t0", "$t2");
    return;
  }

  if (auto* u = dynamic_cast<const Unary*>(&e)) {
    gen_expr(o, *u->e, locals);
    if (u->op == UnOp::Neg) {
      o.emit("\tsub\t$t0, $zero, $t0");
    } else if (u->op == UnOp::Not) {
      std::string l1 = newLabel("NOT_TRUE");
      std::string l2 = newLabel("NOT_DONE");
      o.emit("\tbeq\t$t0, $zero, " + l1);
      emit_load_imm(o, "$t0", 0);
      o.emit("\tj\t" + l2);
      o.emit(l1);
      emit_load_imm(o, "$t0", 1);
      o.emit(l2);
    }
    return;
  }

  if (auto* as = dynamic_cast<const Assign*>(&e)) {
    gen_expr(o, *as->rhs, locals);
    push_t0(o);
    gen_lvalue_addr(o, *as->lhs, locals);
    pop_t1(o);
    o.emit("\tadd\t$t0, $t1, $zero");
    emit_store_word(o, "$t0", "$t2");
    return;
  }

  if (auto* b = dynamic_cast<const Binary*>(&e)) {
    // short-circuit
    if (b->op == BinOp::And || b->op == BinOp::Or) {
      std::string lTrue = newLabel("SC_TRUE");
      std::string lFalse = newLabel("SC_FALSE");
      std::string lDone = newLabel("SC_DONE");

      if (b->op == BinOp::And) {
        gen_expr(o, *b->a, locals);
        o.emit("\tbeq\t$t0, $zero, " + lFalse);
        gen_expr(o, *b->b, locals);
        o.emit("\tbeq\t$t0, $zero, " + lFalse);
        o.emit("\tj\t" + lTrue);
      } else {
        gen_expr(o, *b->a, locals);
        o.emit("\tbne\t$t0, $zero, " + lTrue);
        gen_expr(o, *b->b, locals);
        o.emit("\tbne\t$t0, $zero, " + lTrue);
        o.emit("\tj\t" + lFalse);
      }

      o.emit(lTrue);
      emit_load_imm(o, "$t0", 1);
      o.emit("\tj\t" + lDone);
      o.emit(lFalse);
      emit_load_imm(o, "$t0", 0);
      o.emit(lDone);
      return;
    }

    // general
    gen_expr(o, *b->a, locals);
    push_t0(o);
    gen_expr(o, *b->b, locals);
    pop_t1(o);

    switch (b->op) {
      case BinOp::Add: o.emit("\tadd\t$t0, $t1, $t0"); return;
      case BinOp::Sub: o.emit("\tsub\t$t0, $t1, $t0"); return;
      case BinOp::Mul: o.emit("\tmult\t$t1, $t0"); o.emit("\tmflo\t$t0"); return;
      case BinOp::Div: o.emit("\tdiv\t$t1, $t0"); o.emit("\tmflo\t$t0"); return;
      case BinOp::Mod: o.emit("\tdiv\t$t1, $t0"); o.emit("\tmfhi\t$t0"); return;

      case BinOp::Eq:
      case BinOp::Ne:
      case BinOp::Lt:
      case BinOp::Le:
      case BinOp::Gt:
      case BinOp::Ge: {
        std::string lT = newLabel("CMP_T");
        std::string lD = newLabel("CMP_D");

        if (b->op == BinOp::Eq) {
          o.emit("\tbeq\t$t1, $t0, " + lT);
        } else if (b->op == BinOp::Ne) {
          o.emit("\tbne\t$t1, $t0, " + lT);
        } else if (b->op == BinOp::Lt) {
          o.emit("\tslt\t$t0, $t1, $t0");
          o.emit("\tbne\t$t0, $zero, " + lT);
        } else if (b->op == BinOp::Gt) {
          o.emit("\tslt\t$t0, $t0, $t1");
          o.emit("\tbne\t$t0, $zero, " + lT);
        } else if (b->op == BinOp::Le) {
          o.emit("\tslt\t$t0, $t0, $t1");
          o.emit("\tbeq\t$t0, $zero, " + lT);
        } else if (b->op == BinOp::Ge) {
          o.emit("\tslt\t$t0, $t1, $t0");
          o.emit("\tbeq\t$t0, $zero, " + lT);
        }

        emit_load_imm(o, "$t0", 0);
        o.emit("\tj\t" + lD);
        o.emit(lT);
        emit_load_imm(o, "$t0", 1);
        o.emit(lD);
        return;
      }
      default: break;
    }
  }

  if (auto* c = dynamic_cast<const Call*>(&e)) {
    if (gen_builtin_call(o, *c, locals)) return;

    int n = (int)c->args.size();
    int stackArgs = (n > 4) ? (n - 4) : 0;
    if (stackArgs > 0) {
      o.emit("\taddi\t$sp, $sp, -" + std::to_string(stackArgs * 4));
    }
    for (int i = n - 1; i >= 4; --i) {
      gen_expr(o, *c->args[i], locals);
      int off = (i - 4) * 4;
      o.emit("\tsw\t$t0, " + std::to_string(off) + "($sp)");
    }
    for (int i = 0; i < n && i < 4; ++i) {
      gen_expr(o, *c->args[i], locals);
      o.emit("\tadd\t$a" + std::to_string(i) + ", $t0, $zero");
    }

    o.emit("\tjal\t" + c->callee);

    if (stackArgs > 0) {
      o.emit("\taddi\t$sp, $sp, " + std::to_string(stackArgs * 4));
    }

    o.emit("\tadd\t$t0, $v0, $zero");
    return;
  }

  throw CompileError("Unhandled expression kind");
}

void CodeGen::gen_stmt(AsmOut& o, const Stmt& s,
                       std::unordered_map<std::string, VarInfo>& locals,
                       const std::string& funcEndLabel) {
  if (auto* b = dynamic_cast<const Block*>(&s)) {
    for (auto& st : b->stmts) gen_stmt(o, *st, locals, funcEndLabel);
    return;
  }
  if (auto* v = dynamic_cast<const VarDecl*>(&s)) {
    auto it = locals.find(v->name);
    if (it == locals.end()) throw CompileError("Internal: local not found");
    int off = it->second.fpOffset;

    if (!v->t.isArray) {
      if (v->init) {
        gen_expr(o, *(*v->init), locals);
        o.emit("\tsw\t$t0, " + std::to_string(off) + "($fp)");
      } else {
        o.emit("\tsw\t$zero, " + std::to_string(off) + "($fp)");
      }
    } else {
      int n = v->t.arrayLen;
      std::string loop = newLabel("ZARR");
      std::string done = newLabel("ZARR_DONE");
      o.emit("\taddi\t$t2, $fp, " + std::to_string(off)); // base
      emit_load_imm(o, "$t3", n);
      o.emit(loop);
      o.emit("\tbeq\t$t3, $zero, " + done);
      o.emit("\tsw\t$zero, 0($t2)");
      o.emit("\taddi\t$t2, $t2, 4");
      o.emit("\taddi\t$t3, $t3, -1");
      o.emit("\tj\t" + loop);
      o.emit(done);
    }
    return;
  }
  if (auto* e = dynamic_cast<const ExprStmt*>(&s)) {
    gen_expr(o, *e->e, locals);
    return;
  }
  if (auto* r = dynamic_cast<const Return*>(&s)) {
    if (r->e) {
      gen_expr(o, *(*r->e), locals);
      o.emit("\tadd\t$v0, $t0, $zero");
    } else {
      o.emit("\tadd\t$v0, $zero, $zero");
    }
    o.emit("\tj\t" + funcEndLabel);
    return;
  }
  if (auto* i = dynamic_cast<const If*>(&s)) {
    std::string lElse = newLabel("IF_ELSE");
    std::string lDone = newLabel("IF_DONE");
    gen_expr(o, *i->cond, locals);
    o.emit("\tbeq\t$t0, $zero, " + lElse);
    gen_stmt(o, *i->thenS, locals, funcEndLabel);
    o.emit("\tj\t" + lDone);
    o.emit(lElse);
    if (i->elseS) gen_stmt(o, *i->elseS, locals, funcEndLabel);
    o.emit(lDone);
    return;
  }
  if (auto* w = dynamic_cast<const While*>(&s)) {
    std::string lTop = newLabel("WH_TOP");
    std::string lDone = newLabel("WH_DONE");
    o.emit(lTop);
    gen_expr(o, *w->cond, locals);
    o.emit("\tbeq\t$t0, $zero, " + lDone);
    gen_stmt(o, *w->body, locals, funcEndLabel);
    o.emit("\tj\t" + lTop);
    o.emit(lDone);
    return;
  }
  if (auto* f = dynamic_cast<const For*>(&s)) {
    std::string lTop = newLabel("FOR_TOP");
    std::string lDone = newLabel("FOR_DONE");

    if (f->init) gen_stmt(o, *f->init, locals, funcEndLabel);

    o.emit(lTop);
    if (f->cond) {
      gen_expr(o, *f->cond, locals);
      o.emit("\tbeq\t$t0, $zero, " + lDone);
    }
    gen_stmt(o, *f->body, locals, funcEndLabel);
    if (f->step) gen_expr(o, *f->step, locals);
    o.emit("\tj\t" + lTop);
    o.emit(lDone);
    return;
  }

  throw CompileError("Unhandled statement kind");
}

void CodeGen::gen_func(AsmOut& o, const Func& f) {
  std::unordered_map<std::string, VarInfo> locals;
  int frameBytes = assign_locals_and_frame(f, locals);
  std::string endLabel = f.name + "_END";

  // Coconut style: label without colon
  o.emit(f.name);
  emit_prologue(o, f.name, frameBytes);

  // Spill args into local slots
  for (int i = 0; i < (int)f.params.size() && i < 4; ++i) {
    const auto& pn = f.params[i].name;
    int off = locals.at(pn).fpOffset;
    o.emit("\tsw\t$a" + std::to_string(i) + ", " + std::to_string(off) + "($fp)");
  }

  for (auto& st : f.body->stmts) gen_stmt(o, *st, locals, endLabel);

  // default return 0 if falls off end
  o.emit("\tadd\t$v0, $zero, $zero");

  o.emit(endLabel);
  emit_epilogue(o, f.name, frameBytes);
}

std::string CodeGen::compile(const Program& p) {
  build_symtabs(p);
  gather_strings(p);

  AsmOut out;

  // Coconut formatting:
  out.emit("\tbegin\t" + std::to_string(codeBase));
  out.emit("\tstart\t" + std::to_string(codeBase));
  out.emit("\taddi\t$sp, $zero, 32760");
  out.emit("\tjal\tmain");
  out.emit("\tj\tHALT");
  out.emit("HALT");
  out.emit("\tj\tHALT");
  out.emit("");

  // strings first (matches your sample style)
  for (auto& s : strings) {
    out.emit(s.label);
    for (unsigned char c : s.s) {
      out.emit("\tdw\t" + std::to_string((int)c));
    }
    out.emit("\tdw\t0");
  }
  out.emit("");

  // functions
  for (const auto& fn : p.funcs) gen_func(out, fn);

  // globals (after code for now; still valid for your assembler)
  if (!p.globals.empty()) {
    out.emit("");
    out.emit("; --- globals ---");
    for (const auto& g : p.globals) {
      out.emit(g.name + "_addr");
      int words = align4(g.t.sizeBytes()) / 4;
      int initVal = 0;
      if (g.init) {
        if (auto* il = dynamic_cast<IntLit*>((*g.init).get())) initVal = il->v;
        else if (auto* cl = dynamic_cast<CharLitE*>((*g.init).get())) initVal = (unsigned char)cl->v;
        else throw CompileError("Global initializer must be int/char literal in v1");
      }
      for (int i = 0; i < words; ++i) {
        int v = (i == 0 && !g.t.isArray) ? initVal : 0;
        out.emit("\tdw\t" + std::to_string(v));
      }
    }
  }

  out.emit("");
  out.emit("\tend");

  std::ostringstream ss;
  for (auto& l : out.lines) ss << l << "\n";
  return ss.str();
}
