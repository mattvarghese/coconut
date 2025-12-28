#include "util.h"
#include "lexer.h"
#include "parser.h"
#include "codegen.h"
#include <iostream>
#include <fstream>

static void usage() {
  std::cerr << "Usage: smallc <input.c> [-o output.s]\n";
}

int main(int argc, char** argv) {
  try {
    if (argc < 2) { usage(); return 1; }
    std::string inPath = argv[1];
    std::string outPath = "a.s";

    for (int i = 2; i < argc; ++i) {
      std::string a = argv[i];
      if (a == "-o" && i + 1 < argc) {
        outPath = argv[++i];
      } else {
        usage();
        return 1;
      }
    }

    std::string src = read_file(inPath);
    Lexer lex(src);
    Parser parser(std::move(lex));
    Program prog = parser.parse_program();

    CodeGen cg;
    std::string asmText = cg.compile(prog);

    std::ofstream out(outPath, std::ios::binary);
    if (!out) throw CompileError("Unable to write output file: " + outPath);
    out << asmText;

    return 0;
  } catch (const CompileError& e) {
    std::cerr << "Compile error: " << e.what() << "\n";
    return 2;
  } catch (const std::exception& e) {
    std::cerr << "Fatal error: " << e.what() << "\n";
    return 3;
  }
}
