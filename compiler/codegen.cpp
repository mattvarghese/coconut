#include "codegen.h"
#include "util.h"
#include <sstream>
#include <iomanip>
#include <functional>
#include <cstdint>

static inline int align4(int x) { return (x + 3) & ~3; }

std::string CodeGen::newLabel(const std::string& prefix) {
  return prefix + "_" + std::to_string(curLabelId++);
}

void CodeGen::build_symtabs(const Program& p) {
  // functions
  for (const auto& f : p.funcs) {
    FuncInfo fi;
    fi.ret = f.ret;
    fi.params = f.params;
    sym.funcs[f.name] = fi;
  }
  // globals
  for (const auto& g : p.globals) {
    VarInfo vi;
    vi.t = g.t;
    vi.isGlobal = true;
    sym.globals[g.name] = vi; // addr filled in layout
  }
}

void CodeGen::layout_globals_and_strings(const Program& p, int codeSizeBytes) {
  dataBase = codeBase + codeSizeBytes;
  int addr = dataBase;

  // globals first
  for (const auto& g : p.globals) {
    auto& vi = sym.globals.at(g.name);
    vi.addr = addr;
    addr += align4(g.t.sizeBytes());
  }

  // gather strings from AST (very small scan; enough for v1)
  // We'll assign string addresses after globals.
  int sid = 1;
  auto noteStr = [&](const std::string& s) {
    for (auto& e : strings) if (e.s == s) return; // dedupe
    StrEnt ent;
    ent.label = "str_" + std::to_string(sid++);
    ent.s = s;
    strings.push_back(std::move(ent));
  };

  // scan expressions for StrLitE (recursive)
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

  // assign string addresses, store as 1 byte per char, but still emitted using dw words.
  // We'll pack 4 chars per word for compactness (still teachable).
  for (auto& s : strings) {
    s.addr = addr;
    int bytes = (int)s.s.size() + 1; // null term
    addr += align4(bytes);
  }
}

void CodeGen::emit_load_imm(AsmOut& o, const std::string& rd, int32_t imm) {
  // If fits 16-bit signed, use addi rd, $zero, imm
  if (imm >= -32768 && imm <= 32767) {
    o.emit("addi " + rd + ", $zero, " + std::to_string(imm));
  } else {
    int32_t hi = (imm >> 16) & 0xFFFF;
    int32_t lo = imm & 0xFFFF;
    o.emit("lui " + rd + ", " + std::to_string(hi));
    o.emit("ori " + rd + ", " + rd + ", " + std::to_string(lo));
  }
}

void CodeGen::emit_load_addr(AsmOut& o, const std::string& rd, int32_t addr) {
  // addr treated as 32-bit unsigned value in two halves
  uint32_t u = (uint32_t)addr;
  uint32_t hi = (u >> 16) & 0xFFFF;
  uint32_t lo = u & 0xFFFF;
  o.emit("lui " + rd + ", " + std::to_string((int)hi));
  o.emit("ori " + rd + ", " + rd + ", " + std::to_string((int)lo));
}

void CodeGen::emit_load_word(AsmOut& o, const std::string& rd, const std::string& raddr) {
  // lw rd, 0(raddr)
  o.emit("lw " + rd + ", 0(" + raddr + ")");
}
void CodeGen::emit_store_word(AsmOut& o, const std::string& rs, const std::string& raddr) {
  o.emit("sw " + rs + ", 0(" + raddr + ")");
}

void CodeGen::push_t0(AsmOut& o) {
  o.emit("addi $sp, $sp, -4");
  o.emit("sw $t0, 0($sp)");
}
void CodeGen::pop_t1(AsmOut& o) {
  o.emit("lw $t1, 0($sp)");
  o.emit("addi $sp, $sp, 4");
}

int CodeGen::assign_locals_and_frame(const Func& f, std::unordered_map<std::string, VarInfo>& locals) {
  // Layout:
  // [fp+?] not used; we use negative offsets for locals
  // Save area at top of frame:
  //   0($sp) ... locals/temps ...
  // At function entry after "addi $sp,-frame":
  //   sw $ra, frame-4($sp)
  //   sw $fp, frame-8($sp)
  //
  // We'll set $fp = $sp + frame
  // Locals at negative offsets from $fp.
  int offset = 0;

  // params: we treat $a0-$a3 as incoming, but also spill to stack locals for simplicity
  // We'll create local slots for each param.
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
  o.emit("addi $sp, $sp, -" + std::to_string(frameBytes));
  o.emit("sw $ra, " + std::to_string(frameBytes - 4) + "($sp)");
  o.emit("sw $fp, " + std::to_string(frameBytes - 8) + "($sp)");
  // fp = sp + frame
  o.emit("addi $fp, $sp, " + std::to_string(frameBytes));
}

void CodeGen::emit_epilogue(AsmOut& o, const std::string& fname, int frameBytes) {
  (void)fname;
  o.emit("lw $ra, " + std::to_string(frameBytes - 4) + "($sp)");
  o.emit("lw $fp, " + std::to_string(frameBytes - 8) + "($sp)");
  o.emit("addi $sp, $sp, " + std::to_string(frameBytes));
  o.emit("jr $ra");
}

bool CodeGen::gen_builtin_call(AsmOut& o, const Call& c,
                               std::unordered_map<std::string, VarInfo>& locals) {
  // Builtins:
  //   int getc()         => din $v0
  //   void putc(int x)   => eval x -> $t0 ; dout $t0
  //   void puts(char* s) => expects string literal only (v1 teaching); prints until 0 byte
  //   void print_int(int)=> naive decimal printing (slow) (works for int>=0)
  //
  // Notes:
  // - Your assembler supports "din" and "dout" tokens; your simulator maps them through PortManager.
  if (c.callee == "getc") {
    o.emit("din $v0");
    // move v0 -> t0 as expression result
    o.emit("add $t0, $v0, $zero");
    return true;
  }
  if (c.callee == "putc") {
    if (c.args.size() != 1) throw CompileError("putc expects 1 arg");
    gen_expr(o, *c.args[0], locals); // result in t0
    o.emit("dout $t0");
    // expression result = arg (t0 already)
    return true;
  }
  if (c.callee == "puts") {
    if (c.args.size() != 1) throw CompileError("puts expects 1 arg");
    // We only support puts("literal") in v1 (easy + kid-friendly).
    auto* s = dynamic_cast<const StrLitE*>(c.args[0].get());
    if (!s) throw CompileError("puts(v1) only supports string literal argument");

    // find its addr
    int addr = 0;
    bool found = false;
    for (auto& e : strings) {
      if (e.s == s->s) { addr = e.addr; found = true; break; }
    }
    if (!found) throw CompileError("Internal: string not found");

    std::string loop = newLabel("puts_loop");
    std::string done = newLabel("puts_done");

    emit_load_addr(o, "$t2", addr);     // ptr
    o.emit(loop + ":");
    // load byte: we load word and mask; since we packed, this is more work.
    // For teaching simplicity, we stored bytes but aligned. We'll just treat as 1 byte per word here:
    // (So strings emitted as DW words each containing one byte value.)
    // That makes puts trivial and clear.
    //
    // Therefore: load word at [t2], if 0 -> done, else dout, t2 += 4
    emit_load_word(o, "$t0", "$t2");    // char in t0
    o.emit("beq $t0, $zero, " + done);
    o.emit("dout $t0");
    o.emit("addi $t2, $t2, 4");
    o.emit("j " + loop);
    o.emit(done + ":");
    // result: 0
    o.emit("add $t0, $zero, $zero");
    return true;
  }
  if (c.callee == "print_int") {
    if (c.args.size() != 1) throw CompileError("print_int expects 1 arg");
    gen_expr(o, *c.args[0], locals); // t0
    // very naive base-10 printing for non-negative integers:
    // if t0==0 print '0'
    // else repeated div by 10, push remainders, then pop and output
    std::string isZero = newLabel("pi_zero");
    std::string loop = newLabel("pi_loop");
    std::string push = newLabel("pi_push");
    std::string out = newLabel("pi_out");
    std::string done = newLabel("pi_done");

    o.emit("beq $t0, $zero, " + isZero);

    // use t3 as count, t4 as temp n
    o.emit("add $t4, $t0, $zero"); // n
    o.emit("add $t3, $zero, $zero"); // count = 0

    o.emit(loop + ":");
    // if n==0 goto out
    o.emit("beq $t4, $zero, " + out);

    // quotient = n / 10, remainder = n % 10
    emit_load_imm(o, "$t5", 10);
    o.emit("div $t4, $t5");
    o.emit("mflo $t6"); // q
    o.emit("mfhi $t7"); // r

    // push (r + '0')
    emit_load_imm(o, "$t8", (int)'0');
    o.emit("add $t7, $t7, $t8");
    o.emit("addi $sp, $sp, -4");
    o.emit("sw $t7, 0($sp)");
    o.emit("addi $t3, $t3, 1");

    o.emit("add $t4, $t6, $zero");
    o.emit("j " + loop);

    o.emit(out + ":");
    // pop and output count times
    std::string popLoop = newLabel("pi_pop");
    o.emit(popLoop + ":");
    o.emit("beq $t3, $zero, " + done);
    o.emit("lw $t0, 0($sp)");
    o.emit("addi $sp, $sp, 4");
    o.emit("dout $t0");
    o.emit("addi $t3, $t3, -1");
    o.emit("j " + popLoop);

    o.emit(isZero + ":");
    emit_load_imm(o, "$t0", (int)'0');
    o.emit("dout $t0");

    o.emit(done + ":");
    // leave t0 as 0
    o.emit("add $t0, $zero, $zero");
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
      // addr = fp + offset
      int off = itL->second.fpOffset;
      o.emit("addi $t2, $fp, " + std::to_string(off));
      return;
    }
    auto itG = sym.globals.find(v->name);
    if (itG != sym.globals.end()) {
      emit_load_addr(o, "$t2", itG->second.addr);
      return;
    }
    throw CompileError("Unknown variable: " + v->name);
  }

  if (auto* a = dynamic_cast<const IndexRef*>(&lhs)) {
    // base + idx*elemSize
    // base addr -> t2, idx -> t0, elemSize -> t1
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

    if (isLocal) o.emit("addi $t2, $fp, " + std::to_string(vi.fpOffset));
    else emit_load_addr(o, "$t2", vi.addr);

    gen_expr(o, *a->idx, locals); // idx in t0
    int elem = (vi.t.base == BaseType::Char) ? 1 : 4;
    emit_load_imm(o, "$t1", elem);

    // t0 = t0 * elem
    o.emit("mult $t0, $t1");
    o.emit("mflo $t0");
    // t2 = t2 + t0
    o.emit("add $t2, $t2, $t0");
    return;
  }

  throw CompileError("Invalid assignment target");
}

void CodeGen::gen_expr(AsmOut& o, const Expr& e,
                       std::unordered_map<std::string, VarInfo>& locals) {
  // result in $t0
  if (auto* i = dynamic_cast<const IntLit*>(&e)) {
    emit_load_imm(o, "$t0", i->v);
    return;
  }
  if (auto* c = dynamic_cast<const CharLitE*>(&e)) {
    emit_load_imm(o, "$t0", (unsigned char)c->v);
    return;
  }
  if (auto* s = dynamic_cast<const StrLitE*>(&e)) {
    // expression value = address of string
    for (auto& ent : strings) {
      if (ent.s == s->s) {
        emit_load_addr(o, "$t0", ent.addr);
        return;
      }
    }
    throw CompileError("Internal: string not laid out");
  }
  if (auto* v = dynamic_cast<const VarRef*>(&e)) {
    auto itL = locals.find(v->name);
    if (itL != locals.end()) {
      // load based on size
      if (itL->second.t.base == BaseType::Char) {
        // no lb in your assembler; treat char as word for simplicity in v1:
        // (we store char in a word slot)
        o.emit("lw $t0, " + std::to_string(itL->second.fpOffset) + "($fp)");
      } else {
        o.emit("lw $t0, " + std::to_string(itL->second.fpOffset) + "($fp)");
      }
      return;
    }
    auto itG = sym.globals.find(v->name);
    if (itG != sym.globals.end()) {
      emit_load_addr(o, "$t2", itG->second.addr);
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
      o.emit("sub $t0, $zero, $t0");
    } else if (u->op == UnOp::Not) {
      // t0 = (t0==0)
      std::string l1 = newLabel("not_true");
      std::string l2 = newLabel("not_done");
      o.emit("beq $t0, $zero, " + l1);
      emit_load_imm(o, "$t0", 0);
      o.emit("j " + l2);
      o.emit(l1 + ":");
      emit_load_imm(o, "$t0", 1);
      o.emit(l2 + ":");
    }
    return;
  }
  if (auto* as = dynamic_cast<const Assign*>(&e)) {
    // compute rhs -> t0
    gen_expr(o, *as->rhs, locals);
    // save rhs in stack
    push_t0(o);
    // compute lhs address -> t2
    gen_lvalue_addr(o, *as->lhs, locals);
    // restore rhs to t0
    pop_t1(o); // t1 = rhs
    o.emit("add $t0, $t1, $zero");
    // store
    emit_store_word(o, "$t0", "$t2");
    return;
  }
  if (auto* b = dynamic_cast<const Binary*>(&e)) {
    // short-circuit for && and ||
    if (b->op == BinOp::And || b->op == BinOp::Or) {
      std::string lTrue = newLabel("sc_true");
      std::string lFalse = newLabel("sc_false");
      std::string lDone = newLabel("sc_done");

      if (b->op == BinOp::And) {
        gen_expr(o, *b->a, locals);
        o.emit("beq $t0, $zero, " + lFalse);
        gen_expr(o, *b->b, locals);
        o.emit("beq $t0, $zero, " + lFalse);
        o.emit("j " + lTrue);
      } else {
        gen_expr(o, *b->a, locals);
        o.emit("bne $t0, $zero, " + lTrue);
        gen_expr(o, *b->b, locals);
        o.emit("bne $t0, $zero, " + lTrue);
        o.emit("j " + lFalse);
      }

      o.emit(lTrue + ":");
      emit_load_imm(o, "$t0", 1);
      o.emit("j " + lDone);
      o.emit(lFalse + ":");
      emit_load_imm(o, "$t0", 0);
      o.emit(lDone + ":");
      return;
    }

    // general: eval a -> t0 push, eval b -> t0 pop->t1
    gen_expr(o, *b->a, locals);
    push_t0(o);
    gen_expr(o, *b->b, locals);
    pop_t1(o); // t1=a, t0=b

    switch (b->op) {
      case BinOp::Add: o.emit("add $t0, $t1, $t0"); return;
      case BinOp::Sub: o.emit("sub $t0, $t1, $t0"); return;
      case BinOp::Mul:
        o.emit("mult $t1, $t0"); o.emit("mflo $t0"); return;
      case BinOp::Div:
        o.emit("div $t1, $t0"); o.emit("mflo $t0"); return;
      case BinOp::Mod:
        o.emit("div $t1, $t0"); o.emit("mfhi $t0"); return;

      case BinOp::Eq:
      case BinOp::Ne:
      case BinOp::Lt:
      case BinOp::Le:
      case BinOp::Gt:
      case BinOp::Ge: {
        // comparisons -> 0/1 in t0
        std::string lT = newLabel("cmp_t");
        std::string lD = newLabel("cmp_d");

        if (b->op == BinOp::Eq) {
          o.emit("beq $t1, $t0, " + lT);
        } else if (b->op == BinOp::Ne) {
          o.emit("bne $t1, $t0, " + lT);
        } else if (b->op == BinOp::Lt) {
          o.emit("slt $t0, $t1, $t0"); // t0 = a < b
          o.emit("bne $t0, $zero, " + lT);
        } else if (b->op == BinOp::Gt) {
          o.emit("slt $t0, $t0, $t1"); // b < a
          o.emit("bne $t0, $zero, " + lT);
        } else if (b->op == BinOp::Le) {
          // a <= b  <=> !(b < a)
          o.emit("slt $t0, $t0, $t1"); // b < a
          o.emit("beq $t0, $zero, " + lT);
        } else if (b->op == BinOp::Ge) {
          // a >= b <=> !(a < b)
          o.emit("slt $t0, $t1, $t0"); // a < b
          o.emit("beq $t0, $zero, " + lT);
        }

        emit_load_imm(o, "$t0", 0);
        o.emit("j " + lD);
        o.emit(lT + ":");
        emit_load_imm(o, "$t0", 1);
        o.emit(lD + ":");
        return;
      }
      default: break;
    }
  }

  if (auto* c = dynamic_cast<const Call*>(&e)) {
    if (gen_builtin_call(o, *c, locals)) return;

    // normal call: evaluate args into $a0-$a3 (spill extras to stack)
    int n = (int)c->args.size();
    int stackArgs = (n > 4) ? (n - 4) : 0;
    if (stackArgs > 0) {
      o.emit("addi $sp, $sp, -" + std::to_string(stackArgs * 4));
    }
    // extra args on stack (right-to-left)
    for (int i = n - 1; i >= 4; --i) {
      gen_expr(o, *c->args[i], locals);
      int off = (i - 4) * 4;
      o.emit("sw $t0, " + std::to_string(off) + "($sp)");
    }
    // first 4 in a-registers
    for (int i = 0; i < n && i < 4; ++i) {
      gen_expr(o, *c->args[i], locals);
      o.emit("add $a" + std::to_string(i) + ", $t0, $zero");
    }

    o.emit("jal " + c->callee);

    if (stackArgs > 0) {
      o.emit("addi $sp, $sp, " + std::to_string(stackArgs * 4));
    }

    // return value in $v0 -> t0
    o.emit("add $t0, $v0, $zero");
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
    // default initialize arrays/vars to 0 for teaching simplicity
    auto it = locals.find(v->name);
    if (it == locals.end()) throw CompileError("Internal: local not found");
    int off = it->second.fpOffset;

    // init scalar only
    if (!v->t.isArray) {
      if (v->init) {
        gen_expr(o, *(*v->init), locals);
        o.emit("sw $t0, " + std::to_string(off) + "($fp)");
      } else {
        o.emit("sw $zero, " + std::to_string(off) + "($fp)");
      }
    } else {
      // zero out array (word slots)
      int n = v->t.arrayLen;
      std::string loop = newLabel("zarr");
      std::string done = newLabel("zarr_done");
      emit_load_imm(o, "$t0", 0);
      o.emit("addi $t2, $fp, " + std::to_string(off)); // base
      emit_load_imm(o, "$t3", n);
      o.emit(loop + ":");
      o.emit("beq $t3, $zero, " + done);
      o.emit("sw $zero, 0($t2)");
      o.emit("addi $t2, $t2, 4");
      o.emit("addi $t3, $t3, -1");
      o.emit("j " + loop);
      o.emit(done + ":");
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
      o.emit("add $v0, $t0, $zero");
    } else {
      o.emit("add $v0, $zero, $zero");
    }
    o.emit("j " + funcEndLabel);
    return;
  }
  if (auto* i = dynamic_cast<const If*>(&s)) {
    std::string lElse = newLabel("if_else");
    std::string lDone = newLabel("if_done");
    gen_expr(o, *i->cond, locals);
    o.emit("beq $t0, $zero, " + lElse);
    gen_stmt(o, *i->thenS, locals, funcEndLabel);
    o.emit("j " + lDone);
    o.emit(lElse + ":");
    if (i->elseS) gen_stmt(o, *i->elseS, locals, funcEndLabel);
    o.emit(lDone + ":");
    return;
  }
  if (auto* w = dynamic_cast<const While*>(&s)) {
    std::string lTop = newLabel("wh_top");
    std::string lDone = newLabel("wh_done");
    o.emit(lTop + ":");
    gen_expr(o, *w->cond, locals);
    o.emit("beq $t0, $zero, " + lDone);
    gen_stmt(o, *w->body, locals, funcEndLabel);
    o.emit("j " + lTop);
    o.emit(lDone + ":");
    return;
  }
  if (auto* f = dynamic_cast<const For*>(&s)) {
    std::string lTop = newLabel("for_top");
    std::string lDone = newLabel("for_done");

    if (f->init) gen_stmt(o, *f->init, locals, funcEndLabel);

    o.emit(lTop + ":");
    if (f->cond) {
      gen_expr(o, *f->cond, locals);
      o.emit("beq $t0, $zero, " + lDone);
    }
    gen_stmt(o, *f->body, locals, funcEndLabel);
    if (f->step) gen_expr(o, *f->step, locals);
    o.emit("j " + lTop);
    o.emit(lDone + ":");
    return;
  }

  throw CompileError("Unhandled statement kind");
}

void CodeGen::gen_func(AsmOut& o, const Func& f) {
  std::unordered_map<std::string, VarInfo> locals;
  int frameBytes = assign_locals_and_frame(f, locals);

  std::string endLabel = f.name + "_end";

  o.emit(f.name + ":");
  emit_prologue(o, f.name, frameBytes);

  // Spill args into their local slots (params are first in locals)
  for (int i = 0; i < (int)f.params.size() && i < 4; ++i) {
    const auto& pn = f.params[i].name;
    int off = locals.at(pn).fpOffset;
    o.emit("sw $a" + std::to_string(i) + ", " + std::to_string(off) + "($fp)");
  }
  // (extra params on stack not implemented yet; easy to add later)

  // body
  for (auto& st : f.body->stmts) gen_stmt(o, *st, locals, endLabel);

  // default return 0 if falls off end
  o.emit("add $v0, $zero, $zero");

  o.emit(endLabel + ":");
  emit_epilogue(o, f.name, frameBytes);
}

std::string CodeGen::compile(const Program& p) {
  build_symtabs(p);

  // First pass: generate code with placeholder global addresses? We don’t need placeholder labels,
  // because globals are resolved numerically after we compute code size.
  // We therefore do:
  //  1) generate code once *without* emitting global initializers/data
  //  2) measure code size (each instruction is 4 bytes in your assembler)
  //  3) lay out globals/strings
  //  4) generate final output including data
  //
  // For v1 we assume each instruction emitted becomes 4 bytes and each DW becomes 4 bytes.
  AsmOut temp;
  for (const auto& fn : p.funcs) {
    // just count roughly by emitting; globals not used yet.
    // (String addr not needed in pass1 because we’ll re-emit.)
    gen_func(temp, fn);
  }
  int codeInstrs = (int)temp.lines.size();
  int codeSizeBytes = codeInstrs * 4;

  layout_globals_and_strings(p, codeSizeBytes);

  AsmOut out;
  out.emit("begin " + std::to_string(codeBase));

  // code
  for (const auto& fn : p.funcs) gen_func(out, fn);

  // data: globals + strings using DW
  out.emit("");
  out.emit("; --- globals ---");
  for (const auto& g : p.globals) {
    auto& vi = sym.globals.at(g.name);
    out.emit(g.name + "_addr:"); // label for humans (assembler treats IDENTIFIER as label)
    int words = align4(g.t.sizeBytes()) / 4;
    int initVal = 0;
    if (g.init) {
      if (auto* il = dynamic_cast<IntLit*>((*g.init).get())) initVal = il->v;
      else if (auto* cl = dynamic_cast<CharLitE*>((*g.init).get())) initVal = (unsigned char)cl->v;
      else throw CompileError("Global initializer must be int/char literal in v1");
    }
    // arrays init as zeros
    for (int i = 0; i < words; ++i) {
      int v = (i == 0 && !g.t.isArray) ? initVal : 0;
      out.emit("dw " + std::to_string(v));
    }
    (void)vi;
  }

  out.emit("");
  out.emit("; --- strings (1 char per word, null terminated) ---");
  for (auto& s : strings) {
    out.emit(s.label + ":");
    for (char c : s.s) {
      out.emit("dw " + std::to_string((unsigned char)c));
    }
    out.emit("dw 0");
    // pad to 4 bytes already implicit because each dw is a word
  }

  // start at address 0 (first instruction)
  out.emit("start " + std::to_string(codeBase));
  out.emit("end");

  std::ostringstream ss;
  for (auto& l : out.lines) ss << l << "\n";
  return ss.str();
}
