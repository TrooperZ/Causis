#pragma once

#include "causis/ValueType.h"

#include <memory>
#include <string>
#include <variant>

namespace causis {

struct FunctionValue;

struct Value {
  using Storage = std::variant<int, std::string, bool,
                               std::shared_ptr<FunctionValue>, std::monostate>;

  ValueType type = ValueType::Void;
  Storage data = std::monostate{};
};

} // namespace causis
