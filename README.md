# Syndicate X — Mini Language Compiler

A compiler front-end for a mini programming language, built with **Flex** and **Bison**.
Course project for Compiler Construction Lab, Metropolitan University, Bangladesh.

## Team Members

| Name | Student ID | GitHub |
|------|-----------|--------|
| Azuad Islam Ruhan | 231-115-276 | [@AZUAD-ISLAM-RUHAN](https://github.com/AZUAD-ISLAM-RUHAN) |
| Priya Das | 231-115-107 | [@priyadas142307-rgb](https://github.com/priyadas142307-rgb) |

## Compiler Pipeline

```
Source Code (.mc)
      |
      v
Lexical Analyzer (Flex)       -> Token Stream
      |
      v
Syntax Analyzer (Bison)       -> Abstract Syntax Tree
      |
      v
Semantic Analyzer             -> Validated AST (type checking, scope checking)
   + Symbol Table              (nested scope support)
      |
      v
Intermediate Code Generator   -> Three Address Code (TAC)
```

## Features

- **Lexical Analysis**: recognizes keywords, identifiers, integer/float/boolean constants, operators, delimiters, comments; reports lexical errors with line numbers
- **Syntax Analysis**: full grammar for declarations, assignments, if/if-else, while loops, print statements, and nested blocks, with operator precedence
- **Abstract Syntax Tree**: builds and prints a readable tree representation of the parsed program
- **Symbol Table**: tracks variable name, type, and declaration line, with nested scope support (variables in inner blocks are not visible outside)
- **Semantic Analysis**: detects undeclared variables, redeclaration, type mismatches, invalid assignments, and invalid expressions (e.g. logical operators on numeric operands)
- **Three Address Code Generation**: generates TAC for arithmetic/relational/logical expressions and control flow (if/if-else/while) using temporaries, labels, and jumps

## Language Specification

**Data types:** `int`, `float`, `bool`

**Statements:** variable declaration, assignment, if / if-else, while, print, nested blocks

**Operators:**
- Arithmetic: `+ - * / %`
- Relational: `< > <= >= == !=`
- Logical: `&& || !`

### Sample Program

```
int x;
int y;
bool flag;
x = 10;
y = 0;
flag = true;
while (x > 0) {
    y = y + x;
    x = x - 1;
}
if (flag == true) {
    print y;
} else {
    print x;
}
```

## Build Instructions

Requires `flex`, `bison`, and `gcc` (available by default on most Linux systems / WSL Ubuntu).

```bash
make
```

This builds the `compiler` executable. To rebuild from scratch:

```bash
make clean
make
```

## Usage

```bash
./compiler path/to/program.mc
```

The compiler prints, in order: syntax analysis result, the Abstract Syntax Tree, semantic analysis result, the generated Three Address Code, and the symbol table(s) for each scope.

## Project Structure

```
project-root/
├── docs/                  Project report and design documents
├── src/
│   ├── lexer/             Flex lexical analyzer (lexer.l)
│   ├── parser/            Bison grammar and main driver (parser.y)
│   ├── ast/               AST node definitions and tree printing
│   ├── semantic/          Type checking and semantic error detection
│   ├── symbol_table/      Symbol table with nested scope support
│   └── tac/               Three Address Code generator
├── tests/
├── examples/              Sample valid and invalid test programs
├── Makefile
└── README.md
```

## Example Test Programs

The `examples/` directory contains sample programs demonstrating:
- Valid program compilation (`test1.mc`)
- Syntax errors (`test2_invalid.mc`)
- Redeclaration errors (`test3_redeclare.mc`)
- Nested scope handling (`test4_scope.mc`)
- Undeclared variable errors (`test5_undeclared.mc`)
- Type mismatch errors (`test6_typemismatch.mc`)
- Invalid expression errors (`test7_invalidexpr.mc`)
- TAC generation with while loops (`test8_tac.mc`)

## AI Usage

AI tools were used during development in accordance with the course's AI Usage Policy (Section 10 of the project manual). All team members can explain the implementation of every module.
