#include "treeNode.h"

typedef struct IR_tree
{
    char *stmt; // Only if child == NULL
    struct IR_tree *child;
    struct IR_tree *next;
} IR_tree;

extern IR_tree *root;

IR_tree *build_IR_tree(const treeNode *u);
void output_IR_tree(const IR_tree *u, FILE *f);