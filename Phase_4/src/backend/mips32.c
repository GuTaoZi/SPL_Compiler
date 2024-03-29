#include "mips32.h"

/* the output file descriptor, may not be explicitly used */
FILE *fd;
size_t lru_cnt = 0;
size_t stack_offset;
RegDesc regs[NUM_REGS];
struct VarDesc *varmem;

#define _tac_kind(vtac) (((vtac)->code).kind)
#define _tac_quadruple(vtac) (((vtac)->code).vtac)
#define _reg_name(reg) regs[reg].name

void _mips_printf(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vfprintf(fd, fmt, args);
    va_end(args);
    fputs("\n", fd);
}

void _mips_iprintf(const char *fmt, ...)
{
    va_list args;
    fputs("  ", fd); // `iprintf` stands for indented printf
    va_start(args, fmt);
    vfprintf(fd, fmt, args);
    va_end(args);
    fputs("\n", fd);
}

VarDesc *get_memory_addr(char varname[8])
{
    VarDesc *u = varmem->next;
    while (u != NULL)
    {
        if (strncmp(varname, u->var, 8) == 0)
        {
            return u;
        }
        u = u->next;
    }
    return NULL;
}

void save_reg(Register reg)
{
    _mips_iprintf("sw %s, -%d($sp)", _reg_name(reg), get_memory_addr(regs[reg].var)->offset);
}

void init_gp_counter()
{
}

void set_gp_counter()
{
    Register x = fp;
    _mips_iprintf("addi %s, $gp, -32768", _reg_name(x));
}

void spill_register(Register reg)
{
    /* COMPLETED the register spilling */
    if (!regs[reg].dirty || regs[reg].var[0] == 0)
    {
        return;
    }
    // VarDesc *result = get_memory_addr(regs[reg].var);
    // if (result == NULL)
    // {
    //     _mips_printf("Im Angry!!");
    // }
    // else
    // {
    //     _mips_iprintf("sw %s, -%d($sp)", _reg_name(reg), result->offset);
    // }
    strcpy(regs[reg].var, "");
    regs[reg].dirty = false;
}

void _deref(Register x, tac_opd *opd)
{
    if (opd->kind == OP_POINTER)
        _mips_iprintf("lw %s, 0(%s)", _reg_name(x), _reg_name(x));
    else if (opd->kind == OP_REFERENCE)
    {
        VarDesc *vmi = get_memory_addr(opd->char_val);
        _mips_iprintf("lw %s, -%d($sp)", _reg_name(x), vmi->offset);
    }
}

VarDesc *alloc_stack_space(tac_opd *opd)
{
    if (opd->kind == OP_CONSTANT || opd->kind == OP_LABEL)
    {
        return NULL;
    }
    VarDesc *u = varmem->next, *tail = varmem;
    while (u != NULL)
    {
        if (strcmp(u->var, opd->char_val) == 0)
        {
            return u;
        }
        tail = u;
        u = u->next;
    }
    VarDesc *p = (VarDesc *)malloc(sizeof(VarDesc));
    p->next = NULL;
    p->offset = stack_offset;
    p->first_seen = 1;
    stack_offset += 4;
    strncpy(p->var, opd->char_val, 8);
    tail->next = p;
    return p;
}

Register get_LRU_victim()
{
    Register victim = t0;
    for (Register t = t0; t <= s7; t++)
    {
        if (strcmp(regs[t].var, "") == 0)
        {
            return t;
        }
        else if (regs[t].recent < regs[victim].recent)
        {
            victim = t;
        }
    }
    return victim;
}

Register _get_reg(tac_opd *opd, char need_load)
{
    VarDesc *p;
    if (opd->kind != OP_CONSTANT && opd->kind != OP_LABEL)
    {
        p = alloc_stack_space(opd);
    }
    Register r;
    char *name = opd->char_val;
    // for (r = t0; r <= s7; r++)
    // {
    //     if (strcmp(name, regs[r].var) == 0)
    //     {
    //         regs[r].recent = ++lru_cnt;
    //         return r;
    //     }
    // }
    r = get_LRU_victim();
    spill_register(r);
    regs[r].dirty = false;
    if (opd->kind == OP_CONSTANT)
    {
        strcpy(regs[r].var, "const");
        _mips_printf("li %s, %d", _reg_name(r), opd->int_val);
        regs[r].recent = ++lru_cnt;
        return r;
    }
    if (need_load && opd->kind != OP_CONSTANT && opd->kind != OP_LABEL)
    {
        _mips_iprintf("lw %s, -%d(%s)", _reg_name(r), p->offset, _reg_name(sp));
    }
    // p->first_seen = 0;
    strcpy(regs[r].var, (opd->kind != OP_POINTER && opd->kind != OP_REFERENCE) ? name : "tmp");
    regs[r].recent = ++lru_cnt;
    return r;
}

// get the value of the operand
Register get_register(tac_opd *opd)
{
    Register r = _get_reg(opd, true);
    _deref(r, opd);
    return r;
}

// get the address of the operand
Register get_register_w(tac_opd *opd)
{
    Register r = _get_reg(opd, opd->kind != OP_VARIABLE);
    regs[r].dirty = true;
    return r;
}

void save_sp()
{
    // _mips_iprintf("sw $v1, 0($sp)");
    // _mips_iprintf("move $v1, $sp");
    // _mips_iprintf("addi $sp, $sp, -4");
}

void restore_sp()
{
    // _mips_iprintf("move $sp, $v1");
    // _mips_iprintf("lw $v1, 0($sp)");
}

/* PARAM: a pointer to `struct tac_node` instance
   RETURN: the next instruction to be translated */
tac *emit_label(tac *label)
{
    assert(_tac_kind(label) == LABEL);
    _mips_printf("label%d:", _tac_quadruple(label).labelno->int_val);
    return label->next;
}

tac *_read_params(tac *param, int depth, int *params_num)
{
    if (param->code.kind != PARAM)
        return param;
    tac *ret = _read_params(param->next, depth + 1, params_num);

    Register x = get_register_w(_tac_quadruple(param).p);
    if ((*params_num) < 4)
    {
        _mips_iprintf("move %s, %s", _reg_name(x), _reg_name(a0 + (*params_num)));
    }
    else
    {
        _mips_iprintf("lw %s, %d($sp)", _reg_name(x), 4 * (depth + 1));
    }
    (*params_num) += 1;
    save_reg(x);
    return ret;
}

tac *read_params(tac *param)
{
    int p = 0;
    tac *ret = _read_params(param, 0, &p);
    save_sp();
    return ret;
}

int total_arg_num;

tac *_save_args(tac *arg, int params_num)
{
    if (arg->code.kind != ARG)
    {
        if (params_num > 4)
        {
            _mips_iprintf("addi $sp, $sp, -%d", 4 * (params_num - 4));
            total_arg_num = 4 * (params_num - 4);
        }
        else
        {
            total_arg_num = 0;
        }
        return arg;
    }
    Register x = get_register(_tac_quadruple(arg).var);
    if (params_num < 4)
    {
        _mips_iprintf("move %s, %s", _reg_name(a0 + params_num), _reg_name(x));
    }
    else
    {
        _mips_iprintf("sw %s, -%d($sp)", _reg_name(x), 4 * (params_num - 3) + stack_offset);
    }
    return _save_args(arg->next, params_num + 1);
}

tac *save_args(tac *arg)
{
    return _save_args(arg, 0);
}

tac *emit_function(tac *function)
{
    stack_offset = 0;
    // for (Register r = t0; r <= s7; r++)
    //     strcpy(regs[r].var, "");
    _mips_printf("%s:", _tac_quadruple(function).funcname);
    if (strcmp("main", _tac_quadruple(function).funcname) == 0)
    {
        set_gp_counter();
    }
    if (function->next->code.kind != PARAM)
    {
        save_sp();
        return function->next;
    }
    else
    {
        return read_params(function->next);
    }
}

tac *emit_assign(tac *assign)
{
    Register x, y;

    x = get_register_w(_tac_quadruple(assign).left);
    if (_tac_quadruple(assign).right->kind == OP_CONSTANT)
    {
        _mips_iprintf("li %s, %d", _reg_name(x), _tac_quadruple(assign).right->int_val);
    }
    else
    {
        y = get_register(_tac_quadruple(assign).right);
        _mips_iprintf("move %s, %s", _reg_name(x), _reg_name(y));
    }
    save_reg(x);
    return assign->next;
}

tac *emit_add(tac *add)
{
    Register x, y, z;

    x = get_register_w(_tac_quadruple(add).left);
    if (_tac_quadruple(add).r1->kind == OP_CONSTANT)
    {
        y = get_register(_tac_quadruple(add).r2);
        _mips_iprintf("addi %s, %s, %d", _reg_name(x), _reg_name(y), _tac_quadruple(add).r1->int_val);
    }
    else if (_tac_quadruple(add).r2->kind == OP_CONSTANT)
    {
        y = get_register(_tac_quadruple(add).r1);
        _mips_iprintf("addi %s, %s, %d", _reg_name(x), _reg_name(y), _tac_quadruple(add).r2->int_val);
    }
    else
    {
        y = get_register(_tac_quadruple(add).r1);
        z = get_register(_tac_quadruple(add).r2);
        _mips_iprintf("add %s, %s, %s", _reg_name(x), _reg_name(y), _reg_name(z));
    }
    save_reg(x);
    return add->next;
}

tac *emit_sub(tac *sub)
{
    Register x, y, z;

    x = get_register_w(_tac_quadruple(sub).left);
    if (_tac_quadruple(sub).r1->kind == OP_CONSTANT)
    {
        y = get_register(_tac_quadruple(sub).r2);
        _mips_iprintf("neg %s, %s", _reg_name(y), _reg_name(y));
        _mips_iprintf("addi %s, %s, %d", _reg_name(x), _reg_name(y), _tac_quadruple(sub).r1->int_val);
    }
    else if (_tac_quadruple(sub).r2->kind == OP_CONSTANT)
    {
        y = get_register(_tac_quadruple(sub).r1);
        _mips_iprintf("addi %s, %s, -%d", _reg_name(x), _reg_name(y), _tac_quadruple(sub).r2->int_val);
    }
    else
    {
        y = get_register(_tac_quadruple(sub).r1);
        z = get_register(_tac_quadruple(sub).r2);
        _mips_iprintf("sub %s, %s, %s", _reg_name(x), _reg_name(y), _reg_name(z));
    }
    save_reg(x);
    return sub->next;
}

tac *emit_mul(tac *mul)
{
    Register x, y, z;

    x = get_register_w(_tac_quadruple(mul).left);
    if (_tac_quadruple(mul).r1->kind == OP_CONSTANT)
    {
        y = get_register_w(_tac_quadruple(mul).r1);
        z = get_register(_tac_quadruple(mul).r2);
        _mips_iprintf("li %s, %d", _reg_name(y), _tac_quadruple(mul).r1->int_val);
    }
    else if (_tac_quadruple(mul).r2->kind == OP_CONSTANT)
    {
        y = get_register(_tac_quadruple(mul).r1);
        z = get_register_w(_tac_quadruple(mul).r2);
        _mips_iprintf("li %s, %d", _reg_name(z), _tac_quadruple(mul).r2->int_val);
    }
    else
    {
        y = get_register(_tac_quadruple(mul).r1);
        z = get_register(_tac_quadruple(mul).r2);
    }
    _mips_iprintf("mul %s, %s, %s", _reg_name(x), _reg_name(y), _reg_name(z));
    save_reg(x);
    return mul->next;
}

tac *emit_div(tac *div)
{
    Register x, y, z;

    x = get_register_w(_tac_quadruple(div).left);
    if (_tac_quadruple(div).r1->kind == OP_CONSTANT)
    {
        y = get_register_w(_tac_quadruple(div).r1);
        z = get_register(_tac_quadruple(div).r2);
        _mips_iprintf("li %s, %d", _reg_name(y), _tac_quadruple(div).r1->int_val);
    }
    else if (_tac_quadruple(div).r2->kind == OP_CONSTANT)
    {
        y = get_register(_tac_quadruple(div).r1);
        z = get_register_w(_tac_quadruple(div).r2);
        _mips_iprintf("li %s, %d", _reg_name(z), _tac_quadruple(div).r2->int_val);
    }
    else
    {
        y = get_register(_tac_quadruple(div).r1);
        z = get_register(_tac_quadruple(div).r2);
    }
    _mips_iprintf("div %s, %s", _reg_name(y), _reg_name(z));
    _mips_iprintf("mflo %s", _reg_name(x));
    save_reg(x);
    return div->next;
}

tac *emit_addr(tac *addr)
{
    Register x, y;

    x = get_register_w(_tac_quadruple(addr).left);
    y = get_register(_tac_quadruple(addr).right);
    _mips_iprintf("move %s, %s", _reg_name(x), _reg_name(y));
    save_reg(x);
    return addr->next;
}

tac *emit_fetch(tac *fetch)
{
    Register x, y;

    x = get_register_w(_tac_quadruple(fetch).left);
    y = get_register(_tac_quadruple(fetch).raddr);
    _mips_iprintf("lw %s, 0(%s)", _reg_name(x), _reg_name(y));
    save_reg(x);
    return fetch->next;
}

tac *emit_deref(tac *deref)
{
    Register x, y;

    x = get_register(_tac_quadruple(deref).laddr);
    if (_tac_quadruple(deref).right->kind == OP_CONSTANT)
    {
        y = get_register_w(_tac_quadruple(deref).right);
    }
    else
    {
        y = get_register(_tac_quadruple(deref).right);
    }
    _mips_iprintf("sw %s, 0(%s)", _reg_name(y), _reg_name(x));
    return deref->next;
}

tac *emit_goto(tac *goto_)
{
    _mips_iprintf("j label%d", _tac_quadruple(goto_).labelno->int_val);
    return goto_->next;
}

tac *emit_iflt(tac *iflt)
{
    /* COMPLETED emit function */
    Register x, y;
    x = get_register(_tac_quadruple(iflt).c1);
    y = get_register(_tac_quadruple(iflt).c2);
    _mips_iprintf("blt %s, %s, label%d", _reg_name(x), _reg_name(y), _tac_quadruple(iflt).labelno->int_val);
    return iflt->next;
}

tac *emit_ifle(tac *ifle)
{
    /* COMPLETED emit function */
    Register x, y;
    x = get_register(_tac_quadruple(ifle).c1);
    y = get_register(_tac_quadruple(ifle).c2);
    _mips_iprintf("ble %s, %s, label%d", _reg_name(x), _reg_name(y), _tac_quadruple(ifle).labelno->int_val);
    return ifle->next;
}

tac *emit_ifgt(tac *ifgt)
{
    /* COMPLETED emit function */
    Register x, y;
    x = get_register(_tac_quadruple(ifgt).c1);
    y = get_register(_tac_quadruple(ifgt).c2);
    _mips_iprintf("bgt %s, %s, label%d", _reg_name(x), _reg_name(y), _tac_quadruple(ifgt).labelno->int_val);
    return ifgt->next;
}

tac *emit_ifge(tac *ifge)
{
    /* COMPLETED emit function */
    Register x, y;
    x = get_register(_tac_quadruple(ifge).c1);
    y = get_register(_tac_quadruple(ifge).c2);
    _mips_iprintf("bge %s, %s, label%d", _reg_name(x), _reg_name(y), _tac_quadruple(ifge).labelno->int_val);
    return ifge->next;
}

tac *emit_ifne(tac *ifne)
{
    /* COMPLETED emit function */
    Register x, y;
    x = get_register(_tac_quadruple(ifne).c1);
    y = get_register(_tac_quadruple(ifne).c2);
    _mips_iprintf("bne %s, %s, label%d", _reg_name(x), _reg_name(y), _tac_quadruple(ifne).labelno->int_val);
    return ifne->next;
}

tac *emit_ifeq(tac *ifeq)
{
    /* COMPLETED emit function */
    Register x, y;
    x = get_register(_tac_quadruple(ifeq).c1);
    y = get_register(_tac_quadruple(ifeq).c2);
    _mips_iprintf("beq %s, %s, label%d", _reg_name(x), _reg_name(y), _tac_quadruple(ifeq).labelno->int_val);
    return ifeq->next;
}

tac *emit_return(tac *return_)
{
    /* COMPLETED emit function */
    Register x;
    if (_tac_quadruple(return_).var->kind == OP_CONSTANT)
    {
        _mips_iprintf("li $v0, %d", _tac_quadruple(return_).var->int_val);
    }
    else if (_tac_quadruple(return_).var->kind == OP_VARIABLE)
    {
        x = get_register(_tac_quadruple(return_).var);
        _mips_iprintf("move $v0, %s", _reg_name(x));
    }
    else if (_tac_quadruple(return_).var->kind == OP_POINTER)
    {
        x = get_register(_tac_quadruple(return_).var);
        _mips_iprintf("move $v0, %s", _reg_name(x));
    }
    else if (_tac_quadruple(return_).var->kind == OP_REFERENCE)
    {
        x = get_register(_tac_quadruple(return_).var);
        _mips_iprintf("move $v0, %s", _reg_name(x));
    }
    else
    {
        _mips_iprintf("Too Naive.");
    }
    restore_sp();
    _mips_iprintf("jr $ra");
    return return_->next;
}

int deced_size = -32768;

tac *emit_dec(tac *dec)
{
    /* NO NEED TO IMPLEMENT */
    /* COMPLETE Sorry there are bugs. */
    Register x = fp;
    VarDesc *p = alloc_stack_space(_tac_quadruple(dec).var);
    Register y = get_register_w(_tac_quadruple(dec).var);
    _mips_iprintf("move %s, %s", _reg_name(y), _reg_name(x));
    _mips_iprintf("addi %s, %s, %d", _reg_name(x), _reg_name(x), _tac_quadruple(dec).size);
    _mips_iprintf("sw %s, -%d($sp)", _reg_name(y), p->offset);
    return dec->next;
}

tac *emit_arg(tac *arg)
{
    /* COMPLETED emit function */
    if (arg->prev->code.kind == ARG)
    {
        _mips_printf("That shouldn't happen");
        return arg->next;
    }
    _mips_iprintf("sw $ra, -%d($sp)", stack_offset);
    stack_offset += 4;
    return save_args(arg);
}

tac *emit_call(tac *call)
{
    /* COMPLETED emit function */
    // for (Register r = t0; r <= s7; r++)
    //     spill_register(r);
    if (call->prev->code.kind != ARG)
    {
        _mips_iprintf("sw $ra, -%d($sp)", stack_offset);
        stack_offset += 4;
        total_arg_num = 0;
    }
    _mips_iprintf("addi $sp, $sp, -%d", stack_offset + 4);
    _mips_iprintf("jal %s", _tac_quadruple(call).funcname);

    _mips_iprintf("addi %s, %s, %d", _reg_name(sp), _reg_name(sp), stack_offset + 4 + total_arg_num);
    stack_offset -= 4;
    _mips_iprintf("lw $ra, -%d($sp)", stack_offset);
    Register x;
    x = get_register_w(_tac_quadruple(call).ret);
    _mips_iprintf("move %s, $v0", _reg_name(x));
    save_reg(x);
    return call->next;
}

tac *emit_param(tac *param)
{
    /* COMPLETED emit function */
    _mips_printf("No you shouldn't call me.");
    return param->next;
}

tac *emit_read(tac *read)
{
    Register x = get_register(_tac_quadruple(read).p);

    _mips_iprintf("addi $sp, $sp, -%d", stack_offset + 4);
    _mips_iprintf("sw $ra, 0($sp)");
    _mips_iprintf("jal read");
    _mips_iprintf("lw $ra, 0($sp)");
    _mips_iprintf("addi $sp, $sp, %d", stack_offset + 4);
    _mips_iprintf("move %s, $v0", _reg_name(x));
    save_reg(x);
    return read->next;
}

tac *emit_write(tac *write)
{
    Register x = get_register(_tac_quadruple(write).p);

    _mips_iprintf("move $a0, %s", _reg_name(x));
    _mips_iprintf("addi $sp, $sp, -%d", stack_offset + 4);
    _mips_iprintf("sw $ra, 0($sp)");
    _mips_iprintf("jal write");
    _mips_iprintf("lw $ra, 0($sp)");
    _mips_iprintf("addi $sp, $sp, %d", stack_offset + 4);
    return write->next;
}

void emit_preamble()
{
    _mips_printf("# SPL compiler generated assembly");
    _mips_printf(".data");
    _mips_printf("_prmpt: .asciiz \"Enter an integer: \"");
    _mips_printf("_eol: .asciiz \"\\n\"");
    _mips_printf(".globl main");
    _mips_printf(".text");
    _mips_printf("jal main");
    _mips_printf("li $v0, 10");
    _mips_printf("syscall");
}

void emit_read_function()
{
    _mips_printf("read:");
    _mips_iprintf("li $v0, 4");
    _mips_iprintf("la $a0, _prmpt");
    _mips_iprintf("syscall");
    _mips_iprintf("li $v0, 5");
    _mips_iprintf("syscall");
    _mips_iprintf("jr $ra");
}

void emit_write_function()
{
    _mips_printf("write:");
    _mips_iprintf("li $v0, 1");
    _mips_iprintf("syscall");
    _mips_iprintf("li $v0, 4");
    _mips_iprintf("la $a0, _eol");
    _mips_iprintf("syscall");
    _mips_iprintf("move $v0, $0");
    _mips_iprintf("jr $ra");
}

static tac *(*emitter[])(tac *) = {emit_label, emit_function, emit_assign, emit_add,   emit_sub,  emit_mul,
                                   emit_div,   emit_addr,     emit_fetch,  emit_deref, emit_goto, emit_iflt,
                                   emit_ifle,  emit_ifgt,     emit_ifge,   emit_ifne,  emit_ifeq, emit_return,
                                   emit_dec,   emit_arg,      emit_call,   emit_param, emit_read, emit_write};

tac *emit_code(tac *head)
{
    tac *(*tac_emitter)(tac *);
    tac *tac_code = head;
    emit_preamble();
    emit_read_function();
    emit_write_function();
    while (tac_code != NULL)
    {
        if (_tac_kind(tac_code) != NONE)
        {
            tac_emitter = emitter[_tac_kind(tac_code)];
            tac_code = tac_emitter(tac_code);
        }
        else
        {
            tac_code = tac_code->next;
        }
    }
}

/* translate a TAC list into mips32 assembly
   output the textual assembly code to _fd */
void mips32_gen(tac *head, FILE *_fd)
{
    regs[zero].name = "$zero";
    regs[at].name = "$at";
    regs[v0].name = "$v0";
    regs[v1].name = "$v1";
    regs[a0].name = "$a0";
    regs[a1].name = "$a1";
    regs[a2].name = "$a2";
    regs[a3].name = "$a3";
    regs[t0].name = "$t0";
    regs[t1].name = "$t1";
    regs[t2].name = "$t2";
    regs[t3].name = "$t3";
    regs[t4].name = "$t4";
    regs[t5].name = "$t5";
    regs[t6].name = "$t6";
    regs[t7].name = "$t7";
    regs[s0].name = "$s0";
    regs[s1].name = "$s1";
    regs[s2].name = "$s2";
    regs[s3].name = "$s3";
    regs[s4].name = "$s4";
    regs[s5].name = "$s5";
    regs[s6].name = "$s6";
    regs[s7].name = "$s7";
    regs[t8].name = "$t8";
    regs[t9].name = "$t9";
    regs[k0].name = "$k0";
    regs[k1].name = "$k1";
    regs[gp].name = "$gp";
    regs[sp].name = "$sp";
    regs[fp].name = "$fp";
    regs[ra].name = "$ra";
    varmem = (VarDesc *)malloc(sizeof(VarDesc));
    varmem->next = NULL;
    fd = _fd;
    init_gp_counter();
    emit_code(head);
}
