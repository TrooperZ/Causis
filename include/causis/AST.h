#pragma once

#include "causis/TokenType.h"

#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace causis {

struct Expr {
  virtual ~Expr() = default;
};

struct IntExpr : Expr {
  int value = 0;
};

struct StringExpr : Expr {
  std::string value;
};

struct IdentifierExpr : Expr {
  std::string name;
};

struct BinaryExpr : Expr {
  std::unique_ptr<Expr> left;
  TokenType op {};
  std::unique_ptr<Expr> right;
};

struct CallExpr : Expr {
  std::string callee;
  std::vector<std::unique_ptr<Expr>> args;
};

struct Stmt {
  virtual ~Stmt() = default;
};

struct LetStmt : Stmt {
  std::string name;
  std::string typeName;
  std::unique_ptr<Expr> initializer;
  bool mutableState = false;
};

struct AssignStmt : Stmt {
  std::string name;
  std::unique_ptr<Expr> value;
};

struct PrintStmt : Stmt {
  std::unique_ptr<Expr> expr;
};

struct FnStmt : Stmt {
  std::string name;
  std::vector<std::pair<std::string, std::string>> params;
  std::string returnType;
  std::unique_ptr<Expr> body;
};

} // namespace causis

