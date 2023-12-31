#include "type.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #define IMPLICIT_CONVERT

Type *makeStructType(const char *name, FieldList *fl)
{
    Type *nowType = (Type *)malloc(sizeof(Type));
    nowType->category = STRUCTURE;
    nowType->structure = (Structure *)malloc(sizeof(Structure));
    nowType->structure->data = fl;
    memset(nowType->structure->struct_name, 0, sizeof(nowType->structure->struct_name));
    strncpy(nowType->structure->struct_name, name, 31);
    nowType->structure->typesize = fl->typesize;
    // printf("Struct_size=%zu, %p\n", nowType->structure->typesize, fl);
    return nowType;
}

Type *makePrimType(const char *u)
{
    Type *nowType = (Type *)malloc(sizeof(Type));
    nowType->category = PRIMITIVE;
    if (strcmp(u, "int") == 0)
    {
        nowType->primitive = PINT;
        nowType->typesize = 4;
    }
    else if (strcmp(u, "float") == 0)
    {
        nowType->primitive = PFLOAT;
        nowType->typesize = 4;
    }
    else if (strcmp(u, "char") == 0)
    {
        nowType->primitive = PCHAR;
        nowType->typesize = 1;
    }
    return nowType;
}

FieldList *makeFieldList(Type *nowType, const char *name)
{
    // printf("FL: %p\n", nowType);
    // if(nowType != NULL)
    // printf("%d\n", nowType->category);
    FieldList *nowFieldList = (FieldList *)malloc(sizeof(FieldList));
    nowFieldList->next = NULL;
    nowFieldList->type = nowType;
    memset(nowFieldList->varname, 0, sizeof(nowFieldList->varname));
    strncpy(nowFieldList->varname, name, 31);
    if (nowType != NULL)
        nowFieldList->typesize = nowType->typesize;
    else
        nowFieldList->typesize = 0;
    // printf("FL_size=%zu\n", nowFieldList->typesize);
    return nowFieldList;
}

Type *makeArrayType(Type *b, size_t sz)
{
    Array *p = (Array *)malloc(sizeof(Array));
    p->base = b;
    p->typesize = b->typesize * sz;
    p->size = sz;

    Type *p2 = (Type *)malloc(sizeof(Type));
    p2->typesize = p->typesize;
    p2->category = ARRAY;
    p2->array = p;
    return p2;
}

Type *makeFuncType(const char *name, FieldList *fl)
{
    Type *p = (Type *)malloc(sizeof(Type));
    Function *p2 = (Function *)malloc(sizeof(Function));
    p->category = FUNCTION;
    p->func = p2;
    memset(p2->name, 0, sizeof(p2->name));
    strncpy(p2->name, name, 31);
    p2->params = fl;
    return p;
}

Type *makeErrorType()
{
    Type *p = (Type *)malloc(sizeof(Type));
    p->category = ERRORTYPE;
    p->array = NULL;
    p->typesize = 0;
    return p;
}

void addFuncRet(Type *p, Type *ret)
{
    if (p->category != FUNCTION)
        return;
    p->func->return_type = ret;
}

Type *addArrayTypeRec(Type *nowType, size_t sz)
{
    Type *nxt;
    if (nowType->category == ARRAY)
    {
        nxt = addArrayTypeRec(nowType->array->base, sz);
        nowType->array->base = nxt;
        nowType->array->typesize = nxt->typesize * nowType->array->size;
        nowType->typesize = nowType->array->typesize;
        return nowType;
    }
    else
    {
        nxt = makeArrayType(nowType, sz);
        return nxt;
    }
}

Type *addArrayType(Type *nowType, size_t sz)
{
    if (nowType->category != ARRAY)
    {
        return makeArrayType(nowType, sz);
    }
    else
    {
        addArrayTypeRec(nowType, sz);
        return nowType;
    }
}

FieldList *addFieldList(FieldList *fl, Type *nowType, const char *name)
{
    if (fl->type == NULL)
    {
        fl->type = nowType;
        if (nowType != NULL)
            fl->typesize = nowType->typesize;
        else
            fl->typesize = 0;
        strncpy(fl->varname, name, 31);
        return fl;
    }
    FieldList *nFL = makeFieldList(nowType, name);
    nFL->next = fl;
    nFL->typesize += fl->typesize;
    // printf("add_FL_size=%zu\n", nFL->typesize);
    return nFL;
}

// void deleteArray(Array *arr)
// {
//     if (arr == NULL)
//         return;
//     deleteType(arr->base);
//     free(arr);
// }

// void deleteFieldList(FieldList *fl)
// {
//     if (fl == NULL)
//         return;
//     deleteType(fl->type);
//     deleteFieldList(fl->next);
//     free(fl);
// }

// void deleteStructure(Structure *stru)
// {
//     if (stru == NULL)
//         return;
//     deleteFieldList(stru->data);
// }

// void deleteFunction(Function *func)
// {
//     if (func == NULL)
//         return;
//     deleteFieldList(func->params);
// }

// void deleteType(Type *type)
// {
//     if (type == NULL)
//         return;
//     if (type->category == ARRAY)
//     {
//         deleteArray(type->array);
//     }
//     else if (type->category == STRUCTURE)
//     {
//         deleteStructure(type->structure);
//     }
//     else if (type->category == FUNCTION)
//     {
//         deleteFunction(type->func);
//     }
//     free(type);
// }

char checkTypeEqual(const Type *a, const Type *b)
{
    if (a->category == PRIMITIVE && b->category == PRIMITIVE)
    {
        return checkPrimEqual(a->primitive, b->primitive);
    }
    if (a == b)
        return 1;
    if (a == NULL || b == NULL)
    {
        return 0;
    }
    if (a->category == ERRORTYPE || b->category == ERRORTYPE)
    {
        return 1;
    }
    if (a->typesize != b->typesize)
    {
        return 0;
    }
    if (a->category != b->category)
    {
        return 0;
    }
    if (a->category == ARRAY)
        return checkArrayEqual(a->array, b->array);
    if (a->category == STRUCTURE)
        return checkStructEqual(a->structure, b->structure);
    if (a->category == FUNCTION)
        return checkFunctionEqual(a->func, b->func);
}

#ifdef IMPLICIT_CONVERT
char checkPrimEqual(const int pr1, const int pr2)
{
    if (pr1 != pr2 && (pr1 == PCHAR || pr2 == PCHAR))
    {
        return 0;
    }
    return 1;
}
#else
char checkPrimEqual(const int pr1, const int pr2)
{
    return pr1 == pr2;
}
#endif

char checkStructEqual(const Structure *a, const Structure *b)
{
    return checkFieldEqual(a->data, b->data);
}

char checkFieldEqual(const FieldList *a, const FieldList *b)
{
    // printf("CK_FL_EQ:\nType1:\n");
    // outputFieldList(a);
    // printf("Type2:\n");
    // outputFieldList(b);
    if (a == NULL && b == NULL)
        return 1;
    if (a != NULL && a->type == NULL)
        return checkFieldEqual(a->next, b);
    if (b != NULL && b->type == NULL)
        return checkFieldEqual(a, b->next);
    if (a == NULL || b == NULL)
        return 0;
    if (!checkTypeEqual(a->type, b->type))
        return 0;
    return checkFieldEqual(a->next, b->next);
}

char checkArrayEqual(const Array *a, const Array *b)
{
    if (a->size != b->size)
        return 0;
    return checkTypeEqual(a->base, b->base);
}

char checkFunctionEqual(const Function *a, const Function *b)
{
    if (strcmp(a->name, b->name) != 0)
        return 0;
    // if (!checkTypeEqual(a->return_type, b->return_type))
    //     return 0;
    return checkFieldEqual(a->params, b->params);
}

Type *getTypeAfterOp(Type *a, Type *b, const char *op)
{
    // printf("getafterOP:\n");
    // outputType(a);
    // outputType(b);
    if (a->category == FUNCTION)
        return getTypeAfterOp(a->func->return_type, b, op);
    if (b->category == FUNCTION)
        return getTypeAfterOp(a, b->func->return_type, op);

    if (strcmp(op, "ass") == 0)
    {
        if (checkTypeEqual(a, b))
        {
            return a;
        }
        else
        {
            return makeErrorType();
        }
    }
    if (strcmp(op, "alg") == 0)
    {
        if (checkTypeEqual(a, b))
        {
            if (a->category == ERRORTYPE)
                return a;
            if (a->category == PRIMITIVE)
            {
                if (a->primitive == b->primitive)
                    return a;
                else if (a->primitive == PFLOAT)
                {
                    return a;
                }
                else
                {
                    return b;
                }
            }
            return b;
        }
        else
        {
            return makeErrorType();
        }
    }
    if (strcmp(op, "bin") == 0)
    {
        if (a->category == PRIMITIVE && a->primitive == PINT && b->category == PRIMITIVE && b->primitive == PINT)
        {
            return a;
        }
        else
        {
            return makeErrorType();
        }
    }
}

Type *findNameInField(FieldList *a, const char *name)
{
    if (a == NULL)
        return NULL;
    if (strcmp(a->varname, name) == 0)
        return a->type;
    return findNameInField(a->next, name);
}

Type *findNameInStructure(Type *a, const char *name)
{
    if (a->category == STRUCTURE)
    {
        return findNameInField(a->structure->data, name);
    }
    else
    {
        return NULL;
    }
}
void outputFunction(Function *p)
{
    printf("Function: %s\n", p->name);
    outputFieldList(p->params);
}

void outputType(const Type *t)
{
    if (t == NULL)
        return;
    switch (t->category)
    {
    case PRIMITIVE:
        outputPrime(t->primitive);
        break;
    case STRUCTURE:
        outputStruct(t->structure);
        break;
    case FUNCTION:
        outputFunction(t->func);
        break;
    default:
        break;
    }
}

void outputFieldList(const FieldList *fl)
{
    if (fl == NULL)
        return;
    outputType(fl->type);
    outputFieldList(fl->next);
}

void outputPrime(const int p)
{
    switch (p)
    {
    case PINT:
        printf("int\n");
        break;
    case PFLOAT:
        printf("float\n");
        break;
    case PCHAR:
        printf("char\n");
        break;
    default:
        break;
    }
}

void outputStruct(const Structure *s)
{
    printf("Structure: %s\n", s->struct_name);
    outputFieldList(s->data);
}

void calcTypeSize(Type *p)
{
    switch (p->category)
    {
    case PRIMITIVE:
        p->typesize = (p->primitive == PCHAR ? 1 : 4);
        break;
    case STRUCTURE:
        calcStructSize(p->structure);
        p->typesize = p->structure->typesize;
        break;
    case ARRAY:
        calcArraySize(p->array);
        p->typesize = p->array->typesize;
        break;

    default:
        break;
    }
}
void calcStructSize(Structure *p)
{
    calcFieldSize(p->data);
    p->typesize = p->data->typesize;
}
void calcFieldSize(FieldList *p)
{
    if (p == NULL)
        return;
    calcTypeSize(p->type);
    if (p->next == NULL)
    {
        p->typesize = p->type->typesize;
    }
    else
    {
        calcFieldSize(p->next);
        p->typesize = p->type->typesize + p->next->typesize;
    }
}
void calcArraySize(Array *p)
{
    calcTypeSize(p->base);
    p->typesize = p->base->typesize * p->size;
}