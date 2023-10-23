%{
    #include "lex.yy.c"
    #include "stdlib.h"
    #include <stdarg.h>

    size_t lineno;

    #define YYSTYPE treeNode*
    #define add0(u) $$ = new_node(u, lineno);
    #define add1(u) $$ = new_node(u, lineno); $$->child = $1;

    #define addx(u, v, ...) \
        do { \
            $$ = new_node(u, lineno); \
            $$->child = $1; \
            make_list(v, __VA_ARGS__); \
        } while(0)

    #define add2(u) addx(u, 2, $1, $2);
    #define add3(u) addx(u, 3, $1, $2, $3);
    #define add4(u) addx(u, 4, $1, $2, $3, $4);
    #define add5(u) addx(u, 5, $1, $2, $3, $4, $5);
    #define add6(u) addx(u, 6, $1, $2, $3, $4, $5, $6);
    #define add7(u) addx(u, 7, $1, $2, $3, $4, $5, $6, $7);

    typedef struct _treeNode {
        const char *s;
        size_t lineno;
        struct _treeNode *child;
        struct _treeNode *next;
    } treeNode;

    treeNode new_node(const char *s, const size_t lino) {
        treeNode p = (treeNode*) malloc(sizeof(treeNode));
        p->s = s;
        p->lineno = lino;
        p->child = p->next = NULL;
    }
    
    void output_line(treeNode *u, size_t spaceno) {
        for(size_t i = 0; i < spaceno; i++)
            putchar(' ');
        printf("%s (%zu)\n", u->s, u->lineno);
    }

    void output_tree(treeNode *u, size_t spaceno) {
        output_line(u, spaceno);
        treeNode *p = u->child;
        while(p != NULL) {
            output_tree(p, spaceno+2);
            p=p->next;
        }
    }

    void make_list(int cnt, treeNode *head, ...) {
        va_list args;
        va_start(args, head);

        treeNode *value = head;

        while (cnt --> 1) {
            nxt = va_arg(args, treeNode*);
            value->next = nxt;
            value = nxt;
        }

        va_end(args);
    }

    void yerror(const char *s);
%}

%%

/* high-level definition */
Program : ExtDefList { add1("Program") }
    ;

ExtDefList : { add0("ExtDefList") }
    | ExtDef ExtDefList { add2("ExtDefList") }
    ;

ExtDef : Specifier ExtDecList SEMI { add3("ExtDef") }
    | Specifier SEMI { add2("ExtDef") }
    | Specifier FunDec CompSt { add3("ExtDef") }
    ;

ExtDecList : VarDec { add1("ExtDecList") }
    | VarDec COMMA ExtDecList { add3("ExtDecList") }
    ;

/* specifier */
Specifier : TYPE { add1("Specifier") }
    | StructSpecifier { add1("Specifier") }
    ;

StructSpecifier : STRUCT ID LC DefList RC { add5("StructSpecifier") }
    | STRUCT ID { add2("StructSpecifier") }
    ;

/* declarator */
VarDec : ID { add1("VarDec") }
    | VarDec LB INT RB { add4("VarDec") }
    ;

FunDec : ID LP VarList RP { add4("FunDec") }
    | ID LP RP { add3("FunDec") }
    ;

VarList : ParamDec COMMA VarList { add3("VarList") }
    | ParamDec { add1("VarList") }
    ;

ParamDec : Specifier VarDec { add2("ParamDec") }
    ;

/* statement */
CompSt : LC DefList StmtList RC { add4("CompSt") }
    ;

StmtList : { add0("StmtList") }
    | Stmt StmtList { add2("StmtList") }
    ;

Stmt : Exp SEMI { add2("Stmt") }
    | CompSt { add1("Stmt") }
    | RETURN Exp SEMI { add3("Stmt") }
    | IF LP Exp RP Stmt { add5("Stmt") }
    | IF LP Exp RP Stmt ELSE Stmt { add7("Stmt") }
    | WHILE LP Exp RP Stmt { add5("Stmt") }
    ;

/* local definition */
DefList :  { add0("DefList") }
    | Def DefList { add2("DefList") }
    ;

Def : Specifier DecList SEMI { add3("Def") }
    ;

DecList : Dec { add1("DecList") }
    | Dec COMMA DecList { add3("DecList") }
    ;

Dec : VarDec { add1("Dec") }
    | VarDec ASSIGN Exp { add3("Dec") }
    ;

/* Expression */
Exp : Exp ASSIGN Exp { add3("Exp") }
    | Exp AND Exp { add3("Exp") }
    | Exp OR Exp { add3("Exp") }
    | Exp LT Exp { add3("Exp") }
    | Exp LE Exp { add3("Exp") }
    | Exp GT Exp { add3("Exp") }
    | Exp GE Exp { add3("Exp") }
    | Exp NE Exp { add3("Exp") }
    | Exp EQ Exp { add3("Exp") }
    | Exp PLUS Exp { add3("Exp") }
    | Exp MINUS Exp { add3("Exp") }
    | Exp MUL Exp { add3("Exp") }
    | Exp DIV Exp { add3("Exp") }
    | LP Exp RP { add3("Exp") }
    | MINUS Exp { add2("Exp") }
    | NOT Exp { add2("Exp") }
    | ID LP Args RP { add4("Exp") }
    | ID LP RP { add3("Exp") }
    | Exp LB Exp RB { add4("Exp") }
    | Exp DOT ID { add3("Exp") }
    | ID { add1("Exp") }
    | INT { add1("Exp") }
    | FLOAT { add1("Exp") }
    | CHAR { add1("Exp") }
    ;

Args : Exp COMMA Args { add3("Args") }
    | Exp { add1("Args") }
    ;

%%

void yerror(const char *s)
{
    fprintf(stderr, "%s\n", s);
}

int main(int argc, char **argv)
{
    yyparse();
    return 0;
}