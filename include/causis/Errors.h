#pragma once

#include <cstddef>
#include <filesystem>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>

namespace causis {

struct SourceError : std::runtime_error {
  std::size_t line;
  std::size_t column;

  SourceError(std::size_t line, std::size_t column, std::string message)
      : std::runtime_error(std::move(message)), line(line), column(column) {}
};

void printSourceError(std::string_view stage, const std::filesystem::path &file,
                      std::string_view source, const SourceError &error);

} // namespace causis
