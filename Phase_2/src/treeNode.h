#ifndef __TREENODE_H__
#define __TREENODE_H__
#include <stdio.h>
#include "stdlib.h"

extern FILE* yyout;

// These TOKENs need to output its value
extern const char *need_output[];
extern const int lno;

//// add a tree node with its children ////
#define add0(self, name)                       \
    do                                         \
    {                                          \
        self = new_node(name, NULL, yylineno); \
    } while (0)

#define addn(self, name, n, v1, ...)             \
    do                                           \
    {                                            \
        self = new_node(name, NULL, v1->lineno); \
        self->child = v1;                        \
        make_list(n, v1, __VA_ARGS__);           \
    } while (0)

#define add1(self, name, n, v1)                  \
    do                                           \
    {                                            \
        self = new_node(name, NULL, v1->lineno); \
        self->child = v1;                        \
    } while (0)
//////// tree node add ends here /////////

typedef struct _treeNode
{
    char *name;
    char *val;
    size_t lineno;
    struct _treeNode *child;
    struct _treeNode *next;
} treeNode;

/**
 * create a new node pointer.
 */
treeNode *new_node(char *name, const char *val, const size_t lino);

/**
 * free a node
 */
void del_node(treeNode *p);

/**
 * Output whole tree according to the syntax requirement
 */
void output_tree(treeNode *u, size_t spaceno);

/**
 * Set a list of nodes as `head`'s children
 */
void make_list(int cnt, treeNode *head, ...);

#endif