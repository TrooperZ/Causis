#pragma once

#include "causis/AST.h"
#include "causis/Token.h"
#include "causis/TokenType.h"

#include <cstddef>
#include <initializer_list>
#include <memory>
#include <string>
#include <vector>

namespace causis {

class Parser {
public:
  explicit inline Parser(std::vector<Token> tokens) { _tokens = tokens; }
  std::vector<std::unique_ptr<Stmt>> parse();

private:
  std::vector<Token> _tokens;
  std::size_t _current = 0;

  inline bool isAtEnd() const {
    return _tokens[_current].type == TokenType::EndOfFile;
  }

  inline const Token &peek() const { return _tokens[_current]; }

  const Token &previous() const;
  const Token &advance();

  bool check(TokenType type) const;
  bool match(std::initializer_list<TokenType> types);
  const Token &consume(TokenType type, const std::string &message);

  /*
  Parser chain follows this ordering:

  Declaration
  Let/State
  Print/Assignment

  Expression
  Equality
  Comparison
  Term
  Factor
  Unary
  Func Call
  Primary
  */

  std::unique_ptr<Stmt> parseDeclaration();
  std::unique_ptr<Stmt> parseLetDeclaration(bool mutableState);
  std::unique_ptr<Stmt> parseStatement();
  std::unique_ptr<BlockStmt> parseBlockStatement();
  std::unique_ptr<Stmt> parseAssignmentStatement();
  std::unique_ptr<Stmt> parsePrintStatement();
  std::unique_ptr<Stmt> parseReturnStatement();
  std::unique_ptr<Stmt> parseFnDeclaration();
  std::unique_ptr<Stmt> parseIfStatement();
  std::unique_ptr<Stmt> parseWhileStatement();
  std::unique_ptr<Stmt> parseForStatement();
  std::unique_ptr<Stmt> parseBreakStatement();
  std::unique_ptr<Stmt> parseContinueStatement();

  std::unique_ptr<Expr> parseExpression();
  std::unique_ptr<Expr> parseEquality();
  std::unique_ptr<Expr> parseComparison();
  std::unique_ptr<Expr> parseTerm();
  std::unique_ptr<Expr> parseFactor();
  std::unique_ptr<Expr> parseUnary();
  std::unique_ptr<Expr> parseCall();
  std::unique_ptr<Expr> parsePrimary();

  std::vector<std::unique_ptr<Expr>> parseArguments();
};

} // namespace causis
