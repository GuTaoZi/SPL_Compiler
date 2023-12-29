#ifndef __FRONT_UTILITY_H__
#define __FRONT_UTILITY_H__

#include <syntax.tab.h>
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

const char *enum2str(TN en);

#endif // __FRONT_UTILITY_H__
