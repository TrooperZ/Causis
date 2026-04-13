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

struct FloatExpr : Expr {
  double value = 0.0;
};

struct StringExpr : Expr {
  std::string value;
};

struct IdentifierExpr : Expr {
  std::string name;
};

struct BinaryExpr : Expr {
  std::unique_ptr<Expr> left;
  TokenType op{};
  std::unique_ptr<Expr> right;
};

struct BoolExpr : Expr {
  bool boolean;
};

struct UnaryExpr : Expr {
  TokenType op;
  std::unique_ptr<Expr> operand;
};

struct CallExpr : Expr {
  std::string callee;
  std::vector<std::unique_ptr<Expr>> args;
};

struct CastExpr : Expr {
  std::string targetType;
  std::unique_ptr<Expr> value;
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

struct BlockStmt : Stmt {
  std::vector<std::unique_ptr<Stmt>> statements;
};

struct ReturnStmt : Stmt {
  std::unique_ptr<Expr> value;
};

struct FnStmt : Stmt {
  std::string name;
  std::vector<std::pair<std::string, std::string>> params;
  std::string returnType;
  std::unique_ptr<BlockStmt> body;
};

struct IfStmt : Stmt {
  std::unique_ptr<Expr> condition;
  std::unique_ptr<Stmt> thenBranch;
  std::unique_ptr<Stmt> elseBranch;
};

struct WhileStmt : Stmt {
  std::unique_ptr<Expr> condition;
  std::unique_ptr<Stmt> body;
};

struct ForStmt : Stmt {
  std::unique_ptr<Stmt> initializer;
  std::unique_ptr<Expr> condition;
  std::unique_ptr<Stmt> increment;
  std::unique_ptr<Stmt> body;
};

struct BreakStmt : Stmt {};
struct ContinueStmt : Stmt {};

} // namespace causis
