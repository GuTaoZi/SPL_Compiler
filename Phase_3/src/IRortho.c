#include "IRortho.h"
#include <stdio.h>
#include <string.h>

static IR_hashNode *IR_H = NULL;

static IR_orthoStack *IR_S = NULL;

static IR_orthoNode *IR_new_ortho_node(const char *name, const char *vname)
{
    IR_orthoNode *p = (IR_orthoNode *)malloc(sizeof(IR_orthoNode));
    strncpy(p->name, name, 31);
    p->name[31] = 0;
    p->vname = (char *)malloc(sizeof(char) * (strlen(vname) + 1));
    strcpy(p->vname, vname);
    p->next[0] = p->next[1] = NULL;
    return p;
}

IR_orthoStack *IR_push_stack()
{
    // printf("push_stack\n");
    IR_orthoStack *p = (IR_orthoStack *)malloc(sizeof(IR_orthoStack));
    p->top = NULL;
    p->next = IR_S;
    IR_S = p;
    return IR_S;
}

IR_orthoStack *IR_pop_stack()
{
    // printf("pop_stack\n");
    if (!IR_S)
    {
        fprintf(yyout, "Trying to pop an empty stack.\n");
    }
    IR_orthoNode *it = IR_S->top;
    IR_hashNode *hash_head; // the IR_hashNode containing it.
    while (it)
    {
        HASH_FIND_STR(IR_H, it->name, hash_head);
        HASH_DEL(IR_H, hash_head);
        // deleteType(it->val);
        if (it->next[0])
        {
            IR_hashNode *hn = (IR_hashNode *)malloc(sizeof(IR_hashNode));
            strncpy(hn->name, it->name, 31);
            hn->name[31] = 0;
            hn->head = it->next[0];
            HASH_ADD_STR(IR_H, name, hn);
            // change the head of IR_hashNode to the next IR_orthoNode
        }
        else
        {
            // This is the last IR_orthoNode of this IR_hashNode
        }
        it = it->next[1];
    }
    IR_orthoStack *ttmp = IR_S;
    IR_S = IR_S->next;
    free(ttmp);
    return IR_S;
}

IR_orthoNode *IR_stack_top()
{
    return IR_S ? IR_S->top : NULL;
}

// TODO: check if redefinition here, or maybe outside?
IR_orthoNode *IR_add_ortho_node(const char *name, const char *vname)
{
    if (!IR_S)
    {
        fprintf(yyout, "NULL stack get, need to new a stack node.\n");
        return NULL;
    }
    IR_orthoNode *p = IR_new_ortho_node(name, vname);
    IR_hashNode *hn = (IR_hashNode *)malloc(sizeof(IR_hashNode));
    IR_hashNode *old_hash_head;
    HASH_FIND_STR(IR_H, name, old_hash_head);
    if (old_hash_head)
    {
        p->next[0] = old_hash_head->head;
        // place the new head ahead of the old head
        HASH_DEL(IR_H, old_hash_head);
        // delete the old map <name, IR_hashNode(containing IR_orthoNode)>
    }
    if (IR_S)
    {
        p->next[1] = IR_S->top;
        IR_S->top = p;
    }
    strncpy(hn->name, name, 31);
    hn->name[31] = 0;
    hn->head = p;
    HASH_ADD_STR(IR_H, name, hn);
    // add the new map <name, IR_hashNode(containing IR_orthoNode)>
}

IR_orthoNode *IR_current_scope_seek(const char *name)
{
    IR_orthoNode *it = IR_S->top;
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

IR_orthoNode *IR_global_scope_seek(const char *name)
{
    IR_orthoStack *sit = IR_S;
    while (sit)
    {
        IR_orthoNode *it = sit->top;
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