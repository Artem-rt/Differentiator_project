#ifndef DIFFERENTIATOR_FUNC_CPP
#define DIFFERENTIATOR_FUNC_CPP

#include <stdio.h>
#include <assert.h>
#include  <stdlib.h>

#include "differentiator.h"
#include "DSL_differentiator.h"

Node* create_node (Type_Data type_elem, union Data_Elem value, Node* left_child, Node* right_child)
{
    // no assert (...), because Node* _smth_ can be == NULL.
    Node* new_node = NULL;

    if ((new_node = (Node*)calloc (1, sizeof (struct Node))) == NULL) assert (0 && "ERROR in calloc for new node\n");

    new_node -> parent_node = NULL;
    new_node -> type = type_elem;
    new_node -> value = value;
    new_node -> left_child_node = left_child;
    new_node -> right_child_node = right_child;

    if (left_child) left_child -> parent_node = new_node;
    if (right_child) right_child -> parent_node = new_node;

    return new_node;
}

void delete_tree (Node* root_node)
{
    assert (root_node);
    if (root_node -> left_child_node)
        delete_tree (root_node -> left_child_node);
    if (root_node -> right_child_node)
        delete_tree (root_node -> right_child_node);

    free (root_node);
    root_node = NULL;
}

void print_tree_inorder (Node* start_node)
{
    assert (start_node);

    printf ("(");
    if (start_node -> left_child_node) print_tree_inorder (start_node -> left_child_node);
    else printf ("(#)");
    if (start_node -> type == OP) printf ("%d_oper", start_node -> value.type_oper);
    else if (start_node -> type == NUM) printf ("%d_num", start_node ->value.value_num);
    else printf ("%d_var", start_node ->value.type_var);
    if (start_node -> right_child_node) print_tree_inorder (start_node -> right_child_node);
    else printf ("(#)");
    printf (" )");
}

double calculate_tree (Node* cur_node, Variables* variables)
{
    assert (variables);
    // if (cur_node == NULL) return ; !!!!!!!!!!!!!!!!!!

    if (cur_node -> type == OP)
        switch ((int)cur_node -> value.type_oper)
        {
            case ADD:
                return (calculate_tree (cur_node -> left_child_node, variables) + calculate_tree(cur_node -> right_child_node, variables));
            break;

            case SUB:
                return (calculate_tree (cur_node -> left_child_node, variables) - calculate_tree(cur_node -> right_child_node, variables));
            break;

            case DIV:
                return (calculate_tree (cur_node -> left_child_node, variables) / calculate_tree(cur_node -> right_child_node, variables));
            break;

            case MULL:
                return (calculate_tree (cur_node -> left_child_node, variables) * calculate_tree(cur_node -> right_child_node, variables));
            break;

            default: assert (0 && "Error in calculate_tree in switch of decode type of operators\n");
            break;
        }


    if (cur_node -> type == NUM)
        return cur_node -> value.value_num;

    if (cur_node -> type == VAR)
        switch ((int)cur_node -> value.type_var)
        {
            case A_VAR : return variables -> A_VAR;
            break;

            case B_VAR : return variables -> B_VAR;
            break;

            case C_VAR : return variables -> C_VAR;
            break;

            case X_VAR : return variables -> X_VAR;
            break;

            case Y_VAR : return variables -> Y_VAR;
            break;

            case Z_VAR : return variables -> Z_VAR;
            break;

            default : assert (0 && "Error in calculate_tree in switch of decode variables\n");
        }
    return POISON;
}
#endif
