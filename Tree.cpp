#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <strings.h>

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



/*Node* read_node(FILE* file) {

    char current[MAX_LINE_LEN] = "";

    Node* node = (Node*)calloc(1, sizeof(Node));
    assert(node && "It's impossible to create new node");

    int number = 0;
    int scan_res = fscanf(file, "%d", &number);

    if (scan_res == 1) {
        node->data = number;
        node->type = T_NUM;
    }

    else {
        fscanf(file, "%s", current);
        if (strcmp(current, "add") == 0)
            node->data = ADD;
        else if (strcmp(current, "sub") == 0)
            node->data = SUB;
        else if (strcmp(current, "mul") == 0)
            node->data = MUL;
        else if (strcmp(current, "div") == 0)
            node->data = DIV;
        node->type = T_OP;
    }

    fscanf(file, "%s", current);

    if (strcmp("(", current) == 0)
        node->left = read_node(file);

    else if (strcmp("nil", current) == 0)
        node->left = 0;

    else
        printf("Syntax error!\n");


    fscanf(file, "%s", current);
    if (strcmp("(", current) == 0)
        node->right = read_node(file);

    else if (strcmp("nil", current) == 0)
        node->right = 0;

    else {
        printf("Syntax error!\n");
        printf("current %s\n", current);
    }


    fscanf(file, "%s", current); //закрывающая скобка

    return node;
}


Tree* read_data(FILE* file) {

    int size = count_nodes(file);

    char current[MAX_LINE_LEN] = "";

    fscanf(file, "%s", current);          //открывающая скобка

    Node* new_node = read_node(file);
    Tree* new_tree = tree_ctor(new_node, size);

    return new_tree;
} */

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
        }
    }

}


MathExpression* expression_ctor(Tree* tree) {

    MathExpression* expression = (MathExpression*)calloc(1, sizeof(MathExpression));
    expression->tree = tree;
    //expression->variables = (Variable*)calloc(MAX_VARS_NUM, sizeof(Variable));
    for (int i = 0; i < MAX_VARS_NUM; i++) {
        expression->variables[i].name = "";
        expression->variables[i].value = DEFAULT_VAR_VALUE;
    }

    expression->vars_num = 0;

    return expression;
}

int expression_dtor(MathExpression* expression) {

    tree_dtor(expression->tree);
    //free(expression->variables);
    for (int i = 0; i < MAX_VARS_NUM; i++) {
        expression->variables[i].name = "";
        expression->variables[i].value = DEFAULT_VAR_VALUE;
    }
    expression->vars_num = 0;

    free(expression);

    return 0;
}




Node* read_node(FILE* file, MathExpression* exp) {

    char current[MAX_LINE_LEN] = "";

    Node* node = (Node*)calloc(1, sizeof(Node));
    assert(node && "It's impossible to create new node");

    int number = 0;
    int scan_res = fscanf(file, "%d", &number);

    if (scan_res == 1) {
        node->data = number;
        node->type = T_NUM;
    }

    else {
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
        else {
            //printf("%s\n", current);
            //printf("a %d\n", exp->vars_num);
            exp->variables[exp->vars_num].name = strdup(current);
            //printf("%s\n", exp->variables[exp->vars_num].name);
            exp->variables[exp->vars_num].value = DEFAULT_VAR_VALUE;
            //printf("%d\n", exp->variables[exp->vars_num].value);
            exp->vars_num ++;
            //printf("m %d\n", exp->vars_num);
            node->data = exp->variables[exp->vars_num].value;
            node->type = T_VAR;
        }

    }

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


MathExpression* read_data(FILE* file) {

    MathExpression* new_exp = (MathExpression*)calloc(1, sizeof(MathExpression));

    int size = count_nodes(file);

    char current[MAX_LINE_LEN] = "";

    fscanf(file, "%s", current);          //открывающая скобка

    Node* new_node = read_node(file, new_exp);
    Tree* new_tree = tree_ctor(new_node, size);

    new_exp->tree = new_tree;

    //printf("%lld\n", new_exp->vars_num);

    return new_exp;
}










