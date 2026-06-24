#pragma once

#include "causis/Value.h"

#include <string>

namespace causis {

struct Expr;
class Environment;

struct Binding {
  std::string declaredType;
  Value value;
  bool mutableState = false;
  bool derived = false;
  const Expr *derivedExpr = nullptr;
  Environment *derivedEnv = nullptr;
  bool evaluating = false;
};

} // namespace causis
