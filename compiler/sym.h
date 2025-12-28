#pragma once
#include "ast.h"
#include <string>
#include <unordered_map>

struct VarInfo {
  Type t;
  bool isGlobal = false;
  int addr = 0;      // absolute address for globals
  int fpOffset = 0;  // negative offsets for locals (e.g., -4, -8)
};

struct FuncInfo {
  Type ret;
  std::vector<Param> params;
};

struct SymTab {
  std::unordered_map<std::string, VarInfo> globals;
  std::unordered_map<std::string, FuncInfo> funcs;
};
