#pragma once

#include "causis/Token.h"
#include "causis/TokenType.h"

#include <cstddef>
#include <string>
#include <vector>

namespace causis {

class Lexer {
public:
  explicit inline Lexer(std::string source) { _source = source; }
  std::vector<Token> scanTokens();

private:
  std::string _source;
  std::vector<Token> _tokens;
  std::size_t _start = 0;
  std::size_t _current = 0;
  int _line = 1;

  inline bool isAtEnd() const { return _current >= _source.size(); }
  inline char advance() { return _source[_current++]; }

  bool match(char expected);

  inline char peek() const { return isAtEnd() ? '\0' : _source[_current]; }
  inline char peekNext() const {
    return _current + 1 >= _source.length() ? '0' : _source[_current + 1];
  }

  void scanToken();
  void scanString();
  void scanNumber();
  void scanIdentifier();

  inline void addToken(TokenType type) {
    _tokens.emplace_back(type, _source.substr(_start, _current - _start),
                         _line);
  }
  inline void addToken(TokenType type, const std::string &lexeme) {
    _tokens.emplace_back(type, lexeme, _line);
  }
};

} // namespace causis
