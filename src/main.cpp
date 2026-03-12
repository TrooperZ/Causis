#include "causis/Interpreter.h"
#include "causis/Lexer.h"
#include "causis/Parser.h"

#include <fstream>
#include <iostream>
#include <sstream>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << "Usage: causis <source-file>\n";
    return 1;
  }

  // program takes in arg 1 as file to run
  std::ifstream ifs(argv[1]);

  if (!ifs.is_open()) {
    return -1;
  }

  std::string code;
  std::ostringstream oss;
  oss << ifs.rdbuf();
  ifs.close();
  code = oss.str();

  causis::Lexer lexer(code);

  auto tokens = lexer.scanTokens();

  std::vector<std::unique_ptr<causis::Stmt>> program;
  try {
    causis::Parser parser(tokens);
    program = parser.parse();
  } catch (const std::exception &ex) {
    std::cerr << "Parse error: " << ex.what() << "\n";
    return 1;
  }

  causis::Interpreter interpreter;
  interpreter.execute(program);
  return 0;
}
