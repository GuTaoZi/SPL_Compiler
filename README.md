A compiler for SUSTech Programming Language(SPL), a C-like custom programming language.

Project for CS323 Compilers, SUSTech.

This project is developed in 4 phases. For more details, please check the reports:

- [Phase 1: Lexical and Syntax Analyzer](https://github.com/GuTaoZi/SPL_Parser/blob/main/Phase_1/report/12110524-12111624-12112012-phase1.pdf)

  A parser for SPL that generates parse trees and reports lexical/syntax errors. 

- [Phase 2: Semantic Analyzer](https://github.com/GuTaoZi/SPL_Parser/blob/main/Phase_2/report/12110524-12111624-12112012-phase2.pdf)

  A semantic analyzer for SPL, introducing the symbol table and features including type checking.

- [Phase 3: Intermediate Code Generator](https://github.com/GuTaoZi/SPL_Parser/blob/main/Phase_3/report/12110524-12111624-12112012-phase3.pdf)

  An intermediate code generator, compile the SPL code into intermediate code which could be run with [IR simulator](https://github.com/GuTaoZi/SPL_Parser/blob/main/Phase_3/irsim_linux-x86_64.zip)

- [Phase 4: Target Code Generator](https://github.com/GuTaoZi/SPL_Parser/blob/main/Phase_3/report/12110524-12111624-12112012-phase4.pdf)

  A target code generator, translating the intermediate code into MIPS32 code.

## Environment

|   Tool    | Version |
| :-------: | :-----: |
|     C     |   C99   |
| GNU Bison | 3.0.4+  |
|   Flex    |  2.6.4  |

## Usage Guide

For usage guide of each phase, please refer to the **How to Run** section in the corresponding report.

1. [Lexical and Syntax Analyzer](https://github.com/GuTaoZi/SPL_Parser/blob/main/Phase_1/report/12110524-12111624-12112012-phase1.pdf)

2. [Semantic Analyzer](https://github.com/GuTaoZi/SPL_Parser/blob/main/Phase_2/report/12110524-12111624-12112012-phase2.pdf)

3. [Intermediate Code Generator](https://github.com/GuTaoZi/SPL_Parser/blob/main/Phase_3/report/12110524-12111624-12112012-phase3.pdf)

4. [Target Code Generator](https://github.com/GuTaoZi/SPL_Parser/blob/main/Phase_3/report/12110524-12111624-12112012-phase4.pdf)