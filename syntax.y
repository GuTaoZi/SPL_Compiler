%{
    #include "lex.yy.c"
    #include "stdlib.h"

    #define YYSTYPE treeNode*

    size_t lineno = 0;

    void output_line(const char *s, size_t spaceno) {
        for(size_t i = 0; i < spaceno; i++)
            putchar(' ');
        printf("%s (%zu)\n", s, lineno);
    }

    typedef struct _treeNode {
        const char *s;
        struct _treeNode *child;
        struct _treeNode *next;
    } treeNode;

    treeNode new_node(const char *s) {
        treeNode p = (treeNode*) malloc(sizeof(treeNode));
        p->s = s;
        p->child = p->next = NULL;
    }

    void make_list(treeNode *head, ...) {
        va_list args;
        va_start(args, head);

        treeNode *value = head;

        while (value != NULL) {
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
Program : ExtDefList { $$ = new_node("Program"); $$->child = $1; }
    ;

ExtDefList : {}
    | ExtDef ExtDefList {}
    ;

ExtDef : Specifier ExtDecList SEMI {}
    | Specifier SEMI {}
    | Specifier FunDec CompSt { $$ = new_node("ExtDef"); $$->child = $1; make_list($1, $2, $3);}
    ;

ExtDecList : VarDec {}
    | VarDec COMMA ExtDecList {}
    ;

/* specifier */
Specifier : TYPE {}
    | StructSpecifier {}
    ;

StructSpecifier : STRUCT ID LC DefList RC {}
    | STRUCT ID {}
    ;

/* declarator */
VarDec : ID {}
    | VarDec LB INT RB {}
    ;

FunDec : ID LP VarList RP {}
    | ID LP RP {}
    ;

VarList : ParamDec COMMA VarList {}
    | ParamDec {}
    ;

ParamDec : Specifier VarDec {}
    ;

/* statement */
CompSt : LC DefList StmtList RC {}
    ;

StmtList : {}
    | Stmt StmtList {}
    ;

Stmt : Exp SEMI {}
    | CompSt {}
    | RETURN Exp SEMI {}
    | IF LP Exp RP Stmt {}
    | IF LP Exp RP Stmt ELSE Stmt {}
    | WHILE LP Exp RP Stmt {}
    ;

/* local definition */
DefList :  {}
    |Def DefList {}
    ;

Def : Specifier DecList SEMI {}
    ;

DecList : Dec {}
    | Dec COMMA DecList {}
    ;

Dec : VarDec {}
    | VarDec ASSIGN Exp {}
    ;

/* Expression */
Exp : Exp ASSIGN Exp {}
    | Exp AND Exp {}
    | Exp OR Exp {}
    | Exp LT Exp {}
    | Exp LE Exp {}
    | Exp GT Exp {}
    | Exp GE Exp {}
    | Exp NE Exp {}
    | Exp EQ Exp {}
    | Exp PLUS Exp {}
    | Exp MINUS Exp {}
    | Exp MUL Exp {}
    | Exp DIV Exp {}
    | LP Exp RP {}
    | MINUS Exp {}
    | NOT Exp {}
    | ID LP Args RP {}
    | ID LP RP {}
    | Exp LB Exp RB {}
    | Exp DOT ID {}
    | ID {}
    | INT {}
    | FLOAT {}
    | CHAR {}
    ;

Args : Exp COMMA Args {}
    | Exp {}
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