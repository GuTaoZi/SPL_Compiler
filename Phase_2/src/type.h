#ifndef TYPE_H
#define TYPE_H

#include <stddef.h>

typedef struct Type
{
    size_t typesize;
    enum
    {
        PRIMITIVE,
        ARRAY,
        STRUCTURE,
        FUNCTION,
        ERRORTYPE
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
    size_t typesize;
    struct Type *base;
    int size;
} Array;

typedef struct FieldList
{
    size_t typesize;
    struct Type *type;
    char varname[32];
    struct FieldList *next;
} FieldList;

typedef struct Structure
{
    size_t typesize;
    char struct_name[32];
    struct FieldList *data;
} Structure;

typedef struct Function
{
    char name[32];
    struct FieldList *params;
    Type *return_type;
} Function;

Type *makeStructType(const char *name, FieldList *fl);

Type *makePrimType(const char *u);

FieldList *makeFieldList(Type *nowType, const char *name);

Type *makeArrayType(Type *b, size_t sz);

Type *makeFuncType(const char *name, FieldList *fl);

void addFuncRet(Type *p, Type *ret);

Type *addArrayType(Type *nowType, size_t sz);

FieldList *addFieldList(FieldList *fl, Type *nowType, const char *name);

void deleteArray(Array *arr);

void deleteFieldList(FieldList *fl);

void deleteStructure(Structure *stru);

void deleteFunction(Function *func);

/**
 * So... Please do not use me
 */
void deleteType(Type *type);

#endif