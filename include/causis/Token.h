#pragma once

#include "causis/TokenType.h"

#include <cstddef>
#include <string>

namespace causis {

struct Token {
  TokenType type{};
  std::string lexeme{};
  std::size_t line = 1;
  std::size_t column = 1;
};

} // namespace causis
