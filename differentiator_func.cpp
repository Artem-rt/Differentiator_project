#ifndef DIFFERENTIATOR_FUNC_CPP
#define DIFFERENTIATOR_FUNC_CPP

#include <stdio.h>
#include <assert.h>
#include  <stdlib.h>
#include <math.h>

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
    else if (start_node -> type == NUM) printf ("%lg_num", start_node ->value.value_num);
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

            case POW:
                return pow(calculate_tree (cur_node -> left_child_node, variables), calculate_tree(cur_node -> right_child_node, variables));
            break;

            case LOG:
                return log (calculate_tree (cur_node -> right_child_node, variables)) / log (calculate_tree(cur_node -> left_child_node, variables));
            break;

            case SIN:
                return sin (calculate_tree (cur_node -> left_child_node, variables));
            break;

            case COS:
                return cos (calculate_tree (cur_node -> left_child_node, variables));
            break;

            case TG :
                return tan(calculate_tree (cur_node -> left_child_node, variables));
            break;

            case CTG :
                return 1 / tan(calculate_tree (cur_node -> left_child_node, variables));

            case ARCSIN:
                return asin (calculate_tree (cur_node -> left_child_node, variables));
            break;

            case ARCCOS:
                return acos (calculate_tree (cur_node -> left_child_node, variables));
            break;

            case ARCTG :
                return atan(calculate_tree (cur_node -> left_child_node, variables));
            break;

            case ARCCTG :
                return atan(1) - atan(calculate_tree (cur_node -> left_child_node, variables));

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

void dump_tree (Node* root_node)
{
    assert (root_node);

    FILE* dump_file = NULL;
    static int cnt_call = 0;
    char name_command [512];

    cnt_call ++;
    sprintf (name_command, "dot " DUMP_FILE " -Tsvg -o dumps/dump_%d.svg", cnt_call);

    if ((dump_file = fopen(DUMP_FILE, "w")) == NULL)
    {
        printf ("sorry, dump_file can not open :(\n");
        assert (false);
    }

    fprintf (dump_file, "digraph DUMP_LIST {\n");
    fprintf (dump_file, "newrank = true;\nnode [shape = Mrecord];\n");
    dump_node (root_node, dump_file);
    fprintf (dump_file, "}");

    fflush (dump_file); //clear buffer
    system (name_command);
    fclose (dump_file);
}

void dump_node (Node* node_dump, FILE* dump_file)
{
    assert (node_dump);
    assert (dump_file);

    if (node_dump -> type == OP)
    {
        operator_type_dump (node_dump, dump_file);
    } else if (node_dump -> type == NUM)
    {
        fprintf (dump_file, "node%p [shape = circle, color = \"#480b41\", fillcolor = \"#ff00d953\", style = filled, label = \"%lg\"];\n", node_dump, node_dump -> value.value_num);
    } else
    {
        variable_type_dump (node_dump, dump_file);
    }

    if (node_dump -> left_child_node)
    {
        dump_node (node_dump -> left_child_node, dump_file);
        dump_link (node_dump, node_dump -> left_child_node, dump_file);
    }
    if (node_dump -> right_child_node)
    {
        dump_node (node_dump -> right_child_node, dump_file);
        dump_link (node_dump, node_dump -> right_child_node, dump_file);
    }
    if (node_dump -> left_child_node && node_dump -> right_child_node)
    {
        dump_rang (node_dump -> left_child_node, node_dump -> right_child_node, dump_file);
    }
}

void operator_type_dump (Node* node_dump, FILE* dump_file)
{
    assert (node_dump);
    assert (dump_file);

    switch ((int)node_dump -> value.type_oper)
    {
        case ADD :
            fprintf (dump_file, "node%p [shape = square, color = \"#1f0b48\", fillcolor = \"#0400ff77\", style = filled, label = \"+\"];\n",
             node_dump);
        break;

        case SUB :
            fprintf (dump_file, "node%p [shape = square, color = \"#1f0b48\", fillcolor = \"#0400ff77\", style = filled, label = \"-\"];\n", node_dump);
        break;

        case DIV :
            fprintf (dump_file, "node%p [shape = square, color = \"#1f0b48\", fillcolor = \"#0400ff77\", style = filled, label = \"/\"];\n", node_dump);
        break;

        case MULL :
            fprintf (dump_file, "node%p [shape = square, color = \"#1f0b48\", fillcolor = \"#0400ff77\", style = filled, label = \"*\"];\n", node_dump);
        break;

        case POW :
            fprintf (dump_file, "node%p [shape = square, color = \"#1f0b48\", fillcolor = \"#0400ff77\", style = filled, label = \"^\"];\n", node_dump);
        break;

        case LOG :
            fprintf (dump_file, "node%p [shape = square, color = \"#1f0b48\", fillcolor = \"#0400ff77\", style = filled, label = \"log\"];\n", node_dump);
        break;

        case SIN :
            fprintf (dump_file, "node%p [shape = square, color = \"#1f0b48\", fillcolor = \"#0400ff77\", style = filled, label = \"sin\"];\n", node_dump);
        break;

        case COS :
            fprintf (dump_file, "node%p [shape = square, color = \"#1f0b48\", fillcolor = \"#0400ff77\", style = filled, label = \"cos\"];\n", node_dump);
        break;

        case TG :
            fprintf (dump_file, "node%p [shape = square, color = \"#1f0b48\", fillcolor = \"#0400ff77\", style = filled, label = \"tg\"];\n", node_dump);
        break;

        case CTG :
            fprintf (dump_file, "node%p [shape = square, color = \"#1f0b48\", fillcolor = \"#0400ff77\", style = filled, label = \"ctg\"];\n", node_dump);
        break;

        case ARCSIN :
            fprintf (dump_file, "node%p [shape = square, color = \"#1f0b48\", fillcolor = \"#0400ff77\", style = filled, label = \"arcsin\"];\n", node_dump);
        break;

        case ARCCOS :
            fprintf (dump_file, "node%p [shape = square, color = \"#1f0b48\", fillcolor = \"#0400ff77\", style = filled, label = \"arccos\"];\n", node_dump);
        break;

        case ARCTG :
            fprintf (dump_file, "node%p [shape = square, color = \"#1f0b48\", fillcolor = \"#0400ff77\", style = filled, label = \"arctg\"];\n", node_dump);
        break;

        case ARCCTG :
            fprintf (dump_file, "node%p [shape = square, color = \"#1f0b48\", fillcolor = \"#0400ff77\", style = filled, label = \"arcctg\"];\n", node_dump);
        break;

        default : assert (0 && "Error in oper_type_dump (...) in switch - don't know this operator!\n");

    }
}

void variable_type_dump (Node* node_dump, FILE* dump_file)
{
    assert (node_dump);
    assert (dump_file);

    switch ((int)node_dump -> value.type_var)
    {
        case X_VAR :
            fprintf (dump_file, "node%p [color = \"#20480b\", fillcolor = \"#11712477\", style = filled, label = \"X\"];\n", node_dump);
        break;

        case Y_VAR :
            fprintf (dump_file, "node%p [color = \"#20480b\", fillcolor = \"#11712477\", style = filled, label = \"Y\"];\n", node_dump);
        break;

        case Z_VAR :
            fprintf (dump_file, "node%p [color = \"#20480b\", fillcolor = \"#11712477\", style = filled, label = \"Z\"];\n", node_dump);
        break;

        case A_VAR :
            fprintf (dump_file, "node%p [color = \"#20480b\", fillcolor = \"#11712477\", style = filled, label = \"A\"];\n", node_dump);
        break;

        case B_VAR :
            fprintf (dump_file, "node%p [color = \"#20480b\", fillcolor = \"#11712477\", style = filled, label = \"B\"];\n", node_dump);
        break;

        case C_VAR :
            fprintf (dump_file, "node%p [color = \"#20480b\", fillcolor = \"#11712477\", style = filled, label = \"C\"];\n", node_dump);
        break;
    }
}

void dump_link (Node* parent_node, Node* child_node, FILE* dump_file)
{
    assert (parent_node);
    assert (child_node);
    assert (dump_file);

    if (parent_node -> left_child_node == child_node)
    {
        fprintf (dump_file, "node%p -> node%p;\n", parent_node, child_node);
    } else if (parent_node -> right_child_node == child_node)
    {
        fprintf (dump_file, "node%p -> node%p;\n", parent_node, child_node);
    } else
    {
        printf ("parent_node == %p, parent->left == %p, parent->right == %p, child == %p, child->parent == %p\n",
                parent_node, parent_node -> left_child_node, parent_node -> right_child_node, child_node, child_node -> parent_node);
        assert (0 && "something");
    }
}

void dump_rang (Node* child_one, Node* child_two, FILE* dump_file)
{
    assert (child_one);
    assert (child_two);
    assert (dump_file);

    fprintf (dump_file, "{rank = same; node%p; node%p};\n", child_one, child_two);
}

void latex_dump (Node* start_node, Node* diff_start_node)
{
    assert (start_node);
    assert (diff_start_node);

    FILE* latex_dump = NULL;

    if ((latex_dump = fopen (LATEX_DUMP, "w")) == NULL) assert (0 && "Error in open LaTex file\n");

    fprintf (latex_dump, "\\documentclass{article}\n");
    fprintf (latex_dump, "\\usepackage[utf8]{inputenc}\n");
    fprintf (latex_dump, "\\usepackage[russian]{babel}\n");
    fprintf (latex_dump, "\\usepackage[T2A]{fontenc}\n");
    fprintf (latex_dump, "\\usepackage{amsmath}\n");
    fprintf (latex_dump, "\\DeclareMathOperator{\\arccot}{arccot}\n");
    fprintf (latex_dump, "\\begin{document}\n\n");
    fprintf (latex_dump, "\\section*{Исходная функция f(x)}\n\n$f(x) = ");
    latex_dump_node (start_node, latex_dump);
    fprintf (latex_dump, "$\n\n");
    fprintf (latex_dump, "\\section*{Производная функции - $\\frac{d}{dx}f(x)$}\n\n$f'(x) = ");
    latex_dump_node (diff_start_node, latex_dump);
    fprintf (latex_dump, "$\n\n");
    fprintf (latex_dump, "\\end{document}");

    fclose (latex_dump);
}

void latex_dump_node (Node* cur_node, FILE* latex_dump)
{
    assert (cur_node);
    assert (latex_dump);

    switch ((int)cur_node -> type)
    {
        case OP :
            switch ((int)cur_node -> value.type_oper)
            {
                case ADD : fprintf (latex_dump, "("); latex_dump_node (cur_node -> left_child_node, latex_dump); fprintf (latex_dump, " + "); latex_dump_node (cur_node -> right_child_node, latex_dump); fprintf (latex_dump, ")");
                break;

                case SUB : fprintf (latex_dump, "("); latex_dump_node (cur_node -> left_child_node, latex_dump); fprintf (latex_dump, " - "); latex_dump_node (cur_node -> right_child_node, latex_dump); fprintf (latex_dump, ")");
                break;

                case MULL : latex_dump_node (cur_node -> left_child_node, latex_dump); fprintf (latex_dump, " \\cdot "); latex_dump_node (cur_node -> right_child_node, latex_dump);
                break;

                case DIV : fprintf (latex_dump, "\\frac{"); latex_dump_node (cur_node -> left_child_node, latex_dump); fprintf (latex_dump, "}{"); latex_dump_node (cur_node -> right_child_node, latex_dump); fprintf (latex_dump, "}");
                break;

                case POW : latex_dump_node (cur_node -> left_child_node, latex_dump); fprintf (latex_dump, "^"); latex_dump_node (cur_node -> right_child_node, latex_dump);
                break;

                case LOG : latex_dump_log (cur_node, latex_dump);
                break;

                case SIN : fprintf (latex_dump, "\\sin("); latex_dump_node (cur_node -> left_child_node, latex_dump); fprintf (latex_dump, ")");
                break;

                case COS : fprintf (latex_dump, "\\cos("); latex_dump_node (cur_node -> left_child_node, latex_dump); fprintf (latex_dump, ")");
                break;

                case TG : fprintf (latex_dump, "\\tan("); latex_dump_node (cur_node -> left_child_node, latex_dump); fprintf (latex_dump, ")");
                break;

                case CTG : fprintf (latex_dump, "\\cot("); latex_dump_node (cur_node -> left_child_node, latex_dump); fprintf (latex_dump, ")");
                break;

                case ARCSIN : fprintf (latex_dump, "\\arcsin("); latex_dump_node (cur_node -> left_child_node, latex_dump); fprintf (latex_dump, ")");
                break;

                case ARCCOS : fprintf (latex_dump, "\\arccos("); latex_dump_node (cur_node -> left_child_node, latex_dump); fprintf (latex_dump, ")");
                break;

                case ARCTG : fprintf (latex_dump, "\\arctan("); latex_dump_node (cur_node -> left_child_node, latex_dump); fprintf (latex_dump, ")");
                break;

                case ARCCTG : fprintf (latex_dump, "\\arccot("); latex_dump_node (cur_node -> left_child_node, latex_dump); fprintf (latex_dump, ")");
                break;

                default : assert (0);
            }
        break;

        case VAR :
            switch ((int)cur_node -> value.type_var)
            {
                case X_VAR : fprintf (latex_dump, "x");
                break;

                case Y_VAR : fprintf (latex_dump, "y");
                break;

                case Z_VAR : fprintf (latex_dump, "z");
                break;

                case A_VAR : fprintf (latex_dump, "a");
                break;

                case B_VAR : fprintf (latex_dump, "b");
                break;

                case C_VAR : fprintf (latex_dump, "c");
                break;

                default : assert (0);
            }
        break;

        case NUM : fprintf (latex_dump, "%lg", cur_node -> value.value_num);
        break;

        default : assert (0);
    }
}

void latex_dump_log (Node* cur_node, FILE* latex_dump)
{
    assert (cur_node);
    assert (latex_dump);

    if (cur_node -> left_child_node -> type == NUM && (cur_node -> left_child_node -> value.value_num - exp(1)) < EPS)
    {
        fprintf (latex_dump, "\\ln(");
        latex_dump_node (cur_node -> right_child_node, latex_dump);
        fprintf (latex_dump, ")");
    } else
    {
        fprintf (latex_dump, "\\log_{");
        latex_dump_node (cur_node -> left_child_node, latex_dump);
        fprintf (latex_dump, "}{");
        latex_dump_node (cur_node -> right_child_node, latex_dump);
        fprintf (latex_dump, "}");
    }
}

Node* differentiate (Node* cur_node)
{
    assert (cur_node);

    union Data_Elem spec_var = { } ;

    switch ((int)cur_node -> type)
    {
        case NUM :
            return _NUM(spec_zero);
        break;

        case VAR :
            if (cur_node -> value.type_var == X_VAR)    spec_var.value_num = 1;
            else    spec_var.value_num = 0;
            return _NUM(spec_var);
        break;

        case OP :
            switch ((int)cur_node -> value.type_oper)
            {
                case ADD : return _ADD(_DL , _DR);
                break;

                case SUB : return _SUB(_DL , _DR);
                break;

                case MULL : return _ADD(_MULL(_DL,_CR), _MULL(_CL, _DR));
                break;

                case DIV : return _DIV(_SUB(_MULL(_DL, _CR), _MULL(_CL, _DR)), _MULL(_CR, _CR));
                break;

                case POW : return diff_pow (cur_node);
                break;

                case SIN : return _MULL(_COS(_CL), _DL);
                break;

                case COS : return _MULL(_NUM(spec_min_one), _MULL(_SIN(_CL), _DL));
                break;

                case LOG : return diff_log (cur_node);
                break;

                case TG : return _DIV(_DL, _MULL(_COS(_CL), _COS(_CL)));
                break;

                case CTG : return _DIV(_MULL(_NUM(spec_min_one), _DL), _MULL(_SIN(_CL), _SIN(_CL)));
                break;

                case ARCSIN : return _MULL(_DL, _DIV(_NUM(spec_one), _POW(_SUB(_NUM(spec_one), _MULL(_CL, _CL)), _NUM(spec_one_div_two))));
                break;

                case ARCCOS : return _MULL(_MULL(_NUM(spec_min_one), _DL), _DIV(_NUM(spec_one), _POW(_SUB(_NUM(spec_one), _MULL(_CL, _CL)), _NUM(spec_one_div_two))));
                break;

                case ARCTG : return _MULL(_DL, _DIV(_NUM(spec_one), _ADD(_NUM(spec_one), _MULL(_CL, _CL))));
                break;

                case ARCCTG : return _MULL(_DL, _DIV(_NUM(spec_min_one), _ADD(_NUM(spec_one), _MULL(_CL, _CL))));
                break;

                default : assert (0 && "Error in differetiate (...) in switch - don't know this operator!\n");
            }
    }

    return NULL;
}

Node* cope_node (Node* cur_node)
{
    assert (cur_node);

    // printf ("type:%d///value:%d\n", cur_node ->type, cur_node->value);
    Node* new_node = create_node (cur_node -> type, cur_node -> value, NULL, NULL);
    if (cur_node -> left_child_node)
    {
        Node* new_left = cope_node (cur_node -> left_child_node);
        new_node -> left_child_node = new_left;
        new_left -> parent_node = new_node;
    }
    if (cur_node -> right_child_node)
    {
        Node* new_right = cope_node (cur_node -> right_child_node);
        new_node -> right_child_node = new_right;
        new_right -> parent_node = new_node;
    }
    return new_node;
}

Node* diff_pow (Node* cur_node)
{
    assert (cur_node);

    if (cur_node -> left_child_node -> type == NUM || (cur_node -> left_child_node -> type == VAR && cur_node -> left_child_node -> value.type_var != X_VAR))
    {
        if (cur_node -> right_child_node -> type == NUM || (cur_node -> right_child_node -> type == VAR && cur_node -> right_child_node -> value.type_var != X_VAR)) return _NUM(spec_zero);
        else return _MULL(_POW(_CL, _CR), _LOG(_NUM(spec_exp), _CL));
    } else if (cur_node -> right_child_node -> type == NUM || (cur_node -> right_child_node -> type == VAR && cur_node -> right_child_node -> value.type_var != X_VAR))
    {
        return _MULL(_CR, _POW(_CL, _SUB(_CR, _NUM(spec_one))));
    } else
    {
        return _MULL(_POW(_NUM(spec_exp), _MULL(_LOG(_NUM(spec_exp), _CL), _CR) ), _ADD(_MULL(_LOG(_NUM(spec_exp), _CL), _DR), _MULL(_DIV(_DL,_CL), _CR)));
    }
}

Node* diff_log (Node* cur_node)
{
    assert (cur_node);

    if (cur_node -> left_child_node -> type == NUM)
    {
        if (cur_node -> right_child_node -> type == NUM) return 0;
        else return _DIV(_DR, _MULL(_CR, _LOG(_NUM(spec_exp), _CL)));

    } else
    {
        return _SUB(_DIV(_DR, _MULL(_LOG(_NUM(spec_exp), _CL), _CR)),
            _DIV(_MULL(_DL, _LOG(_NUM(spec_exp), _CR)), _MULL(_CL, _MULL(_LOG(_NUM(spec_exp), _CL), _LOG(_NUM(spec_exp), _CL)))));
    }

    return NULL;
}

int const_holding_num_num (Node* cur_node, Variables* variables, int cnt_call)
{
    assert (cur_node);
    assert (variables);

    if (cur_node -> left_child_node == NULL && cur_node -> right_child_node == NULL)
        return cnt_call;

    if ((cur_node -> left_child_node -> type == NUM && cur_node -> right_child_node -> type == NUM) || (cur_node -> left_child_node -> type == NUM && cur_node -> right_child_node == NULL))
    {
        double result = calculate_tree (cur_node, variables);
        Data_Elem result_of_node = { .value_num = result } ;
        Node* new_node = _NUM(result_of_node);
        new_node -> parent_node = cur_node -> parent_node;
        if (cur_node == cur_node -> parent_node -> left_child_node) cur_node -> parent_node -> left_child_node = new_node;
        else cur_node -> parent_node -> right_child_node = new_node;
        delete_tree (cur_node);
        cnt_call ++ ;
    } else
    {
        const_holding_num_num (cur_node -> left_child_node, variables, cnt_call);
        const_holding_num_num (cur_node -> right_child_node, variables, cnt_call);
    }

    return cnt_call;
}

int const_holding_var_num (Node* cur_node, int cnt_call)
{
    assert (cur_node);

    if (cur_node -> left_child_node == NULL && cur_node -> right_child_node == NULL)
        return cnt_call;

    if (LEFT_1 && RIGHT_NO_NUM && CUR_MULL)
    {
        Node* new_node = _CR;
        if (cur_node == cur_node -> parent_node -> left_child_node) cur_node -> parent_node -> left_child_node = new_node;
        else cur_node -> parent_node -> right_child_node = new_node;
        new_node -> parent_node = cur_node -> parent_node;
        free (cur_node -> left_child_node);
        free (cur_node);
        cnt_call ++ ;
    } else if (LEFT_NO_NUM && RIGHT_1 && CUR_MULL)
    {
        Node* new_node = _CL;
        if (cur_node == cur_node -> parent_node -> left_child_node) cur_node -> parent_node -> left_child_node = new_node;
        else cur_node -> parent_node -> right_child_node = new_node;
        new_node -> parent_node = cur_node -> parent_node;
        free (cur_node -> right_child_node);
        free (cur_node);
        cnt_call ++ ;
    } else if ((LEFT_0 && CUR_MULL) || (RIGHT_0 && CUR_ADD))
    {
        Node* new_node = _CL;
        if (cur_node == cur_node -> parent_node -> left_child_node) cur_node -> parent_node -> left_child_node = new_node;
        else cur_node -> parent_node -> right_child_node = new_node;
        new_node -> parent_node = cur_node -> parent_node;
        delete_tree (cur_node);
        cnt_call ++ ;
    } else if ((RIGHT_0 && CUR_MULL) /*|| (LEFT_0 && CUR_ADD)*/)
    {
        Node* new_node = _CR;
        if (cur_node == cur_node -> parent_node -> left_child_node) cur_node -> parent_node -> left_child_node = new_node;
        else cur_node -> parent_node -> right_child_node = new_node;
        new_node -> parent_node = cur_node -> parent_node;
        delete_tree (cur_node);
        cnt_call ++ ;
    } else
    {
        const_holding_var_num (cur_node -> left_child_node, cnt_call);
        const_holding_var_num (cur_node -> right_child_node, cnt_call);
    }

    return cnt_call ;
}
#endif
