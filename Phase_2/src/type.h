#ifndef TYPE_H
#define TYPE_H

#include "treeNode.h"

typedef struct Type
{
    enum
    {
        PRIMITIVE,
        ARRAY,
        STRUCTURE,
        FUNCTION,
        ERROR
    } category;
    union
    {
        enum
        {
            PINT,
            PFLOAT,
            PCHAR
        } primitive;
        struct Array *array;
        struct Structure *structure;
        struct Function *func;
    };
} Type;

typedef struct Array
{
    struct Type *base;
    int size;
} Array;

typedef struct FieldList
{
    struct Type *type; char varname[32];
    struct FieldList *next;
} FieldList;

typedef struct Structure {
    char struct_name[32];
    struct FieldList *data;
} Structure;

typedef struct Function
{
    char name[32];
    struct FieldList *params;
} Function;

extern Type *nowType;
extern FieldList *nowFieldList;

void makeStructType(){
    nowType = (Type*)malloc(sizeof(Type));
    nowType->category = STRUCTURE;
    nowType->structure = (Structure*)malloc(sizeof(Structure));
}

void makePrimType(const treeNode *u){
    nowType = (Type*)malloc(sizeof(Type));
    nowType->category = PRIMITIVE;
    if(strcmp(u->val, "int") == 0){
        nowType->primitive = PINT;
    } else if(strcmp(u->val, "float") == 0){
        nowType->primitive = PFLOAT;
    } else if(strcmp(u->val, "char") == 0){
        nowType->primitive = PCHAR;
    }
}

void addStructName(const treeNode *u){
    memset(nowType->structure->struct_name, 0, sizeof(nowType->structure->struct_name));
    strncpy(nowType->structure->struct_name, u->val, 31);
}

void makeFiledList(){
    nowFieldList = (FieldList*)malloc(sizeof(FieldList));
    nowFieldList->next = NULL;
}

void addStructField(){
    nowType->structure->data = nowFieldList;
    nowFieldList = NULL;
}

void makeArrayType(const treeNode *u){

}

#endif