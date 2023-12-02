#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "treeNode.h"
#include "ortho.h"

const char *need_output[] = {"INT", "FLOAT", "CHAR", "STRING", "TYPE", "ID", "INCLUDE"};
const int lno = 7;

treeNode *new_node(const char *name, const char *val, size_t lino)
{
    // fprintf(yyout, "NODE GET:%s - %s\n", name, val);
    // fflush(yyout);
    treeNode *p = (treeNode *)malloc(sizeof(treeNode));
    p->name = name;
    if (val == NULL)
        p->val = NULL;
    else
    {
        p->val = (char *)malloc((strlen(val) + 1) * sizeof(char));
        strcpy(p->val, val);
    }
    p->lineno = lino;
    p->child = p->next = p->prev = NULL;
    return p;
}

void del_node(treeNode *p)
{
    if (p->val != NULL)
        free(p->val);
    free(p);
}

void output_line(const treeNode *u, size_t spaceno)
{
    if (u->child != NULL)
    {
        for (size_t i = 0; i < spaceno; i++)
            fprintf(yyout, " ");
        fprintf(yyout, "%s (%zu)\n", u->name, u->lineno);
    }
    else if (u->val != NULL)
    {
        for (size_t i = 0; i < spaceno; i++)
            fprintf(yyout, " ");
        int flg = 1;
        for (int i = 0; i < lno; i++)
        {
            if (strcmp(u->name, need_output[i]) == 0)
            {
                flg = 0;
                break;
            }
        }
        if (flg)
            fprintf(yyout, "%s\n", u->name);
        else
            fprintf(yyout, "%s: %s\n", u->name, u->val);
    }
}

void output_tree(const treeNode *u, size_t spaceno)
{
    output_line(u, spaceno);
    treeNode *p = u->child;
    while (p != NULL)
    {
        output_tree(p, spaceno + 2);
        p = p->next;
    }
}

void make_list(int cnt, treeNode *head, ...)
{
    va_list args;
    va_start(args, head);

    treeNode *value = head, *nxt;

    while (cnt-- > 1)
    {
        nxt = va_arg(args, treeNode *);
        value->next = nxt;
        nxt->prev = value;
        value = nxt;
    }

    va_end(args);
}

const char *getVarDecName(const treeNode *u)
{
    while (u->child != NULL)
        u = u->child;
    return u->val;
}

treeNode *get_child(const treeNode *u, size_t id)
{
    treeNode *v = u->child;
    while (id--)
        v = v->next;
    return v;
}

bool is_lvalue(const treeNode *u)
{
    return (u->child_cnt == 1 && !strcmp(u->child->name, "ID")) ||
        (u->child_cnt == 3 && !strcmp(get_child(u, 1)->name, "DOT")) ||
        (u->child_cnt == 4 && !strcmp(get_child(u, 1)->name, "LB"));
}