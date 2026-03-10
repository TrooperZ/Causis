#include "causis/AST.h"
#include "causis/Interpreter.h"
#include "causis/Lexer.h"
#include "causis/Token.h"

#include <fstream>
#include <iostream>
#include <sstream>

int main(int argc, char *argv[]) {

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

  for (const auto &token : tokens) {
    std::cout << "lexeme=[" << token.lexeme << "] line=" << token.line << "\n";
  }

  // lexer: text to token
  // parser: token to ast
  // interpreter: walks ast

  std::cout << "Causis starter project configured.\n";
  return 0;
}
