#pragma once

#include "causis/TokenType.h"

#include <string>

namespace causis {

struct Token {
  TokenType type{};
  std::string lexeme{};
  int line = 1;
};

} // namespace causis
