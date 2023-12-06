#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <strings.h>

#include "Tree.h"
#include "Descent.h"
#include "ReadData.h"
#include "Diff.h"

static const char* line = NULL;
static int pointer = 0;
static Variable variables_table[MAX_VARS_NUM] = {};
static int vars_num = 0;
static Node* tokens[MAX_TOKENS_NUMB] = {};
static int tokens_p = 0;

MathExpression* new_exp(const char* str) {

    Node* node = get_G(str);

    Tree* new_tree = tree_ctor(node, 6);
    MathExpression* new_expression = expression_ctor(new_tree);
    copy_vars(new_expression, variables_table);
    new_expression->vars_num = vars_num;

    return new_expression;
}

MathExpression* get_expression(const char* str, int size) {

    string_to_tokens(str, size);

    Node* root = get_G(str);
    Tree* new_tree = tree_ctor(root, 6);
    MathExpression* new_expression = expression_ctor(new_tree);
    copy_vars(new_expression, variables_table);
    new_expression->vars_num = vars_num;

    return new_expression;
}


Node* get_G(const char* str) {

    tokens_p = 0;

    Node* node = get_E();

    assert(tokens[tokens_p]->data == LINE_END);

    return node;
}


Node* get_N() {

    Node* node = tokens[tokens_p];
    tokens_p++;
    return node;
}

Node* get_E() {

    Node* node = get_T();
    while  (tokens[tokens_p]->type == T_OP && (tokens[tokens_p]->data == ADD ||
                                               tokens[tokens_p]->data == SUB)) {

        Node* oper = tokens[tokens_p];
        tokens_p++;

        Node* node2 = get_T();

        node = connect_nodes(oper, node, node2);
    }
    return node;
}

Node* get_T() {

    Node* node = get_Power();

    while  (tokens[tokens_p]->type == T_OP && (tokens[tokens_p]->data == MUL ||
                                               tokens[tokens_p]->data == DIV)) {

        Node* oper = tokens[tokens_p];
        tokens_p++;

        Node* node2 = get_Power();
        node = connect_nodes(oper, node, node2);
    }
    return node;
}


Node* get_Power() {

    Node* node = get_P();

    while (tokens[tokens_p]->type == T_OP && tokens[tokens_p]->data == POW) {

        Node* oper = tokens[tokens_p];
        tokens_p++;

        Node* node2 = get_P();
        node = connect_nodes(oper, node, node2);
    }
    return node;
}


Node* get_P() {

    if (tokens[tokens_p]->type == T_OP && tokens[tokens_p]->data == OPEN_BR) {

        tokens_p++;
        Node* node = get_E();

        assert(tokens[tokens_p]->type == T_OP && tokens[tokens_p]->data == CLOSE_BR);
        tokens_p++;
        return node;
    }

    else if (tokens[tokens_p]->type == T_NUM) {
        return get_N();
    }
    else if (tokens[tokens_p]->type == T_VAR) {
        Node* var = tokens[tokens_p];
        tokens_p++;
        return var;
    }
    else {

        Node* oper = tokens[tokens_p];
        tokens_p++;

        if (tokens[tokens_p]->type == T_OP && tokens[tokens_p]->data == OPEN_BR) {

            tokens_p++;
            Node* node = get_E();

            assert(tokens[tokens_p]->type == T_OP && tokens[tokens_p]->data == CLOSE_BR);
            tokens_p++;
            node = connect_nodes(oper, NULL, node);
            return node;
        }
    }
}

int find_variable(char* name, Variable* table) {

    for (int i = 0; i < MAX_VARS_NUM; i++) {
        if (table[i].name == name)
        //if (strcmp(name, table[i].name) == 0)
            return i;
    }
    return -1;
}

void copy_vars(MathExpression* new_exp, const Variable* variables_table) {

    assert(new_exp);

    for (size_t i = 0; i < vars_num; i++)
    {
        new_exp->variables_table[i].name  = strdup(variables_table[i].name);
        new_exp->variables_table[i].value = variables_table[i].value;
    }
}

void skip_spaces() {

    while (line[pointer] == ' ' || line[pointer] == '\t')
        pointer++;
}


void string_to_tokens(const char* str, int size) {

    line = str;
    pointer = 0;
    int i = 0;

    while(pointer < size) {

        switch(line[pointer]) {

            case ' ':
            case '\t': {

                skip_spaces();
                break;
            }

            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9': {

                tokens[i] = get_Number();
                i++;
                break;
            }

            case '+': {

                tokens[i] = node_ctor(T_OP, ADD, NULL, NULL);
                pointer++;
                i++;
                break;
            }

            case '-': {

                tokens[i] = node_ctor(T_OP, SUB, NULL, NULL);
                pointer++;
                i++;
                break;
            }

            case '*': {

                tokens[i] = node_ctor(T_OP, MUL, NULL, NULL);
                pointer++;
                i++;
                break;
            }

            case '/': {

                tokens[i] = node_ctor(T_OP, DIV, NULL, NULL);
                pointer++;
                i++;
                break;
            }

            case '^': {

                tokens[i] = node_ctor(T_OP, POW, NULL, NULL);
                pointer++;
                i++;
                break;
            }

            case '\0': {

                tokens[i] = node_ctor(T_OP, LINE_END, NULL, NULL);
                i++;
                pointer++;
                break;
            }

            case '(': {

                tokens[i] = node_ctor(T_OP, OPEN_BR, NULL, NULL);
                pointer++;
                i++;
                break;
            }

            case ')': {

                tokens[i] = node_ctor(T_OP, CLOSE_BR, NULL, NULL);
                pointer++;
                i++;
                break;
            }

            default: {

                tokens[i] = get_Id();
                i++;
            }

        }
    }
}

Node* get_Id() {

    char token[MAX_LINE_LEN] = "";
    int old_pointer = pointer;
    int i = 0;
    while ('a' <= line[pointer] && line[pointer] <= 'z') {
        token[i] = line[pointer];
        i++;
        pointer++;
    }

    if (strcmp(token, "sin") == 0)
        return node_ctor(T_OP, SIN, NULL, NULL);


    else if (strcmp(token, "cos") == 0)
        return node_ctor(T_OP, COS, NULL, NULL);


    else if (strcmp(token, "tan") == 0)
        return node_ctor(T_OP, TAN, NULL, NULL);


    else if (strcmp(token, "ctg") == 0)
        return node_ctor(T_OP, CTG, NULL, NULL);


    else if (strcmp(token, "ln") == 0)
        return node_ctor(T_OP, LN, NULL, NULL);

    else {

        int variable = find_variable(token, variables_table);

        if (variable == -1) {
            variables_table[vars_num].name = strdup(token);
            variables_table[vars_num].value = DEFAULT_VAR_VALUE;
            variable = vars_num;
            vars_num ++;
        }
        return node_ctor(T_VAR, variable, NULL, NULL);
    }
}

Node* get_Number() {

    int value = 0;
    int old_pointer = pointer;
    while ('0' <= line[pointer] && line[pointer] <= '9') {
        value = value*10 + line[pointer] - '0';
        pointer++;
    }

    if (old_pointer < pointer)
        return node_ctor(T_NUM, value, NULL, NULL);
}


Node* connect_nodes(Node* node, Node* left, Node* right) {

    node->left  = left;
    node->right = right;

    return node;
}




