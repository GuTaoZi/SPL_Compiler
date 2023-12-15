%{
    #define YYSTYPE treeNode*
    #include <stddef.h>
    #include <string.h>
    #include "utstack.h"
    #include "ortho.h"
    #include "type.h"
    #include "type_op.h"
    #include "treeNode.h"
    #include "IRgen.h"
    #include "optimizer.h"
    #include "lex.yy.c"

    extern size_t last_error_lineno;

    treeNode* root = NULL;
    int loop_cnt = 0;
    Type *nowType = NULL;
    treeNode* fakeFuncNode = NULL;

    void yyerror(const char *s);
    
    void print_B_error(const char* node_name, const size_t lineno, const char *msg)
    {
        fprintf(yyout, "Error type B at Line %zu: %s\n", lineno, msg);
        last_error_lineno = lineno;
        has_error = 1;
    }

%}

%token INVALID
%token TYPE 
%token STRUCT IF ELSE WHILE FOR RETURN INCLUDE BREAK CONTINUE
%token DOT SEMI COLON COMMA ASSIGN LT LE GT GE NE EQ PLUS MINUS MUL DIV
%token AND OR NOT
%token LP RP LB RB LC RC
%token ID UINT FLOAT CHAR STRING
%token SINGLE_LINE_COMMENT MULTI_LINE_COMMENT

%left INVALID
%right ASSIGN
%left OR
%left AND
%left LT LE GT GE EQ NE
%left PLUS MINUS
%left MUL DIV
%right NOT
%left DOT RP RB RC LP LB LC


%nonassoc LOWER_ELSE
%nonassoc ELSE
%nonassoc LOWER_FOR
%nonassoc UPPER_FOR

%%

/* high-level definition */
Program : { push_stack();funcRetTypeStack = utstack_push(funcRetTypeStack, makePrimType("int")); structFieldStack = utstack_push(structFieldStack, makeStructType("", makeFieldList(NULL, ""))); }
    HeaderDefList ExtDefList { addn($$, "Program", 2, $2, $3); root = $$; }
    ;

HeaderDefList :             { add0($$, "HeaderDefList"); }
    | Headers HeaderDefList { addn($$, "HeaderDefList", 2, $1, $2); }
    ;

Headers : INCLUDE { add1($$, "Headers", 1, $1); }
    ;

ExtDefList :            { add0($$, "ExtDefList"); }
    | ExtDef ExtDefList { addn($$, "ExtDefList", 2, $1, $2); }
    ;

ExtDef : Specifier ExtDecList SEMI  { addn($$, "ExtDef", 3, $1, $2, $3); }
    | Specifier SEMI                { addn($$, "ExtDef", 2, $1, $2); }
    | Specifier FunDec              { funcRetTypeStack = utstack_push(funcRetTypeStack,$1->inheridata); addFuncRet($2->inheridata, $1->inheridata); }
      CompSt                        { addn($$, "ExtDef", 3, $1, $2, $4); pop_stack(); funcRetTypeStack = utstack_pop(funcRetTypeStack); }
    | Specifier ExtDecList error    { add0($$, "ExtDef"); has_error = 1; print_B_error("ExtDef", $1->lineno, "Missing semicolon \';\'"); }
    | Specifier error               { add0($$, "ExtDef"); has_error = 1; print_B_error("ExtDef", $1->lineno, "Missing semicolon \';\'"); }
    | ExtDecList SEMI               { add0($$, "ExtDef"); has_error = 1; print_B_error("ExtDef", $1->lineno, "Missing specifier"); }
    ;

ExtDecList : VarDec             { add_identifier($1); add1($$, "ExtDecList", 1, $1); }
    | VarDec COMMA ExtDecList   { add_identifier($1); addn($$, "ExtDecList", 3, $1, $2, $3); }
    ;

/* specifier */
Specifier : TYPE        { add1($$, "Specifier", 1, $1); $$->inheridata = makePrimType($1->val); nowType = (Type*)$$->inheridata; }
    | StructSpecifier   { add1($$, "Specifier", 1, $1); $$->inheridata = $1->inheridata; nowType = (Type*)$$->inheridata; }
    ;

StructSpecifier :
      STRUCT ID LC  { push_stack(); structFieldStack = utstack_push(structFieldStack, makeStructType("", makeFieldList(NULL, "")));}
      DefList RC    { addn($$, "StructSpecifier", 5, $1, $2, $3, $5, $6);
                        pop_stack();
                        $$->inheridata = makeStructType($2->val, nowFL);
                        add_identifier($$);
                        structFieldStack = utstack_pop(structFieldStack); }
    
    | STRUCT ID     { addn($$, "StructSpecifier", 2, $1, $2);
                        $$->inheridata = findStruct($2->val, $1->lineno); }
    
    | STRUCT ID LC  { push_stack(); structFieldStack = utstack_push(structFieldStack, makeStructType("", makeFieldList(NULL, "")));}
      DefList error { add0($$, "StructSpecifier");
                        pop_stack();
                        $$->inheridata = makeStructType($2->val, nowFL);
                        add_identifier($$);
                        structFieldStack = utstack_pop(structFieldStack);
                        has_error = 1;
                        print_B_error("StructSpecifier", $3->lineno, "Missing closing curly braces \'}\'"); }
    
    | STRUCT ID     { push_stack(); structFieldStack = utstack_push(structFieldStack, makeStructType("", makeFieldList(NULL, "")));}
      DefList RC    { add0($$, "StructSpecifier");
                        pop_stack();
                        $$->inheridata = makeStructType($2->val, nowFL);
                        add_identifier($$);
                        structFieldStack = utstack_pop(structFieldStack);
                        has_error = 1;
                        print_B_error("StructSpecifier", $3->lineno, "Missing closing curly braces \'{\'"); }
    | STRUCT INVALID LC DefList RC      { add0($$, "StructSpecifier"); has_error = 1; }
    | STRUCT INVALID                    { add0($$, "StructSpecifier"); has_error = 1; }
    | STRUCT INVALID LC DefList error   { add0($$, "StructSpecifier"); has_error = 1; }
    | STRUCT INVALID DefList RC         { add0($$, "StructSpecifier"); has_error = 1; }
    ;

/* declarator */
VarDec : ID                 { add1($$, "VarDec", 1, $1); $$->inheridata = nowType; }
    | VarDec LB UINT RB     { addn($$, "VarDec", 4, $1, $2, $3, $4); $$->inheridata = addArrayType($1->inheridata, my_toint($3->val)); }
    | VarDec LB UINT error  { add0($$, "VarDec"); $$->inheridata = addArrayType($1->inheridata, my_toint($3->val)); has_error = 1; print_B_error("VarDec", $2->lineno, "Missing closing braces \']\'"); }
    // | VarDec UINT RB        { add0($$, "VarDec"); has_error = 1; print_B_error("VarDec", $2->lineno, "Missing closing braces \']\'"); }
    | INVALID               { add0($$, "VarDec"); has_error = 1; $$->inheridata = makeErrorType(); }
    ;

FunDec :
      ID LP         { structFieldStack = utstack_push(structFieldStack, makeStructType("", makeFieldList(NULL, "")));
                        add0(fakeFuncNode, "FunDec");
                        fakeFuncNode->child = $1;
                        fakeFuncNode->inheridata = makeFuncType($1->val, NULL); add_identifier(fakeFuncNode);
                        push_stack(); }
      VarList RP    { $$=fakeFuncNode; $$->child = $1; $$->child_cnt = 4; make_list(4, $1, $2, $4, $5); ((Type*)($$->inheridata))->func->params = nowFL; structFieldStack = utstack_pop(structFieldStack); }
    | ID LP RP      { addn($$, "FunDec", 3, $1, $2, $3); $$->inheridata = makeFuncType($1->val, NULL); add_identifier($$); push_stack(); }
    | ID LP         { structFieldStack = utstack_push(structFieldStack, makeStructType("", makeFieldList(NULL, "")));
                        add0(fakeFuncNode, "FunDec");
                        fakeFuncNode->child = $1;
                        fakeFuncNode->inheridata = makeFuncType($1->val, NULL); add_identifier(fakeFuncNode);
                        push_stack(); }
      VarList error { $$=fakeFuncNode; $$->child_cnt = 0; ((Type*)($$->inheridata))->func->params = nowFL; structFieldStack = utstack_pop(structFieldStack); has_error = 1; print_B_error("FunDec", $2->lineno, "Missing closing parenthesis \')\'"); }
    | ID RP         { add0($$, "FunDec"); $$->inheridata = makeFuncType($1->val, NULL); add_identifier($$->inheridata); push_stack(); has_error = 1; print_B_error("FunDec", $2->lineno, "Missing closing parenthesis \'(\'"); }
    | ID LP error   { add0($$, "FunDec"); $$->inheridata = makeFuncType($1->val, NULL); add_identifier($$->inheridata); push_stack(); has_error = 1; print_B_error("FunDec", $2->lineno, "Missing closing parenthesis \')\'"); }

    | INVALID LP        { structFieldStack = utstack_push(structFieldStack, makeStructType("", makeFieldList(NULL, "")));  push_stack();}
      VarList RP        { add0($$, "FunDec"); has_error = 1; structFieldStack = utstack_pop(structFieldStack); }
    | INVALID LP RP     { push_stack(); add0($$, "FunDec"); has_error = 1; }
    | INVALID LP        { structFieldStack = utstack_push(structFieldStack, makeStructType("", makeFieldList(NULL, ""))); push_stack(); }
      VarList error     { add0($$, "FunDec"); has_error = 1; structFieldStack = utstack_pop(structFieldStack); }
    | INVALID RP        { push_stack(); add0($$, "FunDec"); has_error = 1; }
    | INVALID LP error  { push_stack(); add0($$, "FunDec"); has_error = 1; }
    ;

VarList : ParamDec COMMA VarList    { addn($$, "VarList", 3, $1, $2, $3); }
    | ParamDec                      { add1($$, "VarList", 1, $1); }
    ;

ParamDec :
    Specifier VarDec { addn($$, "ParamDec", 2, $1, $2); nowFL = addFieldList(nowFL, $2->inheridata, getVarDecName($2)); add_identifier($2); }
    ;

/* statement */
CompSt : LC                     { push_stack(); }
      DefList StmtList RC       { addn($$, "CompSt", 4, $1, $3, $4, $5); pop_stack(); }
    | LC { push_stack(); }
      DefList StmtList error { add0($$, "CompSt"); has_error = 1; print_B_error("CompSt", $1->lineno, "Missing closing curly bracket \'}\'"); pop_stack(); }
    //| error DefList StmtList RC { add0($$, "CompSt"); has_error = 1; print_B_error("CompSt", $1->lineno, "Missing closing curly bracket \'{\'"); }
    ;


StmtList :                      { add0($$, "StmtList"); }
    | Stmt StmtList             { addn($$, "StmtList", 2, $1, $2); }
    | Stmt Def DefList StmtList { add0($$, "WrongStmt"); has_error = 1; print_B_error("StmtList", $1->lineno, "Missing specifier"); }
    ;

Stmt : SEMI                                     { add1($$, "Stmt", 1, $1); }
    | Exp SEMI                                  { addn($$, "Stmt", 2, $1, $2); }
    | CompSt                                    { add1($$, "Stmt", 1, $1); }
    | RETURN Exp SEMI                           { checkRetType($2->inheridata, $1->lineno); addn($$, "Stmt", 3, $1, $2, $3); }
    | IF LP Exp RP Stmt  %prec LOWER_ELSE       { addn($$, "Stmt", 5, $1, $2, $3, $4, $5); }
    | IF LP Exp RP Stmt ELSE Stmt               { addn($$, "Stmt", 7, $1, $2, $3, $4, $5, $6, $7); }
    | WHILE LP Exp RP {++loop_cnt;} Stmt        { --loop_cnt; addn($$, "Stmt", 5, $1, $2, $3, $4, $6); }
    | FOR LP DecList SEMI Exp SEMI Exp RP {++loop_cnt;} Stmt  %prec UPPER_FOR { --loop_cnt; addn($$, "Stmt", 9, $1, $2, $3, $4, $5, $6, $7, $8, $10); }
    // | FOR LP VarDec COLON Exp RP {++loop_cnt;} Stmt      %prec LOWER_FOR      { --loop_cnt; addn($$, "Stmt", 7, $1, $2, $3, $4, $5, $6, $7); }
    | BREAK SEMI                                { addn($$, "Stmt", 2, $1, $2); if(loop_cnt == 0) print_type_error(16, $1->lineno, "\'break\' outside of loop."); }
    | CONTINUE SEMI                             { addn($$, "Stmt", 2, $1, $2); if(loop_cnt == 0) print_type_error(16, $1->lineno, "\'continue\' outside of loop."); }
    | Exp error                                 { add0($$, "Stmt"); has_error = 1; print_B_error("Stmt", $1->lineno, "Missing semicolon \';\'"); }
    | RETURN Exp error                          { checkRetType($2->inheridata, $1->lineno); add0($$, "Stmt"); has_error = 1; print_B_error("Stmt", $2->lineno, "Missing semicolon \';\'"); }
    | IF LP RP  %prec LOWER_ELSE                { add0($$, "Stmt"); has_error = 1; print_B_error("Stmt", $1->lineno, "Missing condition"); }
    | IF LP RP ELSE                             { add0($$, "Stmt"); has_error = 1; print_B_error("Stmt", $1->lineno, "Missing condition"); }
    | IF LP Exp RP error  %prec LOWER_ELSE      { add0($$, "Stmt"); has_error = 1; print_B_error("Stmt", $1->lineno, "Expect Stmt after if"); }
    | IF LP Exp RP error ELSE Stmt              { add0($$, "Stmt"); has_error = 1; print_B_error("Stmt", $1->lineno, "Expect Stmt after if"); }
    | IF error Exp RP Stmt %prec LOWER_ELSE     { add0($$, "Stmt"); has_error = 1; print_B_error("Stmt", $1->lineno, "Expected \'(\' after \'if\'"); }
    | IF error Exp RP Stmt ELSE Stmt %prec ELSE { add0($$, "Stmt"); has_error = 1; print_B_error("Stmt", $1->lineno, "Expected \'(\' after \'if\'"); }
    | IF LP Exp error Stmt %prec LOWER_ELSE     { add0($$, "Stmt"); has_error = 1; print_B_error("Stmt", $1->lineno, "Missing closing parenthesis \')\'"); }
    | IF LP Exp error Stmt ELSE Stmt %prec ELSE { add0($$, "Stmt"); has_error = 1; print_B_error("Stmt", $1->lineno, "Missing closing parenthesis \')\'"); }
    | WHILE error Exp RP {++loop_cnt;} Stmt     { --loop_cnt; add0($$, "Stmt"); has_error = 1; print_B_error("Stmt", $1->lineno, "Expected \'(\' after \'while\'"); }
    | WHILE LP Exp error {++loop_cnt;} Stmt     { --loop_cnt; add0($$, "Stmt"); has_error = 1; print_B_error("Stmt", $1->lineno, "Missing closing parenthesis \')\'"); }
    | FOR DecList SEMI Exp SEMI Exp RP {++loop_cnt;} Stmt %prec UPPER_FOR         { --loop_cnt; add0($$, "Stmt"); has_error = 1; print_B_error("Stmt", $1->lineno, "Expected \'(\' after \'for\'"); }
    | FOR error %prec UPPER_FOR                                                     { add0($$, "Stmt"); has_error = 1; print_B_error("Stmt", $1->lineno, "Expected \'(\' after \'for\'"); }
    | FOR LP DecList SEMI Exp SEMI Exp error {++loop_cnt;} Stmt %prec UPPER_FOR   { --loop_cnt; add0($$, "Stmt"); has_error = 1; print_B_error("Stmt", $1->lineno, "Missing closing parenthesis \')\'"); }
    // | FOR VarDec COLON Exp RP {++loop_cnt;} Stmt  %prec LOWER_FOR                 { --loop_cnt; add0($$, "Stmt"); has_error = 1; print_B_error("Stmt", $1->lineno, "Expected \'(\' after \'for\'"); }
    // | FOR LP VarDec COLON Exp error {++loop_cnt;} Stmt  %prec LOWER_FOR           { --loop_cnt; add0($$, "Stmt"); has_error = 1; print_B_error("Stmt", $1->lineno, "Missing closing parenthesis \')\'"); }
    | ELSE Stmt                                 { add0($$, "Stmt"); has_error = 1; print_B_error("Stmt", $1->lineno, "Expected \'if\' before \'else\'"); }
    | BREAK error                               { addn($$, "Stmt", 2, $1, $2); if(loop_cnt == 0) print_type_error(16, $1->lineno, "\'break\' outside of loop."); has_error = 1; print_B_error("Stmt", $1->lineno, "Missing semicolon \';\'"); }
    | CONTINUE error                            { addn($$, "Stmt", 2, $1, $2); if(loop_cnt == 0) print_type_error(16, $1->lineno, "\'continue\' outside of loop."); has_error = 1; print_B_error("Stmt", $1->lineno, "Missing semicolon \';\'"); }
    ;

/* local definition */
DefList :           { add0($$, "DefList"); }
    | Def DefList   { addn($$, "DefList", 2, $1, $2); }
    ;

Def : Specifier DecList SEMI  { addn($$, "Def", 3, $1, $2, $3); }
    | Specifier DecList error { addn($$, "Def", 2, $1, $2); has_error = 1; print_B_error("Def", $2->lineno, "Missing semicolon \';\'"); }
    ;

DecList : Dec           { add1($$, "DecList", 1, $1); }
    | Dec COMMA DecList { addn($$, "DecList", 3, $1, $2, $3); }
    ;

Dec : VarDec            { add1($$, "Dec", 1, $1); add_identifier($1); nowFL = addFieldList(nowFL, $1->inheridata, getVarDecName($1)); }
    | VarDec ASSIGN Exp { addn($$, "Dec", 3, $1, $2, $3); inherit_type($$, $1, $3, "ass"); add_identifier($1); nowFL = addFieldList(nowFL, $1->inheridata, getVarDecName($1)); }
    ;

/* Expression */
Exp : Exp ASSIGN Exp    { addn($$, "Exp", 3, $1, $2, $3); inherit_type($$, $1, $3, "ass"); }
    | Exp AND Exp       { addn($$, "Exp", 3, $1, $2, $3); inherit_type($$, $1, $3, "bin"); }
    | Exp OR Exp        { addn($$, "Exp", 3, $1, $2, $3); inherit_type($$, $1, $3, "bin"); }
    | Exp LT Exp        { addn($$, "Exp", 3, $1, $2, $3); inherit_type($$, $1, $3, "alg"); }
    | Exp LE Exp        { addn($$, "Exp", 3, $1, $2, $3); inherit_type($$, $1, $3, "alg"); }
    | Exp GT Exp        { addn($$, "Exp", 3, $1, $2, $3); inherit_type($$, $1, $3, "alg"); }
    | Exp GE Exp        { addn($$, "Exp", 3, $1, $2, $3); inherit_type($$, $1, $3, "alg"); }
    | Exp NE Exp        { addn($$, "Exp", 3, $1, $2, $3); inherit_type($$, $1, $3, "alg"); }
    | Exp EQ Exp        { addn($$, "Exp", 3, $1, $2, $3); inherit_type($$, $1, $3, "alg"); }
    | Exp PLUS Exp      { addn($$, "Exp", 3, $1, $2, $3); inherit_type($$, $1, $3, "alg"); }
    | Exp MINUS Exp     { addn($$, "Exp", 3, $1, $2, $3); inherit_type($$, $1, $3, "alg"); }
    | Exp MUL Exp       { addn($$, "Exp", 3, $1, $2, $3); inherit_type($$, $1, $3, "alg"); }
    | Exp DIV Exp       { addn($$, "Exp", 3, $1, $2, $3); inherit_type($$, $1, $3, "alg"); }
    | LP Exp RP         { addn($$, "Exp", 3, $1, $2, $3); $$->inheridata = $2->inheridata; }
    | PLUS Exp          { addn($$, "Exp", 2, $1, $2); inherit_type($$, $2, $2, "alg"); }
    | MINUS Exp         { addn($$, "Exp", 2, $1, $2); inherit_type($$, $2, $2, "alg"); }
    | NOT Exp           { addn($$, "Exp", 2, $1, $2); inherit_type($$, $2, $2, "bin"); }
    | ID LP             { structFieldStack = utstack_push(structFieldStack, makeStructType("", makeFieldList(NULL, ""))); }
      Args RP           { addn($$, "Exp", 4, $1, $2, $4, $5); inherit_function($$, $1, nowFL); structFieldStack = utstack_pop(structFieldStack); }                // function call with args
    | ID LP RP          { addn($$, "Exp", 3, $1, $2, $3); inherit_function($$, $1, NULL); }  // function call with no args
    | Exp LB Exp RB     { addn($$, "Exp", 4, $1, $2, $3, $4); inherit_array($$, $1, $3); }   // array
    | Exp DOT ID        { addn($$, "Exp", 3, $1, $2, $3); inherit_struct($$, $1, $3); }      // attribute of structure
    | Var               { add1($$, "Exp", 1, $1); $$->inheridata = $1->inheridata; }
    | STRING            { add1($$, "Exp", 1, $1); $$->inheridata = makeErrorType(); }
    | Exp ASSIGN error  { add0($$, "Exp"); has_error = 1; print_B_error("Exp", $1->lineno, "Missing right operand"); $$->inheridata = makeErrorType(); }
    | Exp AND error     { add0($$, "Exp"); has_error = 1; print_B_error("Exp", $1->lineno, "Missing right operand"); $$->inheridata = makeErrorType(); }
    | Exp OR error      { add0($$, "Exp"); has_error = 1; print_B_error("Exp", $1->lineno, "Missing right operand"); $$->inheridata = makeErrorType(); }
    | Exp LT error      { add0($$, "Exp"); has_error = 1; print_B_error("Exp", $1->lineno, "Missing right operand"); $$->inheridata = makeErrorType(); }
    | Exp LE error      { add0($$, "Exp"); has_error = 1; print_B_error("Exp", $1->lineno, "Missing right operand"); $$->inheridata = makeErrorType(); }
    | Exp GT error      { add0($$, "Exp"); has_error = 1; print_B_error("Exp", $1->lineno, "Missing right operand"); $$->inheridata = makeErrorType(); }
    | Exp GE error      { add0($$, "Exp"); has_error = 1; print_B_error("Exp", $1->lineno, "Missing right operand"); $$->inheridata = makeErrorType(); }
    | Exp NE error      { add0($$, "Exp"); has_error = 1; print_B_error("Exp", $1->lineno, "Missing right operand"); $$->inheridata = makeErrorType(); }
    | Exp EQ error      { add0($$, "Exp"); has_error = 1; print_B_error("Exp", $1->lineno, "Missing right operand"); $$->inheridata = makeErrorType(); }
    | Exp PLUS error    { add0($$, "Exp"); has_error = 1; print_B_error("Exp", $1->lineno, "Missing right operand"); $$->inheridata = makeErrorType(); }
    | Exp MINUS error   { add0($$, "Exp"); has_error = 1; print_B_error("Exp", $1->lineno, "Missing right operand"); $$->inheridata = makeErrorType(); }
    | Exp MUL error     { add0($$, "Exp"); has_error = 1; print_B_error("Exp", $1->lineno, "Missing right operand"); $$->inheridata = makeErrorType(); }
    | Exp DIV error     { add0($$, "Exp"); has_error = 1; print_B_error("Exp", $1->lineno, "Missing right operand"); $$->inheridata = makeErrorType(); }
    | Exp INVALID Exp   { add0($$, "Exp"); has_error = 1; $$->inheridata = makeErrorType(); }
    | LP Exp error      { add0($$, "Exp"); has_error = 1; print_B_error("Exp", $1->lineno, "Missing closing parenthesis \')\'"); $$->inheridata = $2->inheridata; }
    | ID LP             { structFieldStack = utstack_push(structFieldStack, makeStructType("", makeFieldList(NULL, ""))); }
      Args error        { add0($$, "Exp"); has_error = 1; print_B_error("Exp", $2->lineno, "Missing closing parenthesis \')\'"); inherit_function($$, $1, nowFL); structFieldStack = utstack_pop(structFieldStack); }
    | ID LP error       { add0($$, "Exp"); has_error = 1; print_B_error("Exp", $2->lineno, "Missing closing parenthesis \')\'"); inherit_function($$, $1, makeFieldList(NULL, "")); }
    | Exp LB Exp error  { add0($$, "Exp"); has_error = 1; print_B_error("Exp", $2->lineno, "Missing closing braces \']\'"); inherit_array($$, $1, $3); }
    ;

Args : Exp COMMA Args   { addn($$, "Args", 3, $1, $2, $3); nowFL = addFieldList(nowFL, $1->inheridata, ""); }
    | Exp               { add1($$, "Args", 1, $1); nowFL = addFieldList(nowFL, $1->inheridata, ""); }
    ;

Var : UINT      { $$ = $1; $$->inheridata = makePrimType("int"); }
    | ID        {
                $$ = $1;
                $$->inheridata = find_type($1->val);
                if ($$->inheridata == NULL){
                    sprintf(type_error_tmp, "variable \'%s\' is used without a definition.", $1->val);
                    print_type_error(1, $$->lineno, type_error_tmp);
                    $$->inheridata=makeErrorType();
                    // add_something($$->inheridata, $$->val, 0, 0, "");
                }
      }
    | FLOAT     { $$ = $1; $$->inheridata = makePrimType("float"); }
    | CHAR      { $$ = $1; $$->inheridata = makePrimType("char"); }
    | INVALID   { $$ = $1; }
    ;

%%

void yyerror(const char *s)
{
    has_error = 1;
    // fprintf(stderr, "YYERROR: %s\n", s);
}

int main(int argc, char **argv)
{
    FILE *file_in;
    FILE *file_out;
    FILE *file_opt;
    if (argc == 2 || argc == 3)
    {
        file_in = fopen(argv[1], "r");
        if (file_in == NULL)
        {
            perror("Error opening input file");
            return 1;
        }

        int len = strlen(argv[1]);
        while (argv[1][len - 1] != '.')
            len--;
        char *ofname = (char *)malloc((len + 4) * sizeof(char));
        for (int i = 0; i < len; i++)
            ofname[i] = argv[1][i];
        ofname[len] = 'i';
        ofname[len + 1] = 'r';
        ofname[len + 2] = '0';
        ofname[len + 3] = 0;
        file_out = fopen(ofname, "w+");
        if (argc == 2)
        {
            ofname[len + 2] = 0;
            file_opt = fopen(ofname, "w");
        }
        else if (argc == 3)
        {
            file_opt = fopen(argv[2], "w");
        }
        free(ofname);

        if (file_out == NULL)
        {
            perror("Error opening output file");
            return 1;
        }
        // Redirect Flex to read from file
    }
    else if (argc == 1)
    {
        file_in = stdin;
        file_out = stdout;
    }
    else
    {
        fprintf(stderr, "Parameters Error!\nShould be %s [input_file] [output_file]\n", argv[0]);
        return -1;
    }
    yyin = file_in;
    yyout = file_out;
    yyparse();
    if (!has_error)
    {
        if (root != NULL){
            IR_tree *IRroot = build_IR_tree(root);
            output_IR_tree(IRroot, file_out);
            fseek(file_out, 0, SEEK_SET);
            optimizer(file_out, file_opt);
        }
        else
        {
            print_B_error("root", -1, "There is error somewhere...");
        }
    }
    else if (last_error_lineno == -1)
    {
        print_B_error("root", -1, "There is error somewhere...");
    }
    fclose(file_in);
    fclose(file_out);
    return 0;
}