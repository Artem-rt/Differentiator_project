#ifndef DIFFERENTIATOR_H
#define DIFFERENTIATOR_H
// TODO ругистратор имен переменных - массив переменных динамический и тд
#include <stdio.h>
#include <math.h>

#define POISON 0xDED
#define LATEX_DUMP "LaTeX_dump.tex"
#define EPS 0.000000001
#define MAX_LEN_DATA 512
#define CNT_OPER 14

#define DUMP_FILE "DUMP_GRAPHVIZ.txt"
#define FUNCTION_FILE "FUNCTION.TXT"

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
    MULL,
    POW,
    LOG,
    SIN,
    COS,
    TG,
    CTG,
    ARCSIN,
    ARCCOS,
    ARCTG,
    ARCCTG
} ;

enum Type_Var //TODO array for vars - not use in - in union you will have ID
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
    double value_num;
    Type_Var type_var;
} ;

struct Variables //TODO array for var
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

struct Decode_File
{
    const char* oper [CNT_OPER];
    const char* var [CNT_OPER];
} ;

const union Data_Elem spec_min_one = { .value_num = -1};
const union Data_Elem spec_one_div_two = { .value_num = 0.5};
const union Data_Elem spec_zero = { .value_num = 0};
const union Data_Elem spec_one = { .value_num = 1};
const union Data_Elem spec_exp = { .value_num = exp(1)};

Node* create_node (Type_Data type_elem, union Data_Elem value, Node* left_child, Node* right_child);
void delete_tree (Node* root_node);
void print_tree_inorder (Node* start_node);
double calculate_tree (Node* cur_node, Variables* variables);

void dump_tree (Node* root_node);
void dump_node (Node* node_dump, FILE* dump_file);
void operator_type_dump (Node* node_dump, FILE* dump_file);
void variable_type_dump (Node* node_dump, FILE* dump_file);
void dump_link (Node* parent_node, Node* child_node, FILE* dump_file);
void dump_rang (Node* child_one, Node* child_two, FILE* dump_file);

void latex_dump (Node* start_node, Node* diff_start_node);
void latex_dump_node (Node* cur_node, FILE* latex_dump);
void latex_dump_log (Node* cur_node, FILE* latex_dump);

Node* differentiate (Node* cur_node);
Node* cope_node (Node* cur_node);
Node* diff_pow (Node* cur_node);
Node* diff_log (Node* cur_node);

int const_holding_num_num (Node* cur_node, Variables* variables, int cnt_call);
int const_holding_var_num (Node* cur_node, int cnt_call);

Node* parse_preorder_tree (int preorder_file);
long long read_preorder_file (int preorder_file, char** buffer);
Node* parse_node (char** buffer, long long* i_buffer, Node* parent_node, int child_flag);
void decode_type_elem_new_node (char* now_data, Type_Data* type_new_node, Data_Elem* value_new_node);

#endif
