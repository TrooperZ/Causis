/**
 * @file Token.h
 * @author Amin Karic
 * @brief Token value type produced by the lexer.
 */
#pragma once

#include "causis/TokenType.h"

#include <string>

namespace causis {

/**
 * @brief Represents one lexed token from the source stream.
 */
struct Token {
  TokenType type{};
  std::string lexeme{};
  int line = 1;
};

} // namespace causis
