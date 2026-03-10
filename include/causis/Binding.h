/**
 * @file Binding.h
 * @author Amin Karic
 * @brief Runtime binding metadata for named values.
 */
#pragma once

#include "causis/Value.h"

#include <string>

namespace causis {

/**
 * @brief Stores declared type, current value, and mutability for one name.
 */
struct Binding {
  std::string declaredType;
  Value value;
  bool mutableState = false;
};

} // namespace causis
