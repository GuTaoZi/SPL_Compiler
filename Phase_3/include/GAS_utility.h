#ifndef __GAS_UTILITY_H__
#define __GAS_UTILITY_H__

#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <type.h>

enum kwtype
{
    INVALID = 258,
    TYPE = 259,
    STRUCT = 260,
    IF = 261,
    ELSE = 262,
    WHILE = 263,
    FOR = 264,
    RETURN = 265,
    INCLUDE = 266,
    BREAK = 267,
    CONTINUE = 268,
    DOT = 269,
    SEMI = 270,
    COLON = 271,
    COMMA = 272,
    ASSIGN = 273,
    LT = 274,
    LE = 275,
    GT = 276,
    GE = 277,
    NE = 278,
    EQ = 279,
    PLUS = 280,
    MINUS = 281,
    MUL = 282,
    DIV = 283,
    AND = 284,
    OR = 285,
    NOT = 286,
    LP = 287,
    RP = 288,
    LB = 289,
    RB = 290,
    LC = 291,
    RC = 292,
    ID = 293,
    UINT = 294,
    FLOAT = 295,
    CHAR = 296,
    STRING = 297,
    SINGLE_LINE_COMMENT = 298,
    MULTI_LINE_COMMENT = 299,
    LOWER_ELSE = 300,
    LOWER_FOR = 301,
    UPPER_FOR = 302,
    Program,
    HeaderDefList,
    Headers,
    ExtDefList,
    ExtDef,
    ExtDecList,
    Specifier,
    StructSpecifier,
    VarDec,
    FunDec,
    VarList,
    ParamDec,
    CompSt,
    StmtList,
    Stmt,
    DefList,
    Def,
    DecList,
    Dec,
    Exp,
    Args,
    Var
};

typedef long long ll;
typedef enum kwtype TN;

size_t mlg10(size_t u);

void reverse_str(char *s, size_t len);

bool is_ws(const char c);

const char *enum2str(TN en);

#endif // __GAS_UTILITY_H__
