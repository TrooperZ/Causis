#pragma once

#include <memory>
#include <string>
#include <utility>
#include <variant>
#include <vector>

namespace causis {

struct BlockStmt;
class Environment;

enum class ValueType { Int, String, Bool, Function, Void };

struct FunctionValue {
  std::vector<std::pair<std::string, std::string>> params;
  std::string returnType;
  const BlockStmt *body = nullptr;
  Environment *closure = nullptr;
};

struct Value {
  using Storage = std::variant<int, std::string, bool,
                               std::shared_ptr<FunctionValue>, std::monostate>;

  ValueType type = ValueType::Void;
  Storage data = std::monostate{};
};

struct ReturnValue {
  Value value;
};

struct BreakValue {};
struct ContinueValue {};

} // namespace causis
