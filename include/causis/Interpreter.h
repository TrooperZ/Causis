#pragma once

#include "causis/AST.h"
#include "causis/Environment.h"
#include "causis/Value.h"

#include <vector>

namespace causis {

class Interpreter {
public:
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

