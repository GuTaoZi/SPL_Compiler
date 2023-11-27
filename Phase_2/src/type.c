#include "type.h"

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
        nowType->typesize = 4;
    } else if(strcmp(u, "float") == 0){
        nowType->primitive = PFLOAT;
        nowType->typesize = 4;
    } else if(strcmp(u, "char") == 0){
        nowType->primitive = PCHAR;
        nowType->typesize = 1;
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
    nowFieldList->typesize = nowType->typesize;
    return nowFieldList;
}

Type *makeArrayType(Type *b, size_t sz){
    Array *p = (Array*)malloc(sizeof(Array));
    p->base = b;
    p->typesize = b->typesize * sz;
    p->size = sz;

    Type *p2 = (Type*)malloc(sizeof(Type));
    p2->typesize = p->typesize;
    p2->category = ARRAY;
    p2->array = p;
    return p2;
}

Type *addArrayTypeRec(Type *nowType, size_t sz){
    Type *nxt;
    if(nowType->category == ARRAY) {
        nxt = addArrayTypeRec(nowType->array->base, sz);
    } else {
        nxt = makeArrayType(nowType, sz);
    }
    nowType->array->base = nxt;
    nowType->array->typesize = nxt->typesize*nowType->array->size;
    nowType->typesize = nowType->array->typesize;
    return nowType;
}

Type *addArrayType(Type *nowType, size_t sz){
    if(nowType->category != ARRAY){
        return makeArrayType(nowType, sz);
    } else {
        addArrayTypeRec(nowType, sz);
        return nowType;
    }
}

FieldList *addFieldList(FieldList *fl, Type *nowType, const char *name){
    FieldList *nowFL = makeFieldList(nowType, name);
    nowFL->next = fl;
    nowFL->typesize += fl->typesize;
    return nowFL;
}

void addStructField(Type *nowType, FieldList *fl){
    nowType->structure->data = fl;
    nowType->typesize = fl->typesize;
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