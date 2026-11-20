#ifndef DSL_DIFFERETIATOR_H
#define DSL_DIFFERETIATOR_H

#include "differentiator.h"

Data_Elem op_add = {.type_oper = ADD};
Data_Elem op_sub = {.type_oper = SUB};
Data_Elem op_div = {.type_oper = DIV};
Data_Elem op_mull = {.type_oper = MULL};

#define _ADD(left_child, right_child)  create_node (OP, op_add, left_child, right_child)
#define _SUB(left_child, right_child) create_node (OP, op_sub, left_child, right_child)
#define _DIV(left_child, right_child) create_node (OP, op_div, left_child, right_child)
#define _MULL(left_child, right_child) create_node (OP, op_mull, left_child, right_child)

#define _NUM(value) create_node (NUM, value, NULL, NULL)
#define _VAR(type_var) create_node (VAR, type_var, NULL, NULL)

#endif
