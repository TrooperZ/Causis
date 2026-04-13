#include "causis/Language.h"

#include <array>

namespace causis {

namespace {

using KeywordEntry = std::pair<std::string_view, TokenType>;

constexpr std::array<KeywordEntry, 27> kKeywords = {{
    {"let", TokenType::KwLet},
    {"state", TokenType::KwState},
    {"fn", TokenType::KwFn},
    {"return", TokenType::KwReturn},
    {"print", TokenType::KwPrint},
    {"bool", TokenType::KwBool},
    {"string", TokenType::KwString},
    {"int32", TokenType::KwInt32},
    {"uint8", TokenType::KwUint8},
    {"int8", TokenType::KwInt8},
    {"uint16", TokenType::KwUint16},
    {"int16", TokenType::KwInt16},
    {"uint32", TokenType::KwUint32},
    {"uint64", TokenType::KwUint64},
    {"int64", TokenType::KwInt64},
    {"float32", TokenType::KwFloat32},
    {"float64", TokenType::KwFloat64},
    {"true", TokenType::KwTrue},
    {"false", TokenType::KwFalse},
    {"if", TokenType::KwIf},
    {"else", TokenType::KwElse},
    {"for", TokenType::KwFor},
    {"while", TokenType::KwWhile},
    {"break", TokenType::KwBreak},
    {"continue", TokenType::KwContinue},
    {"cast_as", TokenType::KwCastAs},
}};

} // namespace

std::optional<TokenType> lookupKeyword(std::string_view text) {
  for (const auto &[keyword, type] : kKeywords) {
    if (keyword == text) {
      return type;
    }
  }

  return std::nullopt;
}

bool isTypeKeyword(TokenType type) { return typeNameForToken(type).has_value(); }

std::optional<std::string_view> typeNameForToken(TokenType type) {
  switch (type) {
  case TokenType::KwString:
    return "string";
  case TokenType::KwBool:
    return "bool";
  case TokenType::KwUint8:
    return "uint8";
  case TokenType::KwInt8:
    return "int8";
  case TokenType::KwUint16:
    return "uint16";
  case TokenType::KwInt16:
    return "int16";
  case TokenType::KwUint32:
    return "uint32";
  case TokenType::KwInt32:
    return "int32";
  case TokenType::KwUint64:
    return "uint64";
  case TokenType::KwInt64:
    return "int64";
  case TokenType::KwFloat32:
    return "float32";
  case TokenType::KwFloat64:
    return "float64";
  default:
    return std::nullopt;
  }
}

bool isIntegerTypeName(std::string_view typeName) {
  return typeName == "uint8" || typeName == "int8" || typeName == "uint16" ||
         typeName == "int16" || typeName == "uint32" ||
         typeName == "int32" || typeName == "uint64" || typeName == "int64";
}

bool isFloatTypeName(std::string_view typeName) {
  return typeName == "float32" || typeName == "float64";
}

bool isSupportedTypeName(std::string_view typeName) {
  return typeName == "string" || typeName == "bool" ||
         isIntegerTypeName(typeName) || isFloatTypeName(typeName);
}

} // namespace causis
