%option yylineno
%{
    #include "treeNode.h"
    #include "syntax.tab.h"
    #define YYSTYPE treeNode*
    extern int has_error = 0;

    void print_A_error(size_t lineno, char *text)
    {
        fprintf(yyout,"Error type A at Line %d: unknown lexeme %s\n", lineno, text);
    }
%}

letter [a-zA-Z]
letter_ [a-zA-Z_]
digit [0-9]
pos_digit [1-9]
hex_digit [0-9a-fA-F]

decimal [+-]?(0|{pos_digit}{digit}*)
hexadecimal [+-]?(0x)({hex_digit}+)
FLOAT ({digit}+.{digit}+)

new_line \n|(\r\n)
VALID_STRING_CHAR [^\"{new_line}]
STRING \"({VALID_STRING_CHAR}*\\{new_line})*{VALID_STRING_CHAR}*\"

ID {letter_}+({letter_}|{digit})*
INT {decimal}|{hexadecimal}
INVALID_CHAR \'.*\'
INVALID_NUMBER (0{digit}*)|(({FLOAT}.({digit}|.)*))|({hexadecimal}.({digit}|.)*)

%%

"int"|"float"|"char"                       { yylval = new_node("TYPE", yytext, yylineno); return TYPE; }

"struct"                                 { yylval = new_node("STRUCT", yytext, yylineno); return STRUCT; }
"if"                                         { yylval = new_node("IF", yytext, yylineno); return IF; }
"else"                                     { yylval = new_node("ELSE", yytext, yylineno); return ELSE; }
"while"                                   { yylval = new_node("WHILE", yytext, yylineno); return WHILE; }
"for"                                       { yylval = new_node("FOR", yytext, yylineno); return FOR; }
"return"                                 { yylval = new_node("RETURN", yytext, yylineno); return RETURN; }
"include"                               { yylval = new_node("INCLUDE", yytext, yylineno); return INCLUDE; }

"#"                                       { yylval = new_node("SHARP", yytext, yylineno); return SHARP; }
"."                                         { yylval = new_node("DOT", yytext, yylineno); return DOT; }
";"                                        { yylval = new_node("SEMI", yytext, yylineno); return SEMI; }
":"                                       { yylval = new_node("COLON", yytext, yylineno); return COLON; }
","                                       { yylval = new_node("COMMA", yytext, yylineno); return COMMA; }
"="                                      { yylval = new_node("ASSIGN", yytext, yylineno); return ASSIGN; }
"<"                                          { yylval = new_node("LT", yytext, yylineno); return LT; }
"<="                                         { yylval = new_node("LE", yytext, yylineno); return LE; }
">"                                          { yylval = new_node("GT", yytext, yylineno); return GT; }
">="                                         { yylval = new_node("GE", yytext, yylineno); return GE; }
"!="                                         { yylval = new_node("NE", yytext, yylineno); return NE; }
"=="                                         { yylval = new_node("EQ", yytext, yylineno); return EQ; }
"+"                                        { yylval = new_node("PLUS", yytext, yylineno); return PLUS; }
"-"                                       { yylval = new_node("MINUS", yytext, yylineno); return MINUS; }
"*"                                         { yylval = new_node("MUL", yytext, yylineno); return MUL; }
"/"                                         { yylval = new_node("DIV", yytext, yylineno); return DIV; }

"&&"                                        { yylval = new_node("AND", yytext, yylineno); return AND; }
"||"                                         { yylval = new_node("OR", yytext, yylineno); return OR; }
"!"                                         { yylval = new_node("NOT", yytext, yylineno); return NOT; }

"("                                          { yylval = new_node("LP", yytext, yylineno); return LP; }
")"                                          { yylval = new_node("RP", yytext, yylineno); return RP; }
"["                                          { yylval = new_node("LB", yytext, yylineno); return LB; }
"]"                                          { yylval = new_node("RB", yytext, yylineno); return RB; }
"{"                                          { yylval = new_node("LC", yytext, yylineno); return LC; }
"}"                                          { yylval = new_node("RC", yytext, yylineno); return RC; }

"<.>"                                     { yylval = new_node("ABSTR", yytext, yylineno); return ABSTR; }
{ID}                                         { yylval = new_node("ID", yytext, yylineno); return ID; }
{INT}                                       { yylval = new_node("INT", yytext, yylineno); return INT; }
{FLOAT}                                   { yylval = new_node("FLOAT", yytext, yylineno); return FLOAT; }
\'(\\x{hex_digit}{2}|.)\'                  { yylval = new_node("CHAR", yytext, yylineno); return CHAR; }
{STRING}                                 { yylval = new_node("STRING", yytext, yylineno); return STRING; }
{INVALID_CHAR}                     { yylval = new_node("INVALID_CHAR", yytext, yylineno); has_error = 1; print_A_error(yylineno, yytext); return INVALID_CHAR; }
({INT}{ID})|{INVALID_CHAR}           { yylval = new_node("INVALID_ID", yytext, yylineno); has_error = 1; print_A_error(yylineno, yytext); return INVALID_ID; }
{INVALID_NUMBER}                    {yylval = new_node("INVALID_NUMBER",yytext,yylineno); has_error = 1; print_A_error(yylineno, yytext); return INVALID_NUMBER;}
"//".*$                     { yylval = new_node("SINGLE_LINE_COMMENT", yytext, yylineno); return SINGLE_LINE_COMMENT; }
"/*"((("*"[^/])?)|[^*])*"*/" { yylval = new_node("MULTI_LINE_COMMENT", yytext, yylineno); return MULTI_LINE_COMMENT; }
.|{new_line} {;}
