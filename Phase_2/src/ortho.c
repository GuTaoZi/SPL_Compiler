#include "type.h"
#include "uthash.h"

typedef struct _orthoNode
{
    char name[32];
    orthoNode *next[2];
    // next[0]: ptr to next node with same name
    // next[1]: ptr to next node within same scope
    // NULL for tail of the list
    Type *val;
} orthoNode;

typedef struct _hashNode
{
    char name[32];
    orthoNode *head;
    UT_hash_handle hh;
}hashNode;

typedef struct _orthoStack
{
    orthoNode *head;
    orthoStack *next;
} orthoStack;

orthoNode *new_node(char *name, Type *val)
{
    orthoNode *p = (orthoNode *)malloc(sizeof(orthoNode));
    p->name = name;
    p->val = val;
    p->next[0] = p->next[1] = NULL;
    return p;
}

orthoNode *get_head_by_name(char *name, )
{

}