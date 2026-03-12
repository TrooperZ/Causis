#include "causis/AST.h"
#include "causis/Lexer.h"
#include "causis/Parser.h"
#include "causis/Token.h"

#include <fstream>
#include <iostream>
#include <sstream>

namespace {

const char *tokenTypeName(causis::TokenType type) {
  switch (type) {
  case causis::TokenType::Plus:
    return "Plus";
  case causis::TokenType::Minus:
    return "Minus";
  case causis::TokenType::Star:
    return "Star";
  case causis::TokenType::Slash:
    return "Slash";
  case causis::TokenType::EqualEqual:
    return "EqualEqual";
  case causis::TokenType::NotEqual:
    return "NotEqual";
  case causis::TokenType::Greater:
    return "Greater";
  case causis::TokenType::GreaterEqual:
    return "GreaterEqual";
  case causis::TokenType::Less:
    return "Less";
  case causis::TokenType::LessEqual:
    return "LessEqual";
  default:
    return "Other";
  }
}

void printExpr(const causis::Expr *expr, int indent = 0) {
  std::string pad(indent, ' ');

  if (auto *intExpr = dynamic_cast<const causis::IntExpr *>(expr)) {
    std::cout << pad << "IntExpr(" << intExpr->value << ")\n";
    return;
  }

  if (auto *stringExpr = dynamic_cast<const causis::StringExpr *>(expr)) {
    std::cout << pad << "StringExpr(\"" << stringExpr->value << "\")\n";
    return;
  }

  if (auto *boolExpr = dynamic_cast<const causis::BoolExpr *>(expr)) {
    std::cout << pad << "BoolExpr(" << (boolExpr->boolean ? "true" : "false")
              << ")\n";
    return;
  }

  if (auto *identifierExpr =
          dynamic_cast<const causis::IdentifierExpr *>(expr)) {
    std::cout << pad << "IdentifierExpr(" << identifierExpr->name << ")\n";
    return;
  }

  if (auto *unaryExpr = dynamic_cast<const causis::UnaryExpr *>(expr)) {
    std::cout << pad << "UnaryExpr(op=" << tokenTypeName(unaryExpr->op)
              << ")\n";
    printExpr(unaryExpr->operand.get(), indent + 2);
    return;
  }

  if (auto *binaryExpr = dynamic_cast<const causis::BinaryExpr *>(expr)) {
    std::cout << pad << "BinaryExpr(op=" << tokenTypeName(binaryExpr->op)
              << ")\n";
    printExpr(binaryExpr->left.get(), indent + 2);
    printExpr(binaryExpr->right.get(), indent + 2);
    return;
  }

  if (auto *callExpr = dynamic_cast<const causis::CallExpr *>(expr)) {
    std::cout << pad << "CallExpr(" << callExpr->callee << ")\n";
    for (const auto &arg : callExpr->args) {
      printExpr(arg.get(), indent + 2);
    }
    return;
  }

  std::cout << pad << "UnknownExpr\n";
}

} // namespace

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << "Usage: parser_debug <source-file>\n";
    return 1;
  }

  std::ifstream ifs(argv[1]);
  if (!ifs.is_open()) {
    std::cerr << "Failed to open source file.\n";
    return 1;
  }

  std::ostringstream oss;
  oss << ifs.rdbuf();
  std::string code = oss.str();

  causis::Lexer lexer(code);
  auto tokens = lexer.scanTokens();

  for (const auto &token : tokens) {
    std::cout << "lexeme=[" << token.lexeme << "] line=" << token.line << "\n";
  }

  std::vector<std::unique_ptr<causis::Stmt>> program;
  try {
    causis::Parser parser(tokens);
    program = parser.parse();
    std::cout << "Parsed " << program.size() << " statements\n";
  } catch (const std::exception &ex) {
    std::cerr << "Parse error: " << ex.what() << "\n";
    return 1;
  }

  for (const auto &stmt : program) {
    if (dynamic_cast<causis::LetStmt *>(stmt.get())) {
      std::cout << "LetStmt\n";
    } else if (dynamic_cast<causis::AssignStmt *>(stmt.get())) {
      std::cout << "AssignStmt\n";
    } else if (dynamic_cast<causis::PrintStmt *>(stmt.get())) {
      std::cout << "PrintStmt\n";
    } else {
      std::cout << "UnknownStmt\n";
    }
  }

  for (const auto &stmt : program) {
    if (auto *letStmt = dynamic_cast<causis::LetStmt *>(stmt.get())) {
      std::cout << "Let name=" << letStmt->name
                << " mutable=" << letStmt->mutableState << "\n";
    } else if (auto *assignStmt =
                   dynamic_cast<causis::AssignStmt *>(stmt.get())) {
      std::cout << "Assign name=" << assignStmt->name << "\n";
    } else if (dynamic_cast<causis::PrintStmt *>(stmt.get())) {
      std::cout << "PrintStmt\n";
    }
  }

  if (program.size() > 2) {
    if (auto *assignStmt =
            dynamic_cast<causis::AssignStmt *>(program[2].get())) {
      std::cout << "Assignment expression tree:\n";
      printExpr(assignStmt->value.get(), 2);
    }
  }

  return 0;
}
