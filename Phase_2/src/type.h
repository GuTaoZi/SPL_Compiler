#ifndef TYPE_H
#define TYPE_H

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
} Function;

Type *makeStructType();

Type *makePrimType(const char *u);

void addStructName(Type *nowType, const char *u);

FieldList *makeFieldList(Type *nowType, const char *name);

FieldList *addFieldList(FieldList *fl, Type *nowType, const char *name);

void addStructField(Type *nowType, FieldList *fl);

void deleteArray(Array *arr);

void deleteFieldList(FieldList *fl);

void deleteStructure(Structure *stru);

void deleteFunction(Function *func);

void deleteType(Type *type);

#endif