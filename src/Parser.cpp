#include "causis/Parser.h"
#include "causis/Token.h"
#include "causis/TokenType.h"
#include <initializer_list>
#include <stdexcept>
#include <vector>

namespace causis {

const Token &Parser::previous() const {
  if (_current == 0) {
    throw std::runtime_error("previous() called before advancing");
  }
  return _tokens[_current - 1];
}

const Token &Parser::advance() {
  if (!isAtEnd()) {
    ++_current;
    return _tokens[_current - 1];
  }
  return _tokens[_current];
}

bool Parser::check(TokenType type) const {
  if (isAtEnd()) {
    return false;
  }
  return peek().type == type;
}

bool Parser::match(std::initializer_list<TokenType> types) {
  for (const auto &t : types) {
    if (check(t)) {
      advance();
      return true;
    }
  }
  return false;
}

const Token &Parser::consume(TokenType type, const std::string &message) {
  if (check(type)) {
    return advance();
  }

  throw std::runtime_error(message + " Found: " + peek().lexeme);
}

std::unique_ptr<Stmt> Parser::parseDeclaration() {}

} // namespace causis