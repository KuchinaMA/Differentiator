#ifndef TREE_H_INCLUDED
#define TREE_H_INCLUDED

const int DATA_POISON = -555;
const int TYPE_POISON = 0;
const int VAR_POISON = -777;
const size_t MAX_LINE_LEN = 100;
const int MAX_VARS_NUM = 50;
const int DEFAULT_VAR_VALUE = 0;

enum Types {
    T_NUM  = 1,
    T_OP   = 2,
    T_VAR  = 3,
};

enum Operations {
    ADD = 21,
    SUB = 22,
    MUL = 23,
    DIV = 24,

    POW = 25,
    LN  = 26,

    SIN = 27,
    COS = 28,
    TAN = 29,
    CTG = 30,

    OPEN_BR  = 40,
    CLOSE_BR = 41,
    LINE_END = 42,
};

struct Node {
    int type;
    int data;
    Node* left;
    Node* right;
};

struct Tree {
    Node* root;
    size_t size;
};

struct Variable {
    char* name;
    int value;
};

struct MathExpression {
    Tree* tree;
    Variable variables_table[MAX_VARS_NUM];
    size_t vars_num;
};


Node* node_ctor(int type, int data, Node* left, Node* right);
int node_dtor(Node* node);

Tree* tree_ctor(Node* root, size_t size);
int tree_dtor(Tree* tree);

MathExpression* read_data(FILE* file);
Node* read_node(FILE* file, MathExpression* exp);
void read_operation(FILE* file, MathExpression* exp, Node* node);
void read_variable(FILE* file, MathExpression* exp, Node* node, char* current);
bool find_variable_in_table(char* name, MathExpression* exp);

int count_nodes(FILE* file);

int tree_calculate(Node* node);

MathExpression* expression_ctor(Tree* tree);
int expression_dtor(MathExpression* expression);

void copy_variables(MathExpression* new_exp, const MathExpression* old_exp);

#endif // TREE_H_INCLUDED
