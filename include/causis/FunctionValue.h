#pragma once

#include "causis/AST.h"
#include "causis/Environment.h"

#include <string>
#include <utility>
#include <vector>

namespace causis {

struct FunctionValue {
  std::vector<std::pair<std::string, std::string>> params;
  std::string returnType;
  const Expr *body = nullptr;
  Environment *closure = nullptr;
};

} // namespace causis
