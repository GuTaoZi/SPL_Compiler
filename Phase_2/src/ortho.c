#include "ortho.h"
#include <stdio.h>
#include <string.h>

static hashNode *H = NULL;

static orthoStack *S = NULL;

static orthoNode *new_ortho_node(const char *name, Type *val)
{
    orthoNode *p = (orthoNode *)malloc(sizeof(orthoNode));
    strncpy(p->name, name, 31);
    p->name[31] = 0;
    p->val = val;
    p->next[0] = p->next[1] = NULL;
    return p;
}

orthoStack *push_stack()
{
    // printf("push_stack\n");
    orthoStack *p = (orthoStack *)malloc(sizeof(orthoStack));
    p->top = NULL;
    p->next = S;
    S = p;
    return S;
}

orthoStack *pop_stack()
{
    // printf("pop_stack\n");
    if (!S)
    {
        fprintf(yyout, "Trying to pop an empty stack.\n");
    }
    orthoNode *it = S->top;
    // if (!it)
    // {
    //     fprintf(yyout, "Trying to del a NULL node.\n");
    //     return NULL;
    // }
    hashNode *hash_head; // the hashNode containing it.
    while (it)
    {
        HASH_FIND_STR(H, it->name, hash_head);
        HASH_DEL(H, hash_head);
        // deleteType(it->val);
        if (it->next[0])
        {
            hashNode *hn = (hashNode *)malloc(sizeof(hashNode));
            strncpy(hn->name, it->name, 31);
            hn->name[31] = 0;
            hn->head = it->next[0];
            HASH_ADD_STR(H, name, hn);
            // change the head of hashNode to the next orthoNode
        }
        else
        {
            // This is the last orthoNode of this hashNode
        }
        it = it->next[1];
    }
    orthoStack *ttmp = S;
    S = S->next;
    free(ttmp);
    return S;
}

orthoNode *stack_top()
{
    return S ? S->top : NULL;
}

// TODO: check if redefinition here, or maybe outside?
orthoNode *add_ortho_node(const char *name, Type *val)
{
    if (!S)
    {
        fprintf(yyout, "NULL stack get, need to new a stack node.\n");
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
    strncpy(hn->name, name, 31);
    hn->name[31] = 0;
    hn->head = p;
    HASH_ADD_STR(H, name, hn);
    // add the new map <name, hashNode(containing orthoNode)>
}

orthoNode *current_scope_seek(const char *name)
{
    orthoNode *it = S->top;
    while (it)
    {
        if (strncmp(name, it->name, 32) == 0)
        {
            return it;
        }
        it = it->next[1];
    }
    return NULL;
}

orthoNode *global_scope_seek(const char *name)
{
    orthoStack *sit = S;
    while (sit)
    {
        orthoNode *it = sit->top;
        while (it)
        {
            if (strncmp(name, it->name, 31) == 0)
            {
                return it;
            }
            it = it->next[1];
        }
        sit = sit->next;
    }
    return NULL;
}