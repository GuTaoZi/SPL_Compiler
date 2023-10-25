%{
    #define YYSTYPE treeNode*
    #include "lex.yy.c"
    #include "treeNode.h"
    #include "string.h"

    treeNode* root;

    void yyerror(const char *s);
    
    void print_B_error(const char *cause, size_t lineno)
    { fprintf(yyout, "Error type B at Line %d: %s \';\'\n", lineno); }
%}

%token TYPE 
%token STRUCT IF ELSE WHILE FOR RETURN INCLUDE
%token SHARP DOT SEMI COLON COMMA ASSIGN LT LE GT GE NE EQ PLUS MINUS MUL DIV
%token AND OR NOT
%token LP RP LB RB LC RC
%token ABSTR ID INT FLOAT CHAR INVALID_CHAR INVALID_ID
%token SINGLE_LINE_COMMENT MULTI_LINE_COMMENT

%left ASSIGN
%left OR
%left AND
%left LT LE GT GE EQ NE
%left PLUS MINUS
%left MUL DIV
%left NOT
%left DOT RP RB RC LP LB LC

%nonassoc LOWER_ELSE
%nonassoc ELSE
%nonassoc LOWER_FOR
%nonassoc UPPER_FOR

%%

/* high-level definition */
Program : IncDefList ExtDefList { addn($$, "Program", 2, $1, $2); root = $$;}
    ;

IncDef : SHARP INCLUDE ABSTR { addn($$, "IncDef", 3, $1, $2, $3); }

IncDefList :            { add0($$, "IncDefList"); }
    | IncDef IncDefList { addn($$, "IncDefList", 2, $1, $2); }

ExtDefList :            { add0($$, "ExtDefList"); }
    | ExtDef ExtDefList { addn($$, "ExtDefList", 2, $1, $2); }
    ;

ExtDef : Specifier ExtDecList SEMI  { addn($$, "ExtDef", 3, $1, $2, $3); }
    | Specifier SEMI                { addn($$, "ExtDef", 2, $1, $2); }
    | Specifier FunDec CompSt       { addn($$, "ExtDef", 3, $1, $2, $3); }
    | Specifier ExtDecList          { has_error = 1; print_B_error("Missing semicolon", $1->lineno); }
    | Specifier error               { has_error = 1; print_B_error("Missing semicolon", $1->lineno); }
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

ForeachType: TYPE VarDec   { addn($$, "ForeachType", 2, $1, $2); }
    | STRUCT ID VarDec     { addn($$, "ForeachType", 3, $1, $2, $3); }
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

Stmt : Exp SEMI                                 { addn($$, "Stmt", 2, $1, $2); }
    | CompSt                                    { add1($$, "Stmt", 1, $1); }
    | RETURN Exp SEMI                           { addn($$, "Stmt", 3, $1, $2, $3); }
    | IF LP Exp RP Stmt  %prec LOWER_ELSE       { addn($$, "Stmt", 5, $1, $2, $3, $4, $5); }
    | IF LP Exp RP Stmt ELSE Stmt               { addn($$, "Stmt", 7, $1, $2, $3, $4, $5, $6, $7); }
    | WHILE LP Exp RP Stmt                      { addn($$, "Stmt", 5, $1, $2, $3, $4, $5); }
    | FOR LP DefList SEMI Exp SEMI Exp RP Stmt  %prec UPPER_FOR { addn($$, "Stmt", 9, $1, $2, $3, $4, $5, $6, $7, $8, $9); }
    | FOR LP ForeachType COLON VarDec RP Stmt   %prec LOWER_FOR { addn($$, "Stmt", 7, $1, $2, $3, $4, $5, $6, $7); }
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
    FILE* file_in;
    FILE* file_out;
    if(argc == 2 || argc == 3) {
        file_in = fopen(argv[1], "r");
        if (file_in == NULL) {
            perror("Error opening input file");
            return 1;
        }
        if(argc == 2) {
            int la = strlen(argv[1]);
            argv[1][la-3] = 'o';
            argv[1][la-2] = 'u';
            argv[1][la-1] = 't';
            file_out = fopen(argv[1], "w");
        } else if(argc == 3) {
            file_out = fopen(argv[2], "w");
        }
        if (file_out == NULL) {
            perror("Error opening output file");
            return 1;
        }
        // Redirect Flex to read from file
    } else if(argc == 1) {
        file_in = stdin;
        file_out = stdout;
    } else {
        fprintf(stderr, "Parameters Error!\nShould be %s [input_file] [output_file]\n", argv[0]);
        return -1;
    }
    yyin = file_in;
    yyout = file_out;
    yyparse();
    if (!has_error)
        output_tree(root, 0);
    fclose(file_in);
    fclose(file_out);
    return 0;
}