/**
 * @file Parser.h
 * @author Amin Karic
 * @brief Recursive-descent parser interface for Causis.
 *
 * @details
 * The parser consumes tokens from the lexer and builds the abstract syntax
 * tree used by later stages such as interpretation and type checking.
 */
#pragma once

#include "causis/AST.h"
#include "causis/Token.h"

#include <cstddef>
#include <initializer_list>
#include <string>
#include <vector>

namespace causis {

/**
 * @brief Converts a token stream into Causis AST nodes.
 */
class Parser {
public:
  /**
   * @brief Construct a parser for a token sequence.
   *
   * @param tokens Tokens produced by the lexer.
   */
  explicit inline Parser(std::vector<Token> tokens) { _tokens = tokens; }

  /**
   * @brief Parse the full token stream into a top-level program AST.
   *
   * @return std::vector<std::unique_ptr<Stmt>> Parsed top-level statements in
   * source order.
   */
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

  std::unique_ptr<Stmt> parseDeclaration();
  std::unique_ptr<Stmt> parseLetDeclaration(bool mutableState);
  std::unique_ptr<Stmt> parseStatement();
  std::unique_ptr<Stmt> parseAssignmentStatement();
  std::unique_ptr<Stmt> parsePrintStatement();

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
