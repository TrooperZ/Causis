#include "causis/Errors.h"
#include "causis/Lexer.h"
#include "causis/TokenType.h"

#include <cassert>
#include <string>

namespace {

void expectSourceError(const std::string &source, std::size_t line,
                       std::size_t column) {
  try {
    causis::Lexer(source).scanTokens();
  } catch (const causis::SourceError &error) {
    assert(error.line == line);
    assert(error.column == column);
    return;
  }

  assert(false);
}

} // namespace

int main() {
  causis::Lexer lexer("let x = 1;\n  print(\"a\nb\");");
  const auto tokens = lexer.scanTokens();

  assert(tokens.size() == 11);
  assert(tokens[0].type == causis::TokenType::KwLet && tokens[0].line == 1 &&
         tokens[0].column == 1);
  assert(tokens[5].type == causis::TokenType::KwPrint && tokens[5].line == 2 &&
         tokens[5].column == 3);
  assert(tokens[7].type == causis::TokenType::StringLiteral &&
         tokens[7].line == 2 && tokens[7].column == 9);
  assert(tokens[10].type == causis::TokenType::EndOfFile &&
         tokens[10].line == 3 && tokens[10].column == 5);

  expectSourceError("let x = 1;\n  @", 2, 3);
  expectSourceError("\"bad\\q\"", 1, 5);
  expectSourceError("  \"unterminated", 1, 3);
}
