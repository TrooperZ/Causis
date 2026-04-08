#include "causis/Interpreter.h"
#include "causis/AST.h"
#include "causis/Binding.h"
#include "causis/Environment.h"
#include "causis/TokenType.h"

#include <iostream>
#include <memory>
#include <stdexcept>

namespace causis {

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

      execStmt(*s->body);
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

        execStmt(*s->body);

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
      if (right.type != ValueType::Int) {
        throw std::runtime_error("Expected an int in unary operator.");
      }
      return Value(ValueType::Int, -std::get<int>(right.data));
    }
  }

  if (auto e = dynamic_cast<const BinaryExpr *>(&expr)) {
    Value left = evalExpr(*(e->left));
    Value right = evalExpr(*(e->right));
    switch (e->op) {
    case (TokenType::Plus):
      if (left.type != ValueType::Int || right.type != ValueType::Int) {
        throw std::runtime_error("Expected + of two ints.");
      }
      return Value(ValueType::Int,
                   std::get<int>(left.data) + std::get<int>(right.data));

    case (TokenType::Minus):
      if (left.type != ValueType::Int || right.type != ValueType::Int) {
        throw std::runtime_error("Expected - of two ints.");
      }
      return Value(ValueType::Int,
                   std::get<int>(left.data) - std::get<int>(right.data));

    case (TokenType::Star):
      if (left.type != ValueType::Int || right.type != ValueType::Int) {
        throw std::runtime_error("Expected * of two ints.");
      }
      return Value(ValueType::Int,
                   std::get<int>(left.data) * std::get<int>(right.data));

    case (TokenType::Slash):
      if (left.type != ValueType::Int || right.type != ValueType::Int) {
        throw std::runtime_error("Expected / of two ints.");
      }
      return Value(ValueType::Int,
                   std::get<int>(left.data) / std::get<int>(right.data));

    case (TokenType::Greater):
      if (left.type != ValueType::Int || right.type != ValueType::Int) {
        throw std::runtime_error("Expected > of two ints.");
      }
      return Value(ValueType::Bool,
                   std::get<int>(left.data) > std::get<int>(right.data));

    case (TokenType::GreaterEqual):
      if (left.type != ValueType::Int || right.type != ValueType::Int) {
        throw std::runtime_error("Expected >= of two ints.");
      }
      return Value(ValueType::Bool,
                   std::get<int>(left.data) >= std::get<int>(right.data));

    case (TokenType::Less):
      if (left.type != ValueType::Int || right.type != ValueType::Int) {
        throw std::runtime_error("Expected < of two ints.");
      }
      return Value(ValueType::Bool,
                   std::get<int>(left.data) < std::get<int>(right.data));

    case (TokenType::LessEqual):
      if (left.type != ValueType::Int || right.type != ValueType::Int) {
        throw std::runtime_error("Expected <= of two ints.");
      }
      return Value(ValueType::Bool,
                   std::get<int>(left.data) <= std::get<int>(right.data));

    case (TokenType::EqualEqual):
      if (left.type != ValueType::Int || right.type != ValueType::Int) {
        throw std::runtime_error("Expected == of two ints.");
      }
      return Value(ValueType::Bool,
                   std::get<int>(left.data) == std::get<int>(right.data));

    case (TokenType::NotEqual):
      if (left.type != ValueType::Int || right.type != ValueType::Int) {
        throw std::runtime_error("Expected != of two ints.");
      }
      return Value(ValueType::Bool,
                   std::get<int>(left.data) != std::get<int>(right.data));
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

  if (declaredType == "Int") {
    if (value.type != ValueType::Int) {
      throw std::runtime_error("Type error: expected Int.");
    }
    return;
  }

  if (declaredType == "String") {
    if (value.type != ValueType::String) {
      throw std::runtime_error("Type error: expected String.");
    }
    return;
  }

  if (declaredType == "Bool") {
    if (value.type != ValueType::Bool) {
      throw std::runtime_error("Type error: expected Bool.");
    }
    return;
  }

  throw std::runtime_error("Unknown declared type: " + declaredType);
}

} // namespace causis
