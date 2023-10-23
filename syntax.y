%{
    #define YYSTYPE treeNode*
    #include "lex.yy.c"
    #include "treeNode.h"
    #include "string.h"
    #include <stdarg.h>

    char* need_output[] = {"INT", "CHAR", "FLOAT", "TYPE", "ID"};
    int lno = 5;

    #define add0(self, name) \
        do { \
            self = new_node(name, NULL, yylineno); \
        } while(0)

    #define addn(self, name, n, v1, ...) \
        do { \
            self = new_node(name, NULL, v1->lineno); \
            self->child = v1; \
            make_list(n, v1, __VA_ARGS__); \
        } while(0)
    
    #define add1(self, name, n, v1) \
        do { \
            self = new_node(name, NULL, v1->lineno); \
            self->child = v1; \
        } while(0)

    treeNode* new_node(char *name, const char *val, const size_t lino) {
        treeNode* p = (treeNode*) malloc(sizeof(treeNode));
        p->name = name;
        if(val == NULL) p->val = NULL;
        else {
            p->val = (char*) malloc((strlen(val)+1)*sizeof(char));
            strcpy(p->val, val);
        }
        p->lineno = lino;
        p->child = p->next = NULL;
        return p;
    }
    
    void output_line(treeNode *u, size_t spaceno) {
        if(u->child != NULL){
            for(size_t i = 0; i < spaceno; i++)
                putchar(' ');
            printf("%s (%zu)\n", u->name, u->lineno);
        }
        else if(u->val != NULL) {
            for(size_t i = 0; i < spaceno; i++)
                putchar(' ');
            int flg = 1;
            for (int i = 0; i < lno; i++) {
                if(strcmp(u->name, need_output[i]) == 0){
                    flg = 0;
                    break;
                }
            }
            if(flg) printf("%s\n", u->name);
            else printf("%s: %s\n", u->name, u->val);
        }
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

        treeNode *value = head, *nxt;

        while (cnt --> 1) {
            nxt = va_arg(args, treeNode*);
            value->next = nxt;
            value = nxt;
        }

        va_end(args);
    }

    treeNode* root;

    void yyerror(const char *s);
%}

%token TYPE 
%token STRUCT IF ELSE WHILE RETURN
%token DOT SEMI COMMA ASSIGN LT LE GT GE NE EQ PLUS MINUS MUL DIV
%token AND OR NOT
%token LP RP LB RB LC RC
%token ID INT FLOAT CHAR INVALID_CHAR INVALID_ID
%token SINGLE_LINE_COMMENT MULTI_LINE_COMMENT

%left ASSIGN
%left OR
%left AND
%left LT LE GT GE EQ NE
%left PLUS MINUS
%left MUL DIV
%left NOT
%left DOT RP RB RC

%nonassoc LOWER_ELSE
%nonassoc ELSE

%%

/* high-level definition */
Program : ExtDefList { add1($$, "Program", 1, $1); root = $$;}
    ;

ExtDefList :            { add0($$, "ExtDefList"); }
    | ExtDef ExtDefList { addn($$, "ExtDefList", 2, $1, $2); }
    ;

ExtDef : Specifier ExtDecList SEMI  { addn($$, "ExtDef", 3, $1, $2, $3); }
    | Specifier SEMI                { addn($$, "ExtDef", 2, $1, $2); }
    | Specifier FunDec CompSt       { addn($$, "ExtDef", 3, $1, $2, $3); }
    ;

ExtDecList : VarDec             { add1($$, "ExtDecList", 1, $1); }
    | VarDec COMMA ExtDecList   { addn($$, "ExtDecList", 3, $1, $2, $3); }
    ;

/* specifier */
Specifier : TYPE        { add1($$, "Specifier", 1, $1); }
    | StructSpecifier   { add1($$, "Specifier", 1, $1); }
    ;

StructSpecifier : STRUCT ID LC DefList RC   { addn($$, "StructSpecifier", 5, $1, $2, $3, $4, $5); }
    | STRUCT ID                             { addn($$, "StructSpecifier", 2, $1, $2); }
    ;

/* declarator */
VarDec : ID             { add1($$, "VarDec", 1, $1); }
    | VarDec LB INT RB  { addn($$, "VarDec", 4, $1, $2, $3, $4); }
    ;

FunDec : ID LP VarList RP   { addn($$, "FunDec", 4, $1, $2, $3, $4); }
    | ID LP RP              { addn($$, "FunDec", 3, $1, $2, $3); }
    ;

VarList : ParamDec COMMA VarList    { addn($$, "VarList", 3, $1, $2, $3); }
    | ParamDec                      { add1($$, "VarList", 1, $1); }
    ;

ParamDec : Specifier VarDec { addn($$, "ParamDec", 2, $1, $2); }
    ;

/* statement */
CompSt : LC DefList StmtList RC { addn($$, "CompSt", 4, $1, $2, $3, $4); }
    ;

StmtList : { add0($$, "StmtList"); }
    | Stmt StmtList { addn($$, "StmtList", 2, $1, $2); }
    ;

Stmt : Exp SEMI                     { addn($$, "Stmt", 2, $1, $2); }
    | CompSt                        { add1($$, "Stmt", 1, $1); }
    | RETURN Exp SEMI               { addn($$, "Stmt", 3, $1, $2, $3); }
    | IF LP Exp RP Stmt  %prec LOWER_ELSE  { addn($$, "Stmt", 5, $1, $2, $3, $4, $5); }
    | IF LP Exp RP Stmt ELSE Stmt   { addn($$, "Stmt", 7, $1, $2, $3, $4, $5, $6, $7); }
    | WHILE LP Exp RP Stmt          { addn($$, "Stmt", 5, $1, $2, $3, $4, $5); }
    ;

/* local definition */
DefList :           { add0($$, "DefList"); }
    | Def DefList   { addn($$, "DefList", 2, $1, $2); }
    ;

Def : Specifier DecList SEMI { addn($$, "Def", 3, $1, $2, $3); }
    ;

DecList : Dec           { add1($$, "DecList", 1, $1); }
    | Dec COMMA DecList { addn($$, "DecList", 3, $1, $2, $3); }
    ;

Dec : VarDec            { add1($$, "Dec", 1, $1); }
    | VarDec ASSIGN Exp { addn($$, "Dec", 3, $1, $2, $3); }
    ;

/* Expression */
Exp : Exp ASSIGN Exp    { addn($$, "Exp", 3, $1, $2, $3); }
    | Exp AND Exp       { addn($$, "Exp", 3, $1, $2, $3); }
    | Exp OR Exp        { addn($$, "Exp", 3, $1, $2, $3); }
    | Exp LT Exp        { addn($$, "Exp", 3, $1, $2, $3); }
    | Exp LE Exp        { addn($$, "Exp", 3, $1, $2, $3); }
    | Exp GT Exp        { addn($$, "Exp", 3, $1, $2, $3); }
    | Exp GE Exp        { addn($$, "Exp", 3, $1, $2, $3); }
    | Exp NE Exp        { addn($$, "Exp", 3, $1, $2, $3); }
    | Exp EQ Exp        { addn($$, "Exp", 3, $1, $2, $3); }
    | Exp PLUS Exp      { addn($$, "Exp", 3, $1, $2, $3); }
    | Exp MINUS Exp     { addn($$, "Exp", 3, $1, $2, $3); }
    | Exp MUL Exp       { addn($$, "Exp", 3, $1, $2, $3); }
    | Exp DIV Exp       { addn($$, "Exp", 3, $1, $2, $3); }
    | LP Exp RP         { addn($$, "Exp", 3, $1, $2, $3); }
    | MINUS Exp         { addn($$, "Exp", 2, $1, $2); }
    | NOT Exp           { addn($$, "Exp", 2, $1, $2); }
    | ID LP Args RP     { addn($$, "Exp", 4, $1, $2, $3, $4); }
    | ID LP RP          { addn($$, "Exp", 3, $1, $2, $3); }
    | Exp LB Exp RB     { addn($$, "Exp", 4, $1, $2, $3, $4); }
    | Exp DOT ID        { addn($$, "Exp", 3, $1, $2, $3); }
    | ID                { add1($$, "Exp", 1, $1); }
    | INT               { add1($$, "Exp", 1, $1); }
    | FLOAT             { add1($$, "Exp", 1, $1); }
    | CHAR              { add1($$, "Exp", 1, $1); }
    ;

Args : Exp COMMA Args   { addn($$, "Args", 3, $1, $2, $3); }
    | Exp               { add1($$, "Args", 1, $1); }
    ;

%%

void yyerror(const char *s)
{
    fprintf(stderr, "YYERROR: %s\n", s);
}

int main(int argc, char **argv)
{
    yyparse();
    output_tree(root, 0);
    return 0;
}