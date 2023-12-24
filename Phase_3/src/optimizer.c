#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>

#include "GAS_utility.h"


size_t label_cnt, tmp_cnt, var_cnt;
FILE *debug;

char s[32768];
typedef struct IR_list
{
    char *ss[10];
    struct IR_list *next, *prev;
} IR_list;

IR_list *rootw = NULL;
char ss[10][32768];
int lss;

void output_list(const IR_list *u, FILE *fout)
{
    while (u != NULL)
    {
        for (int i = 0; u->ss[i] != NULL; ++i)
        {
            fprintf(fout, "%s ", u->ss[i]);
        }
        fprintf(fout, "\n");
        u = u->next;
    }
    fflush(fout);
}

void debug_IR_list(IR_list *ir, bool endl)
{
    for (int i = 0; ir->ss[i] != NULL; i++)
        fprintf(debug, "%s ", ir->ss[i]);
    fprintf(debug, "\n");
    if (endl)
        fputc('\n', debug);
    fflush(debug);
}

IR_list *new_IR_list(IR_list *prev)
{
    IR_list *p = (IR_list *)malloc(sizeof(IR_list));
    for (int i = 0; i < 10; i++)
    {
        size_t ll = strlen(ss[i]);
        if (ll == 0 || i>=lss)
        {
            p->ss[i] = NULL;
        }
        else
        {
            p->ss[i] = (char *)malloc(sizeof(char) * (ll + 1));
            strcpy(p->ss[i], ss[i]);
        }
    }
    p->next = NULL;
    p->prev = prev;
    if (prev != NULL)
    {
        prev->next = p;
    }
    return p;
}

void split_str(const char *s)
{
    int l1 = 0, l2 = 0, ls = 0;
    while (s[ls])
    {
        while (is_ws(s[ls]))
            ++ls;

        while (s[ls] && !is_ws(s[ls]))
        {
            ss[l1][l2] = s[ls];
            ++l2;
            ++ls;
        }
        ss[l1][l2] = 0;
        l2 = 0;
        ++l1;
    }
    lss=l1;
}

void del_list(IR_list *p)
{
fprintf(debug, "del: ");
debug_IR_list(p, false);
    if (p->prev != NULL)
    {
        p->prev->next = p->next;
    }
    if (p->next != NULL)
    {
        p->next->prev = p->prev;
    }
    for(int i=0;i<10;++i)
        if(p->ss[i] != NULL)
            free(p->ss[i]);
    free(p);
}

size_t get_list_len(IR_list *ir)
{
    for (int i = 0; ; i++)
        if (ir->ss[i] == NULL)
            return i;
}

bool opt_if(IR_list *u)
{
    bool flg = false;
    while (u != NULL)
    {
        if (strcmp(u->ss[0], "IF") == 0)
        {
            // if (u->next != NULL && strcmp(u->next->ss[0], "GOTO") == 0 &&
            //     u->next->next != NULL && strcmp(u->next->next->ss[0], "LABEL") == 0)
            // {
            //     if (strcmp(u->ss[5], u->next->next->ss[1]) == 0)
            //     {
            //         free(u->ss[5]);
            //         int ll = strlen(u->next->ss[1]);
            //         u->ss[5] = (char *)malloc(sizeof(char) * (ll + 1));
            //         strcpy(u->ss[5], u->next->ss[1]);
            //         u->ss[5][ll] = 0;

            //         del_list(u->next);

            //         if (u->ss[2][0] == '=')
            //         {
            //             u->ss[2][0] = '!';
            //         }
            //         else if (u->ss[2][0] == '!')
            //         {
            //             u->ss[2][0] = '=';
            //         }
            //         else if (u->ss[2][0] == '<')
            //         {
            //             free(u->ss[2]);
            //             if(u->ss[2][1] == '='){
            //                 u->ss[2] = (char*)malloc(sizeof(char)*2);
            //                 u->ss[2][0]='>';
            //                 u->ss[2][1]=0;
            //             } else {
            //                 u->ss[2] = (char*)malloc(sizeof(char)*3);
            //                 u->ss[2][0]='>';
            //                 u->ss[2][1]='=';
            //                 u->ss[2][2]=0;
            //             }
            //         }
            //         else if (u->ss[2][0] == '>')
            //         {
            //             free(u->ss[2]);
            //             if(u->ss[2][1] == '='){
            //                 u->ss[2] = (char*)malloc(sizeof(char)*2);
            //                 u->ss[2][0]='<';
            //                 u->ss[2][1]=0;
            //             } else {
            //                 u->ss[2] = (char*)malloc(sizeof(char)*3);
            //                 u->ss[2][0]='<';
            //                 u->ss[2][1]='=';
            //                 u->ss[2][2]=0;
            //             }
            //         }
            //         flg = 1;
            //     }
            // }
            if (u->next != NULL && strcmp(u->next->ss[0], "LABEL") == 0){
                if(strcmp(u->ss[5], u->next->ss[1]) == 0){
                    /* IF xxx GOTO Lu
                       LABEL: Lu*/
                    u=u->next;
                    del_list(u->prev);
                    flg = true;
                }
            }
        }
        if(strcmp(u->ss[0], "GOTO") == 0){
            if (u->next != NULL && strcmp(u->next->ss[0], "LABEL") == 0){
                if(strcmp(u->ss[1], u->next->ss[1]) == 0){
                    /* GOTO Lu
                       LABEL: Lu*/
                    u=u->next;
                    del_list(u->prev);
                    flg = true;
                }
            }
        }
        u = u->next;
    }
    return flg;
}


struct _Var;
typedef enum
{
    VAL,
    PTR,
    ADDR,
} Usage;
typedef struct _Parent
{
    Usage usage;
    struct _Var *var;
    IR_list *recent;
} Parent;

Usage get_usage(char *name)
{
    switch (name[0])
    {
        case '*':
            return PTR;
        case '&':
            return ADDR;
        default:
            return VAL;
    }
}

typedef struct _Var
{
    enum
    {
        VAR,
        CONST,
        FUNC,
    } type;
    IR_list *recent;
    int val;
    struct _Parent parent[2];
} Var;

#define MAX_VAR_CNT USHRT_MAX
Var v_var[MAX_VAR_CNT], t_var[MAX_VAR_CNT];
bool useful_v[MAX_VAR_CNT], useful_t[MAX_VAR_CNT];

Var *new_constant_var(const char *s)
{
    if (s[0] == '#')
        s = s + 1;
    Var *var = (Var *)malloc(sizeof(Var));
    var->type = CONST;
    var->val = atoi(s);
    var->parent[0] = (Parent){VAL, NULL, NULL};
    var->parent[1] = (Parent){VAL, NULL, NULL};
    return var;
}

bool not_tmp(Var *x)
{
    ll offset = ((ll)x - (ll)v_var) / sizeof(Var);
    return offset >= 0 && offset < MAX_VAR_CNT;
}

Var *get_var(const char *name)
{
    switch (name[0])
    {
        case 'v':
            return &v_var[atoi(name + 1)];
        case 't':
            return &t_var[atoi(name + 1)];
        case '#':
            return new_constant_var(name + 1);
        case '&':
        case '*':
            return get_var(name + 1);
    }
}

bool equal_parent(Parent a, Parent b)
{
    if (a.var == NULL && b.var == NULL)
        return true;
    if (!not_tmp(a.var) || !not_tmp(b.var) || (a.var == NULL) + (b.var == NULL) == 1)
        return false;
    return  a.usage == b.usage &&
        ((a.var == b.var && a.recent == b.recent) ||
        (a.var->type == CONST && b.var->type == CONST && a.var->val == b.var->val));
}

bool equal_var(Var *a, Var *b) // a and b can't both be & need to be consist of 2 elements
{
    return a == b || 
        (equal_parent(a->parent[0], b->parent[0]) && equal_parent(a->parent[1], b->parent[1])) ||
        (equal_parent(a->parent[0], b->parent[1]) && equal_parent(a->parent[1], b->parent[0]));
}

char *val_to_const(int val) // #val
{
    bool neg = val < 0;
    val = abs(val);
    size_t len = mlg10(abs(val)) + neg;
    char *s = (char *)malloc(sizeof(char) * (len + 2));
    for (size_t i = 0; i < len; i++)
    {
        s[i] = val % 10 + '0';
        val /= 10;
    }
    if (neg)
        s[len - 1] = '-';
    s[len] = '#';
    s[len + 1] = '\0';
    reverse_str(s, len + 1);
    return s;
}

int calc(int a, int b, char op)
{
    switch (op)
    {
        case '+':
            return a + b;
        case '-':
            return a - b;
        case '*':
            return a * b;
        case '/':
            return a / b;
    }
}

char *get_name(Var *var)
{
    if (!not_tmp(var) && var->type == CONST)
        return val_to_const(var->val);
    char type = 'v';
    ll idx = ((ll)var - (ll)v_var) / (ll)sizeof(Var);
    if (idx < 0 || idx >= MAX_VAR_CNT)
    {
        type = 't';
        idx = ((ll)var - (ll)t_var) / (ll)sizeof(Var);
    }
    size_t len = mlg10(idx);
    char *s = (char *)malloc(sizeof(char) * (len + 2));
    for (size_t i = 0; i < len; i++)
    {
        s[i] = idx % 10 + '0';
        idx /= 10;
    }
    s[len] = type;
    s[len + 1] = '\0';
    reverse_str(s, len + 1);
    return s;
}

char *prefixed_name(Usage usage, char *name)
{
    if (usage == VAL)
        return name;
    char *ret = (char *)malloc(sizeof(char) * (strlen(name) + usage));
    ret[0] = usage == PTR ? '*' : '&';
    ret[1] = '\0';
    strcat(ret, name);
    return ret;
}

bool single_parent(Var *var)
{ return var->parent[1].var == NULL; }

bool find_identity(IR_list *ir) // x is not const
{
    bool optimized = false;
    Var *x = get_var(ir->ss[0]);
    Usage usage = get_usage(ir->ss[0]);
    // (*)x := (*&)y
    if (get_list_len(ir) == 3)
    {
fprintf(debug, "len = 3\n");
        Var *y = get_var(ir->ss[2]);
        if (not_tmp(y) || ir->ss[2][0] == '&')
            return optimized;
        // (*)x := *y
fprintf(debug, "y is singel? %d\n", single_parent(y));
        if (ir->ss[2][0] == '*')
        {
            if (!single_parent(y) || y->parent[0].usage != VAL)
                return optimized;
            Parent py = y->parent[0];
            if (py.var == NULL || py.recent != py.var->recent)
                return optimized;
            free(ir->ss[2]);
            ir->ss[2] = prefixed_name(PTR, get_name(py.var));
            if (usage != PTR)
            {
                x->parent[0] = (Parent){PTR, py.var, py.recent};
            }
            optimized = true;
        }
        // (*)x := (&)y
        else
        {
            if (not_tmp(y))
                return optimized;
            if (single_parent(y))
            {
fprintf(debug, "single, (&)y\n");
                Parent py = y->parent[0];
                if (py.var == NULL || py.recent != py.var->recent)
                    return optimized;
                free(ir->ss[2]);
                ir->ss[2] = prefixed_name(py.usage, get_name(py.var));
                if (usage != PTR)
                    x->parent[0] = py;
                optimized = true;
            }
            else
            {
                if (usage == PTR)
                    return optimized;
                Parent py[2] = {y->parent[0], y->parent[1]};
                // x := *y
                if (x->parent[0].usage == PTR)
                    return optimized;
                // x := y
                free(ir->ss[2]);
                ir->ss[2] = prefixed_name(py[0].usage, get_name(py[0].var));
                ir->ss[4] = prefixed_name(py[1].usage, get_name(py[1].var));
                ir->ss[3] = (char *)malloc(sizeof(char) * 2);
                ir->ss[3][0] = y->recent->ss[3][0];
                ir->ss[3][1] = '\0';
                if (usage != PTR)
                {
                    x->parent[0] = py[0];
                    x->parent[1] = py[1];
                }
                optimized = true;
            }
        }
    }
    // (*)x := (*&)y ? (*&)z
    else
    {
fprintf(debug, "(*)x := (*&)y ? (*&)z\n");
        Var *y = get_var(ir->ss[2]), *z = get_var(ir->ss[4]);
        // optimize (*)y
        if (!not_tmp(y) && ir->ss[2][0] != '&' && single_parent(y))
        {
fprintf(debug, "y is single\n");
            // *y
            if (ir->ss[2][0] == '*')
            {
                if (y->parent[0].usage == VAL)
                {
                    Parent py = y->parent[0];
                    if (py.recent == py.var->recent)
                    {
                        free(ir->ss[2]);
                        ir->ss[2] = prefixed_name(PTR, get_name(py.var));
                        if (usage != PTR)
                            x->parent[0] = (Parent){PTR, py.var, py.recent};
                        optimized = true;
                    }
                }
            }
            // y
            else
            {
fprintf(debug, "y is not a PTR\n");
                if (y->type == CONST)
                {
                    free(ir->ss[2]);
                    ir->ss[2] = val_to_const(y->val);
                    if (usage != PTR)
                        x->parent[0] = (Parent){VAL, new_constant_var(ir->ss[2]), NULL};
                    optimized = true;
                }
                else
                {
                    Parent py = y->parent[0];
fprintf(debug, "y is not a CONST\n");
                    if (py.recent == py.var->recent)
                    {
fprintf(debug, "py: %d %s\n", py.usage, get_name(py.var));
                        free(ir->ss[2]);
                        ir->ss[2] = prefixed_name(py.usage, get_name(py.var));
                        if (usage != PTR)
                            x->parent[0] = py;
                        optimized = true;
                    }
                }
            }
        }
        // optimize z
        if (!not_tmp(z) && ir->ss[4][0] != '&' && single_parent(z))
        {
            // *z
            if (ir->ss[4][0] == '*')
            {
                if (z->parent[0].usage == VAL)
                {
                    Parent pz = z->parent[0];
                    if (pz.recent == pz.var->recent)
                    {
                        free(ir->ss[4]);
                        ir->ss[4] = prefixed_name(PTR, get_name(pz.var));
                        if (usage != PTR)
                            x->parent[1] = (Parent){PTR, pz.var, pz.recent};
                        optimized = true;
                    }
                }
            }
            // z
            else
            {
                if (z->type == CONST)
                {
                    free(ir->ss[4]);
                    ir->ss[4] = val_to_const(z->val);
                    if (usage != PTR)
                        x->parent[1] = (Parent){VAL, new_constant_var(ir->ss[4]), NULL};
                    optimized = true;
                }
                else
                {
                    Parent pz = z->parent[0];
                    if (pz.recent == pz.var->recent)
                    {
                        free(ir->ss[4]);
                        ir->ss[4] = prefixed_name(pz.usage, get_name(pz.var));
                        if (usage != PTR)
                            x->parent[1] = pz;
                        optimized = true;
                    }
                }
            }
        }
        // common subexpression
        // for (size_t i = 0; i < tmp_cnt; i++)
        // {
        //     y = &t_var[i];
        //     if (y != x && equal_var(x, y))
        //         goto CHEKCOUT;
        // }
        // for (size_t i = 0; i < var_cnt; i++)
        // {
        //     y = &v_var[i];
        //     if (y != x && equal_var(x, y))
        //         goto CHEKCOUT;
        // }
        return optimized;
    CHEKCOUT:
        for (size_t i = 2; i <= 4; i++)
        {
            free(ir->ss[i]);
            ir->ss[i] = NULL;
        }
        ir->ss[2] = get_name(y);
        if (ir->ss[0][0] != '*')
        {
            x->parent[0] = (Parent){get_usage(ir->ss[2]), y, y->recent};
            x->parent[1] = (Parent){VAL, NULL, NULL};
        }
        optimized = true;
    }
    return optimized;
}

bool simplify_assign(IR_list *ir)
{
    bool optimized = false;
    Var *x = get_var(ir->ss[0]), *y, *z;
    
    Usage usage = get_usage(ir->ss[0]);
    if (usage != PTR)
    {
        x->recent = ir;
        x->type = VAR;
    }
fprintf(debug, "in sa(): %d\n", get_list_len(ir));
debug_IR_list(ir, false);
    switch (get_list_len(ir))
    {
        // (*)x := (*&)y
        case 3:
            // if (ir->ss[2][0] == '&' || ir->ss[2][0] == '*')
            //     break;
            y = get_var(ir->ss[2]);
            if (usage != PTR)
            {
                x->type = y->type;
                x->val = y->val;
                x->parent[0] = (Parent){get_usage(ir->ss[2]), y, y->recent};
// fprintf(debug, "in sa(): case 3: %d %s", get_usage(ir->ss[2]), ir->ss[2]);
                x->parent[1] = (Parent){VAL, NULL, NULL};
            }
            if (!not_tmp(y) && y->type == CONST)
            {
                ir->ss[2] = val_to_const(y->val);
                x->parent[0] = (Parent){VAL, new_constant_var(ir->ss[2]), NULL};
                optimized = true;
            }
            break;
        // x := (*&)y ? (*&)z
        case 5:
            y = get_var(ir->ss[2]), z = get_var(ir->ss[4]);
            char op = ir->ss[3][0];
            x->parent[0] = (Parent){get_usage(ir->ss[2]), y, y->recent};
            x->parent[1] = (Parent){get_usage(ir->ss[4]), z, z->recent};
fprintf(debug, "in sa(): case 5: %d %s\n", x->parent[0].usage, ir->ss[2]);
            // #? 
            if (!not_tmp(y) && !not_tmp(z) && y->type == CONST && z->type == CONST)
            {
                for (size_t i = 2; i <= 4; i++)
                {
                    free(ir->ss[i]);
                    ir->ss[i] = NULL;
                }
                int val = calc(y->val, z->val, op);
                ir->ss[2] = val_to_const(val);
                x->type = CONST;
                x->val = val;
                x->parent[0] = (Parent){VAL, get_var(ir->ss[2]), NULL};
                x->parent[1] = (Parent){VAL, NULL, NULL};
                optimized = true;
            }
            // #0
            else if ((op == '*' && ((!not_tmp(y) && y->type == CONST && y->val == 0) || (!not_tmp(z) && z->type == CONST && z->val == 0))) ||
                     (op == '-' && ((!not_tmp(y) && !not_tmp(z) && equal_var(y, z)) || (y == z))) ||
                     (op == '/' && !not_tmp(y) && y->type == CONST && y->val == 0))
            {
                x->type = CONST;
                x->val = 0;
                x->parent[0] = (Parent){VAL, new_constant_var("#0"), NULL};
                x->parent[1] = (Parent){VAL, NULL, NULL};
                for (size_t i = 2; i <= 4; i++)
                {
                    free(ir->ss[i]);
                    ir->ss[i] = NULL;
                }
                ir->ss[2] = val_to_const(0);
                optimized = true;
            }
            // ? + 0, ? - 0
            else if (!not_tmp(z) &&
                ((op == '+' && z->type == CONST && z->val == 0) ||
                (op == '-' && z->type == CONST && z->val == 0)))
            {
fprintf(debug, "+- 0\n");
                free(ir->ss[3]);
                free(ir->ss[4]);
                ir->ss[3] = ir->ss[4] = NULL;
                x->parent[1] = (Parent){VAL, NULL, NULL};
                optimized = true;
            }
            // #0 + ?
            else if (!not_tmp(y) && op == '+' && z->type == CONST && z->val == 0)
            {
                free(ir->ss[2]);
                free(ir->ss[3]);
                ir->ss[2] = ir->ss[4];
                ir->ss[3] = ir->ss[4] = NULL;
                x->parent[0] = x->parent[1];
                x->parent[1] = (Parent){VAL, NULL, NULL};
                optimized = true;
            }
            break;
    }
debug_IR_list(ir, false);
    if (x->type != CONST)
        optimized |= find_identity(ir);
debug_IR_list(ir, true);
    return optimized;
}

bool *get_useful(char *name)
{
    bool sp = name[0] == '*' || name[0] == '&';
    size_t idx = atoi(name + 1 + sp);
    if (name[sp] == 'v')
        return &useful_v[idx];
    else
        return &useful_t[idx];
}

void set_useful(char *name, bool useful)
{
    if (name[0] == '#')
        return;
    *(get_useful(name)) = useful;
}

bool opt_exp(IR_list *u)
{
    bool optimized = false;
    IR_list *ir = u, *tail;

    // pos: simplify const ops
    memset(v_var, 0, sizeof(v_var));
    memset(t_var, 0, sizeof(t_var));
    for (size_t i = 0; i < var_cnt; i++)
    {
        v_var[i].parent[0] = (Parent){VAL, new_constant_var("#0"), NULL};
        v_var[i].parent[1] = (Parent){VAL, NULL, NULL};
    }
    for (size_t i = 0; i < tmp_cnt; i++)
    {
        t_var[i].parent[0] = (Parent){VAL, new_constant_var("#0"), NULL};
        t_var[i].parent[1] = (Parent){VAL, NULL, NULL};
    }
    
    while (ir != NULL)
    {
        if (strcmp(ir->ss[1], ":=") == 0)
        {
            if (strcmp(ir->ss[2], "CALL") == 0)
            {
                Var *x = get_var(ir->ss[0]);
                if (get_usage(ir->ss[0]) != PTR)
                {
                    x->type = VAR;
                    x->recent = ir;
                    x->parent[0] = (Parent){VAL, x, ir};
                }
            }
            else
                optimized |= simplify_assign(ir);
        }
        else if (strcmp(ir->ss[0], "READ") == 0)
        {
            Var *x = get_var(ir->ss[1]);
            x->parent[0] = (Parent){VAL, x, ir};
        }

        if (ir->next == NULL)
            tail = ir;
        ir = ir->next;
    }


    // neg: remove useless vars
    memset(useful_v, false, sizeof(useful_v));
    memset(useful_t, false, sizeof(useful_v));
    ir = tail;
    while (ir != NULL)
    {
        IR_list *next = ir->prev;
        if (strcmp(ir->ss[0], "LABEL") == 0);
        else if (strcmp(ir->ss[0], "FUNCTION") == 0);
        else if (strcmp(ir->ss[0], "GOTO") == 0);
        else if (strcmp(ir->ss[0], "IF") == 0)
        {
            set_useful(ir->ss[1], true);
            set_useful(ir->ss[3], true);
        }
        else if (strcmp(ir->ss[0], "RETURN") == 0)
            set_useful(ir->ss[1], true);
        else if (strcmp(ir->ss[0], "DEC") == 0)
        {
            // if (!*(get_useful(ir->ss[2])))
            //     goto DEL_IR; 
        }
        else if (strcmp(ir->ss[0], "PARAM") == 0);
        else if (strcmp(ir->ss[0], "ARG") == 0)
            set_useful(ir->ss[1], true);
        else if (strcmp(ir->ss[0], "READ") == 0);
        else if (strcmp(ir->ss[0], "WRITE") == 0)
            set_useful(ir->ss[1], true);
        else // x := ?
        {
            Var *x = get_var(ir->ss[0]);
            size_t len = get_list_len(ir);
            bool useful = not_tmp(x) || *get_useful(ir->ss[0]);
            Usage usage = get_usage(ir->ss[0]);
            if (!useful && len != 4 && usage != PTR)
                goto DEL_IR;
            if (useful || usage == PTR)
                switch (len)
                {
                    case 3: // (*)x := (*&)y
                        set_useful(ir->ss[0], usage == PTR);
                        set_useful(ir->ss[2], true);
                        break;
                    case 5: // (*)x := (*&)y + (*&)z
                        set_useful(ir->ss[0], usage == PTR);
                        set_useful(ir->ss[2], true);
                        set_useful(ir->ss[4], true);
                        break;
                }
        }
        ir = next;
        continue;
    DEL_IR:
        del_list(ir);
        ir = next;
        optimized = true;
    }
    return optimized;
}

int label_trans[65536];
int getlf(int u){
    return (label_trans[u] == u) ? u : (label_trans[u] = getlf(label_trans[u]));
}
void mergel(int x, int y){
    label_trans[getlf(x)] = getlf(y);
}

char opt_label_tmp[256];
bool opt_label(IR_list *rootw){
    for(int i=0;i<65536;++i) label_trans[i] = i;
    IR_list *p = rootw;
    char flg = 0;
    while(p != NULL){
        if(p->next != NULL){
            if(strcmp(p->ss[0], "LABEL") == 0 && strcmp(p->next->ss[0], "GOTO") == 0){
                int l1 = atoi(p->ss[1]+1);
                int l2 = atoi(p->next->ss[1]+1);
                mergel(l1, l2);
                p=p->next;
                del_list(p->prev);
                flg = 1;
            } else if(strcmp(p->ss[0], "LABEL") == 0 && strcmp(p->next->ss[0], "LABEL") == 0){
                int l1 = atoi(p->ss[1]+1);
                int l2 = atoi(p->next->ss[1]+1);
                mergel(l1, l2);
                p=p->next;
                del_list(p->prev);
                flg = 1;
            }
        }
        p=p->next;
    }
    p = rootw;
    while(p != NULL){
        if(strcmp(p->ss[0], "IF") == 0){
            int lll = atoi(p->ss[5]+1);
            free(p->ss[5]);
            p->ss[5] = (char*)malloc(mlg10(label_trans[lll])+2);
            sprintf(p->ss[5], "l%d", label_trans[lll]);
        } else if(strcmp(p->ss[0], "LABEL") == 0){
            int lll = atoi(p->ss[1]+1);
            free(p->ss[1]);
            p->ss[1] = (char*)malloc(mlg10(label_trans[lll])+2);
            sprintf(p->ss[1], "l%d", label_trans[lll]);
        } else if(strcmp(p->ss[0], "GOTO") == 0){
            int lll = atoi(p->ss[1]+1);
            free(p->ss[1]);
            p->ss[1] = (char*)malloc(mlg10(label_trans[lll])+2);
            sprintf(p->ss[1], "l%d", label_trans[lll]);
        }
        p=p->next;
    }
    return flg;
}

bool opt_read_func(IR_list *root)
{
    // t := ...
    // x := t 
    IR_list *ir = root;
    bool optimized = false;
    while (ir != NULL)
    {
        if (strcmp(ir->ss[0], "READ") == 0)
        {
            IR_list *next = ir->next;
            if (strcmp(next->ss[1], ":=") == 0 && next->ss[0][0] != '*' && strcmp(ir->ss[1], next->ss[2]) == 0 && next->ss[3] == NULL)
            {
                free(ir->ss[1]);
                ir->ss[1] = (char *)malloc(sizeof(char) * strlen(next->ss[0] + 1));
                strcpy(ir->ss[1], next->ss[0]);
                del_list(ir->next);
                optimized = true;
            }
        }
        else if (strcmp(ir->ss[1], ":=") == 0 && strcmp(ir->ss[2], "CALL") == 0)
        {
            IR_list *next = ir->next;
            if (strcmp(next->ss[1], ":=") == 0 && strcmp(ir->ss[0], next->ss[2]) == 0 && next->ss[3] == NULL)
            {
                free(ir->ss[0]);
                ir->ss[0] = (char *)malloc(sizeof(char) * strlen(next->ss[0] + 1));
                strcpy(ir->ss[0], next->ss[0]);
                del_list(ir->next);
                optimized = true;
            }
        }
        ir = ir->next;
    }
    return optimized;
}

void optimize(FILE *fin, FILE *fout)
{
    IR_list *nowp;
    while (fscanf(fin, "%[^\n]", s) != EOF)
    {
        fscanf(fin, "%*c");
        split_str(s); // similar to ss = s.split(whitespace), ss : list
        if (rootw == NULL)
        {
            rootw = new_IR_list(NULL);
            nowp = rootw;
        }
        else
        {
            nowp = new_IR_list(nowp);
        }
    }

    while (1)
    {
        opt_exp(rootw);
        if (opt_if(rootw))
            continue;
        if (opt_label(rootw))
            continue;
        if (opt_read_func(rootw))
            continue;
        break;
    }

    output_list(rootw, fout);
}

int main(int argc, char **argv)
{
    if (argc < 6)
    {
        fprintf(stderr, "useage: %s <in_file> <out_file> <label_cnt> <tmp_cnt> <var_cnt>\n", argv[0]);
        exit(1);
    }
    FILE *fin = fopen(argv[1], "r");
    FILE *fout = fopen(argv[2], "w");
    // fprintf(debug, "%s %s %s %s %s\n", argv[1], argv[2], argv[3], argv[4], argv[5]);
debug = fopen("./test-tmp/debug_info.txt", "w");
    if (fin == NULL || fout == NULL)
    {
        fprintf(stderr, "Open file error\n");
        exit(0);
    }
    label_cnt = atoi(argv[3]);
    tmp_cnt = atoi(argv[4]);
    var_cnt = atoi(argv[5]);
    optimize(fin, fout);
    return 0;
}