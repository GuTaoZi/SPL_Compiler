#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "treeNode.h"

const char *need_output[] = {"INT", "FLOAT", "CHAR", "STRING", "TYPE", "ID", "INCLUDE"};
const int lno = 7;

treeNode *new_node(const char *name, const char *val, size_t lino)
{
    // fprintf(yyout, "NODE GET:%s - %s\n", name, val);
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

void output_line(treeNode *u, size_t spaceno)
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

void output_tree(treeNode *u, size_t spaceno)
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

const char *getVarDecName(treeNode *u)
{
    while (u->child != NULL)
        u = u->child;
    return u->val;
}

treeNode *get_child(treeNode *u, size_t id)
{
    treeNode *v = u->child;
    while (id--)
        v = v->next;
    return v;
}

bool is_lvalue(treeNode *u)
{
    return (u->child_cnt == 1 && !strcmp(u->child->child->name, "ID")) ||
        (u->child_cnt == 3 && get_child(u, 1)->name == "DOT") ||
        (u->child_cnt == 4 && get_child(u, 1)->name == "LB");
}

void try_define(treeNode *u)
{
    char *name = getVarDecName(u);
    if (current_scope_seek(name))
        print_scope_error(name, u->lineno);
    else
        add_ortho_node(name, u->inheridata);
}

bool try_cast(treeNode *u, treeNode *v)
{
    // TODO
}

void try_assign(treeNode *u, treeNode *v)
{
    if (!is_lvalue(u))
    {
        fprintf(yyout, "Line %zu: expression must be a modifiable lvalue", u->lineno);
        return;
    }
    if (!try_cast(u, v))
        return;
    u->val = v->val;
}

void check_ID(treeNode *u)
{
    char *name = u->val;
    if (!global_scope_seek(name))
        fprintf("Line: %zu: identifier \"%s\" is undefined\n", u->lineno, name);
}

void check_array(treeNode *u)
{
    
}

void check_struct(treeNode *u, treeNode *v)
{
    char *struct_name = u->val, attribute_name = v->val;
    // TODO
}
