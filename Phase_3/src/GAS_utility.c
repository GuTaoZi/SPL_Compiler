#include "GAS_utility.h"

static const char *tokenStrings[] = {"INVALID",
                                     "TYPE",
                                     "STRUCT",
                                     "IF",
                                     "ELSE",
                                     "WHILE",
                                     "FOR",
                                     "RETURN",
                                     "INCLUDE",
                                     "BREAK",
                                     "CONTINUE",
                                     "DOT",
                                     "SEMI",
                                     "COLON",
                                     "COMMA",
                                     "ASSIGN",
                                     "LT",
                                     "LE",
                                     "GT",
                                     "GE",
                                     "NE",
                                     "EQ",
                                     "PLUS",
                                     "MINUS",
                                     "MUL",
                                     "DIV",
                                     "AND",
                                     "OR",
                                     "NOT",
                                     "LP",
                                     "RP",
                                     "LB",
                                     "RB",
                                     "LC",
                                     "RC",
                                     "ID",
                                     "UINT",
                                     "FLOAT",
                                     "CHAR",
                                     "STRING",
                                     "SINGLE_LINE_COMMENT",
                                     "MULTI_LINE_COMMENT",
                                     "LOWER_ELSE",
                                     "LOWER_FOR",
                                     "UPPER_FOR"};

size_t mlg10(size_t u)
{
    size_t n = 0;
    do
    {
        u /= 10;
        n += 1;
    } while (u);
    return n;
}

void reverse_str(char *s, size_t len)
{
    for (size_t i = 0; i < len / 2; i++)
    {
        char t = s[i];
        s[i] = s[len - 1 - i];
        s[len - 1 - i] = t;
    }
}

bool is_ws(const char c)
{
    return c == ' ' || c == '\n' || c == '\r' || c == '\t';
}

// Function to convert enum to string
const char *enum2str(TN enum_val)
{
    if (enum_val >= INVALID && enum_val <= UPPER_FOR)
    {
        return tokenStrings[enum_val - INVALID];
    }
    else
    {
        return "UNKNOWN";
    }
}