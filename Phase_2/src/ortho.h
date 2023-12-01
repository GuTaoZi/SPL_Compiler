#include "type.h"
#include "uthash.h"

#include <stdio.h>

extern FILE *yyout;

typedef struct _orthoNode
{
    char name[32];
    struct _orthoNode *next[2];
    // next[0]: ptr to next node with same name(hash)
    // next[1]: ptr to next node within same scope(stack)
    // NULL for tail of the list
    Type *val;
} orthoNode;

typedef struct _hashNode
{
    char name[32];
    orthoNode *head;
    UT_hash_handle hh;
} hashNode;

typedef struct _orthoStack
{
    orthoNode *top;
    struct _orthoStack *next;
} orthoStack;

/**
 * @description: Append a new stack node to the top of the stack list, invoked when opening a new scope.
 * @return {*} The current stack top orthoStack
 */
orthoStack *push_stack();

/**
 * @description: Clear all orthoNodes in the stack-top scope, then pop the stack top, invoked when leaving a scope.
 * @return {*} The stack top after popping
 */
orthoStack *pop_stack();

/**
 * @description: Get the current top of the stack
 * @return {*} The current stack top orthoStack
 */
orthoNode *stack_top();

/**
 * @description: Insert a new orthoNode, put it to the stack top of current scope, and to the head of the hash list
 * @param {char} *name name of the orthoNode
 * @param {Type} *val type of the orthoNode
 * @return {*}
 */
orthoNode *add_ortho_node(const char *name, Type *val);

/**
 * @description: Look for orthoNode with given name within the current scope.
 * @param {char} *name
 * @return {*} An orthoNode with given name, NULL for 404
 */
orthoNode *current_scope_seek(const char *name);

/**
 * @description: Look for the "nearest" orthoNode with given name in all valid scopes.
 * @param {char} *name
 * @return {*} An orthoNode with given name, NULL for 404
 */
orthoNode *global_scope_seek(const char *name);