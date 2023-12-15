#include "treeNode.h"

typedef struct IR_tree
{
    char *stmt; // Only if child == NULL
    struct IR_tree *child;
    struct IR_tree *next;
} IR_tree;

//// add a IR node with its children ////
#define addIRn(self, n, v1, ...)          \
    do                                    \
    {                                     \
        self = new_IR_node(NULL);         \
        self->child = v1;                 \
        make_IR_list(n, v1, __VA_ARGS__); \
    } while (0)

#define addIR1(self, n, v1)       \
    do                            \
    {                             \
        self = new_IR_node(NULL); \
        self->child = v1;         \
    } while (0)
//////// IR node add ends here /////////

IR_tree *new_IR_node(const char *stmt);
void make_IR_list(int cnt, IR_tree *head, ...);
IR_tree *alloc_var_mem(const treeNode *u);

/// @brief Remember to free()
/// @return 
char *alloc_label();

/// @brief Remember to free()
/// @return 
char *alloc_tmpval();

IR_tree *build_paramDec_IR_tree(const treeNode *u);
IR_tree *build_params_IR_tree(const treeNode *u);
IR_tree *build_assign_IR_tree(const char *result, const treeNode *u);
IR_tree *build_arg_IR_tree(const char *varname);
IR_tree *build_args_IR_tree(const treeNode *u);
IR_tree *build_FunDec_IR_tree(const treeNode *u);
IR_tree *build_CompSt_IR_tree(const treeNode *u);

IR_tree *build_defList_IR_tree(const treeNode *u);
IR_tree *build_stmtList_IR_tree(const treeNode *u);
IR_tree *build_def_IR_tree(const treeNode *u);
IR_tree *build_stmt_IR_tree(const treeNode *u);

IR_tree *build_normExp_IR_tree(const treeNode *u);
IR_tree *build_ifExp_IR_tree(const treeNode *u, const char *ltrue, const char *lfalse);
IR_tree *build_default_IR_tree(const treeNode *u);

IR_tree *build_IR_tree(const treeNode *u);
void output_IR_tree(const IR_tree *u, FILE *f);