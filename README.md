# Causis

<div align="center">

**A small interpreted language for experimenting with typed state, control flow, and reactive language ideas.**

[Getting started](#getting-started) | [Language snapshot](#language-snapshot) | [Project layout](#project-layout) | [Roadmap](#roadmap)

</div>

## What Is Causis?

Causis is a C++-built programming language project focused on growing a clean, understandable core before moving into its longer-term reactive ambitions.

Today, the repository contains:

- a lexer
- a recursive-descent parser
- a tree-walk interpreter
- lexical block scoping
- typed bindings with runtime checks
- functions and basic control flow

The current implementation is best thought of as an early language core, not a finished language product.

## Why Causis?

The project explores a language model built around a few ideas:

- `let` for immutable bindings
- `state` for explicit mutable state
- approachable syntax with familiar control flow
- incremental development toward richer reactive constructs

That means the README should describe the language as it exists now, while still making the direction clear: Causis is building toward reactivity on top of a simple executable core.

## Language Snapshot

The current language supports:

- immutable declarations with `let`
- mutable declarations with `state`
- optional declared types on bindings
- integer, floating-point, string, and boolean literals
- arithmetic: `+`, `-`, `*`, `/`
- comparisons: `>`, `>=`, `<`, `<=`, `==`, `!=`
- boolean logic: `&&`, `||`, `!`, `^`
- reassignment with `=`
- block scopes with `{ ... }`
- functions with typed parameters and optional return types
- `return`
- `if`, `else if`, `else`
- `while`
- `for`
- `break` and `continue`
- `print(...)`

Supported declared types currently include:

- `bool`
- `string`
- `uint8`, `int8`
- `uint16`, `int16`
- `uint32`, `int32`
- `uint64`, `int64`
- `float32`, `float64`

Type checks are enforced at runtime. Integer declarations also perform range checks for the smaller integer types currently modeled by the interpreter.

## Example

```causis
let seed: int32 = 2;
state total: int32 = 0;

fn add(a: int32, b: int32) -> int32 {
  return a + b;
}

for (state i: int32 = 0; i < 3; i = i + 1) {
  total = total + i;
}

if (add(seed, total) > 4) {
  print("ready\n");
} else {
  print("waiting\n");
}
```

You can find a larger end-to-end example in [examples/program.au](/Users/aminkaric/DevelopmentProjects/ProgrammingLanguage/Causis/examples/program.au).

## Current Status

Running the sample program today:

```bash
./build/causis examples/program.au
```

produces:

```text
=== logic ===
true
=== types ===
255
-8
65000
-1234
42
9000
3.75
typed values
=== blocks ===
5
=== functions ===
9
4
0
=== if / else if / else ===
200
=== while ===
0
1
2
while total=3
=== for ===
0
1
2
=== break / continue while ===
1
3
=== break / continue for ===
0
2
3
```

That sample exercises the main pieces of the language that are implemented in this repository right now.

## Getting Started

### Prerequisites

- CMake 3.20 or newer
- A C++20-compatible compiler

### Build

```bash
cmake -S . -B build
cmake --build build
```

### Run Causis

```bash
./build/causis examples/program.au
```

### Run the Parser Debug Tool

```bash
./build/parser_debug examples/program.au
```

`parser_debug` is a developer utility that prints lexer output, statement kinds, and expression tree information to help inspect parsing behavior.

## Project Layout

- [include/causis](/Users/aminkaric/DevelopmentProjects/ProgrammingLanguage/Causis/include/causis) contains the language headers, AST definitions, runtime value model, and environment types.
- [src/Lexer.cpp](/Users/aminkaric/DevelopmentProjects/ProgrammingLanguage/Causis/src/Lexer.cpp) implements tokenization.
- [src/Parser.cpp](/Users/aminkaric/DevelopmentProjects/ProgrammingLanguage/Causis/src/Parser.cpp) implements recursive-descent parsing.
- [src/Interpreter.cpp](/Users/aminkaric/DevelopmentProjects/ProgrammingLanguage/Causis/src/Interpreter.cpp) executes the AST with runtime type checks and scoped environments.
- [src/main.cpp](/Users/aminkaric/DevelopmentProjects/ProgrammingLanguage/Causis/src/main.cpp) provides the CLI entrypoint.
- [tests/parser_debug.cpp](/Users/aminkaric/DevelopmentProjects/ProgrammingLanguage/Causis/tests/parser_debug.cpp) provides the parser inspection executable.
- [examples](/Users/aminkaric/DevelopmentProjects/ProgrammingLanguage/Causis/examples) contains sample Causis programs.

## What Is Not There Yet

Causis is still in an early stage. Some important pieces are intentionally missing or incomplete:

- no static type checker
- no modules or imports
- no user-defined data types
- no collections or arrays
- no closures beyond storing function environments for calls
- no dedicated diagnostic system beyond thrown runtime and parse errors
- no reactive `derive` or `when` model yet

## Roadmap

Near-term work likely includes:

- improving diagnostics and source-aware error reporting
- expanding the value model and type system
- adding richer expression and statement forms
- introducing the first real reactive constructs
- strengthening tests around parsing and interpretation

## Design Direction

The long-term goal for Causis is not just to be another small interpreter. The interesting part is the language direction: explicit state, predictable control flow, and eventually a stronger reactive model built on top of a readable core.

For now, the emphasis is on making the implementation easy to reason about and extending it one solid piece at a time.
