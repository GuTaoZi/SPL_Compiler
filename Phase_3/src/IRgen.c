#include "IRgen.h"
#include "type.h"

#include <assert.h>
#include <stdarg.h>
#include <string.h>

static treeNode *tntmp;

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
    add_IR_stmt(p, stmt);
    p->should_print = true;
    return p;
}

void add_IR_stmt(IR_tree *p, const char *stmt){
    if (stmt == NULL)
        p->stmt = NULL;
    else
    {
        size_t ll = strlen(stmt);
        p->stmt = (char *)malloc(sizeof(char) * (ll + 1));
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
char *alloc_tmpvar()
{
    char *tname = (char *)malloc(sizeof(char) * (2 + mlg10(tmp_val_cnt)));
    sprintf(tname, "t%zu", tmp_val_cnt);
    return tname;
}

size_t var_val_cnt = 0;
char *alloc_varval()
{
    char *vname = (char *)malloc(sizeof(char) * (2 + mlg10(var_val_cnt)));
    sprintf(vname, "v%zu", var_val_cnt);
    return vname;
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
        char *tmp_val = alloc_tmpvar();
        IR_tree *c1 = build_assign_IR_tree(tmp_val, u->child);
        IR_tree *c2 = build_arg_IR_tree(tmp_val);
        addIRn(p, 2, c1, c2);
        free(tmp_val);
        return p;
    }
    else
    {
        IR_tree *p;
        char *tmp_val = alloc_tmpvar();
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
    sprintf(ttmp, "FUNCTION %s :", (tntmp = u->child->next->child)->val);
    IR_tree *c1 = new_IR_node(ttmp);
    IR_tree *c2;
    if (u->child->next->child_cnt == 4)
        c2 = build_params_IR_tree(tntmp->next->next);
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
    IR_push_stack();
    IR_tree *p;
    IR_tree *c1 = build_defList_IR_tree(tntmp = u->child->next);
    IR_tree *c2 = build_stmtList_IR_tree(tntmp->next, lloop_head, lloop_end);
    IR_pop_stack();
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
        addIRn(p, 2, c1, c2);
        return p;
    }
    else if (strcmp(u->child->name, "IF") == 0)
    {
        if (u->child_cnt == 5) // No else
        {
            char *endlabel = alloc_label();
            IR_tree *p;
            IR_tree *c1 = build_ifExp_IR_tree(tntmp = u->child->next->next, NULL, endlabel, endlabel);
            IR_tree *c2 = build_stmt_IR_tree(tntmp = tntmp->next->next, lloop_head, lloop_end);
            sprintf(ttmp, "LABEL %s", endlabel);
            IR_tree *c3 = new_IR_node(ttmp);
            addIRn(p, 3, c1, c2, c3);
            free(endlabel);
        }
        else
        {
            char *falselabel = alloc_label();
            char *endlabel = alloc_label();
            IR_tree *p;
            IR_tree *c1 = build_ifExp_IR_tree(tntmp = u->child->next->next, NULL, falselabel, endlabel);

            IR_tree *c2 = build_stmt_IR_tree(tntmp = tntmp->next->next, lloop_head, lloop_end);

            sprintf(ttmp, "GOTO %s", endlabel);
            IR_tree *c3 = new_IR_node(ttmp);

            sprintf(ttmp, "LABEL %s", falselabel);
            IR_tree *c4 = new_IR_node(ttmp);

            IR_tree *c5 = build_stmt_IR_tree(tntmp = tntmp->next->next, lloop_head, lloop_end);

            sprintf(ttmp, "LABEL %s", endlabel);
            IR_tree *c6 = new_IR_node(ttmp);
            addIRn(p, 6, c1, c2, c3, c4, c5, c6);
            free(endlabel);
            free(falselabel);
        }
    }
    else if (strcmp(u->child->name, "WHILE") == 0)
    {
        char *nloop_head = alloc_label();
        char *nloop_tail = alloc_label();

        IR_tree *p;
        sprintf(ttmp, "LABEL %s", nloop_head);
        IR_tree *c1 = new_IR_node(ttmp);
        IR_tree *c2 = build_ifExp_IR_tree(tntmp = u->child->next->next, NULL, nloop_tail, nloop_tail);
        IR_tree *c3 = build_stmt_IR_tree(tntmp->next->next, nloop_head, nloop_tail);
        sprintf(ttmp, "GOTO %s", nloop_head);
        IR_tree *c4 = new_IR_node(ttmp);
        sprintf(ttmp, "LABEL %s", nloop_tail);
        IR_tree *c5 = new_IR_node(ttmp);
        addIRn(p, 5, c1, c2, c3, c4, c5);
    }
    else if (strcmp(u->child->name, "FOR") == 0)
    {
        IR_tree *p;
        char *nloop_head = alloc_label();
        char *nloop_tail = alloc_label();

        IR_tree *c1 = build_normExp_IR_tree(tntmp = u->child->next->next);

        sprintf(ttmp, "LABEL %s", nloop_head);
        IR_tree *c2 = new_IR_node(ttmp);

        IR_tree *c3 = build_ifExp_IR_tree(tntmp = tntmp->next->next, NULL, nloop_tail, nloop_tail);

        IR_tree *c5 = build_normExp_IR_tree(tntmp = tntmp->next->next);

        IR_tree *c4 = build_stmt_IR_tree(tntmp = tntmp->next->next, nloop_head, nloop_tail);

        sprintf(ttmp, "GOTO %s", nloop_head);
        IR_tree *c6 = new_IR_node(ttmp);
        sprintf(ttmp, "LABEL %s", nloop_tail);
        IR_tree *c7 = new_IR_node(ttmp);

        addIRn(p, 7, c1, c2, c3, c4, c5, c6, c7);
        return p;
    }
    else
    {
        // Raise an error
        return new_IR_node("error := #1");
    }
}

IR_tree *build_normExp_IR_tree(const treeNode *u)
{
    // Called by stmt
    // u: Exp
    IR_tree *p;
    if (u->child_cnt == 1)
    {
        treeNode *uc = u->child;
        if (!strcmp(uc->name, "Var"))
        {
            if (strcmp(uc->child->name, "ID"))
            {
                char *vname;
                IR_orthoNode *iron = IR_global_scope_seek(uc->child->name);
                if (!iron) // no allocated vname for this ID
                {
                    vname = alloc_varval();
                    IR_add_ortho_node(uc->child->name, vname);
                }
                else // previously allocated vname for this ID
                {
                    vname = iron->vname;
                }
                sprintf(ttmp, "%s", vname);
                p = new_IR_node(ttmp);
                p->should_print = false;
                return p;
            }
            else
            {
                sprintf(ttmp, "#%s", uc->child->val);
                p = new_IR_node(ttmp);
                p->should_print = false;
                return p;
            }
        }
        else if (!strcmp(uc->name, "STRING"))
        {
            sprintf(ttmp, "\"%s\"", uc->child->val);
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
                IR_tree *c2 = build_normExp_IR_tree(u2);
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
                IR_tree *c2 = build_normExp_IR_tree(u2);
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
                IR_tree *c2 = build_normExp_IR_tree(u2);
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
                IR_tree *c2 = build_normExp_IR_tree(u2);
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
                IR_tree *c2 = build_normExp_IR_tree(u2);
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
                IR_tree *c2 = build_normExp_IR_tree(u2);
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
            sprintf(ttmp, "%s := CALL %s", tname, u1->val);
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
        else // Exp LB Exp RB
        {
            return p = build_ref_IR_tree(u, false);
        }
    }
}

IR_tree *build_ifExp_IR_tree(const treeNode *u, const char *ltrue, const char *lfalse, const char *lend)
{
    // Called by `if` or `loop` stmt
    // u: Exp
}

/// @brief
/// @param u Exp treenode, can be var, arr[x], struct.member
/// @param is_ptr should return a ptr(true) or variable(false)
/// @return IR treenode, pointer assign + offset, return->stmt = *t
IR_tree *build_ref_IR_tree(const treeNode *u, bool is_ptr)
{
    if(u->child_cnt == 1){
        return build_normExp_IR_tree(u);
    } else if(u->child_cnt == 3){
        IR_tree *p;
        IR_tree *c1 = build_ref_IR_tree(p->child, true);
        sprintf(ttmp, "%s := %s + #%d", c1->stmt, c1->stmt, get_offset_Struct(u->child->inheridata, u->child->next->next->val));
        IR_tree *c2 = new_IR_node(ttmp);
        addIRn(p, 2, c1,c2);
        if(is_ptr){
            sprintf(ttmp, "*%s", c1->stmt);
            add_IR_stmt(p, ttmp);
        } else {
            add_IR_stmt(p, c1->stmt);
        }
        return p;
    } else {
        IR_tree *p;
        IR_tree *c1 = build_ref_IR_tree(p->child, true);
        IR_tree *c2 = build_normExp_IR_tree(p->child->next->next);
        char *ttva = alloc_tmpvar();
        sprintf(ttmp, "%s := %d * %s", ttva, get_array_size(u->child->inheridata), c2->stmt);
        IR_tree *c3 = new_IR_node(ttmp);
        sprintf(ttmp, "%s := %s + %s", c1->stmt, c1->stmt, ttva);
        IR_tree *c4 = new_IR_node(ttmp);
        addIRn(p, 4, c1,c2,c3,c4);
        free(ttva);
        if(is_ptr){
            sprintf(ttmp, "*%s", c1->stmt);
            add_IR_stmt(p, ttmp);
        } else {
            add_IR_stmt(p, c1->stmt);
        }
        return p;
    }
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
        if (strcmp(u->child->name, "Specifier") == 0 && strcmp(u->child->next->name, "ExtDecList") == 0 &&
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
        return p;
    }
    else if (strcmp(u->name, "Def") == 0)
    {
        // Similar as above
        // Be aware of Dec: VarDec ASSIGN Exp
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
        p->child = build_default_IR_tree(u);
    }
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