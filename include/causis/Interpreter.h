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
  void execBlock(const BlockStmt &blockStmt, Environment &blockEnv);

  Value castValue(const std::string &targetType, const Value &value);

  Value evalExpr(const Expr &expr);

  void checkType(const std::string &declaredType, const Value &value);
};

} // namespace causis

/*

let: immutable
state: mutable
+ / - *

<= = >= > <



*/