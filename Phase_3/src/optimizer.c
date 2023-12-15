#include "optimizer.h"

char s[32768];

void optimize(FILE *fin, FILE *fout){
    while(fscanf(fin, "%[^\n]", s) != EOF){
        fprintf(fout, "%s\n", s);
    }
}