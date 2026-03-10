#pragma once

namespace causis {

enum class TokenType {
  // User-defined names such as variable names, function names, and type names.
  Identifier,

  // Literal values written directly in source.
  IntLiteral,    // Example: 123
  StringLiteral, // Example: "hello"

  // Keywords for declarations and built-in language constructs.
  KwLet,   // Immutable variable declaration: let x: Int = 5
  KwState, // Mutable state declaration: state counter: Int = 0
  KwFn,    // Function declaration: fn add(a: Int, b: Int) -> Int => a + b
  KwPrint, // Built-in print statement/function: print(x)

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
  LParen, // (
  RParen, // )
  LBrace, // {
  RBrace, // }

  // Assignment and function syntax operators.
  Equal,       // =   Used in declarations: let x = 5
  AssignArrow, // <-  Used for mutable assignment: x <- 10
  FatArrow,    // =>  Used in expression-bodied functions
  ThinArrow,   // ->  Used before function return type

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

  // Marks the logical end of the input stream.
  EndOfFile
};

} // namespace causis

