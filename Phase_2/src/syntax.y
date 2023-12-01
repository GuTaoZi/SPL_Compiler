%{
    #define YYSTYPE treeNode*
    #define treeInheri void*
    #include <string.h>
    #include "lex.yy.c"
    #include "treeNode.h"
    #include "type.h"
    #include "ortho.h"
    #include "utstack.h"

    treeNode* root = NULL;
    extern size_t last_error_lineno;

    void yyerror(const char *s);
    
    void print_B_error(const char* node_name, const size_t lineno, const char *msg)
    {
        fprintf(yyout, "Error type B at Line %zu: %s\n", lineno, msg);
        last_error_lineno = lineno;
    }

    void print_type_error(const int typeID, const size_t lineno, const char* msg){
        fprintf(yyout, "Error type %d at Line %zu: %s\n", typeID, lineno, msg);
    }

    Type *find_type(char *name)
    {
        orthoNode *node = global_scope_seek(name);
        return node ? node->val : makeErrorType();
    }

    void inherit_type(treeNode *u, const treeNode *v, const treeNode *w, const char *op)
    {
        Type *tu, *tv = v->inheridata, *tw = w->inheridata;
        tu = getTypeAfterOp(tv, tw, op);
        if (tu->category == ERRORTYPE && tv->category != ERRORTYPE && tw->category != ERRORTYPE)
        {
            if (strcmp(op, "ass") == 0)
                print_type_error(5, u->lineno, "unmatching types");
            else
                print_type_error(7, u->lineno, "unmatching operands");
        }
        u->inheridata = tu;
    }

    void inherit_function(treeNode *u, const treeNode *v, const FieldList *fl)
    {
        Type *t = find_type(v->val);
        if (t->category == ERRORTYPE)
            u->inheridata = t;
        else if (!checkFieldEqual(t->func->params, fl))
        {
            u->inheridata = makeErrorType();
            print_type_error(9, u->lineno, "parameters missmatch");
            return;
        }
        else
            u->inheridata = t->func->return_type;
    }

    void inherit_array(treeNode *u, const treeNode *v, const treeNode *w)
    {
        Type *tv = v->inheridata, *tw = w->inheridata;
        if (tv->category != ARRAY)
        {
            u->inheridata = makeErrorType();
            print_type_error(10, u->lineno, "not an array");
        }
        else if (tw->category != PRIMITIVE || tw->primitive != PINT)
        {
            u->inheridata = makeErrorType();
            print_type_error(12, u->lineno, "index should be an integer");
        }
        else
            u->inheridata = tv->array->base;
    }

    void inherit_struct(treeNode *u, const treeNode *v, const treeNode *w)
    {
        Type *tv = v->inheridata;
        Type *tw = findNameInStructure(tv, w->val);
        if (tv->category != STRUCT)
        {
            u->inheridata = makeErrorType();
            print_type_error(13, u->lineno, "not a structure");
        }
        else if (tw == NULL)
        {
            u->inheridata = makeErrorType();
            print_type_error(14, u->lineno, "attribute not found");
        }
        else
            u->inheridata = tw;
    }

    void add_something(const Type *p, const char *name, const int errorID, const size_t lineno, const char *error_msg){
        if(current_scope_seek(name) == NULL){
            add_ortho_node(name, p);
        } else {
            print_type_error(errorID, lineno, error_msg);
        }
    }

    void add_others(const Type *p, const size_t lineno) {
        if(p->category == STRUCTURE){
            add_something(p, p->structure->struct_name, lineno, 15, "redefine the same structure type.");
        } else {
            add_something(p, p->func->name, lineno, 4, "a function is redefined.");
        }
    }

    void add_identifier(const treeNode *p) {
        if(((Type*)p->inheridata)->category == PRIMITIVE || ((Type*)p->inheridata)->category == ARRAY) {
            add_something(p->inheridata, getVarDecName(p), p->lineno, 3, "a variable is redefined in the same scope.");
        } else if(((Type*)p->inheridata)->category == STRUCTURE || ((Type*)p->inheridata)->category == FUNCTION) {
            add_others(p->inheridata, p->lineno);
        } else {
            print_type_error(-1, 0, "What the hell? At add_identifier.");
        }
    }

    typedef struct rettype_stack {
        const Type *data;
        struct rettype_stack *next;
    } rettype_stack;
    rettype_stack *funcRetTypeStack = NULL;

    void rettype_push(const Type *nowType) {
        rettype_stack *p = (rettype_stack*)malloc(sizeof(rettype_stack));
        p->data = nowType;
        STACK_PUSH(funcRetTypeStack, p);
    }
    Type *rettype_pop(){
        rettype_stack *p;
        STACK_POP(funcRetTypeStack, p);
    }
    void checkRetType(const Type *ret2, const size_t lineno){
        const Type *ret1 = funcRetTypeStack->data;
        const Type *tu = getTypeAfterOp(ret1, ret2, "ass");
        if (tu->category == ERRORTYPE && ret1->category != ERRORTYPE && ret2->category != ERRORTYPE)
            print_type_error(8, lineno, "a function's return value type mismatches the declared type.");;
    }
    void try_define(const treeNode *u)
    {
        const char *name = getVarDecName(u);
        if (current_scope_seek(name))
            print_type_error(3, u->lineno, "a variable is redefined in the same scop.");
        else
            add_ortho_node(name, u->inheridata);
    }
    Type *findStruct(const char *name, const size_t lineno) {
        orthoNode *p=global_scope_seek(name);
        if(p==NULL){
            print_type_error(17, lineno, "Cannot find structure definition.");
            return makeErrorType();
        } else {
            return p->val;
        }
    }

    Type *nowType = NULL;
    FieldList *nowFL = NULL;

    int loop_cnt = 0;
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
Program : HeaderDefList ExtDefList { addn($$, "Program", 2, $1, $2); root = $$; }
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
    | Specifier FunDec              { rettype_push($1->inheridata); }
      CompSt                        { rettype_pop(); addn($$, "ExtDef", 3, $1, $2, $3); addFuncRet($2->inheridata, $1->inheridata); add_identifier($2);}
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
      STRUCT ID LC  { push_stack(); }
      DefList RC    { addn($$, "StructSpecifier", 5, $1, $2, $3, $4, $5);
                        pop_stack();
                        $$->inheridata = makeStructType($2->val, $4->inheridata);
                        add_identifier($$); }
    
    | STRUCT ID     { addn($$, "StructSpecifier", 2, $1, $2);
                        $$->inheridata = findStruct($2->val, $1->lineno); }
    
    | STRUCT ID LC  { push_stack(); }
      DefList error { add0($$, "StructSpecifier");
                        pop_stack();
                        $$->inheridata = makeStructType($2->val, $4->inheridata);
                        add_identifier($$);
                        has_error = 1;
                        print_B_error("StructSpecifier", $3->lineno, "Missing closing curly braces \'}\'"); }
    
    | STRUCT ID     { push_stack(); }
      DefList RC    { add0($$, "StructSpecifier");
                        pop_stack();
                        $$->inheridata = makeStructType($2->val, $4->inheridata);
                        add_identifier($$);
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
    | VarDec LB UINT error  { add0($$, "VarDec"); $$->inheridata = makeArrayType($1->inheridata, my_toint($3->val)); has_error = 1; print_B_error("VarDec", $2->lineno, "Missing closing braces \']\'"); }
    // | VarDec UINT RB        { add0($$, "VarDec"); has_error = 1; print_B_error("VarDec", $2->lineno, "Missing closing braces \']\'"); }
    | INVALID               { add0($$, "VarDec"); has_error = 1;}
    ;

FunDec :
      ID LP         { nowFL = makeFieldList(NULL, ""); }
      VarList RP    { addn($$, "FunDec", 4, $1, $2, $3, $4); $$->inheridata = makeFuncType($1->val, nowFL); }
    | ID LP RP      { addn($$, "FunDec", 3, $1, $2, $3); $$->inheridata = makeFuncType($1->val, NULL); }
    | ID LP         { nowFL = makeFieldList(NULL, ""); }
      VarList error { add0($$, "FunDec"); $$->inheridata = makeFuncType($1->val, nowFL); has_error = 1; print_B_error("FunDec", $2->lineno, "Missing closing parenthesis \')\'"); }
    | ID RP         { add0($$, "FunDec"); $$->inheridata = makeFuncType($1->val, NULL); has_error = 1; print_B_error("FunDec", $2->lineno, "Missing closing parenthesis \'(\'"); }
    | ID LP error   { add0($$, "FunDec"); $$->inheridata = makeFuncType($1->val, NULL); has_error = 1; print_B_error("FunDec", $2->lineno, "Missing closing parenthesis \')\'"); }

    | INVALID LP        { nowFL = makeFieldList(NULL, ""); }
      VarList RP        { add0($$, "FunDec"); has_error = 1; }
    | INVALID LP RP     { add0($$, "FunDec"); has_error = 1; }
    | INVALID LP        { nowFL = makeFieldList(NULL, ""); }
      VarList error     { add0($$, "FunDec"); has_error = 1; }
    | INVALID RP        { add0($$, "FunDec"); has_error = 1; }
    | INVALID LP error  { add0($$, "FunDec"); has_error = 1; }
    ;

VarList : ParamDec COMMA VarList    { addn($$, "VarList", 3, $1, $2, $3); }
    | ParamDec                      { add1($$, "VarList", 1, $1); }
    ;

ParamDec :
    Specifier VarDec { addn($$, "ParamDec", 2, $1, $2); nowFL = addFieldList(nowFL, $2->inheridata, getVarDecName($2)); }
    ;

/* statement */
CompSt : LC                     { push_stack(); }
      DefList StmtList RC       { addn($$, "CompSt", 4, $1, $2, $3, $4); pop_stack(); }
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
    | WHILE LP Exp RP {++loop_cnt;} Stmt        { --loop_cnt; addn($$, "Stmt", 5, $1, $2, $3, $4, $5); }
    | FOR LP DecList SEMI Exp SEMI Exp RP {++loop_cnt;} Stmt  %prec UPPER_FOR { --loop_cnt; addn($$, "Stmt", 9, $1, $2, $3, $4, $5, $6, $7, $8, $9); }
    | FOR LP VarDec COLON Exp RP {++loop_cnt;} Stmt      %prec LOWER_FOR      { --loop_cnt; addn($$, "Stmt", 7, $1, $2, $3, $4, $5, $6, $7); }
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
    | FOR VarDec COLON Exp RP {++loop_cnt;} Stmt  %prec LOWER_FOR                 { --loop_cnt; add0($$, "Stmt"); has_error = 1; print_B_error("Stmt", $1->lineno, "Expected \'(\' after \'for\'"); }
    | FOR LP VarDec COLON Exp error {++loop_cnt;} Stmt  %prec LOWER_FOR           { --loop_cnt; add0($$, "Stmt"); has_error = 1; print_B_error("Stmt", $1->lineno, "Missing closing parenthesis \')\'"); }
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

Dec : VarDec            { add1($$, "Dec", 1, $1); try_define($1); }
    | VarDec ASSIGN Exp { addn($$, "Dec", 3, $1, $2, $3); try_define($1); }
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
    | ID LP             { nowFL = makeFieldList(NULL, ""); }
      Args RP           { addn($$, "Exp", 4, $1, $2, $3, $4); inherit_function($$, $1, nowFL); }                // function call with args
    | ID LP RP          { addn($$, "Exp", 3, $1, $2, $3); inherit_function($$, $1, makeFieldList(NULL, "")); }  // function call with no args
    | Exp LB Exp RB     { addn($$, "Exp", 4, $1, $2, $3, $4); inherit_array($$, $1, $3); }                      // array
    | Exp DOT ID        { addn($$, "Exp", 3, $1, $2, $3); inherit_struct($$, $1, $3); }                            // attribute of structure
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
    | LP Exp error      { add0($$, "Exp"); has_error = 1; print_B_error("Exp", $1->lineno, "Missing closing parenthesis \')\'"); $$->inheridata = makeErrorType(); }
    | ID LP             { nowFL = makeFieldList(NULL, ""); }
      Args error        { add0($$, "Exp"); has_error = 1; print_B_error("Exp", $2->lineno, "Missing closing parenthesis \')\'"); $$->inheridata = makeErrorType(); }
    | ID LP error       { add0($$, "Exp"); has_error = 1; print_B_error("Exp", $2->lineno, "Missing closing parenthesis \')\'"); $$->inheridata = makeErrorType(); }
    | Exp LB Exp error  { add0($$, "Exp"); has_error = 1; print_B_error("Exp", $2->lineno, "Missing closing braces \']\'"); $$->inheridata = makeErrorType(); }
    ;

Args : Exp COMMA Args   { addn($$, "Args", 3, $1, $2, $3); }
    | Exp               { add1($$, "Args", 1, $1); }
    ;

Var : UINT          { $$ = $1; $$->inheridata = makePrimType("int"); }
    | ID            {
          $$ = $1;
          $$->inheridata = find_type($1->val);
          if (((Type *)($$->inheridata))->category == ERRORTYPE)
            print_type_error(1, $$->lineno, "a variable is used without a definition.");
      }
    | FLOAT         { $$ = $1; $$->inheridata = makePrimType("float"); }
    | CHAR          { $$ = $1; $$->inheridata = makePrimType("char"); }
    | INVALID       { $$ = $1; }
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
    if (argc == 2 || argc == 3)
    {
        file_in = fopen(argv[1], "r");
        if (file_in == NULL)
        {
            perror("Error opening input file");
            return 1;
        }
        if (argc == 2)
        {
            int len = strlen(argv[1]);
            while (argv[1][len - 1] != '.')
                len--;
            char *ofname = (char *)malloc((len + 4) * sizeof(char));
            for (int i = 0; i < len; i++)
                ofname[i] = argv[1][i];
            ofname[len] = 'o';
            ofname[len + 1] = 'u';
            ofname[len + 2] = 't';
            ofname[len + 3] = 0;
            file_out = fopen(ofname, "w");
            free(ofname);
        }
        else if (argc == 3)
        {
            file_out = fopen(argv[2], "w");
        }
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
    output_tree(root, 0);
    if (!has_error)
    {
        if (root != NULL)
            output_tree(root, 0);
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