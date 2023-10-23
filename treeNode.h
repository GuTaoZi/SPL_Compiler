#ifndef __TREENODE_H__
#define __TREENODE_H__
#include "stdlib.h"

typedef struct _treeNode {
    char *name;
    char *val;
    size_t lineno;
    struct _treeNode *child;
    struct _treeNode *next;
} treeNode;

treeNode* new_node(char *name, const char *val, const size_t lino);

#endif