A compiler for SUSTech Programming Language(SPL), a C-like custom programming language.

Project for CS323 Compilers, SUSTech.

This project is developed in 4 phases. For more details, please check the reports:

- [Phase 1: Lexical and Syntax Analyzer](https://github.com/GuTaoZi/SPL_Parser/blob/main/Phase_1/report/12110524-12111624-12112012-phase1.pdf)

  A parser for SPL that generates parse trees and reports lexical/syntax errors. 

- [Phase 2: Semantic Analyzer](https://github.com/GuTaoZi/SPL_Parser/blob/main/Phase_2/report/12110524-12111624-12112012-phase2.pdf)

  A semantic analyzer for SPL, introducing the symbol table and features including type checking.

## Environment

|   Tool    | Version |
| :-------: | :-----: |
|     C     |   C11   |
| GNU Bison | 3.0.4+  |
|   Flex    |  2.6.4  |

## Usage Guide

1. Move to the directory of one of the phases.
2. Run `make splc` in the root directory to generate a parser executable `bin/splc`.
3. Run the parser by `splc test/filename.spl`, the output of the parser will be saved in `test/filename.out`.

