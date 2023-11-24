#include "type.h"
#include "uthash.h"
#include <stdio.h>
#include <string.h>

typedef struct _orthoNode
{
    char name[32];
    orthoNode *next[2];
    // next[0]: ptr to next node with same name(hash)
    // next[1]: ptr to next node within same scope(stack)
    // NULL for tail of the list
    Type *val;
} orthoNode;

typedef struct _hashNode
{
    char name[32];
    orthoNode *head;
    UT_hash_handle hh;
} hashNode;

static hashNode *H = NULL;

typedef struct _orthoStack
{
    orthoNode *top;
    orthoStack *next;
} orthoStack;

static orthoStack *S = NULL;

static orthoNode *new_ortho_node(char *name, Type *val)
{
    orthoNode *p = (orthoNode *)malloc(sizeof(orthoNode));
    p->name = name;
    p->val = val;
    p->next[0] = p->next[1] = NULL;
    return p;
}

orthoStack *new_stack_node()
{
    orthoStack *p = (orthoStack *)malloc(sizeof(orthoStack));
    p->top = NULL;
    p->next = S;
    S = p;
    return S;
}

orthoNode *add_ortho_node(char *name, Type *val)
{
    if (!S)
    {
        fprintf("NULL stack get, need to new a stack node.\n");
        return NULL;
    }
    orthoNode *p = new_ortho_node(name, val);
    hashNode *hn = (hashNode *)malloc(sizeof(hashNode));

    hashNode *old_hash_head;
    HASH_FIND_STR(H, name, old_hash_head);
    if (old_hash_head)
    {
        p->next[0] = old_hash_head->head;
        // place the new head ahead of the old head
        HASH_DEL(H, old_hash_head);
        // delete the old map <name, hashNode(containing orthoNode)>
    }
    if (S)
    {
        p->next[1] = S->top;
        S->top = p;
    }
    strcpy(hn->name, name);
    hn->head = p;
    HASH_ADD(H, name, hn);
    // add the new map <name, hashNode(containing orthoNode)>
}

orthoStack *pop_stack()
{
    orthoNode *p = S->top;
    if (!p)
    {
        fprintf("Trying to del a NULL node.\n");
        return NULL;
    }
}

orthoNode *get_head_by_name(char *name)
{
    hashNode *p;
    HASH_FIND_STR(H, name, p);
    if (p)
    {
        return p->head;
    }
    return NULL;
}

orthoNode *stack_top()
{
    return S ? S->top : NULL;
}