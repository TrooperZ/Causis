#include "causis/Language.h"
#include "causis/TokenType.h"

#include <array>
#include <optional>

namespace causis {

namespace {

using KeywordEntry = std::pair<std::string_view, TokenType>;

constexpr std::array<KeywordEntry, 31> kKeywords = {{
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
    {"ptr", TokenType::KwPtr},
    {"null", TokenType::KwNull},
    {"void", TokenType::KwVoid},
    {"derive", TokenType::KwDerive},
    {"when", TokenType::KwWhen},

}};

constexpr std::array<KeywordEntry, 13> kTypeKeywords = {{
    {"bool", TokenType::KwBool},
    {"string", TokenType::KwString},
    {"uint8", TokenType::KwUint8},
    {"int8", TokenType::KwInt8},
    {"uint16", TokenType::KwUint16},
    {"int16", TokenType::KwInt16},
    {"uint32", TokenType::KwUint32},
    {"int32", TokenType::KwInt32},
    {"uint64", TokenType::KwUint64},
    {"int64", TokenType::KwInt64},
    {"float32", TokenType::KwFloat32},
    {"float64", TokenType::KwFloat64},
    {"void", TokenType::KwVoid},
}};

} // namespace

std::optional<TokenType> lookupKeyword(std::string_view text) {
  for (const auto &[keyword, type] : kKeywords) {
    // While a hash-map might give O(1) lookup time, a small enough static array
    // is faster due to cache performance
    if (keyword == text) {
      return type;
    }
  }

  return std::nullopt;
}

bool isTypeKeyword(TokenType type) {
  return typeNameForToken(type).has_value();
}

std::optional<std::string_view> typeNameForToken(TokenType type) {
  for (const auto &[keyword, tokenType] : kTypeKeywords) {
    if (tokenType == type) {
      return keyword;
    }
  }

  return std::nullopt;
}

bool isIntegerTypeName(std::string_view typeName) {
  return typeName == "uint8" || typeName == "int8" || typeName == "uint16" ||
         typeName == "int16" || typeName == "uint32" || typeName == "int32" ||
         typeName == "uint64" || typeName == "int64";
}

bool isFloatTypeName(std::string_view typeName) {
  return typeName == "float32" || typeName == "float64";
}

bool isSupportedTypeName(std::string_view typeName) {
  for (const auto &[keyword, _] : kTypeKeywords) {
    if (keyword == typeName) {
      return true;
    }
  }

  return false;
}

} // namespace causis
