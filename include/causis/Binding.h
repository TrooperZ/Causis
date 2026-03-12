#pragma once

#include "causis/Value.h"

#include <string>

namespace causis {

struct Binding {
  std::string declaredType;
  Value value;
  bool mutableState = false;
};

} // namespace causis
