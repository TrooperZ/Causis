#pragma once

namespace causis {

enum class TokenType {
  // User-defined names such as variable names, function names, and type names.
  Identifier,

  // End of line marker
  Semicolon,

  // Literal values written directly in source.
  IntLiteral,    // 123
  StringLiteral, // "hello"

  // Keywords for declarations and built-in language constructs.
  KwLet,    // Immutable variable declaration: let x: Int = 5
  KwState,  // Mutable state declaration: state counter: Int = 0
  KwFn,     // Function declaration: fn add(a: Int, b: Int) -> Int => a + b
  KwReturn, // Return statement inside a function block
  KwPrint,  // Built-in print statement/function: print(x)

  // Built-in primitive type keywords.
  KwInt,    // Type name Int
  KwString, // Type name String
  KwBool,   // Type name Bool

  // Boolean literal keywords.
  KwTrue,  // Boolean literal true
  KwFalse, // Boolean literal false

  // Punctuation used for grouping and separating syntax.
  Colon,  // :
  Comma,  // ,
  Dot,    // .
  LParen, // (
  RParen, // )
  LBrace, // {
  RBrace, // }

  // Assignment and function syntax operators.
  Equal,     // =   Used in declarations and assignment statements
  ThinArrow, // ->  Used before function return type

  // Arithmetic operators.
  Plus,  // +
  Minus, // -
  Star,  // *
  Slash, // /

  // Comparison and equality operators.
  Greater,      // >
  GreaterEqual, // >=
  Less,         // <
  LessEqual,    // <=
  EqualEqual,   // ==
  NotEqual,     // !=

  // Conditionals
  KwIf,   // If statement declaration
  KwElse, // Else declaration for if statements, can chain if, else if, else

  // Loops
  KwFor,      // For loop declaration
  KwWhile,    // While loop declaration
  KwBreak,    // Stop a loop
  KwContinue, // Skip over a loop iteration

  // Marks the logical end of the input stream.
  EndOfFile
};

} // namespace causis
