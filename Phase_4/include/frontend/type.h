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
    union {
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

Type *makeErrorType();

void addFuncRet(Type *p, Type *ret);

Type *addArrayType(Type *nowType, size_t sz);

FieldList *addFieldList(FieldList *fl, Type *nowType, const char *name);

// void deleteArray(Array *arr);

// void deleteFieldList(FieldList *fl);

// void deleteStructure(Structure *stru);

// void deleteFunction(Function *func);
/**
 * So... Please do not use me
 */
// void deleteType(Type *type);

char checkTypeEqual(const Type *a, const Type *b);

char checkPrimEqual(const int pr1, const int pr2);

char checkStructEqual(const Structure *a, const Structure *b);

char checkFieldEqual(const FieldList *a, const FieldList *b);

char checkArrayEqual(const Array *a, const Array *b);

char checkFunctionEqual(const Function *a, const Function *b);

Type *getTypeAfterOp(Type *a, Type *b, const char *op);

Type *findNameInStructure(Type *a, const char *name);

Type *findNameInField(FieldList *a, const char *name);

void outputType(const Type *t);

void outputFieldList(const FieldList *fl);

void outputPrime(const int p);

void outputStruct(const Structure *s);

void calcTypeSize(Type *p);
void calcStructSize(Structure *p);
void calcFieldSize(FieldList *p);
void calcArraySize(Array *p);

size_t get_offset(const FieldList *p, const char *name);
size_t get_offset_Struct(const Type *p, const char *name);
size_t get_array_size(const Type *p);

#endif