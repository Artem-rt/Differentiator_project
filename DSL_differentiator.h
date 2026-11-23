#ifndef DSL_DIFFERENTIATOR_H
#define DSL_DIFFERENTIATOR_H

#include "differentiator.h"

const Data_Elem op_add = {ADD};
const Data_Elem op_sub = {.type_oper = SUB};
const Data_Elem op_div = {.type_oper = DIV};
const Data_Elem op_mull = {.type_oper = MULL};
const Data_Elem op_pow = {.type_oper = POW};
const Data_Elem op_log = {.type_oper = LOG};
const Data_Elem op_sin = {.type_oper = SIN};
const Data_Elem op_cos = {.type_oper = COS};
const Data_Elem op_tg = {.type_oper = TG};
const Data_Elem op_ctg = {.type_oper = CTG};
const Data_Elem op_arcsin = {.type_oper = ARCSIN};
const Data_Elem op_arccos = {.type_oper = ARCCOS};
const Data_Elem op_arctg = {.type_oper = ARCTG};
const Data_Elem op_arcctg = {.type_oper = ARCCTG};

#define _ADD(left_child, right_child)  create_node (OP, op_add, left_child, right_child)
#define _SUB(left_child, right_child) create_node (OP, op_sub, left_child, right_child)
#define _DIV(left_child, right_child) create_node (OP, op_div, left_child, right_child)
#define _MULL(left_child, right_child) create_node (OP, op_mull, left_child, right_child)
#define _POW(left_child, right_child) create_node (OP, op_pow, left_child, right_child)
#define _LOG(left_child, right_child) create_node (OP, op_log, left_child, right_child)

#define _SIN(child) create_node (OP, op_sin, child, NULL)
#define _COS(child) create_node (OP, op_cos, child, NULL)
#define _TG(child) create_node (OP, op_tg, child, NULL)
#define _CTG(child) create_node (OP, op_ctg, child, NULL)
#define _ARCSIN(child) create_node (OP, op_arcsin, child, NULL)
#define _ARCCOS(child) create_node (OP, op_arccos, child, NULL)
#define _ARCTG(child) create_node (OP, op_arctg, child, NULL)
#define _ARCCTG(child) create_node (OP, op_arcctg, child, NULL)

#define _NUM(value) create_node (NUM, value, NULL, NULL)
#define _VAR(type_var) create_node (VAR, type_var, NULL, NULL)

#define _DL differentiate (cur_node -> left_child_node)
#define _DR differentiate (cur_node -> right_child_node)

#define _CL cope_node (cur_node -> left_child_node)
#define _CR cope_node (cur_node -> right_child_node)

#define LEFT_1 cur_node -> left_child_node -> type == NUM && fabs(cur_node -> left_child_node -> value.value_num - 1) < EPS
#define LEFT_0 cur_node -> left_child_node -> type == NUM && fabs(cur_node -> left_child_node -> value.value_num - 0) < EPS
#define RIGHT_1 cur_node -> right_child_node -> type == NUM && fabs(cur_node -> right_child_node -> value.value_num - 1) < EPS
#define RIGHT_0 cur_node -> right_child_node -> type == NUM && fabs(cur_node -> right_child_node -> value.value_num - 0) < EPS
#define LEFT_NO_NUM cur_node -> left_child_node -> type != NUM
#define RIGHT_NO_NUM cur_node -> right_child_node -> type != NUM
#define CUR_MULL cur_node -> type == OP && cur_node -> value.type_oper == MULL
#define CUR_POW cur_node -> type == OP && cur_node -> value.type_oper == POW
#define CUR_ADD cur_node -> type == OP && cur_node -> value.type_oper == ADD

#endif
