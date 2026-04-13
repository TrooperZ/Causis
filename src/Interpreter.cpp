#include "causis/Interpreter.h"
#include "causis/AST.h"
#include "causis/Binding.h"
#include "causis/Environment.h"
#include "causis/TokenType.h"

#include <iostream>
#include <limits>
#include <memory>
#include <stdexcept>

namespace causis {

namespace {

bool isIntegerValue(const Value &value) { return value.type == ValueType::Int; }

bool isFloatValue(const Value &value) { return value.type == ValueType::Float; }

bool isNumericValue(const Value &value) {
  return isIntegerValue(value) || isFloatValue(value);
}

double toDouble(const Value &value) {
  if (value.type == ValueType::Float) {
    return std::get<double>(value.data);
  }
  return static_cast<double>(std::get<int>(value.data));
}

Value numericBinaryResult(const Value &left, const Value &right,
                          TokenType operation) {
  if (!isNumericValue(left) || !isNumericValue(right)) {
    throw std::runtime_error("Expected numeric operands.");
  }

  const bool useFloat = isFloatValue(left) || isFloatValue(right);

  if (useFloat) {
    const double lhs = toDouble(left);
    const double rhs = toDouble(right);

    switch (operation) {
    case TokenType::Plus:
      return Value(ValueType::Float, lhs + rhs);
    case TokenType::Minus:
      return Value(ValueType::Float, lhs - rhs);
    case TokenType::Star:
      return Value(ValueType::Float, lhs * rhs);
    case TokenType::Slash:
      return Value(ValueType::Float, lhs / rhs);
    case TokenType::Greater:
      return Value(ValueType::Bool, lhs > rhs);
    case TokenType::GreaterEqual:
      return Value(ValueType::Bool, lhs >= rhs);
    case TokenType::Less:
      return Value(ValueType::Bool, lhs < rhs);
    case TokenType::LessEqual:
      return Value(ValueType::Bool, lhs <= rhs);
    case TokenType::EqualEqual:
      return Value(ValueType::Bool, lhs == rhs);
    case TokenType::NotEqual:
      return Value(ValueType::Bool, lhs != rhs);
    default:
      break;
    }
  }

  const int lhs = std::get<int>(left.data);
  const int rhs = std::get<int>(right.data);

  switch (operation) {
  case TokenType::Plus:
    return Value(ValueType::Int, lhs + rhs);
  case TokenType::Minus:
    return Value(ValueType::Int, lhs - rhs);
  case TokenType::Star:
    return Value(ValueType::Int, lhs * rhs);
  case TokenType::Slash:
    return Value(ValueType::Int, lhs / rhs);
  case TokenType::Greater:
    return Value(ValueType::Bool, lhs > rhs);
  case TokenType::GreaterEqual:
    return Value(ValueType::Bool, lhs >= rhs);
  case TokenType::Less:
    return Value(ValueType::Bool, lhs < rhs);
  case TokenType::LessEqual:
    return Value(ValueType::Bool, lhs <= rhs);
  case TokenType::EqualEqual:
    return Value(ValueType::Bool, lhs == rhs);
  case TokenType::NotEqual:
    return Value(ValueType::Bool, lhs != rhs);
  default:
    break;
  }

  throw std::runtime_error("Unsupported numeric operator.");
}

} // namespace

void Interpreter::execute(const std::vector<std::unique_ptr<Stmt>> &program) {
  for (const auto &stmt : program) {
    execStmt(*stmt);
  }
}

void Interpreter::execStmt(const Stmt &stmt) {
  if (auto s = dynamic_cast<const BlockStmt *>(&stmt)) {
    Environment blockEnv(_env);
    execBlock(*s, blockEnv);
    return;
  }

  if (dynamic_cast<const BreakStmt *>(&stmt)) {
    throw BreakValue{};
  }

  if (dynamic_cast<const ContinueStmt *>(&stmt)) {
    throw ContinueValue{};
  }

  if (auto s = dynamic_cast<const LetStmt *>(&stmt)) {
    Value value = evalExpr(*(s->initializer));
    checkType(s->typeName, value);
    Binding b = Binding();
    b.declaredType = s->typeName;
    b.value = value;
    b.mutableState = s->mutableState;
    _env->define(s->name, b);
    return;
  }

  if (auto s = dynamic_cast<const FnStmt *>(&stmt)) {
    auto fn = std::make_shared<FunctionValue>();
    fn->params = s->params;
    fn->returnType = s->returnType;
    fn->body = s->body.get();
    fn->closure = _env;

    Binding b;
    b.declaredType = "";
    b.value = Value(ValueType::Function, fn);
    b.mutableState = false;

    _env->define(s->name, b);
    return;
  }

  if (auto s = dynamic_cast<const ReturnStmt *>(&stmt)) {
    Value value = evalExpr(*s->value);
    throw ReturnValue{value};
  }

  if (auto s = dynamic_cast<const AssignStmt *>(&stmt)) {
    Value value = evalExpr(*(s->value));
    const Binding &b = _env->get(s->name);
    checkType(b.declaredType, value);
    if (!b.mutableState) {
      throw std::runtime_error("Cannot assign to immutable binding");
    }
    _env->assign(s->name, value);
    return;
  }

  if (auto s = dynamic_cast<const PrintStmt *>(&stmt)) {
    Value value = evalExpr(*(s->expr));

    switch (value.type) {
    case ValueType::Int:
      std::cout << std::get<int>(value.data);
      break;
    case ValueType::Float:
      std::cout << std::get<double>(value.data);
      break;
    case ValueType::String:
      std::cout << std::get<std::string>(value.data);
      break;
    case ValueType::Bool:
      std::cout << (std::get<bool>(value.data) ? "true" : "false");
      break;
    case ValueType::Function:
      std::cout << "<function>";
      break;
    case ValueType::Void:
      std::cout << "void";
      break;
    }

    return;
  }

  if (auto s = dynamic_cast<const IfStmt *>(&stmt)) {
    Value condition = evalExpr(*s->condition);

    if (condition.type != ValueType::Bool) {
      throw std::runtime_error("If condition must be Bool.");
    }

    if (std::get<bool>(condition.data)) {
      execStmt(*s->thenBranch);
    } else if (s->elseBranch != nullptr) {
      execStmt(*s->elseBranch);
    }

    return;
  }

  if (auto s = dynamic_cast<const WhileStmt *>(&stmt)) {
    while (true) {
      Value condition = evalExpr(*s->condition);

      if (condition.type != ValueType::Bool) {
        throw std::runtime_error("While condition must be Bool.");
      }

      if (!std::get<bool>(condition.data)) {
        break;
      }

      try {
        execStmt(*s->body);
      } catch (const ContinueValue &) {
        continue;
      } catch (const BreakValue &) {
        break;
      }
    }

    return;
  }

  if (auto s = dynamic_cast<const ForStmt *>(&stmt)) {
    Environment loopEnv(_env);
    Environment *old = _env;
    _env = &loopEnv;

    try {
      if (s->initializer != nullptr) {
        execStmt(*s->initializer);
      }

      while (true) {
        if (s->condition != nullptr) {
          Value condition = evalExpr(*s->condition);

          if (condition.type != ValueType::Bool) {
            throw std::runtime_error("For condition must be Bool.");
          }

          if (!std::get<bool>(condition.data)) {
            break;
          }
        }

        try {
          execStmt(*s->body);
        } catch (const ContinueValue &) {
          if (s->increment != nullptr) {
            execStmt(*s->increment);
          }
          continue;
        } catch (const BreakValue &) {
          break;
        }

        if (s->increment != nullptr) {
          execStmt(*s->increment);
        }
      }
    } catch (...) {
      _env = old;
      throw;
    }
    _env = old;
    return;
  }

  return;
}

void Interpreter::execBlock(const BlockStmt &blockStmt, Environment &blockEnv) {
  Environment *old = _env;
  _env = &blockEnv;

  try {
    for (const auto &stmt : blockStmt.statements) {
      execStmt(*stmt);
    }
  } catch (...) {
    _env = old;
    throw;
  }

  _env = old;
}

Value Interpreter::evalExpr(const Expr &expr) {
  if (auto e = dynamic_cast<const CallExpr *>(&expr)) {
    const Binding &binding = _env->get(e->callee);

    if (binding.value.type != ValueType::Function) {
      throw std::runtime_error("Can only call functions.");
    }

    std::shared_ptr<causis::FunctionValue> fn =
        std::get<std::shared_ptr<FunctionValue>>(binding.value.data);

    if (e->args.size() != fn->params.size()) {
      throw std::runtime_error("Wrong number of arguments.");
    }

    Environment callEnv(fn->closure);

    for (size_t i = 0; i < e->args.size(); ++i) {
      Value argValue = evalExpr(*e->args[i]);

      Binding paramBinding;
      paramBinding.declaredType = fn->params[i].second;
      checkType(paramBinding.declaredType, argValue);
      paramBinding.value = argValue;
      paramBinding.mutableState = false;

      callEnv.define(fn->params[i].first, paramBinding);
    }

    try {
      execBlock(*fn->body, callEnv);
    } catch (const ReturnValue &result) {
      checkType(fn->returnType, result.value);
      return result.value;
    }

    Value voidValue;
    checkType(fn->returnType, voidValue);
    return voidValue;
  }

  if (auto e = dynamic_cast<const IntExpr *>(&expr)) {
    return Value(ValueType::Int, e->value);
  }

  if (auto e = dynamic_cast<const CastExpr *>(&expr)) {
    Value input = evalExpr(*e->value);
    return castValue(e->targetType, input);
  }

  if (auto e = dynamic_cast<const FloatExpr *>(&expr)) {
    return Value(ValueType::Float, e->value);
  }

  if (auto e = dynamic_cast<const StringExpr *>(&expr)) {
    return Value(ValueType::String, e->value);
  }

  if (auto e = dynamic_cast<const BoolExpr *>(&expr)) {
    return Value(ValueType::Bool, e->boolean);
  }

  if (auto e = dynamic_cast<const IdentifierExpr *>(&expr)) {
    const Binding &b = _env->get(e->name);
    return b.value;
  }

  if (auto e = dynamic_cast<const UnaryExpr *>(&expr)) {
    Value right = evalExpr(*(e->operand));
    if (e->op == TokenType::Minus) {
      if (right.type == ValueType::Int) {
        return Value(ValueType::Int, -std::get<int>(right.data));
      }
      if (right.type == ValueType::Float) {
        return Value(ValueType::Float, -std::get<double>(right.data));
      }
      throw std::runtime_error("Expected a numeric value in unary operator.");
    }
    if (e->op == TokenType::Bang) {
      if (right.type != ValueType::Bool) {
        throw std::runtime_error("Expected a Bool in unary operator.");
      }
      return Value(ValueType::Bool, !std::get<bool>(right.data));
    }
  }

  if (auto e = dynamic_cast<const BinaryExpr *>(&expr)) {
    Value left = evalExpr(*(e->left));
    switch (e->op) {
    case (TokenType::AndAnd):
      if (left.type != ValueType::Bool) {
        throw std::runtime_error("Expected && left operand to be Bool.");
      }
      if (!std::get<bool>(left.data)) {
        return Value(ValueType::Bool, false);
      }
      {
        Value right = evalExpr(*(e->right));
        if (right.type != ValueType::Bool) {
          throw std::runtime_error("Expected && right operand to be Bool.");
        }
        return Value(ValueType::Bool, std::get<bool>(right.data));
      }

    case (TokenType::OrOr):
      if (left.type != ValueType::Bool) {
        throw std::runtime_error("Expected || left operand to be Bool.");
      }
      if (std::get<bool>(left.data)) {
        return Value(ValueType::Bool, true);
      }
      {
        Value right = evalExpr(*(e->right));
        if (right.type != ValueType::Bool) {
          throw std::runtime_error("Expected || right operand to be Bool.");
        }
        return Value(ValueType::Bool, std::get<bool>(right.data));
      }

    case (TokenType::Caret): {
      Value right = evalExpr(*(e->right));
      if (left.type != ValueType::Bool || right.type != ValueType::Bool) {
        throw std::runtime_error("Expected ^ of two Bools.");
      }
      return Value(ValueType::Bool,
                   std::get<bool>(left.data) != std::get<bool>(right.data));
    }

    case (TokenType::Plus):
      return numericBinaryResult(left, evalExpr(*(e->right)), e->op);

    case (TokenType::Minus):
      return numericBinaryResult(left, evalExpr(*(e->right)), e->op);

    case (TokenType::Star):
      return numericBinaryResult(left, evalExpr(*(e->right)), e->op);

    case (TokenType::Slash):
      return numericBinaryResult(left, evalExpr(*(e->right)), e->op);

    case (TokenType::Greater):
      return numericBinaryResult(left, evalExpr(*(e->right)), e->op);

    case (TokenType::GreaterEqual):
      return numericBinaryResult(left, evalExpr(*(e->right)), e->op);

    case (TokenType::Less):
      return numericBinaryResult(left, evalExpr(*(e->right)), e->op);

    case (TokenType::LessEqual):
      return numericBinaryResult(left, evalExpr(*(e->right)), e->op);

    case (TokenType::EqualEqual):
      return numericBinaryResult(left, evalExpr(*(e->right)), e->op);

    case (TokenType::NotEqual):
      return numericBinaryResult(left, evalExpr(*(e->right)), e->op);
    default:
      break;
    }
  }

  throw std::runtime_error("Unsupported expression type.");
}

void Interpreter::checkType(const std::string &declaredType,
                            const Value &value) {
  if (declaredType.empty()) {
    return;
  }

  if (declaredType == "string") {
    if (value.type != ValueType::String) {
      throw std::runtime_error("Type error: expected string.");
    }
    return;
  }

  if (declaredType == "bool") {
    if (value.type != ValueType::Bool) {
      throw std::runtime_error("Type error: expected bool.");
    }
    return;
  }

  if (declaredType == "float32" || declaredType == "float64") {
    if (!isNumericValue(value)) {
      throw std::runtime_error("Type error: expected " + declaredType + ".");
    }
    return;
  }

  if (declaredType == "uint8" || declaredType == "int8" ||
      declaredType == "uint16" || declaredType == "int16" ||
      declaredType == "uint32" || declaredType == "int32" ||
      declaredType == "uint64" || declaredType == "int64") {
    if (value.type != ValueType::Int) {
      throw std::runtime_error("Type error: expected " + declaredType + ".");
    }

    const int numericValue = std::get<int>(value.data);

    if (declaredType == "uint8" &&
        (numericValue < 0 ||
         numericValue > std::numeric_limits<unsigned char>::max())) {
      throw std::runtime_error("Type error: value out of range for uint8.");
    }
    if (declaredType == "int8" &&
        (numericValue < std::numeric_limits<signed char>::min() ||
         numericValue > std::numeric_limits<signed char>::max())) {
      throw std::runtime_error("Type error: value out of range for int8.");
    }
    if (declaredType == "uint16" &&
        (numericValue < 0 ||
         numericValue > std::numeric_limits<unsigned short>::max())) {
      throw std::runtime_error("Type error: value out of range for uint16.");
    }
    if (declaredType == "int16" &&
        (numericValue < std::numeric_limits<short>::min() ||
         numericValue > std::numeric_limits<short>::max())) {
      throw std::runtime_error("Type error: value out of range for int16.");
    }
    if (declaredType == "uint32" && numericValue < 0) {
      throw std::runtime_error("Type error: value out of range for uint32.");
    }
    if (declaredType == "uint64" && numericValue < 0) {
      throw std::runtime_error("Type error: value out of range for uint64.");
    }
    return;
  }

  throw std::runtime_error("Unknown declared type: " + declaredType);
}

Value Interpreter::castValue(const std::string &targetType,
                             const Value &value) {
  if (targetType == "float32" || targetType == "float64") {
    if (value.type == ValueType::Int) {
      return Value(ValueType::Float,
                   static_cast<double>(std::get<int>(value.data)));
    }
    if (value.type == ValueType::Float) {
      return value;
    }
    throw std::runtime_error("Cannot cast to " + targetType);
  }

  if (targetType == "uint8" || targetType == "int8" || targetType == "uint16" ||
      targetType == "int16" || targetType == "uint32" ||
      targetType == "int32" || targetType == "uint64" ||
      targetType == "int64") {
    int converted = 0;

    if (value.type == ValueType::Int) {
      converted = std::get<int>(value.data);
    } else if (value.type == ValueType::Float) {
      converted = static_cast<int>(std::get<double>(value.data));
    } else {
      throw std::runtime_error("Cannot cast to " + targetType);
    }

    Value result(ValueType::Int, converted);
    checkType(targetType, result);
    return result;
  }

  throw std::runtime_error("Unsupported cast target: " + targetType);
}

} // namespace causis
