#include <stdio.h>

char s[32768];

void optimize(FILE *fin, FILE *fout){
    while(fscanf(fin, "%[^\n]", s) != EOF){
        fprintf(fout, "%s\n", s);
    }
}

int main(int argc, char **argv){
    if(argc < 3){
        fprintf(stderr, "useage: %s <in_file> <out_file>\n",argv[0]);
        exit(1);
    }
    FILE *fin = fopen(argv[1], "r");
    FILE *fout = fopen(argv[2], "w");
    if(fin == NULL || fout == NULL){
        fprintf(stderr, "Open file error\n");
        exit(0);
    }
    optimize(fin, fout);
    return 0;
}