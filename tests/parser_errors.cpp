#include "causis/Errors.h"
#include "causis/Lexer.h"
#include "causis/Parser.h"

#include <cassert>
#include <string>

namespace {

void expectSourceError(const std::string &source, std::size_t line,
                       std::size_t column, const std::string &message) {
  try {
    causis::Lexer lexer(source);
    causis::Parser parser(lexer.scanTokens());
    parser.parse();
  } catch (const causis::SourceError &error) {
    assert(error.line == line);
    assert(error.column == column);
    assert(error.what() == message);
    return;
  }

  assert(false);
}

} // namespace

int main() {
  expectSourceError("let x = 1", 1, 10,
                    "Expected ';' after declaration. Found: end of file");
  expectSourceError("let x: nope = 1;", 1, 8,
                    "Expected type definition after ':'. Found: nope");
  expectSourceError("let x = (1)(2);", 1, 12, "Can only call named functions.");
  expectSourceError("let x = ;", 1, 9, "Expected expression");
  expectSourceError("let x = 999999999999999999999;", 1, 9,
                    "Integer literal out of range.");
}
