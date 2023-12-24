#ifndef __GAS_UTILITY_H__
#define __GAS_UTILITY_H__

#include <../src/syntax.tab.h>
#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <type.h>

enum kwtype
{
    Program = UPPER_FOR + 1,
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
    VARs
};

typedef long long ll;
typedef enum kwtype TN;

size_t mlg10(size_t u);

void reverse_str(char *s, size_t len);

bool is_ws(const char c);

const char *enum2str(TN en);

#endif // __GAS_UTILITY_H__
