# Causis

<p align="center">
  <em>A reactive interpreted language.</em>
</p>

<p align="center">
  <a href="#overview">Overview</a> •
  <a href="#why-causis">Why Causis</a> •
  <a href="#current-language">Current Language</a> •
  <a href="#how-it-works">How It Works</a> •
  <a href="#project-layout">Project Layout</a> •
  <a href="#getting-started">Getting Started</a> •
  <a href="#roadmap">Roadmap</a>
</p>

---

## Overview



---

## Why Causis


## Current Language

The current subset supports:

- `let` declarations
- `state` declarations
- reassignment with `=`
- arithmetic expressions
- comparisons and equality
- `print(...)`
- semicolon-terminated statements

### Example

```causis
let x = 5;
state y = 10;

y = x + 2 * 3;
print(y);
```

Current output:

```txt
11
```

### Example With More Expressions

```causis
let x = 5;
state y = 10;

y = x + 1 * 3;
print(y);
print("\n");

let xA = y + 10 * 2 - 200 / 4;
print(xA + 1);
```

---



## Getting Started

### Prerequisites

- CMake 3.20+
- A C++20-compatible compiler

### Build

```bash
cmake -S . -B build
cmake --build build
```

### Run the Language

```bash
./build/causis examples/program.au
```

### Run the Parser Debug Tool

```bash
./build/parser_debug examples/program.au
```

This prints:

- lexer output
- parsed statement kinds
- a basic expression tree for debugging precedence

---

## What’s Implemented

- [x] token model
- [x] lexer for identifiers, integers, strings, operators, and semicolons
- [x] recursive-descent parser
- [x] expression precedence
- [x] `let`, `state`, reassignment, and `print`
- [x] tree-walk interpreter for the current subset
- [x] runtime environment for bindings

---

## Roadmap

- [ ] typed declarations like `let x: Int = 5;`
- [ ] real type checking through `checkType()`
- [ ] function declarations and function calls
- [ ] scoped function execution
- [ ] better diagnostics and error messages
- [ ] arrays and collection operations
- [ ] `derive` values for reactive computation
- [ ] `when` blocks for change-driven execution
- [ ] streams / event-driven features

---

## Design Direction

The long-term language direction for Causis is:

- **clear syntax** inspired by approachable languages
- **mutable state with explicit intent** via `state`
- **optional types** rather than forcing a strict static system from day one
- **reactive features** built on top of a solid runtime model

The immediate priority is correctness and clarity, not language complexity.

---

## Notes

This project is intentionally iterative.

A lot of the value in Causis is in building the pieces step by step, validating each phase, and learning from the tradeoffs before adding more surface area.

The README will evolve as the language evolves.
