#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char s[32768];
typedef struct IR_list
{
    char *ss[10];
    struct IR_list *next, *prev;
} IR_list;

IR_list root;
char ss[10][32768];
int lss;
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

inline static char is_ws(const char c)
{
    return c == ' ' || c == '\n' || c == '\r' || c == '\t';
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
    if (p->prev != NULL)
    {
        p->prev->next = p->next;
    }
    if (p->next != NULL)
    {
        p->next->prev = p->prev;
    }
    free(p);
}

char opt_if(IR_list *u)
{
    char flg = 0;
    while (u != NULL)
    {
        if (strcmp(u->ss[0], "IF") == 0)
        {
            if (u->next != NULL && strcmp(u->next->ss[0], "GOTO") == 0 &&
                u->next->next != NULL && strcmp(u->next->next->ss[0], "LABEL") == 0)
            {
                if (strcmp(u->ss[5], u->next->next->ss[1]) == 0)
                {
                    free(u->ss[5]);
                    int ll = strlen(u->next->ss[1]);
                    u->ss[5] = (char *)malloc(sizeof(char) * (ll + 1));
                    strcpy(u->ss[5], u->next->ss[1]);
                    u->ss[5][ll] = 0;

                    del_list(u->next);

                    if (u->ss[2][0] == '=')
                    {
                        u->ss[2][0] = '!';
                    }
                    else if (u->ss[2][0] == '<')
                    {
                        u->ss[2][0] = '>';
                    }
                    else if (u->ss[2][0] == '>')
                    {
                        u->ss[2][0] = '<';
                    }
                    else if (u->ss[2][0] == '!')
                    {
                        u->ss[2][0] = '=';
                    }
                    flg = 1;
                }
            }
        }
        u = u->next;
    }
    return flg;
}

char opt_exp(IR_list *u)
{
    return 0;
}

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
}

void optimize(FILE *fin, FILE *fout)
{
    IR_list *root = NULL, *nowp;
    while (fscanf(fin, "%[^\n]", s) != EOF)
    {
        fscanf(fin, "%*c");
        split_str(s); // similar to ss = s.split(whitespace), ss : list
        if (root == NULL)
        {
            root = new_IR_list(NULL);
            nowp = root;
        }
        else
        {
            nowp = new_IR_list(nowp);
        }
    }

    while (1)
    {
        if (opt_if(root))
        {
            continue;
        }
        else if (opt_exp(root))
        {
            continue;
        }
        break;
    }

    output_list(root, fout);

    // if (strncmp(s, "LABEL", 5) == 0)
    // {
    //     // LABEL
    // }
    // else if (strncmp(s, "GOTO", 4) == 0)
    // {
    //     // GOTO
    // }
    // else if (strncmp(s, "IF", 2) == 0)
    // {
    //     // IF
    // }
    // else if (strncmp(s, "RETURN", 6) == 0)
    // {
    //     // RETURN
    // }
    // else if (strncmp(s, "DEC", 3) == 0)
    // {
    //     // DEC
    // }
    // else if (strncmp(s, "PARAM", 5) == 0)
    // {
    //     // PARAM
    // }
    // else if (strncmp(s, "ARG", 3) == 0)
    // {
    //     // ARG
    // }
    // else if (strncmp(s, "READ", 4) == 0)
    // {
    //     // READ
    // }
    // else if (strncmp(s, "WRITE", 5) == 0)
    // {
    //     // WRITE
    // }
    // else
    // {
    //     // :=
    // }
}

int main(int argc, char **argv)
{
    printf("START OPTMIZER\n");
    if (argc < 3)
    {
        fprintf(stderr, "useage: %s <in_file> <out_file>\n", argv[0]);
        exit(1);
    }
    FILE *fin = fopen(argv[1], "r");
    FILE *fout = fopen(argv[2], "w");
    if (fin == NULL || fout == NULL)
    {
        fprintf(stderr, "Open file error\n");
        exit(0);
    }
    optimize(fin, fout);
    return 0;
}