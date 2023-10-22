%{
    #include "string.h"
    #include "syntax.tab.h"
%}

TYPE "int"|"float"|"char"

STRUCT "struct"
IF "if"
ELSE "else"
WHILE "while"
RETURN "return"

DOT "."
SEMI ";"
COMMA ","
ASSIGN "="
LT "<"
LE "<="
GT ">"
GE ">="
NE "!="
EQ "=="
PLUS "+"
MINUS "-"
MUL "*"
DIV "/"

AND "&&"
OR "||"
NOT "!"

LP "("
RP ")"
LB "["
RB "]"
LC "{"
RC "}"


letter [a-zA-Z]
letter_ [a-zA-Z_]
digit [0-9]
pos_digit [1-9]
hex_digit [0-9a-fA-F]

decimal [+-]?(0|{pos_digit}{digit}*)
hexadecimal [+-]?(0x)({hex_digit}+)

ID {letter_}+({letter_}|{digit})*
INT {decimal}|{hexadecimal}
FLOAT ({digit}+.{digit}+)
CHAR \'(\\x{hex_digit}{2}|.)\'
INVALID_CHAR \'.*\'
INVALID_ID ({INT}{ID})|{INVALID_CHAR}
SINGLE_LINE_COMMENT "//".*$
MULTI_LINE_COMMENT "/*"((("*"[^/])?)|[^*])*"*/" 


%%


%%
