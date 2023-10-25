/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_SYNTAX_TAB_H_INCLUDED
# define YY_YY_SYNTAX_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    TYPE = 258,                    /* TYPE  */
    STRUCT = 259,                  /* STRUCT  */
    IF = 260,                      /* IF  */
    ELSE = 261,                    /* ELSE  */
    WHILE = 262,                   /* WHILE  */
    RETURN = 263,                  /* RETURN  */
    DOT = 264,                     /* DOT  */
    SEMI = 265,                    /* SEMI  */
    COMMA = 266,                   /* COMMA  */
    ASSIGN = 267,                  /* ASSIGN  */
    LT = 268,                      /* LT  */
    LE = 269,                      /* LE  */
    GT = 270,                      /* GT  */
    GE = 271,                      /* GE  */
    NE = 272,                      /* NE  */
    EQ = 273,                      /* EQ  */
    PLUS = 274,                    /* PLUS  */
    MINUS = 275,                   /* MINUS  */
    MUL = 276,                     /* MUL  */
    DIV = 277,                     /* DIV  */
    AND = 278,                     /* AND  */
    OR = 279,                      /* OR  */
    NOT = 280,                     /* NOT  */
    LP = 281,                      /* LP  */
    RP = 282,                      /* RP  */
    LB = 283,                      /* LB  */
    RB = 284,                      /* RB  */
    LC = 285,                      /* LC  */
    RC = 286,                      /* RC  */
    ID = 287,                      /* ID  */
    INT = 288,                     /* INT  */
    FLOAT = 289,                   /* FLOAT  */
    CHAR = 290,                    /* CHAR  */
    INVALID_CHAR = 291,            /* INVALID_CHAR  */
    INVALID_ID = 292,              /* INVALID_ID  */
    SINGLE_LINE_COMMENT = 293,     /* SINGLE_LINE_COMMENT  */
    MULTI_LINE_COMMENT = 294,      /* MULTI_LINE_COMMENT  */
    LOWER_ELSE = 295               /* LOWER_ELSE  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_SYNTAX_TAB_H_INCLUDED  */
