#ifndef MIPS_H
#define MIPS_H

#include "tac.h"
#define FALSE 0
#define TRUE 1

typedef enum
{
    zero,
    at,
    v0,
    v1,
    a0,
    a1,
    a2,
    a3,
    t0,
    t1,
    t2,
    t3,
    t4,
    t5,
    t6,
    t7,
    s0,
    s1,
    s2,
    s3,
    s4,
    s5,
    s6,
    s7,
    t8,
    t9,
    k0,
    k1,
    gp,
    sp,
    fp,
    ra,
    NUM_REGS
} Register;

struct RegDesc
{ // the register descriptor
    const char *name;
    char var[8];
    bool dirty; // value updated but not stored
    /* add other fields as you need */
    size_t recent;
};

extern struct RegDesc regs[NUM_REGS];

struct MemDesc{
    char var[8];
    int offset;
    int first_seen;
    struct MemDesc *next;
};

extern struct MemDesc *varmem;

typedef struct MemDesc MemDesc;
typedef struct RegDesc RegDesc;

void _mips_printf(const char *fmt, ...);
void mips32_gen(tac *head, FILE *_fd);
MemDesc *get_memory_addr(char varname[8]);

#endif // MIPS_H
