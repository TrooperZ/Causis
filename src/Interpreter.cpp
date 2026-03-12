#include "causis/Interpreter.h"
#include "causis/AST.h"
#include "causis/Binding.h"
#include "causis/TokenType.h"
#include "causis/ValueType.h"
#include <iostream>
#include <stdexcept>

namespace causis {

void Interpreter::execute(const std::vector<std::unique_ptr<Stmt>> &program) {
  for (const auto &stmt : program) {
    execStmt(*stmt);
  }
}

void Interpreter::execStmt(const Stmt &stmt) {
  if (auto s = dynamic_cast<const LetStmt *>(&stmt)) {
    Value value = evalExpr(*(s->initializer));
    Binding b = Binding();
    b.declaredType = s->typeName;
    b.value = value;
    b.mutableState = s->mutableState;
    _env->define(s->name, b);
    return;
  }

  if (auto s = dynamic_cast<const AssignStmt *>(&stmt)) {
    Value value = evalExpr(*(s->value));
    Binding b = _env->get(s->name);
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

  return;
}

Value Interpreter::evalExpr(const Expr &expr) {
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
    Binding b = _env->get(e->name);
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
  return;
}

} // namespace causis
