#include "type.h"

#include <stddef.h>

Type *makeStructType(){
    Type *nowType = (Type*)malloc(sizeof(Type));
    nowType->category = STRUCTURE;
    nowType->structure = (Structure*)malloc(sizeof(Structure));
    nowType->structure->data = NULL;
    memset(nowType->structure->data, 0, sizeof(nowType->structure->data));
    return nowType;
}

Type *makePrimType(const char *u){
    Type *nowType = (Type*)malloc(sizeof(Type));
    nowType->category = PRIMITIVE;
    if(strcmp(u, "int") == 0){
        nowType->primitive = PINT;
    } else if(strcmp(u, "float") == 0){
        nowType->primitive = PFLOAT;
    } else if(strcmp(u, "char") == 0){
        nowType->primitive = PCHAR;
    }
    return nowType;
}

void addStructName(Type *nowType, const char *u){
    memset(nowType->structure->struct_name, 0, sizeof(nowType->structure->struct_name));
    strncpy(nowType->structure->struct_name, u, 31);
}

FieldList *makeFieldList(Type *nowType, const char *name){
    FieldList *nowFieldList = (FieldList*)malloc(sizeof(FieldList));
    nowFieldList->next = NULL;
    nowFieldList->type = nowType;
    memset(nowFieldList->varname, 0, sizeof(nowFieldList->varname));
    strncpy(nowFieldList->varname, name, 31);
    return nowFieldList;
}

FieldList *addFieldList(FieldList *fl, Type *nowType, const char *name){
    FieldList *nowFL = makeFieldList(nowType, name);
    nowFL->next = fl;
    return nowFL;
}

void addStructField(Type *nowType, FieldList *fl){
    nowType->structure->data = fl;
}

void deleteArray(Array *arr){
    if(arr == NULL) return;
    deleteType(arr->base);
    free(arr);
}

void deleteFieldList(FieldList *fl){
    if(fl == NULL) return;
    deleteType(fl->type);
    deleteFieldList(fl->next);
    free(fl);
}

void deleteStructure(Structure *stru){
    if(stru == NULL) return;
    deleteFieldList(stru->data);
}

void deleteFunction(Function *func){
    if(func == NULL) return;
    deleteFieldList(func->params);
}

void deleteType(Type *type){
    if(type == NULL) return;
    if(type->category == ARRAY){
        deleteArray(type->array);
    } else if(type->category == STRUCTURE){
        deleteStructure(type->structure);
    } else if(type->category == FUNCTION){
        deleteFunction(type->func);
    }
    free(type);
}