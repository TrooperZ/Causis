#include "causis/Parser.h"
#include "causis/AST.h"
#include "causis/Token.h"
#include "causis/TokenType.h"

#include <initializer_list>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

namespace causis {

const Token &Parser::previous() const {
  if (_current == 0) {
    throw std::runtime_error("previous() called before advancing.");
  }
  return _tokens[_current - 1];
}

const Token &Parser::advance() {
  if (!isAtEnd()) {
    ++_current;
    return _tokens[_current - 1];
  }
  return _tokens[_current];
}

bool Parser::check(TokenType type) const {
  if (isAtEnd()) {
    return false;
  }
  return peek().type == type;
}

bool Parser::match(std::initializer_list<TokenType> types) {
  for (const auto &t : types) {
    if (check(t)) {
      advance();
      return true;
    }
  }
  return false;
}

const Token &Parser::consume(TokenType type, const std::string &message) {
  if (check(type)) {
    return advance();
  }

  throw std::runtime_error(message + " Found: " + peek().lexeme);
}

std::vector<std::unique_ptr<Stmt>> Parser::parse() {
  std::vector<std::unique_ptr<Stmt>> prog;
  while (!isAtEnd()) {
    prog.emplace_back(parseDeclaration());
  }

  return prog;
}

std::unique_ptr<Stmt> Parser::parseDeclaration() {
  // Declarations are either a let, state, or a statement
  if (match({TokenType::KwLet})) {
    return parseLetDeclaration(false);
  }
  if (match({TokenType::KwState})) {
    return parseLetDeclaration(true);
  }
  return parseStatement();
}

std::unique_ptr<Stmt> Parser::parseStatement() {
  if (match({TokenType::KwPrint})) {
    return parsePrintStatement();
  }
  return parseAssignmentStatement();
}

std::unique_ptr<Expr> Parser::parseExpression() { return parseEquality(); }

std::unique_ptr<Expr> Parser::parseEquality() {
  std::unique_ptr<Expr> expr = parseComparison();

  while (match({TokenType::EqualEqual, TokenType::NotEqual})) {
    TokenType operation = previous().type;
    std::unique_ptr<Expr> right = parseComparison();
    auto node = std::make_unique<BinaryExpr>();
    node->left = std::move(expr);
    node->op = operation;
    node->right = std::move(right);
    expr = std::move(node);
  }
  return expr;
}

std::unique_ptr<Expr> Parser::parseComparison() {
  std::unique_ptr<Expr> expr = parseTerm();

  while (match({TokenType::Greater, TokenType::GreaterEqual, TokenType::Less,
                TokenType::LessEqual})) {
    TokenType operation = previous().type;
    std::unique_ptr<Expr> right = parseTerm();
    auto node = std::make_unique<BinaryExpr>();
    node->left = std::move(expr);
    node->op = operation;
    node->right = std::move(right);
    expr = std::move(node);
  }
  return expr;
}

std::unique_ptr<Expr> Parser::parseTerm() {
  std::unique_ptr<Expr> expr = parseFactor();

  while (match({TokenType::Plus, TokenType::Minus})) {
    TokenType operation = previous().type;
    std::unique_ptr<Expr> right = parseFactor();
    auto node = std::make_unique<BinaryExpr>();
    node->left = std::move(expr);
    node->op = operation;
    node->right = std::move(right);
    expr = std::move(node);
  }
  return expr;
}

std::unique_ptr<Expr> Parser::parseFactor() {
  std::unique_ptr<Expr> expr = parseUnary();

  while (match({TokenType::Star, TokenType::Slash})) {
    TokenType operation = previous().type;
    std::unique_ptr<Expr> right = parseUnary();
    auto node = std::make_unique<BinaryExpr>();
    node->left = std::move(expr);
    node->op = operation;
    node->right = std::move(right);
    expr = std::move(node);
  }
  return expr;
}

std::unique_ptr<Expr> Parser::parseUnary() {
  if (match({TokenType::Minus})) {
    TokenType operation = previous().type;
    std::unique_ptr<Expr> right = parseUnary();
    auto node = std::make_unique<UnaryExpr>();
    node->op = operation;
    node->operand = std::move(right);
    return node;
  }
  return parseCall();
}

std::unique_ptr<Expr> Parser::parseCall() {
  std::unique_ptr<Expr> expr = parsePrimary();

  while (match({TokenType::LParen})) {
    std::vector<std::unique_ptr<Expr>> args = parseArguments();
    consume(TokenType::RParen, "Expected ')' after arguments.");

    auto *callee = dynamic_cast<IdentifierExpr *>(expr.get());
    if (callee == nullptr) {
      throw std::runtime_error("Can only call named functions.");
    }

    auto node = std::make_unique<CallExpr>();
    node->callee = callee->name;
    node->args = std::move(args);
    expr = std::move(node);
  }
  return expr;
}

std::unique_ptr<Expr> Parser::parsePrimary() {
  if (match({TokenType::IntLiteral})) {
    auto node = std::make_unique<IntExpr>();
    node->value = std::stoi(previous().lexeme);
    return node;
  }

  if (match({TokenType::StringLiteral})) {
    auto node = std::make_unique<StringExpr>();
    node->value = previous().lexeme;
    return node;
  }

  if (match({TokenType::KwTrue})) {
    auto node = std::make_unique<BoolExpr>();
    node->boolean = true;
    return node;
  }

  if (match({TokenType::KwFalse})) {
    auto node = std::make_unique<BoolExpr>();
    node->boolean = false;
    return node;
  }

  if (match({TokenType::Identifier})) {
    auto node = std::make_unique<IdentifierExpr>();
    node->name = previous().lexeme;
    return node;
  }

  if (match({TokenType::LParen})) {
    std::unique_ptr<Expr> expr = parseExpression();
    consume(TokenType::RParen, "Expected ')' after expression.");
    return expr;
  }

  throw std::runtime_error("Expected expression");
}

std::unique_ptr<Stmt> Parser::parseLetDeclaration(bool mutableState) {
  const Token &name = consume(TokenType::Identifier,
                              "Expected variable name after declaration.");

  consume(TokenType::Equal, "Expected '=' after variable name in declaration.");

  std::unique_ptr<Expr> initializer = parseExpression();

  consume(TokenType::Semicolon, "Expected ';' after declaration.");

  auto statement = std::make_unique<LetStmt>();
  statement->name = name.lexeme;
  statement->typeName = "";
  statement->initializer = std::move(initializer);
  statement->mutableState = mutableState;

  return statement;
}

std::unique_ptr<Stmt> Parser::parseAssignmentStatement() {
  const Token &name =
      consume(TokenType::Identifier, "Expected assignment target.");

  consume(TokenType::Equal, "Expected '=' after assignment target.");
  std::unique_ptr<Expr> expr = parseExpression();
  consume(TokenType::Semicolon, "Expected ';' after assignment.");

  auto statement = std::make_unique<AssignStmt>();
  statement->name = name.lexeme;
  statement->value = std::move(expr);

  return statement;
}

std::unique_ptr<Stmt> Parser::parsePrintStatement() {
  consume(TokenType::LParen, "Expected '(' after print.");

  std::unique_ptr<Expr> expr = parseExpression();
  consume(TokenType::RParen, "Expected ')' after print expression.");
  consume(TokenType::Semicolon, "Expected ';' after print.");

  auto statement = std::make_unique<PrintStmt>();
  statement->expr = std::move(expr);

  return statement;
}

std::vector<std::unique_ptr<Expr>> Parser::parseArguments() {
  std::vector<std::unique_ptr<Expr>> args;
  if (check(TokenType::RParen)) {
    return args;
  }
  do {
    args.emplace_back(parseExpression());
  } while (match({TokenType::Comma}));
  return args;
}

} // namespace causis
