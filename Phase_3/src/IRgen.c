#include "IRgen.h"
#include "type.h"
#include "IRortho.h"

#include <assert.h>
#include <stdarg.h>
#include <string.h>

// #define DEBUG_OUTPUT
// #define DEBUG_MUCH_OUTPUT

size_t mlg10(size_t u)
{
    size_t n = 0;
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
    add_IR_stmt(p, stmt);
    p->should_print = true;
    return p;
}

void add_IR_stmt(IR_tree *p, const char *stmt)
{
    if (stmt == NULL)
        p->stmt = NULL;
    else
    {
        size_t ll = strlen(stmt);
        p->stmt = (char *)malloc(sizeof(char) * (ll + 1));
        memset(p->stmt, 0, sizeof(p->stmt));
        strncpy(p->stmt, stmt, ll);
    }
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

size_t label_cnt = 0;
char *alloc_label()
{
    char *tlabel = (char *)malloc(sizeof(char) * (2 + mlg10(label_cnt)));
    sprintf(tlabel, "l%zu", label_cnt++);
    return tlabel;
}

size_t tmp_val_cnt = 0;
char *alloc_tmpvar()
{
    char *tname = (char *)malloc(sizeof(char) * (2 + mlg10(tmp_val_cnt)));
    sprintf(tname, "t%zu", tmp_val_cnt++);
    return tname;
}

size_t var_val_cnt = 0;
char *alloc_varval()
{
    char *vname = (char *)malloc(sizeof(char) * (2 + mlg10(var_val_cnt)));
    sprintf(vname, "v%zu", var_val_cnt++);
    return vname;
}

char ttmp[32768];

IR_tree *build_ExtDecList_tree(const treeNode *u)
{
#ifdef DEBUG_OUTPUT
    printf("%s: %s\n", __FUNCTION__, u->name);
#endif
    // u: ExtDecList
    // TODO DEC x typesize
    // insert into IRstack
    char *vname = alloc_varval();
    IR_tree *p;
    if (((Type *)u->child->inheridata)->category != PRIMITIVE)
    {
        sprintf(ttmp, "&%s", vname);
        IR_add_ortho_node(getVarDecName(u), ttmp);
        sprintf(ttmp, "DEC %s %zu", vname, ((Type *)u->child->inheridata)->typesize);
        IR_tree *c1 = new_IR_node(ttmp);
        IR_tree *c2 = build_ExtDecList_tree(u->child->next->next);
        addIRn(p, 2, c1, c2);
    }
    else
    {
        IR_add_ortho_node(getVarDecName(u), vname);
        p = build_ExtDecList_tree(u->child->next->next);
    }
    free(vname);
#ifdef DEBUG_OUTPUT
    printf("leave %s\n", __FUNCTION__);
#endif
    return p;
}

IR_tree *build_Dec_tree(const treeNode *u)
{
#ifdef DEBUG_OUTPUT
    printf("%s: %s\n", __FUNCTION__, u->name);
#endif
    char *vname = alloc_varval();
    IR_tree *p;
    if (u->child_cnt == 1)
    {
        if (((Type *)u->child->inheridata)->category != PRIMITIVE)
        {
            sprintf(ttmp, "&%s", vname);
            IR_add_ortho_node(getVarDecName(u), ttmp);
            sprintf(ttmp, "DEC %s %zu", vname, ((Type *)u->child->inheridata)->typesize);
            p = new_IR_node(ttmp);
        }
        else
        {
            IR_add_ortho_node(getVarDecName(u), vname);
            p = new_IR_node("");
            p->should_print = false;
        }
    }
    else
    {
        IR_tree *c1;
        if (((Type *)u->child->inheridata)->category != PRIMITIVE)
        {
            sprintf(ttmp, "&%s", vname);
            IR_add_ortho_node(getVarDecName(u), ttmp);
            sprintf(ttmp, "DEC %s %zu", vname, ((Type *)u->child->inheridata)->typesize);
            c1 = new_IR_node(ttmp);
        }
        else
        {
            IR_add_ortho_node(getVarDecName(u), vname);
            c1 = new_IR_node("");
            c1->should_print = false;
        }
        IR_tree *c2 = build_assign_IR_tree(vname, u->child->next->next);
        addIRn(p, 2, c1, c2);
    }

    free(vname);
#ifdef DEBUG_OUTPUT
    printf("leave %s\n", __FUNCTION__);
#endif
    return p;
}
IR_tree *build_DecList_tree(const treeNode *u)
{
#ifdef DEBUG_OUTPUT
    printf("%s: %s\n", __FUNCTION__, u->name);
#endif
    // u: DecList
    IR_tree *p;
    IR_tree *c1 = build_Dec_tree(u->child);
    IR_tree *c2 = NULL;
    if (u->child_cnt == 3)
        c2 = build_DecList_tree(u->child->next->next);
    addIRn(p, 2, c1, c2);
#ifdef DEBUG_OUTPUT
    printf("leave %s\n", __FUNCTION__);
#endif
    return p;
}

IR_tree *build_paramDec_IR_tree(const treeNode *u)
{
#ifdef DEBUG_OUTPUT
    printf("%s: %s\n", __FUNCTION__, u->name);
#endif
    // u: ParamDec
    char *vname = alloc_varval();
    sprintf(ttmp, "PARAM %s", vname);
    IR_add_ortho_node(getVarDecName(u->child->next), vname);
    free(vname);
#ifdef DEBUG_OUTPUT
    printf("leave %s\n", __FUNCTION__);
#endif
    return new_IR_node(ttmp);
}

IR_tree *build_params_IR_tree(const treeNode *u)
{
#ifdef DEBUG_OUTPUT
    printf("%s: %s\n", __FUNCTION__, u->name);
#endif
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
#ifdef DEBUG_OUTPUT
    printf("%s: %s\n", __FUNCTION__, u->name);
#endif
    // u: Exp
    // return result := xxx
    IR_tree *p;
    IR_tree *c1 = build_normExp_IR_tree(u);
    sprintf(ttmp, "%s := %s", result, c1->stmt);
    IR_tree *c2 = new_IR_node(ttmp);
    addIRn(p, 2, c1, c2);
    add_IR_stmt(p, result);
#ifdef DEBUG_OUTPUT
    printf("leave %s\n", __FUNCTION__);
#endif
    return p;
}

IR_tree *build_arg_IR_tree(const char *varname)
{
    sprintf(ttmp, "ARG %s", varname);
    return new_IR_node(ttmp);
}

IR_tree *build_args_IR_tree(const treeNode *u)
{
#ifdef DEBUG_OUTPUT
    printf("%s: %s\n", __FUNCTION__, u->name);
#endif
    // u: Args
    IR_tree *p;
    if (u->child_cnt == 1)
    {
        char *tmp_val = alloc_tmpvar();
        IR_tree *c1 = build_assign_IR_tree(tmp_val, u->child);
        IR_tree *c2 = build_arg_IR_tree(tmp_val);
        addIRn(p, 2, c1, c2);
        free(tmp_val);
    }
    else
    {
        char *tmp_val = alloc_tmpvar();
        IR_tree *c1 = build_assign_IR_tree(tmp_val, u->child);
        IR_tree *c2 = build_args_IR_tree(u->child->next->next);
        IR_tree *c3 = build_arg_IR_tree(tmp_val);
        addIRn(p, 3, c1, c2, c3);
        free(tmp_val);
    }
#ifdef DEBUG_OUTPUT
    printf("leave %s\n", __FUNCTION__);
#endif
    return p;
}

IR_tree *build_FunDec_IR_tree(const treeNode *u)
{
#ifdef DEBUG_OUTPUT
    printf("%s: %s\n", __FUNCTION__, u->name);
#endif
    // u: ExtDef
    IR_tree *p;
    treeNode *tntmp;
    sprintf(ttmp, "FUNCTION %s :", (tntmp = u->child->next->child)->val);
    IR_tree *c1 = new_IR_node(ttmp);
    IR_tree *c2;
    IR_push_stack();
    if (u->child->next->child_cnt == 4)
        c2 = build_params_IR_tree(tntmp->next->next);
    else
        c2 = new_IR_node(NULL);
    IR_tree *c3 = build_CompSt_IR_tree(u->child->next->next, NULL, NULL);
    IR_tree *c4 = new_IR_node("RETURN #0");
    addIRn(p, 4, c1, c2, c3, c4);
    IR_pop_stack();
#ifdef DEBUG_OUTPUT
    printf("leave %s\n", __FUNCTION__);
#endif
    return p;
}

IR_tree *build_CompSt_IR_tree(const treeNode *u, const char *lloop_head, const char *lloop_end)
{
#ifdef DEBUG_OUTPUT
    printf("%s: %s\n", __FUNCTION__, u->name);
#endif
    // All Stmts go here
    // Called by build_FunDec
    // u: CompSt
    IR_push_stack();
    IR_tree *p;
    treeNode *tntmp;
    IR_tree *c1 = build_defList_IR_tree(tntmp = u->child->next);
    IR_tree *c2 = build_stmtList_IR_tree(tntmp->next, lloop_head, lloop_end);
    IR_pop_stack();
    addIRn(p, 2, c1, c2);
#ifdef DEBUG_OUTPUT
    printf("leave %s\n", __FUNCTION__);
#endif
    return p;
}

IR_tree *build_defList_IR_tree(const treeNode *u)
{
#ifdef DEBUG_OUTPUT
    printf("%s: %s\n", __FUNCTION__, u->name);
#endif
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
        IR_tree *p = new_IR_node(NULL);
        p->should_print = false;
        return p;
        // return NULL;
    }
}
IR_tree *build_stmtList_IR_tree(const treeNode *u, const char *lloop_head, const char *lloop_end)
{
#ifdef DEBUG_OUTPUT
    printf("%s: %s\n", __FUNCTION__, u->name);
#endif
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
        IR_tree *p = new_IR_node(NULL);
        p->should_print = false;
        return p;
        // return NULL;
    }
}

IR_tree *build_def_IR_tree(const treeNode *u)
{
    // from CompSt
    // u: Def
    return build_DecList_tree(u->child->next);
}

IR_tree *build_stmt_IR_tree(const treeNode *u, const char *lloop_head, const char *lloop_end)
{
#ifdef DEBUG_OUTPUT
    printf("%s: %s\n", __FUNCTION__, u->name);
#endif
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
        addIRn(p, 2, c1, c2);
        return p;
    }
    else if (strcmp(u->child->name, "IF") == 0)
    {
        if (u->child_cnt == 5) // No else
        {
            char *truelabel = alloc_label();
            char *endlabel = alloc_label();
            IR_tree *p;
            treeNode *tntmp;
            IR_tree *c1 = build_ifExp_IR_tree(tntmp = u->child->next->next, truelabel, endlabel, endlabel);
            sprintf(ttmp, "LABEL %s :", truelabel);
            IR_tree *c2 = new_IR_node(ttmp);
            IR_tree *c3 = build_stmt_IR_tree(tntmp = tntmp->next->next, lloop_head, lloop_end);
            sprintf(ttmp, "LABEL %s :", endlabel);
            IR_tree *c4 = new_IR_node(ttmp);
            addIRn(p, 4, c1, c2, c3, c4);
            free(endlabel);
            free(truelabel);
            return p;
        }
        else
        {
            char *truelabel = alloc_label();
            char *falselabel = alloc_label();
            char *endlabel = alloc_label();
            IR_tree *p;
            treeNode *tntmp;
            IR_tree *c1 = build_ifExp_IR_tree(tntmp = u->child->next->next, truelabel, falselabel, endlabel);

            sprintf(ttmp, "LABEL %s :", truelabel);
            IR_tree *c2 = new_IR_node(ttmp);

            IR_tree *c3 = build_stmt_IR_tree(tntmp = tntmp->next->next, lloop_head, lloop_end);

            sprintf(ttmp, "GOTO %s", endlabel);
            IR_tree *c4 = new_IR_node(ttmp);

            sprintf(ttmp, "LABEL %s :", falselabel);
            IR_tree *c5 = new_IR_node(ttmp);
            
            IR_tree *c6 = build_stmt_IR_tree(tntmp->next->next, lloop_head, lloop_end);

            sprintf(ttmp, "LABEL %s :", endlabel);
            IR_tree *c7 = new_IR_node(ttmp);
            addIRn(p, 7, c1, c2, c3, c4, c5, c6, c7);
            free(endlabel);
            free(falselabel);
            free(truelabel);
            return p;
        }
    }
    else if (strcmp(u->child->name, "WHILE") == 0)
    {
        char *nloop_head = alloc_label();
        char *nloop_body = alloc_label();
        char *nloop_tail = alloc_label();

        IR_tree *p;
        treeNode *tntmp;
        sprintf(ttmp, "LABEL %s :", nloop_head);
        IR_tree *c1 = new_IR_node(ttmp);
        IR_tree *c2 = build_ifExp_IR_tree(tntmp = u->child->next->next, nloop_body, nloop_tail, nloop_tail);

        sprintf(ttmp, "LABEL %s :", nloop_body);
        IR_tree *c3 = new_IR_node(ttmp);
        IR_tree *c4 = build_stmt_IR_tree(tntmp->next->next, nloop_head, nloop_tail);
        sprintf(ttmp, "GOTO %s", nloop_head);
        IR_tree *c5 = new_IR_node(ttmp);
        sprintf(ttmp, "LABEL %s :", nloop_tail);
        IR_tree *c6 = new_IR_node(ttmp);
        addIRn(p, 6, c1, c2, c3, c4, c5, c6);

        free(nloop_tail);
        free(nloop_body);
        free(nloop_head);
        return p;
    }
    else if (strcmp(u->child->name, "FOR") == 0)
    {
        IR_tree *p;
        char *nloop_head = alloc_label();
        char *nloop_body = alloc_label();
        char *nloop_tail = alloc_label();
        char *nloop_cont = alloc_label();

        treeNode *tntmp;
        IR_tree *c1 = build_normExp_IR_tree(tntmp = u->child->next->next); // first

        sprintf(ttmp, "LABEL %s :", nloop_head);
        IR_tree *c2 = new_IR_node(ttmp);

        IR_tree *c3 = build_ifExp_IR_tree(tntmp = tntmp->next->next, nloop_body, nloop_tail, nloop_tail); // if

        sprintf(ttmp, "LABEL %s :", nloop_body);
        IR_tree *c4 = new_IR_node(ttmp);

        IR_tree *c5 = build_normExp_IR_tree(tntmp = tntmp->next->next); // increase

        IR_tree *c6 = build_stmt_IR_tree(tntmp = tntmp->next->next, nloop_cont, nloop_tail); // body

        sprintf(ttmp, "LABEL %s :", nloop_cont);
        IR_tree *c7 = new_IR_node(ttmp);
        sprintf(ttmp, "GOTO %s", nloop_head);
        IR_tree *c8 = new_IR_node(ttmp);
        sprintf(ttmp, "LABEL %s :", nloop_tail);
        IR_tree *c9 = new_IR_node(ttmp);

        addIRn(p, 9, c1, c2, c3, c4, c6, c7, c5, c8, c9);
        free(nloop_tail);
        free(nloop_body);
        free(nloop_head);
        free(nloop_cont);
        return p;
    }
    else
    {
        // Raise an error
        return new_IR_node("error := #1");
    }
}

#ifdef DEBUG_MUCH_OUTPUT
char tttt[65536];
#endif
IR_tree *build_normExp_IR_tree(const treeNode *u)
{
#ifdef DEBUG_OUTPUT
    printf("%s: %s\n", __FUNCTION__, u->name);
#ifdef DEBUG_MUCH_OUTPUT
    output_tree_array(u, tttt);
    printf("%s\n", tttt);
    // output_tree(u, 0);
#endif
#endif
    // Called by stmt
    // u: Exp
    IR_tree *p;
    if (u->child_cnt == 1)
    {
        treeNode *uc = u->child;
        if (strcmp(uc->name, "ID") == 0)
        {
            char *vname;
            IR_orthoNode *iron = IR_global_scope_seek(uc->val);
            if (!iron) // no allocated vname for this ID
            {
                vname = alloc_varval();
                IR_add_ortho_node(uc->val, vname);
            }
            else // previously allocated vname for this ID
            {
                vname = iron->vname;
            }
            p = new_IR_node(vname);
            p->should_print = false;
            if (!iron)
                free(vname);
            return p;
        }
        else if (!strcmp(uc->name, "STRING"))
        {
            sprintf(ttmp, "\"%s\"", uc->child->val);
            p = new_IR_node(ttmp);
            p->should_print = false;
            return p;
        }
        else
        {
            sprintf(ttmp, "#%s", uc->val);
            p = new_IR_node(ttmp);
            p->should_print = false;
            return p;
        }
    }
    else if (u->child_cnt == 2)
    {
        treeNode *opc = u->child;
        treeNode *expc = u->child->next;
        if (!strcmp(opc->name, "PLUS"))
        {
            return p = build_normExp_IR_tree(expc);
        }
        else if (!strcmp(opc->name, "MINUS"))
        {
            IR_tree *c1 = build_normExp_IR_tree(expc);
            char *tname = alloc_tmpvar();
            sprintf(ttmp, "%s := #0 - %s", tname, c1->stmt);
            IR_tree *c2 = new_IR_node(ttmp);
            addIRn(p, 2, c1, c2);
            add_IR_stmt(p, tname);
            free(tname);
            return p;
        }
        else if (!strcmp(opc->name, "NOT"))
        {
            IR_tree *c1 = build_normExp_IR_tree(expc);
            char *tname = alloc_tmpvar();
            sprintf(ttmp, "%s := #1 - %s", tname, c1->stmt);
            IR_tree *c2 = new_IR_node(ttmp);
            addIRn(p, 2, c1, c2);
            add_IR_stmt(p, tname);
            free(tname);
            return p;
        }
    }
    else if (u->child_cnt == 3)
    {
        treeNode *u1 = u->child;
        treeNode *u2 = u1->next;
        treeNode *u3 = u2->next;
        if ((!strcmp(u1->name, "Exp")) && (!strcmp(u3->name, "Exp")))
        {
            if (!strcmp(u2->name, "ASSIGN"))
            {
                IR_tree *c1 = build_ref_IR_tree(u1, false);
                IR_tree *c2 = build_normExp_IR_tree(u3);
                sprintf(ttmp, "%s := %s", c1->stmt, c2->stmt);
                IR_tree *c3 = new_IR_node(ttmp);
                addIRn(p, 3, c1, c2, c3);
                add_IR_stmt(p, c1->stmt);
                return p;
            }
            else if (!strcmp(u2->name, "AND"))
            {
                IR_tree *c1 = build_normExp_IR_tree(u1);
                IR_tree *c2 = build_normExp_IR_tree(u3);
                char *tname = alloc_tmpvar();
                sprintf(ttmp, "%s := %s & %s", tname, c1->stmt, c2->stmt);
                IR_tree *c3 = new_IR_node(ttmp);
                addIRn(p, 3, c1, c2, c3);
                add_IR_stmt(p, tname);
                free(tname);
                return p;
            }
            else if (!strcmp(u2->name, "OR"))
            {
                IR_tree *c1 = build_normExp_IR_tree(u1);
                IR_tree *c2 = build_normExp_IR_tree(u3);
                char *tname = alloc_tmpvar();
                sprintf(ttmp, "%s := %s | %s", tname, c1->stmt, c2->stmt);
                IR_tree *c3 = new_IR_node(ttmp);
                addIRn(p, 3, c1, c2, c3);
                add_IR_stmt(p, tname);
                free(tname);
                return p;
            }
            else if (!strcmp(u2->name, "PLUS"))
            {
                IR_tree *c1 = build_normExp_IR_tree(u1);
                IR_tree *c2 = build_normExp_IR_tree(u3);
                char *tname = alloc_tmpvar();
                sprintf(ttmp, "%s := %s + %s", tname, c1->stmt, c2->stmt);
                IR_tree *c3 = new_IR_node(ttmp);
                addIRn(p, 3, c1, c2, c3);
                add_IR_stmt(p, tname);
                free(tname);
                return p;
            }
            else if (!strcmp(u2->name, "MINUS"))
            {
                IR_tree *c1 = build_normExp_IR_tree(u1);
                IR_tree *c2 = build_normExp_IR_tree(u3);
                char *tname = alloc_tmpvar();
                sprintf(ttmp, "%s := %s - %s", tname, c1->stmt, c2->stmt);
                IR_tree *c3 = new_IR_node(ttmp);
                addIRn(p, 3, c1, c2, c3);
                add_IR_stmt(p, tname);
                free(tname);
                return p;
            }
            else if (!strcmp(u2->name, "MUL"))
            {
                IR_tree *c1 = build_normExp_IR_tree(u1);
                IR_tree *c2 = build_normExp_IR_tree(u3);
                char *tname = alloc_tmpvar();
                sprintf(ttmp, "%s := %s * %s", tname, c1->stmt, c2->stmt);
                IR_tree *c3 = new_IR_node(ttmp);
                addIRn(p, 3, c1, c2, c3);
                add_IR_stmt(p, tname);
                free(tname);
                return p;
            }
            else if (!strcmp(u2->name, "DIV"))
            {
                IR_tree *c1 = build_normExp_IR_tree(u1);
                IR_tree *c2 = build_normExp_IR_tree(u3);
                char *tname = alloc_tmpvar();
                sprintf(ttmp, "%s := %s / %s", tname, c1->stmt, c2->stmt);
                IR_tree *c3 = new_IR_node(ttmp);
                addIRn(p, 3, c1, c2, c3);
                add_IR_stmt(p, tname);
                free(tname);
                return p;
            }
        }
        else if ((!strcmp(u1->name, "LP")) && (!strcmp(u3->name, "RP"))) // LP Exp RP
        {
            return p = build_normExp_IR_tree(u2);
        }
        else if ((!strcmp(u1->name, "ID")) /*&& (!strcmp(u2->name, "LP")) && (!strcmp(u3->name, "RP"))*/) // ID LP RP
        {
            char *tname = alloc_tmpvar();
            if (strcmp(u1->val, "read") == 0)
            {
                sprintf(ttmp, "READ %s", tname);
            }
            else
            {
                sprintf(ttmp, "%s := CALL %s", tname, u1->val);
            }
            IR_tree *c1 = new_IR_node(ttmp);
            addIR1(p, 1, c1);
            add_IR_stmt(p, tname);
            free(tname);
            return p;
        }
        else // EXP DOT ID
        {
            return p = build_ref_IR_tree(u, false);
        }
    }
    else if (u->child_cnt == 4)
    {
        if (!strcmp(u->child->name, "ID")) // ID LP Args RP
        {
            if (strcmp(u->child->val, "write") == 0)
            {
                IR_tree *c1 = build_normExp_IR_tree(u->child->next->next->child);
                sprintf(ttmp, "WRITE %s", c1->stmt);
                IR_tree *c2 = new_IR_node(ttmp);
                addIRn(p, 2, c1, c2);
                add_IR_stmt(p, c1->stmt);
                return p;
            }
            else
            {
                treeNode *arg = u->child->next->next;
                IR_tree *c1 = build_args_IR_tree(arg);
                char *tname = alloc_tmpvar();
                sprintf(ttmp, "%s := CALL %s", tname, u->child->val);
                IR_tree *c2 = new_IR_node(ttmp);
                addIRn(p, 2, c1, c2);
                add_IR_stmt(p, tname);
                free(tname);
                return p;
            }
        }
        else // Exp LB Exp RB
        {
            return p = build_ref_IR_tree(u, false);
        }
    }
}

IR_tree *build_ifExp_IR_tree(const treeNode *u, const char *ltrue, const char *lfalse, const char *lend)
{
#ifdef DEBUG_OUTPUT
    printf("%s: %s\n", __FUNCTION__, u->name);
#endif
    // Called by `if` or `loop` stmt
    // u: Exp
    IR_tree *p;
    if (u->child_cnt == 2)
    {
        if (strcmp(u->child->name, "NOT") == 0)
        {
            return build_ifExp_IR_tree(u->child->next, lfalse, ltrue, lend);
        }
        else
        {
            goto default_if;
        }
    }
    else if (u->child_cnt == 3)
    {
        treeNode *u1 = u->child;
        treeNode *u2 = u1->next;
        treeNode *u3 = u2->next;
        if (strcmp(u1->name, "LP") == 0 && strcmp(u3->name, "RP") == 0)
        {
            return build_ifExp_IR_tree(u2, ltrue, lfalse, lend);
        }
        else if (strcmp(u2->name, "AND") == 0)
        {
            char *lptrue = alloc_label();
            IR_tree *c1 = build_ifExp_IR_tree(u1, lptrue, lfalse, lend);
            sprintf(ttmp, "LABEL %s :", lptrue);
            IR_tree *c2 = new_IR_node(ttmp);
            IR_tree *c3 = build_ifExp_IR_tree(u3, ltrue, lfalse, lend);
            addIRn(p, 3, c1, c2, c3);
            free(lptrue);
            return p;
        }
        else if (strcmp(u2->name, "OR") == 0)
        {
            char *lpfalse = alloc_label();
            IR_tree *c1 = build_ifExp_IR_tree(u1, ltrue, lpfalse, lend);
            sprintf(ttmp, "LABEL %s :", lpfalse);
            IR_tree *c2 = new_IR_node(ttmp);
            IR_tree *c3 = build_ifExp_IR_tree(u3, ltrue, lfalse, lend);
            addIRn(p, 3, c1, c2, c3);
            free(lpfalse);
            return p;
        }
        else if (strcmp(u2->name, "LT") == 0)
        {
            IR_tree *c1 = build_normExp_IR_tree(u1);
            IR_tree *c2 = build_normExp_IR_tree(u3);
            sprintf(ttmp, "IF %s < %s GOTO %s", c1->stmt, c2->stmt, ltrue);
            IR_tree *c3 = new_IR_node(ttmp);
            sprintf(ttmp, "GOTO %s", lfalse);
            IR_tree *c4 = new_IR_node(ttmp);
            addIRn(p, 4, c1, c2, c3, c4);
            return p;
        }
        else if (strcmp(u2->name, "LE") == 0)
        {
            IR_tree *c1 = build_normExp_IR_tree(u1);
            IR_tree *c2 = build_normExp_IR_tree(u3);
            sprintf(ttmp, "IF %s <= %s GOTO %s", c1->stmt, c2->stmt, ltrue);
            IR_tree *c3 = new_IR_node(ttmp);
            sprintf(ttmp, "GOTO %s", lfalse);
            IR_tree *c4 = new_IR_node(ttmp);
            addIRn(p, 4, c1, c2, c3, c4);
            return p;
        }
        else if (strcmp(u2->name, "GT") == 0)
        {
            IR_tree *c1 = build_normExp_IR_tree(u1);
            IR_tree *c2 = build_normExp_IR_tree(u3);
            sprintf(ttmp, "IF %s > %s GOTO %s", c1->stmt, c2->stmt, ltrue);
            IR_tree *c3 = new_IR_node(ttmp);
            sprintf(ttmp, "GOTO %s", lfalse);
            IR_tree *c4 = new_IR_node(ttmp);
            addIRn(p, 4, c1, c2, c3, c4);
            return p;
        }
        else if (strcmp(u2->name, "GE") == 0)
        {
            IR_tree *c1 = build_normExp_IR_tree(u1);
            IR_tree *c2 = build_normExp_IR_tree(u3);
            sprintf(ttmp, "IF %s >= %s GOTO %s", c1->stmt, c2->stmt, ltrue);
            IR_tree *c3 = new_IR_node(ttmp);
            sprintf(ttmp, "GOTO %s", lfalse);
            IR_tree *c4 = new_IR_node(ttmp);
            addIRn(p, 4, c1, c2, c3, c4);
            return p;
        }
        else if (strcmp(u2->name, "NE") == 0)
        {
            IR_tree *c1 = build_normExp_IR_tree(u1);
            IR_tree *c2 = build_normExp_IR_tree(u3);
            sprintf(ttmp, "IF %s != %s GOTO %s", c1->stmt, c2->stmt, ltrue);
            IR_tree *c3 = new_IR_node(ttmp);
            sprintf(ttmp, "GOTO %s", lfalse);
            IR_tree *c4 = new_IR_node(ttmp);
            addIRn(p, 4, c1, c2, c3, c4);
            return p;
        }
        else if (strcmp(u2->name, "EQ") == 0)
        {
            IR_tree *c1 = build_normExp_IR_tree(u1);
            IR_tree *c2 = build_normExp_IR_tree(u3);
            sprintf(ttmp, "IF %s == %s GOTO %s", c1->stmt, c2->stmt, ltrue);
            IR_tree *c3 = new_IR_node(ttmp);
            sprintf(ttmp, "GOTO %s", lfalse);
            IR_tree *c4 = new_IR_node(ttmp);
            addIRn(p, 4, c1, c2, c3, c4);
            return p;
        }
        else
        {
            goto default_if;
        }
    }
    else
    {
        goto default_if;
    }
default_if:;
    IR_tree *c1 = build_normExp_IR_tree(u);
    sprintf(ttmp, "IF %s == #0 GOTO %s", c1->stmt, lfalse);
    IR_tree *c2 = new_IR_node(ttmp);
    sprintf(ttmp, "GOTO %s", ltrue);
    IR_tree *c3 = new_IR_node(ttmp);
    addIRn(p, 3, c1, c2, c3);
    return p;
}

/// @brief
/// @param u Exp treenode, can be var, arr[x], struct.member
/// @param is_ptr should return a ptr(true) or variable(false)
/// @return IR treenode, pointer assign + offset, return->stmt = *t
IR_tree *build_ref_IR_tree(const treeNode *u, bool is_ptr)
{
#ifdef DEBUG_OUTPUT
    printf("%s: %s\n", __FUNCTION__, u->name);
#endif
    if (u->child_cnt == 1)
    {
        return build_normExp_IR_tree(u);
    }
    else if (u->child_cnt == 3)
    {
        IR_tree *p;
        IR_tree *c1 = build_ref_IR_tree(u->child, true);
        char *tname = alloc_tmpvar();
        sprintf(ttmp, "%s := %s + #%zu", tname, c1->stmt, get_offset_Struct(u->child->inheridata, u->child->next->next->val));
        IR_tree *c2 = new_IR_node(ttmp);
        addIRn(p, 2, c1, c2);
        if (!is_ptr)
        {
            sprintf(ttmp, "*%s", tname);
            add_IR_stmt(p, ttmp);
        }
        else
        {
            add_IR_stmt(p, tname);
        }
        free(tname);
        return p;
    }
    else
    {
        IR_tree *p;
        IR_tree *c1 = build_ref_IR_tree(u->child, true);
        IR_tree *c2 = build_normExp_IR_tree(u->child->next->next);
        char *ttva = alloc_tmpvar();
        sprintf(ttmp, "%s := #%zu * %s", ttva, get_array_size(u->child->inheridata), c2->stmt);
        IR_tree *c3 = new_IR_node(ttmp);
        char *tname = alloc_tmpvar();
        sprintf(ttmp, "%s := %s + %s", tname, c1->stmt, ttva);
        IR_tree *c4 = new_IR_node(ttmp);
        addIRn(p, 4, c1, c2, c3, c4);
        free(ttva);
        if (!is_ptr)
        {
            sprintf(ttmp, "*%s", tname);
            add_IR_stmt(p, ttmp);
        }
        else
        {
            add_IR_stmt(p, tname);
        }
        free(tname);
        return p;
    }
}

IR_tree *build_default_IR_tree(const treeNode *u)
{
    IR_tree *p = build_IR_tree(u->child);
    treeNode *un = u->child;
    IR_tree *pn = p;
    while (un != NULL && pn != NULL)
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
#ifdef DEBUG_OUTPUT
    printf("%s: %s\n", __FUNCTION__, u->name);
#endif
    IR_tree *p;
    if (u == NULL)
    {
        p = new_IR_node(NULL);
        p->should_print = false;
        return p;
    }
    if (strcmp(u->name, "ExtDef") == 0)
    {
        if (strcmp(u->child->name, "Specifier") == 0 && strcmp(u->child->next->name, "ExtDecList") == 0)
        {
            IR_tree *c1 = build_ExtDecList_tree(u->child->next);
            addIR1(p, 1, c1);
        }
        else if (strcmp(u->child->next->name, "FunDec") == 0)
        {
            IR_tree *c1 = build_FunDec_IR_tree(u);
            addIR1(p, 1, c1);
        }
        else
        {
            p = new_IR_node(NULL);
            p->should_print = false;
        }
#ifdef DEBUG_OUTPUT
        printf("leave: %s: %s\n", __FUNCTION__, u->name);
#endif
        return p;
    }
    else if (strcmp(u->name, "Def") == 0)
    {
        // Similar as above
        // Be aware of Dec: VarDec ASSIGN Exp
        return p = build_def_IR_tree(u);
    }
    else if (strcmp(u->name, "Program") == 0)
    {
        IR_push_stack();
        p = new_IR_node(NULL);
        p->child = build_default_IR_tree(u);
    }
    else if (strcmp(u->name, "") == 0)
    {
    }
    else
    {
        p = new_IR_node(NULL);
        p->should_print = false;
        p->child = NULL;
        if (u->child_cnt != 0)
        {
            p->child = build_default_IR_tree(u);
        }
    }
#ifdef DEBUG_OUTPUT
    printf("leave: %s: %s\n", __FUNCTION__, u->name);
#endif
    return p;
}

void output_IR_tree(const IR_tree *u, FILE *f)
{
    if (u->child == NULL)
    {
        if (u->stmt != NULL && u->should_print)
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