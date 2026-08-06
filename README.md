<div align="center">
  <img src="assets/logos/Causis_Banner_Space_Grotesk_Medium_Rounded.png" alt="Causis — a reactive programming language" width="100%">
</div>

> ***[causīs](https://en.wiktionary.org/wiki/causis)*** - Noun, Latin, dative/ablative plural of *causa*: “cause, reason.”

<div align="center">

  <p><strong>Explicit state. Reactive values. A readable runtime.</strong></p>
  <p>C++20 · Recursive-descent parser · Tree-walk interpreter</p>
  <p>
    <a href="#quick-start">Quick start</a> ·
    <a href="#language-snapshot">Language</a> ·
    <a href="#how-it-works">Architecture</a> ·
    <a href="#roadmap">Roadmap</a>
  </p>
</div>

## Reactive by design

Causis makes the relationship between immutable values, mutable state, and computed state visible in the language:

```causis
state count: int32 = 2;
derive doubled: int32 = count * 2;

print(doubled); // 4
count = 5;
print(doubled); // 10
```

`let` declares an immutable value, `state` declares mutable state, and `derive` declares a read-only value that is recomputed from its dependencies when read.

## Why Causis?

Causis explores reactive behavior without hiding mutation or control flow. Its implementation stays deliberately readable: a hand-written lexer tokenizes the source, a recursive-descent parser builds the AST, and a tree-walk interpreter executes it.

The language currently includes:

- typed `let`, `state`, and `derive` bindings
- lexical block scopes and runtime type checks
- functions with typed parameters and return values
- `if`, `else if`, `else`, `while`, and `for`
- `break`, `continue`, and `return`
- arithmetic, comparison, and boolean operators
- numeric conversion with `cast_as<T>(value)`
- experimental `ptr<T>` values with `null`
- distinct lexer, parser, and runtime error reporting

## Quick start

### Requirements

- CMake 3.20+
- A C++20-compatible compiler

### Build and run

```bash
cmake -S . -B build
cmake --build build
./build/causis examples/program.au
```

Run the focused reactive example:

```bash
./build/causis examples/derive.au
```

Expected output:

```text
=== derive ===
2
4
6
8
10
```

## Language snapshot

| Area | Status |
| --- | --- |
| Lexer, parser, and interpreter | Working |
| Primitive types and runtime checking | Working |
| Functions and control flow | Working |
| Derived bindings | Working |
| Chained derived bindings | Working |
| Null pointer values | Experimental |
| Allocation and dereference | Planned |
| `when` reactions | Planned |

Supported primitive types are `bool`, `string`, `uint8`–`uint64`, `int8`–`int64`, and `float32`–`float64`. Integer bindings enforce the range of their declared type at runtime.

## How it works

```text
source file
    ↓
  lexer  →  tokens
    ↓
  parser →  abstract syntax tree
    ↓
interpreter → scoped runtime values
```

The implementation is split into a small set of components:

- [`src/Lexer.cpp`](src/Lexer.cpp) tokenizes source code.
- [`src/Parser.cpp`](src/Parser.cpp) builds the abstract syntax tree.
- [`src/Interpreter.cpp`](src/Interpreter.cpp) executes programs and enforces runtime types.
- [`include/causis/AST.h`](include/causis/AST.h) defines expressions and statements.
- [`tests/parser_debug.cpp`](tests/parser_debug.cpp) provides a parser inspection utility.

Build and run the inspection tool with:

```bash
./build/parser_debug examples/program.au
```

## Examples

| Example | Demonstrates |
| --- | --- |
| [`program.au`](examples/program.au) | End-to-end language features |
| [`derive.au`](examples/derive.au) | Reactive derived values |
| [`derive_chain.au`](examples/derive_chain.au) | Chained derivation |
| [`derive_assign_error.au`](examples/derive_assign_error.au) | Derived-binding immutability error |
| [`derive_type_error.au`](examples/derive_type_error.au) | Derived-value type error |
| [`pointers.au`](examples/pointers.au) | Experimental null pointer values |

## Roadmap

- source-aware diagnostics
- pointer allocation, dereference, and assignment
- reactive `when` blocks
- automated interpreter regression tests
- collections and user-defined types

## Project status

Causis is under active development. The core interpreter and derived bindings are usable; pointer support and broader reactive features remain experimental.
