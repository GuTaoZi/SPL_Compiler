#include "IRgen.h"

#include <string.h>

IR_tree *root = NULL;

IR_tree *build_IR_tree(const treeNode *u)
{
    if (u == NULL)
        return NULL;
    IR_tree *p = new_IR_node(NULL);
    if (strcmp(u->name, "") == 0)
    {
    }
    else if (strcmp(u->name, "") == 0)
    {
    }
    else
    {
        p->child = build_IR_tree(u->child);
        treeNode *un = u->child;
        IR_tree *pn = p->child;
        while (un != NULL)
        {
            un = un->next;
            if (un == NULL)
                break;
            pn->next = build_IR_tree(un);
            pn = pn->next;
        }
    }
    return p;
}

void output_IR_tree(const IR_tree *u, FILE *f)
{
    if (u->child == NULL)
    {
        fprintf(f, "%s\n", u->stmt);
    }
    else
    {
        IR_tree *q = u->child;
        while (q != NULL)
        {
            output_IR_tree(q, f);
            q = q->next;
        }
    }
}