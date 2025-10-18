# GenZ-Lang Interpreter

A custom programming language and interpreter built as a mini-project for a Compiler Design course. The language features a unique syntax based on modern Gen Z slang.

The interpreter is built from scratch in C using Flex and Bison and utilizes an Abstract Syntax Tree (AST) architecture to cleanly separate parsing from execution.

## Features
- **Variable Declaration:** `the tea on x is 10 ~`
- **Conditional Logic:** `vibe check if (x > 5) then ... no cap`
- **While Loops:** `on repeat while (x > 0) ... finna stop`
- **Arithmetic Operations:** Supports `+`, `-`, `*`, and `/`.
- **Data Types:** Supports integers and strings.

## Tools & Architecture
- **Flex:** For lexical analysis (tokenizing the source code).
- **Bison:** For syntax analysis (parsing the token stream).
- **C/GCC:** For the AST data structures, tree-walking interpreter, and symbol table.
- **Abstract Syntax Tree (AST):** The parser builds an AST to represent the code's structure, which is then traversed and executed by the interpreter.

## How to Build and Run

### Prerequisites
- Flex
- Bison
- A C compiler like GCC (e.g., via MSYS2 on Windows)

### Build Steps
To compile the interpreter, run the following commands in order:
```bash
bison -d genz.y
flex genz.l
gcc main.c genz.tab.c lex.yy.c -o genz.exe
