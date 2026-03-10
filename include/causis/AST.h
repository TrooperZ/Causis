/**
 * @file AST.h
 * @author Amin Karic
 * @brief Abstract syntax tree node definitions for Causis.
 *
 * @details
 * These types represent the structured form of a parsed Causis program. Later
 * stages operate on these nodes instead of raw source text or tokens.
 */
#pragma once

#include "causis/TokenType.h"

#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace causis {

/**
 * @brief Base type for all expression nodes.
 */
struct Expr {
  virtual ~Expr() = default;
};

/**
 * @brief Integer literal expression.
 */
struct IntExpr : Expr {
  int value = 0;
};

/**
 * @brief String literal expression.
 */
struct StringExpr : Expr {
  std::string value;
};

/**
 * @brief Reference to a named value or function.
 */
struct IdentifierExpr : Expr {
  std::string name;
};

/**
 * @brief Binary operator expression such as addition or comparison.
 */
struct BinaryExpr : Expr {
  std::unique_ptr<Expr> left;
  TokenType op {};
  std::unique_ptr<Expr> right;
};

/**
 * @brief Function call expression.
 */
struct CallExpr : Expr {
  std::string callee;
  std::vector<std::unique_ptr<Expr>> args;
};

/**
 * @brief Base type for all statement nodes.
 */
struct Stmt {
  virtual ~Stmt() = default;
};

/**
 * @brief Declaration of an immutable variable or mutable state binding.
 */
struct LetStmt : Stmt {
  std::string name;
  std::string typeName;
  std::unique_ptr<Expr> initializer;
  bool mutableState = false;
};

/**
 * @brief Mutable assignment to an existing state binding.
 */
struct AssignStmt : Stmt {
  std::string name;
  std::unique_ptr<Expr> value;
};

/**
 * @brief Statement that evaluates and prints a value.
 */
struct PrintStmt : Stmt {
  std::unique_ptr<Expr> expr;
};

/**
 * @brief Function declaration with a parameter list and expression body.
 */
struct FnStmt : Stmt {
  std::string name;
  std::vector<std::pair<std::string, std::string>> params;
  std::string returnType;
  std::unique_ptr<Expr> body;
};

} // namespace causis
