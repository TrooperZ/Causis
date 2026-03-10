/**
 * @file FunctionValue.h
 * @author Amin Karic
 * @brief Runtime representation of a user-defined Causis function.
 */
#pragma once

#include "causis/AST.h"
#include "causis/Environment.h"

#include <string>
#include <utility>
#include <vector>

namespace causis {

/**
 * @brief Captures function signature, expression body, and closure scope.
 */
struct FunctionValue {
  std::vector<std::pair<std::string, std::string>> params;
  std::string returnType;
  const Expr *body = nullptr;
  Environment *closure = nullptr;
};

} // namespace causis
