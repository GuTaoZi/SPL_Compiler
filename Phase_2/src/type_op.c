#include <stdio.h>
#include "utstack.h"
#include "treeNode.h"
#include "ortho.h"
#include "type.h"
#include "type_op.h"

char type_error_tmp[MAX_ERROR_LEN];
char tree_output_tmp[MAX_ERROR_LEN-1024];
type_stack *funcRetTypeStack = NULL, *structFieldStack = NULL;

void print_type_error(const int typeID, const size_t lineno, const char* msg){
    fprintf(yyout, "Error type %d at Line %zu: %s\n", typeID, lineno, msg);
    last_error_lineno = lineno;
    has_error = 1;
}

Type *find_type(const char *name)
{
    orthoNode *node = global_scope_seek(name);
    return node ? node->val : NULL;
}

void inherit_type(treeNode *u, const treeNode *v, const treeNode *w, const char *op)
{
    Type *tv = v->inheridata, *tw = w->inheridata;
    u->inheridata = getTypeAfterOp(tv, tw, op);
    if (tv->category == ERRORTYPE || tw->category == ERRORTYPE)
        return;
    if (strcmp(op, "ass") == 0 && !is_lvalue(v))
    {
        output_tree_array(v, tree_output_tmp);
        sprintf(type_error_tmp, "rvalue: \'%s\' appears on the left-hand side of the assignment operator.", tree_output_tmp);
        print_type_error(6, u->lineno, type_error_tmp);
        u->inheridata = makeErrorType();
    }
    else if (((Type*)u->inheridata)->category == ERRORTYPE){
        if (strcmp(op, "ass") == 0){
            print_type_error(5, u->lineno, "unmatching types for assign(=).");
        }
        else{
            print_type_error(7, u->lineno, "unmatching operands.");
        }
    }
    
}

void inherit_function(treeNode *u, const treeNode *v, const FieldList *fl)
{
    Type *t = find_type(v->val);
    if (t == NULL){
        u->inheridata = makeErrorType();
        output_tree_array(v, tree_output_tmp);
        sprintf(type_error_tmp, "Function: \'%s\' is invoked without a definition.", tree_output_tmp);
        print_type_error(2, u->lineno, type_error_tmp);
    }
    else if (t->category != FUNCTION)
    {
        u->inheridata = makeErrorType();
        output_tree_array(v, tree_output_tmp);
        sprintf(type_error_tmp, "applying function invocation operator on non-function name: \'%s\'.", tree_output_tmp);
        print_type_error(11, u->lineno, type_error_tmp);
    }
    else if (!checkFieldEqual(t->func->params, fl))
    {
        u->inheridata = makeErrorType();
        output_tree_array(v, tree_output_tmp);
        sprintf(type_error_tmp, "parameters missmatch for function: \'%s\'", tree_output_tmp);
        print_type_error(9, u->lineno, type_error_tmp);
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
        output_tree_array(v, tree_output_tmp);
        sprintf(type_error_tmp, "\'%s\' is not an array", tree_output_tmp);
        print_type_error(10, u->lineno, type_error_tmp);
    }
    else if (tw->category != PRIMITIVE || tw->primitive != PINT)
    {
        u->inheridata = makeErrorType();
        output_tree_array(w, tree_output_tmp);
        sprintf(type_error_tmp, "index: \'%s\' should be an integer", tree_output_tmp);
        print_type_error(12, u->lineno, type_error_tmp);
    }
    else
        u->inheridata = tv->array->base;
}

void inherit_struct(treeNode *u, const treeNode *v, const treeNode *w)
{
    Type *tv = v->inheridata;
    Type *tw = findNameInStructure(tv, w->val);
    if (tv->category != STRUCTURE)
    {
        u->inheridata = makeErrorType();
        output_tree_array(v, tree_output_tmp);
        sprintf(type_error_tmp, "\'%s\' is not a structure", tree_output_tmp);
        print_type_error(13, u->lineno, type_error_tmp);
    }
    else if (tw == NULL)
    {
        u->inheridata = makeErrorType();
        output_tree_array(w, tree_output_tmp);
        char *tt = type_error_tmp + sprintf(type_error_tmp, "attribute \'%s\' in struct: ", tree_output_tmp);
        output_tree_array(v, tree_output_tmp);
        sprintf(tt, "\'%s\' not found", tree_output_tmp);
        print_type_error(14, u->lineno, type_error_tmp);
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

void add_others(const Type *p, const size_t lineno, const char *name) {
    if(p->category == STRUCTURE){
        add_something(p, p->structure->struct_name, 15, lineno, "redefine the same structure type.");
    } else {
        sprintf(type_error_tmp, "function \'%s\' is redefined.", name);
        add_something(p, p->func->name, 4, lineno, type_error_tmp);
    }
}

void add_identifier(const treeNode *p) {
    if(strcmp(p->name, "VarDec") == 0){
        output_tree_array(p, tree_output_tmp);
        sprintf(type_error_tmp, "variable \'%s\' is redefined in the same scope.", tree_output_tmp);
        add_something(p->inheridata, getVarDecName(p), 3, p->lineno, type_error_tmp);
    } else if(((Type*)p->inheridata)->category == PRIMITIVE || ((Type*)p->inheridata)->category == ARRAY) {
        // add_something(p->inheridata, getVarDecName(p), 3, p->lineno, "a variable is redefined in the same scope.");
        fprintf(yyout, "At add_identifier: lineno: %zu: Sorry but you cannot do this.", p->lineno);
    } else if(((Type*)p->inheridata)->category == STRUCTURE || ((Type*)p->inheridata)->category == FUNCTION) {
        output_tree_array(p, tree_output_tmp);
        add_others(p->inheridata, p->lineno, tree_output_tmp);
    } else {
        print_type_error(-1, 0, "What the hell? At add_identifier.");
    }
}

type_stack *utstack_push(type_stack *root, const Type *nowType) {
    type_stack *p = (type_stack*)malloc(sizeof(type_stack));
    p->data = nowType;
    STACK_PUSH(root, p);
    return root;
}
type_stack *utstack_pop(type_stack *root){
    type_stack *p;
    STACK_POP(root, p);
    return root;
}
void checkRetType(const Type *ret2, const size_t lineno){
    const Type *ret1 = funcRetTypeStack->data;
    const Type *tu = getTypeAfterOp(ret1, ret2, "ass");
    if (tu->category == ERRORTYPE && ret1->category != ERRORTYPE && ret2->category != ERRORTYPE)
        print_type_error(8, lineno, "function's return value type mismatches the declared type.");
}

Type *findStruct(const char *name, const size_t lineno) {
    orthoNode *p=global_scope_seek(name);
    if(p == NULL || p->val->category != STRUCTURE){
        sprintf(type_error_tmp, "Cannot find structure definition for name: \'%s\'.", name);
        print_type_error(17, lineno, type_error_tmp);
        return makeErrorType();
    } else {
        return p->val;
    }
}
