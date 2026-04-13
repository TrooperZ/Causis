#pragma once

#include "causis/TokenType.h"

#include <optional>
#include <string_view>

namespace causis {

std::optional<TokenType> lookupKeyword(std::string_view text);

bool isTypeKeyword(TokenType type);
std::optional<std::string_view> typeNameForToken(TokenType type);

bool isIntegerTypeName(std::string_view typeName);
bool isFloatTypeName(std::string_view typeName);
bool isSupportedTypeName(std::string_view typeName);

} // namespace causis
