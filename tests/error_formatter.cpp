#include "causis/Errors.h"

#include <iostream>
#include <sstream>
#include <string>

int main() {
  std::ostringstream output;
  std::streambuf *previous = std::cerr.rdbuf(output.rdbuf());

  causis::printSourceError("parse", "/tmp/program.au", "let x = ;",
                           causis::SourceError(1, 9, "Expected expression"));

  std::cerr.rdbuf(previous);

  const std::string expected = "\n"
                               "==============================================="
                               "=================================\n"
                               "error[parse]: Expected expression\n"
                               " --> program.au:1:9\n"
                               "  |\n"
                               "1 | let x = ;\n"
                               "  |         ^\n";
  if (output.str() != expected) {
    std::cerr << "EXPECTED:\n" << expected << "ACTUAL:\n" << output.str();
    return 1;
  }
}
