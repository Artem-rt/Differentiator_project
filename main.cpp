#include <stdio.h>

#include "differentiator.h"
#include "DSL_differentiator.h"
#include "differentiator_func.cpp"

int main ()
{
    struct Variables var_one = { POISON } ;
    union Data_Elem num_1 = {.value_num = 2} ;
    union Data_Elem num_2 = {.value_num = 5} ;
    union Data_Elem var_1 = {.type_var = X_VAR} ;
    union Data_Elem var_2 = {.type_var = Y_VAR} ;

    var_one.X_VAR = 10;
    var_one.Y_VAR = 20;

    Node* root_node = _DIV(_ADD(_NUM(num_1), _VAR(var_1)), _SUB(_NUM(num_2), _VAR(var_2)));
    print_tree_inorder (root_node);
    printf ("\ncalculate_tree == %lg\n", calculate_tree (root_node, &var_one));
}
