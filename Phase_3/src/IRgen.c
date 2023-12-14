#include "IRgen.h"
#include "type.h"

#include <stdarg.h>
#include <assert.h>
#include <string.h>

IR_tree *new_IR_node(const char *stmt)
{
    IR_tree *p = (IR_tree *)malloc(sizeof(IR_tree));
    p->child = p->next = 0;
    if (stmt == NULL)
        p->stmt = NULL;
    else
    {
        size_t ll = strlen(stmt);
        p->stmt = (char *)malloc(sizeof(char) * (ll + 1));
        strncpy(p->stmt, stmt, ll);
    }
    return p;
}

void make_IR_list(int cnt, IR_tree *head, ...)
{
    va_list args;
    va_start(args, head);

    IR_tree *value = head, *nxt;

    while (cnt-- > 1)
    {
        nxt = va_arg(args, IR_tree *);
        value->next = nxt;
        value = nxt;
    }

    va_end(args);
}

IR_tree *alloc_var_mem(const treeNode *u)
{
    // u: DecList-like
    assert(strcmp(u->name, "ExtDecList") == 0 || strcmp(u->name, "DecList") == 0);
}

void *alloc_label()
{
}

IR_tree *build_FunDec_IR_tree(const treeNode *u)
{
}

IR_tree *build_CompSt_IR_tree(const treeNode *u)
{
    // All Stmts go here
    // Called by build_FunDec
}

IR_tree *build_normExp_IR_tree(const treeNode *u)
{
    // Called by stmt
}

IR_tree *build_ifExp_IR_tree(const treeNode *u)
{
    // Called by `if` or `loop` stmt
}

IR_tree *build_default_IR_tree(const treeNode *u)
{
    IR_tree *p = build_IR_tree(u->child);
    treeNode *un = u->child;
    IR_tree *pn = p;
    while (un != NULL)
    {
        un = un->next;
        if (un == NULL)
            break;
        pn->next = build_IR_tree(un);
        pn = pn->next;
    }
    return p;
}

// TODO: Need to build a Compst Stack
// to store malloced variables

IR_tree *build_IR_tree(const treeNode *u)
{
    if (u == NULL)
        return NULL;
    IR_tree *p = new_IR_node(NULL);
    if (strcmp(u->name, "ExtDef") == 0)
    {
        if (strcmp(u->child->name, "Specifier") == 0 &&
            strcmp(u->child->next->name, "ExtDecList") == 0 &&
            ((Type *)(u->child->inheridata))->category != PRIMITIVE)
        {
            IR_tree *c1 = alloc_var_mem(u->child->next);
            addIR1(p, NULL, 1, c1);
        }
        else if (strcmp(u->child->next->name, "FunDec") == 0)
        {
            IR_tree *c1 = build_FunDec_IR_tree(u);
            addIR1(p, NULL, 1, c1);
        }
        else
        {
            ;
        }
    }
    else if (strcmp(u->name, "Def") == 0)
    {
        // Similar as above
        // Be aware of Dec: VarDec ASSIGN Exp
    }
    else if (strcmp(u->name, "") == 0)
    {
    }
    else if (strcmp(u->name, "") == 0)
    {
    }
    else
    {
        p->child = build_default_IR_tree(u);
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