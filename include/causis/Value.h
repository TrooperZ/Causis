#pragma once

#include <memory>
#include <cstdint>
#include <string>
#include <utility>
#include <variant>
#include <vector>

namespace causis {

struct BlockStmt;
class Environment;
struct PointerValue {
  std::size_t allocationId = 0;
  std::size_t offset = 0;
  std::string elementType;
};

enum class ValueType { Int, Float, String, Bool, Function, Void, Pointer, Rational};

struct FunctionValue {
  std::vector<std::pair<std::string, std::string>> params;
  std::string returnType;
  const BlockStmt *body = nullptr;
  Environment *closure = nullptr;
};

struct RationalValue{
  std::int64_t numerator = 0;
  std::int64_t denominator = 1;
};

struct Value {
  using Storage = std::variant<int, double, std::string, bool,
                               std::shared_ptr<FunctionValue>, PointerValue,
                               RationalValue, std::monostate>;

  ValueType type = ValueType::Void;
  Storage data = std::monostate{};
};

struct ReturnValue {
  Value value;
};

struct BreakValue {};
struct ContinueValue {};

struct HeapAllocation {
  std::string elementType;
  Value value;
  bool alive = true;
};


} // namespace causis
