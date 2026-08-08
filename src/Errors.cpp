#include "causis/Errors.h"

#include <cstddef>
#include <iostream>
#include <sstream>
#include <string>
#include <sys/ioctl.h>
#include <unistd.h>

namespace causis {

std::size_t terminalWidth() {
  winsize size{};

  if (ioctl(STDERR_FILENO, TIOCGWINSZ, &size) == 0 && size.ws_col > 0) {
    return size.ws_col;
  }

  return 80;
}

void printSourceError(std::string_view stage, const std::filesystem::path &file,
                      std::string_view source, const SourceError &error) {
  std::istringstream lines{std::string(source)};
  std::string sourceLine;

  for (std::size_t line = 1; line <= error.line; ++line) {
    if (!std::getline(lines, sourceLine)) {
      sourceLine.clear();
      break;
    }
  }

  const std::string lineNumber = std::to_string(error.line);
  const std::string gutter(lineNumber.size(), ' ');
  const std::string border(terminalWidth(), '=');

  const bool useColor = isatty(STDERR_FILENO) != 0;
  const char *red = useColor ? "\033[31m" : "";
  const char *blue = useColor ? "\033[34m" : "";
  const char *green = useColor ? "\033[32m" : "";
  const char *reset = useColor ? "\033[0m" : "";

  std::cerr << '\n'
            << red << border << '\n'
            << "error" << reset << '[' << blue << stage << reset
            << "]: " << error.what() << '\n'
            << " --> " << file.filename().string() << ':' << error.line << ':'
            << error.column << '\n'
            << green << gutter << " |" << reset << '\n'
            << green << lineNumber << " |" << reset << ' ' << sourceLine << '\n'
            << green << gutter << " |" << reset << ' ';

  for (std::size_t column = 1; column < error.column; ++column) {
    const char character =
        column <= sourceLine.size() ? sourceLine[column - 1] : ' ';
    std::cerr << (character == '\t' ? '\t' : ' ');
  }

  std::cerr << "^\n";
}

} // namespace causis
