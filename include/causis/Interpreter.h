/**
 * @file Interpreter.h
 * @author Amin Karic
 * @brief Tree-walk interpreter interface for Causis programs.
 *
 * @details
 * The interpreter executes AST nodes directly and manages runtime environments,
 * values, and semantic checks such as mutability and type compatibility.
 */
#pragma once

#include "causis/AST.h"
#include "causis/Environment.h"
#include "causis/Value.h"

#include <vector>

namespace causis {

/**
 * @brief Executes parsed Causis programs.
 */
class Interpreter {
public:
  /**
   * @brief Execute a top-level program AST.
   *
   * @param program Top-level statements to evaluate.
   */
  void execute(const std::vector<std::unique_ptr<Stmt>> &program) {
    (void) program;
  }

private:
  Environment globals_;
  Environment *env_ = &globals_;

  void execStmt(const Stmt &stmt) {
    (void) stmt;
  }

  Value evalExpr(const Expr &expr) {
    (void) expr;
    return {};
  }

  void checkType(const std::string &declaredType, const Value &value) {
    (void) declaredType;
    (void) value;
  }
};

} // namespace causis
