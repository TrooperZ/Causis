/**
 * @file Value.h
 * @author Amin Karic
 * @brief Runtime value representation for Causis.
 */
#pragma once

#include "causis/ValueType.h"

#include <memory>
#include <string>
#include <variant>

namespace causis {

struct FunctionValue;

/**
 * @brief Tagged runtime value used by the interpreter.
 */
struct Value {
  using Storage = std::variant<int, std::string, bool,
                               std::shared_ptr<FunctionValue>, std::monostate>;

  ValueType type = ValueType::Void;
  Storage data = std::monostate {};
};

} // namespace causis
