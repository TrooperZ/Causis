#include "causis/Interpreter.h"
#include "causis/Lexer.h"
#include "causis/Parser.h"

#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <vector>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << "Usage: causis <source-file>\n";
    return 1;
  }

  // program takes in arg 1 as file to run
  std::ifstream ifs(argv[1]);

  if (!ifs.is_open()) {
    std::cerr << "Failed to open source file: " << argv[1] << "\n";
    return 1;
  }

  std::string code;
  std::ostringstream oss;
  oss << ifs.rdbuf();
  ifs.close();
  code = oss.str();

  std::vector<causis::Token> tokens;
  try {
    causis::Lexer lexer(code);
    tokens = lexer.scanTokens();
  } catch (const std::exception &ex) {
    std::cerr << "Lex error: " << ex.what() << "\n";
    return 1;
  }

  std::vector<std::unique_ptr<causis::Stmt>> program;
  try {
    causis::Parser parser(tokens);
    program = parser.parse();
  } catch (const std::exception &ex) {
    std::cerr << "Parse error: " << ex.what() << "\n";
    return 1;
  }

  causis::Interpreter interpreter;
  try {
    interpreter.execute(program);
  } catch (const std::exception &ex) {
    std::cerr << "Runtime error: " << ex.what() << "\n";
    return 1;
  }

  return 0;
}
