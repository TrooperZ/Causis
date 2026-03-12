#pragma once

#include "causis/AST.h"
#include "causis/Environment.h"
#include "causis/Value.h"

#include <vector>

namespace causis {

class Interpreter {
public:
  void execute(const std::vector<std::unique_ptr<Stmt>> &program);

private:
  Environment _globals;
  Environment *_env = &_globals;

  void execStmt(const Stmt &stmt);

  Value evalExpr(const Expr &expr);

  void checkType(const std::string &declaredType, const Value &value);
};

} // namespace causis
