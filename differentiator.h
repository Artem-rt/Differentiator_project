#ifndef DIFFERENTIATOR_H
#define DIFFERENTIATOR_H

#define POISON 0xDED

enum Type_Data
{
    OP = 1,
    NUM = 2,
    VAR = 3
};

enum Type_Op
{
    ADD,
    SUB,
    DIV,
    MULL
} ;

enum Type_Var
{
    A_VAR,
    B_VAR,
    C_VAR,
    X_VAR,
    Y_VAR,
    Z_VAR,
}   ;

union Data_Elem
{
    Type_Op type_oper;
    int value_num;
    Type_Var type_var;
} ;

struct Variables
{
    double A_VAR;
    double B_VAR;
    double C_VAR;
    double X_VAR;
    double Y_VAR;
    double Z_VAR;
} ;

struct Node
{
    Type_Data type;
    union Data_Elem value;
    Node* parent_node;
    Node* left_child_node;
    Node* right_child_node;
} ;


Node* create_node (Type_Data type_elem, union Data_Elem value, Node* left_child, Node* right_child);
void delete_tree (Node* root_node);
void print_tree_inorder (Node* start_node);
double calculate_tree (Node* cur_node, Variables* variables);

#endif
