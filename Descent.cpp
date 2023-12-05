#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <strings.h>

#include "Tree.h"
#include "Descent.h"

static const char* line = NULL;
static int pointer = 0;
static Variable variables_table[MAX_VARS_NUM] = {};
static int vars_num = 0;

MathExpression* new_exp(const char* str) {

    Node* node = get_G(str);

    Tree* new_tree = tree_ctor(node, 6);
    MathExpression* new_expression = expression_ctor(new_tree);
    copy_variables(new_expression, variables_table);

    return new_expression;
}


Node* get_G(const char* str) {

    line = str;
    pointer = 0;

    Node* node = get_E();
    assert(line[pointer] == '\0');


    return node;
}

Node* get_N() {

    int value = 0;
    int old_pointer = pointer;
    while ('0' <= line[pointer] && line[pointer] <= '9') {
        value = value*10 + line[pointer] - '0';
        pointer++;
    }
    if (old_pointer < pointer)
        return node_ctor(T_NUM, value, NULL, NULL);
}

Node* get_E() {

    Node* node = get_T();

    while  (line[pointer] == '+' || line[pointer] == '-') {

        char oper = line[pointer];
        pointer++;

        Node* node2 = get_T();

        switch (oper) {

            case '+': {
                node = node_ctor(T_OP, ADD, node, node2);
                break;
            }

            case '-': {
                node = node_ctor(T_OP, SUB, node, node2);
                break;
            }
            default:
                printf("Syntax error!\n");
        }
    }
    return node;
}

Node* get_T() {

    Node* node = get_Power();

    while (line[pointer] == '*' || line[pointer] == '/') {

        char oper = line[pointer];
        pointer++;

        Node* node2 = get_Power();

        switch (oper) {

            case '*': {
                node = node_ctor(T_OP, MUL, node, node2);
                break;
            }

            case '/': {
                node = node_ctor(T_OP, DIV, node, node2);
                break;
            }
            default:
                printf("Syntax error!\n");
        }
    }
    return node;
}

Node* get_Power() {

    Node* node = get_P();

    while (line[pointer] == '^') {

        char oper = line[pointer];
        pointer++;

        Node* node2 = get_P();

        if (oper == '^')
            node = node_ctor(T_OP, POW, node, node2);

        else
            printf("Syntax error!\n");
    }

    return node;
}


Node* get_P() {

    if (line[pointer] == '(') {

        pointer++;
        Node* node = get_E();

        assert(line[pointer] == ')');
        pointer++;
        return node;
    }

    else if ('0' <= line[pointer] && line[pointer] <= '9')
        return get_N();

    else {

        int ident = get_Id();
        switch(ident) {

            case SIN: {

                if (line[pointer] == '(') {

                    pointer++;
                    Node* node = get_E();

                    assert(line[pointer] == ')');
                    pointer++;
                    return node_ctor(T_OP, SIN, NULL, node);
                }
            }

            case COS: {
                if (line[pointer] == '(') {

                    pointer++;
                    Node* node = get_E();

                    assert(line[pointer] == ')');
                    pointer++;
                    return node_ctor(T_OP, COS, NULL, node);
                }
            }

            case TAN: {
                if (line[pointer] == '(') {

                    pointer++;
                    Node* node = get_E();

                    assert(line[pointer] == ')');
                    pointer++;
                    return node_ctor(T_OP, TAN, NULL, node);
                }
            }

            case CTG: {
                if (line[pointer] == '(') {

                    pointer++;
                    Node* node = get_E();

                    assert(line[pointer] == ')');
                    pointer++;
                    return node_ctor(T_OP, CTG, NULL, node);
                }
            }

            case LN: {
                if (line[pointer] == '(') {

                    pointer++;
                    Node* node = get_E();

                    assert(line[pointer] == ')');
                    pointer++;
                    return node_ctor(T_OP, LN, NULL, node);
                }
            }

            default:
                return node_ctor(T_VAR, (-1)*ident, NULL, NULL);
        }

    }

}

int get_Id() {

    char token[MAX_LINE_LEN] = "";
    int old_pointer = pointer;
    int i = 0;
    while ('a' <= line[pointer] && line[pointer] <= 'z') {
        token[i] = line[pointer];
        i++;
        pointer++;
    }

    if (strcmp(token, "sin") == 0)
        return SIN;


    else if (strcmp(token, "cos") == 0)
        return COS;


    else if (strcmp(token, "tan") == 0)
        return TAN;


    else if (strcmp(token, "ctg") == 0)
        return CTG;


    else if (strcmp(token, "ln") == 0)
        return LN;

    else {

        int variable = find_variable((char*)line[pointer], variables_table);

        if (variable == -1) {
            variables_table[vars_num].name = strdup(token);
            variables_table[vars_num].value = DEFAULT_VAR_VALUE;
            variable = vars_num;
            vars_num ++;
        }
        return (-1)*variable;
    }
}



int find_variable(char* name, Variable* table) {

    for (int i = 0; i < MAX_VARS_NUM; i++) {
        if (table[i].name == name)
            return i;
    }
    return -1;
}

void copy_variables(MathExpression* new_exp, const Variable* variables_table) {

    assert(new_exp);

    for (size_t i = 0; i < vars_num; i++)
    {
        new_exp->variables_table[i].name  = strdup(variables_table[i].name);
        new_exp->variables_table[i].value = variables_table[i].value;
    }
}




