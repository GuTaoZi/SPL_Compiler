#include "IRgen.h"
#include "type.h"

#include <stdarg.h>
#include <assert.h>
#include <string.h>

size_t mlg10(size_t u)
{
    int n = 0;
    do
    {
        u /= 10;
        n += 1;
    } while (u);
    return n;
}

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

size_t label_cnt = 0;
char *alloc_label()
{
    char *tlabel = (char *)malloc(sizeof(char) * (2 + mlg10(label_cnt)));
    sprintf(tlabel, "l%zu", label_cnt);
    return tlabel;
}

size_t tmp_val_cnt = 0;
char *alloc_tmpval()
{
    char *tlabel = (char *)malloc(sizeof(char) * (2 + mlg10(tmp_val_cnt)));
    sprintf(tlabel, "t%zu", tmp_val_cnt);
    return tlabel;
}

size_t var_val_cnt = 0;
char *alloc_varval()
{
    char *tlabel = (char *)malloc(sizeof(char) * (2 + mlg10(var_val_cnt)));
    sprintf(tlabel, "v%zu", var_val_cnt);
    return tlabel;
}

char ttmp[32768];

IR_tree *build_paramDec_IR_tree(const treeNode *u)
{
    // u: ParamDec
    sprintf(ttmp, "PARAM %s", get_IR_name(getVarDecName(u->child->next)));
    return new_IR_node(ttmp);
}

IR_tree *build_params_IR_tree(const treeNode *u)
{
    // u: VarList
    if (u->child_cnt == 1)
    {
        return build_paramDec_IR_tree(u->child);
    }
    else
    {
        IR_tree *p;
        IR_tree *c1 = build_paramDec_IR_tree(u->child);
        IR_tree *c2 = build_params_IR_tree(u->child->next->next);
        addIRn(p, 2, c1, c2);
        return p;
    }
}

IR_tree *build_assign_IR_tree(const char *result, const treeNode *u)
{
    // u: Exp
    // return result := xxx
    IR_tree *p;
    IR_tree *c1 = build_normExp_IR_tree(u);
    sprintf(ttmp, "%s := %s", result, c1->stmt);
    IR_tree *c2 = new_IR_node(ttmp);
    addIRn(p, 2, c1, c2);
    return p;
}

IR_tree *build_arg_IR_tree(const char *varname)
{
    sprintf(ttmp, "ARG %s", varname);
    return new_IR_node(ttmp);
}

IR_tree *build_args_IR_tree(const treeNode *u)
{
    // u: Args
    if (u->child_cnt == 1)
    {
        IR_tree *p;
        char *tmp_val = alloc_tmpval();
        IR_tree *c1 = build_assign_IR_tree(tmp_val, u->child);
        IR_tree *c2 = build_arg_IR_tree(tmp_val);
        addIRn(p, 2, c1, c2);
        free(tmp_val);
        return p;
    }
    else
    {
        IR_tree *p;
        char *tmp_val = alloc_tmpval();
        IR_tree *c1 = build_assign_IR_tree(tmp_val, u->child);
        IR_tree *c2 = build_args_IR_tree(u->child->next->next);
        IR_tree *c3 = build_arg_IR_tree(tmp_val);
        addIRn(p, 3, c1, c2, c3);
        free(tmp_val);
        return p;
    }
}

IR_tree *build_FunDec_IR_tree(const treeNode *u)
{
    // u: ExtDef
    IR_tree *p;
    sprintf(ttmp, "FUNCTION %s :", u->child->next->child->val);
    IR_tree *c1 = new_IR_node(ttmp);
    IR_tree *c2;
    if (u->child->next->child_cnt == 4)
        c2 = build_params_IR_tree(u->child->next->child->next->next);
    else
        c2 = new_IR_node(NULL);
    IR_tree *c3 = build_CompSt_IR_tree(u->child->next->next, NULL, NULL);
    IR_tree *c4 = new_IR_node("RETURN #0");
    addIRn(p, 4, c1, c2, c3, c4);
    return p;
}

IR_tree *build_CompSt_IR_tree(const treeNode *u, const char *lloop_head, const char *lloop_end)
{
    // All Stmts go here
    // Called by build_FunDec
    // u: CompSt
    IR_tree *p;
    IR_tree *c1 = build_defList_IR_tree(u->child->next);
    IR_tree *c2 = build_stmtList_IR_tree(u->child->next->next, lloop_head, lloop_end);
    addIRn(p, 2, c1, c2);
    return p;
}

IR_tree *build_defList_IR_tree(const treeNode *u)
{
    if (u->child_cnt == 2)
    {
        IR_tree *p;
        IR_tree *c1 = build_def_IR_tree(u->child);
        IR_tree *c2 = build_defList_IR_tree(u->child->next);
        addIRn(p, 2, c1, c2);
        return p;
    }
    else
    {
        // return new_IR_node(NULL);
        return NULL;
    }
}
IR_tree *build_stmtList_IR_tree(const treeNode *u, const char *lloop_head, const char *lloop_end)
{
    if (u->child_cnt == 2)
    {
        IR_tree *p;
        IR_tree *c1 = build_stmt_IR_tree(u->child, lloop_head, lloop_end);
        IR_tree *c2 = build_stmtList_IR_tree(u->child->next, lloop_head, lloop_end);
        addIRn(p, 2, c1, c2);
        return p;
    }
    else
    {
        // return new_IR_node(NULL);
        return NULL;
    }
}

IR_tree *build_def_IR_tree(const treeNode *u)
{
    // from CompSt
    // u: Def
}

IR_tree *build_stmt_IR_tree(const treeNode *u, const char *lloop_head, const char *lloop_end)
{
    // from CompSt
    // u: Stmt
    if (strcmp(u->child->name, "CompSt") == 0)
    {
        return build_CompSt_IR_tree(u->child, lloop_head, lloop_end);
    }
    else if (strcmp(u->child->name, "SEMI") == 0)
    {
        return new_IR_node("");
    }
    else if (u->child_cnt == 2)
    {
        if (strcmp(u->child->name, "Exp") == 0)
        {
            return build_normExp_IR_tree(u->child);
        }
        else if (strcmp(u->child->name, "BREAK") == 0)
        {
            sprintf(ttmp, "GOTO %s", lloop_end);
            return new_IR_node(ttmp);
        }
        else if (strcmp(u->child->name, "CONTINUE") == 0)
        {
            sprintf(ttmp, "GOTO %s", lloop_head);
            return new_IR_node(ttmp);
        }
        else
        {
            // Raise an error
            return new_IR_node("error := #1");
        }
    }
    else if (strcmp(u->child->name, "RETURN") == 0)
    {
        IR_tree *p;
        IR_tree *c1 = build_normExp_IR_tree(u->child->next);
        sprintf(ttmp, "RETURN %s", c1->stmt);
        IR_tree *c2 = new_IR_node(ttmp);
        addIRn(p, 2, c1,c2);
        return p;
    }
    else if (strcmp(u->child->name, "IF") == 0)
    {
        if(u->child_cnt == 5) // No else
        {
            char *endlabel = alloc_label();
            IR_tree *p;
            IR_tree *c1 = build_ifExp_IR_tree(u->child->next->next, NULL, endlabel, endlabel);
            IR_tree *c2 = build_stmt_IR_tree(u->child->next->next->next->next, lloop_head, lloop_end);
            sprintf(ttmp, "LABEL %s", endlabel);
            IR_tree *c3 = new_IR_node(ttmp);
            addIRn(p, 3, c1,c2,c3);
            free(endlabel);
        } else {
            char *falselabel = alloc_label();
            char *endlabel = alloc_label();
            IR_tree *p;
            IR_tree *c1 = build_ifExp_IR_tree(u->child->next->next, NULL, falselabel, endlabel);

            IR_tree *c2 = build_stmt_IR_tree(u->child->next->next->next->next, lloop_head, lloop_end);
            
            sprintf(ttmp, "GOTO %s", endlabel);
            IR_tree *c3 = new_IR_node(ttmp);

            sprintf(ttmp, "LABEL %s", falselabel);
            IR_tree *c4 = new_IR_node(ttmp);

            IR_tree *c5 = build_stmt_IR_tree(u->child->next->next->next->next->next->next, lloop_head, lloop_end);

            sprintf(ttmp, "LABEL %s", endlabel);
            IR_tree *c6 = new_IR_node(ttmp);
            addIRn(p, 6, c1,c2,c3,c4,c5,c6);
            free(endlabel);
            free(falselabel);
        }
    }
    else if (strcmp(u->child->name, "WHILE") == 0)
    {
        IR_tree *p;
        char *loop_head = alloc_label();
        char *loop_tail = alloc_label();
        IR_tree *c1 = build_ifExp_IR_tree(u->child->next->next, NULL, loop_tail, loop_tail);
        IR_tree *c2 = 
    }
    else
    {
        ;
    }
}

IR_tree *build_normExp_IR_tree(const treeNode *u)
{
    // Called by stmt
    // u: Exp
}

IR_tree *build_ifExp_IR_tree(const treeNode *u, const char *ltrue, const char *lfalse, const char *lend)
{
    // Called by `if` or `loop` stmt
    // u: Exp
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
    IR_tree *p;
    if (strcmp(u->name, "ExtDef") == 0)
    {
        if (strcmp(u->child->name, "Specifier") == 0 &&
            strcmp(u->child->next->name, "ExtDecList") == 0 &&
            ((Type *)(u->child->inheridata))->category != PRIMITIVE)
        {
            IR_tree *c1 = alloc_var_mem(u->child->next);
            addIR1(p, 1, c1);
        }
        else if (strcmp(u->child->next->name, "FunDec") == 0)
        {
            IR_tree *c1 = build_FunDec_IR_tree(u);
            addIR1(p, 1, c1);
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
        p = new_IR_node(NULL);
        p->child = build_default_IR_tree(u);
    }
    return p;
}

void output_IR_tree(const IR_tree *u, FILE *f)
{
    if (u->child == NULL)
    {
        if (u->stmt != NULL)
        {
            fprintf(f, "%s\n", u->stmt);
        }
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