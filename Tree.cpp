#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <strings.h>
#include <math.h>

#include "Tree.h"

Node* node_ctor(int type, int data, Node* left, Node* right) {

    Node* node = (Node*)calloc(1, sizeof(Node));
    assert(node && "It's impossible to create new node");

    node->type = type;
    node->data = data;
    node->left = left;
    node->right = right;

    return node;
}

int node_dtor(Node* node) {

    if (node->left != 0 && node->right != 0) {
        node_dtor(node->left);
        node_dtor(node->right);
    }

    node->left = NULL;
    node->right = NULL;
    node->data = DATA_POISON;
    node->type = TYPE_POISON;

    free(node);

    return 0;
}

Tree* tree_ctor(Node* root, size_t size) {

    assert(root);

    Tree* tree = (Tree*)calloc(1, sizeof(Tree));
    assert(tree && "It's impossible to create new node");

    tree->root = root;
    tree->size = size;

    return tree;
}


int tree_dtor(Tree* tree) {

    node_dtor(tree->root);
    tree->root = 0;
    tree->size = 0;

    free(tree);

    return 0;
}


int count_nodes(FILE* file) {

    int nodes_num = 0;
    int pos = ftell(file);

    char brackets[MAX_LINE_LEN] = "";
    while (fscanf(file, "%s", brackets) > 0) {
        if (strcmp(brackets, "(") == 0)
            nodes_num++;
    }

    fseek(file, pos, SEEK_SET);

    return nodes_num;
}

int tree_calculate(Node* node) {

    assert(node);

    if (node->type == T_NUM)
        return node->data;
    else if (node->type == T_OP) {
        int left = tree_calculate(node->left);
        int right = tree_calculate(node->right);
        switch (node->data) {
            case ADD:
                return left + right;
            case SUB:
                return left - right;
            case MUL:
                return left * right;
            case DIV:
                return left / right;
            //case LN:
                //return log(right);
            case POW:
                return pow(left, right);
            /*case SIN:
                return sin(right);
            case COS:
                return cos(right);
            case TAN:
                return tan(right);
            case CTG:
                return 1 / tan(right);*/
        }
    }

}


MathExpression* expression_ctor(Tree* tree) {

    assert(tree);

    MathExpression* expression = (MathExpression*)calloc(1, sizeof(MathExpression));
    expression->tree = tree;

    for (int i = 0; i < MAX_VARS_NUM; i++) {
        expression->variables_table[i].name = "";
        expression->variables_table[i].value = DEFAULT_VAR_VALUE;
    }

    expression->vars_num = 0;

    return expression;
}

int expression_dtor(MathExpression* expression) {

    tree_dtor(expression->tree);

    for (int i = 0; i < MAX_VARS_NUM; i++) {
        expression->variables_table[i].name = "";
        expression->variables_table[i].value = DEFAULT_VAR_VALUE;
    }
    expression->vars_num = 0;

    free(expression);

    return 0;
}


Node* read_node(FILE* file, MathExpression* exp) {

    assert(exp);

    char current[MAX_LINE_LEN] = "";

    Node* node = (Node*)calloc(1, sizeof(Node));
    assert(node && "It's impossible to create new node");

    int number = 0;
    int scan_res = fscanf(file, "%d", &number);

    if (scan_res == 1) {
        node->data = number;
        node->type = T_NUM;
    }

    else
        read_operation(file, exp, node);

    fscanf(file, "%s", current);

    if (strcmp("(", current) == 0)
        node->left = read_node(file, exp);

    else if (strcmp("nil", current) == 0)
        node->left = 0;

    else
        printf("Syntax error!\n");


    fscanf(file, "%s", current);
    if (strcmp("(", current) == 0)
        node->right = read_node(file, exp);

    else if (strcmp("nil", current) == 0)
        node->right = 0;

    else {
        printf("Syntax error!\n");
        printf("current %s\n", current);
    }


    fscanf(file, "%s", current); //закрывающая скобка

    return node;
}


void read_operation(FILE* file, MathExpression* exp, Node* node) {

    assert(exp);
    assert(node);

    char current[MAX_LINE_LEN] = "";
    fscanf(file, "%s", current);
    if (strcmp(current, "add") == 0) {
        node->data = ADD;
        node->type = T_OP;
    }
    else if (strcmp(current, "sub") == 0) {
        node->data = SUB;
        node->type = T_OP;
    }
    else if (strcmp(current, "mul") == 0) {
        node->data = MUL;
        node->type = T_OP;
    }
    else if (strcmp(current, "div") == 0) {
        node->data = DIV;
        node->type = T_OP;
    }
    else if (strcmp(current, "ln") == 0) {
        node->data = LN;
        node->type = T_OP;
    }
    else if (strcmp(current, "pow") == 0) {
        node->data = POW;
        node->type = T_OP;
    }
    else if (strcmp(current, "sin") == 0) {
        node->data = SIN;
        node->type = T_OP;
    }
    else if (strcmp(current, "cos") == 0) {
        node->data = COS;
        node->type = T_OP;
    }
    else if (strcmp(current, "tan") == 0) {
        node->data = TAN;
        node->type = T_OP;
    }
    else if (strcmp(current, "ctg") == 0) {
        node->data = CTG;
        node->type = T_OP;
    }
    else
        read_variable(file, exp, node, current);
}


void read_variable(FILE* file, MathExpression* exp, Node* node, char* current) {

    assert(exp);
    assert(node);

    node->data = exp->vars_num;
    node->type = T_VAR;

    if (!find_variable_in_table(current, exp)) {
        exp->variables_table[exp->vars_num].name = strdup(current);
        exp->variables_table[exp->vars_num].value = DEFAULT_VAR_VALUE;
        exp->vars_num ++;
    }
}

bool find_variable_in_table(char* name, MathExpression* exp) {

    assert(exp);

    for (int i = 0; i < MAX_VARS_NUM; i++) {
        if (exp->variables_table[i].name == name)
            return true;
    }
    return false;
}


MathExpression* read_data(FILE* file) {

    MathExpression* new_exp = (MathExpression*)calloc(1, sizeof(MathExpression));

    int size = count_nodes(file);

    char current[MAX_LINE_LEN] = "";

    fscanf(file, "%s", current);          //открывающая скобка

    Node* new_node = read_node(file, new_exp);
    Tree* new_tree = tree_ctor(new_node, size);

    new_exp->tree = new_tree;

    return new_exp;
}


void copy_variables(MathExpression* new_exp, const MathExpression* old_exp) {

    assert(new_exp);
    assert(old_exp);

    for (size_t i = 0; i < old_exp->vars_num; i++) {

        new_exp->variables_table[i].name  = strdup(old_exp->variables_table[i].name);
        new_exp->variables_table[i].value = old_exp->variables_table[i].value;
    }
}










