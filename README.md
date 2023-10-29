A parser for SUSTech Programming Language(SPL) that generates parse trees and reports lexical/syntax errors. Project for CS323 Compilers.

For more details, please check [the report](https://github.com/GuTaoZi/SPL_Parser/blob/main/report/12110524-12111624-12112012-phase1.pdf).

## Contributors

| SID      | Name                                            | Contributions                  |
| -------- | ----------------------------------------------- | ------------------------------ |
| 12111624 | [GuTaoZi](https://github.com/GuTaoZi)           | Error Handling, Comment, Doc   |
| 12110524 | [Artanisax](https://github.com/Artanisax)       | String, Samples, Debugging     |
| 12112012 | [ShadowStorm](https://github.com/Jayfeather233) | Tree Structure, For, Inclusion |

## Environment

|   Tool    | Version |
| :-------: | :-----: |
|     C     |   C11   |
| GNU Bison | 3.0.4+  |
|   Flex    |  2.6.4  |

## Usage Guide

1. Run `make splc` in the root directory to generate a parser executable `bin/splc`.
2. Run the parser by `splc test/filename.spl`, the output of the parser will be saved in `test/filename.out`.

