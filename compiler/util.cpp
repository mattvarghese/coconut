#include "util.h"
#include <fstream>
#include <sstream>

std::string read_file(const std::string& path) {
  std::ifstream in(path, std::ios::binary);
  if (!in) throw CompileError("Unable to open file: " + path);
  std::ostringstream ss;
  ss << in.rdbuf();
  return ss.str();
}
