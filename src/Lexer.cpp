#include "causis/Lexer.h"
#include "causis/TokenType.h"
#include <_ctype.h>
#include <stdexcept>

namespace causis {

bool Lexer::match(char expected) {
  if (isAtEnd()) {
    return false;
  }
  if (_source[_current] != expected) {
    return false;
  }
  _current++;
  return true;
}

void Lexer::scanToken() {
  if (isAtEnd()) {
    return;
  }

  _start = _current;
  char c = advance();

  switch (c) {

  case ';':
    addToken(TokenType::Semicolon);
    break;

  case '(':
    addToken(TokenType::LParen);
    break;

  case ')':
    addToken(TokenType::RParen);
    break;

  case '{':
    addToken(TokenType::LBrace);
    break;

  case '}':
    addToken(TokenType::RBrace);
    break;

  case ',':
    addToken(TokenType::Comma);
    break;

  case '.':
    addToken(TokenType::Dot);
    break;

  case '-':
    if (match('>')) {
      addToken(TokenType::ThinArrow);
    } else {
      addToken(TokenType::Minus);
    }
    break;

  case '+':
    addToken(TokenType::Plus);
    break;

  case '*':
    addToken(TokenType::Star);
    break;

  /* Two‑char operators */
  case '=':
    if (match('=')) {
      addToken(TokenType::EqualEqual);
    } else if (match('>')) {
      addToken(TokenType::FatArrow);
    } else {
      addToken(TokenType::Equal);
    }
    break;
  case '<':
    if (match('=')) {
      addToken(TokenType::LessEqual);
    } else {
      addToken(TokenType::Less);
    }
    break;
  case '>':
    if (match('=')) {
      addToken(TokenType::GreaterEqual);
    } else {
      addToken(TokenType::Greater);
    }
    break;
  case '!':
    if (match('=')) {
      addToken(TokenType::NotEqual);
    } else {
      throw std::runtime_error("Unexpected character");
    }
    break;

  /* Whitespace */
  case ' ':
  case '\r':
  case '\t':
    // ignore
    break;

  case '\n':
    ++_line; // new line
    break;

  /* Literals */
  case '"':
    scanString();
    break;

  default:
    if (isnumber(c)) {
      scanNumber();
    } else if (isalpha(c)) {
      scanIdentifier();
    } else {
      throw std::runtime_error("Unexpected character");
    }
  }
}

void Lexer::scanString() {
  while (!isAtEnd() && peek() != '"') {
    if (peek() == '\n') {
      _line++;
    }
    if (peek() == '\\' && !isAtEnd()) {
      advance();          // consume backslash
      char c = advance(); // escaped char
      switch (c) {
      case '\"':
        _source[_current - 1] = '\"';
        break;

      case '\\':
        _source[_current - 1] = '\\';
        break;

      case 'n':
        _source[_current - 1] = '\n';
        break;

      case 't':
        _source[_current - 1] = '\t';
        break;

      default:
        throw std::runtime_error("Unknown escape sequence: \\" +
                                 std::string(1, c));
      }
    } else {
      advance(); // consume as normal
    }

    if (isAtEnd()) {
      throw std::runtime_error("Unterminated string literal");
      return;
    }

    advance(); // consume closing quote

    // The lexeme is everything between _start+1 and _current-1
    std::string value = _source.substr(_start + 1, _current - _start - 2);
    addToken(TokenType::StringLiteral, value);
  }
}
void Lexer::scanNumber() {
  while (!isAtEnd() && std::isdigit(static_cast<unsigned char>(peek()))) {
    advance();
  }

  addToken(TokenType::IntLiteral);
}

void Lexer::scanIdentifier() {
  while (!isAtEnd() && (isalpha(peek()) ||
                        std::isdigit(static_cast<unsigned char>(peek())))) {
    advance();
  }

  std::string text = _source.substr(_start, _current - _start);

  auto it = _keywords.find(text);
  if (it != _keywords.end()) {
    addToken(it->second);
  } else {
    addToken(TokenType::Identifier);
  }
}

std::vector<Token> Lexer::scanTokens() {
  while (!isAtEnd()) {
    _start = _current;
    scanToken();
  }

  addToken(TokenType::EndOfFile, "");
  return _tokens;
}

} // namespace causis
