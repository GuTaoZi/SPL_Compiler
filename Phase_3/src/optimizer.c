#include <stdio.h>
#include <string.h>

char s[32768];
char ss[10][32768];

inline char is_ws(const char c)
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
}

void optimize(FILE *fin, FILE *fout)
{
    while (fscanf(fin, "%[^\n]", s) != EOF)
    {
        split_str(s);
        // similar to ss = s.split(whitespace), ss : list
        if (strncmp(s, "LABEL", 5) == 0)
        {
            // LABEL
        }
        else if (strncmp(s, "GOTO", 4) == 0)
        {
            // GOTO
        }
        else if (strncmp(s, "IF", 2) == 0)
        {
            // IF
        }
        else if (strncmp(s, "RETURN", 6) == 0)
        {
            // RETURN
        }
        else if (strncmp(s, "DEC", 3) == 0)
        {
            // DEC
        }
        else if (strncmp(s, "PARAM", 5) == 0)
        {
            // PARAM
        }
        else if (strncmp(s, "ARG", 3) == 0)
        {
            // ARG
        }
        else if (strncmp(s, "READ", 4) == 0)
        {
            // READ
        }
        else if (strncmp(s, "WRITE", 5) == 0)
        {
            // WRITE
        }
        else
        {
            // :=
        }
    }
}

int main(int argc, char **argv)
{
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