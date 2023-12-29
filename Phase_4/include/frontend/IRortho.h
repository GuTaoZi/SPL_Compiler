#ifndef IR_ORTHO_H
#define IR_ORTHO_H

#include "uthash.h"

#include <stdio.h>

extern FILE *yyout;

typedef struct _IR_orthoNode
{
    char name[32];
    struct _IR_orthoNode *next[2];
    // next[0]: ptr to next node with same name(hash)
    // next[1]: ptr to next node within same scope(stack)
    // NULL for tail of the list
    char *vname;
} IR_orthoNode;

typedef struct _IR_hashNode
{
    char name[32];
    IR_orthoNode *head;
    UT_hash_handle hh;
} IR_hashNode;

typedef struct _IR_orthoStack
{
    IR_orthoNode *top;
    struct _IR_orthoStack *next;
} IR_orthoStack;

/**
 * @description: Append a new stack node to the top of the stack list, invoked when opening a new scope.
 * @return {*} The current stack top IR_orthoStack
 */
IR_orthoStack *IR_push_stack();

/**
 * @description: Clear all orthoNodes in the stack-top scope, then pop the stack top, invoked when leaving a scope.
 * @return {*} The stack top after popping
 */
IR_orthoStack *IR_pop_stack();

/**
 * @description: Get the current top of the stack
 * @return {*} The current stack top IR_orthoStack
 */
IR_orthoNode *IR_stack_top();

/**
 * @description: Insert a new IR_orthoNode, put it to the stack top of current scope, and to the head of the hash list
 * @param {char} *name name of the IR_orthoNode
 * @param {Type} *val type of the IR_orthoNode
 * @return {*}
 */
IR_orthoNode *IR_add_ortho_node(const char *name, const char *vname);

/**
 * @description: Look for IR_orthoNode with given name within the current scope.
 * @param {char} *name
 * @return {*} An IR_orthoNode with given name, NULL for 404
 */
IR_orthoNode *IR_current_scope_seek(const char *name);

/**
 * @description: Look for the "nearest" IR_orthoNode with given name in all valid scopes.
 * @param {char} *name
 * @return {*} An IR_orthoNode with given name, NULL for 404
 */
IR_orthoNode *IR_global_scope_seek(const char *name);

#endif