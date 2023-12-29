#ifndef TYPE_OP_H
#define TYPE_OP_H

#include "ortho.h"
#include "treeNode.h"
#include "type.h"
#include <stdio.h>

#define MAX_ERROR_LEN 32768
#define nowFL (structFieldStack->data->structure->data)

extern size_t last_error_lineno;
extern int has_error;
extern char type_error_tmp[], tree_output_tmp[];

typedef struct type_stack
{
    Type *data;
    struct type_stack *next;
} type_stack;
extern type_stack *funcRetTypeStack, *structFieldStack;

void print_type_error(const int typeID, const size_t lineno, const char *msg);

Type *find_type(const char *name);

void inherit_type(treeNode *u, const treeNode *v, const treeNode *w, const char *op);

void inherit_function(treeNode *u, const treeNode *v, const FieldList *fl);

void inherit_array(treeNode *u, const treeNode *v, const treeNode *w);

void inherit_struct(treeNode *u, treeNode *v, const treeNode *w);

void add_initial_functions();
void add_something(Type *p, const char *name, const int errorID, const size_t lineno, const char *error_msg);

void add_others(Type *p, const size_t lineno, const char *name);

void add_identifier(treeNode *p);

type_stack *utstack_push(type_stack *rootx, Type *nowType);

type_stack *utstack_pop(type_stack *rootx);

void checkRetType(Type *ret2, const size_t lineno);

Type *findStruct(const char *name, const size_t lineno);

#endif // TYPE_OP_H
