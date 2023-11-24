#ifndef TREE_H_INCLUDED
#define TREE_H_INCLUDED

const int DATA_POISON = -555;
const int TYPE_POISON = 0;
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
    Variable variables[MAX_VARS_NUM];
    size_t vars_num;
};


Node* node_ctor(int type, int data, Node* left, Node* right);
int node_dtor(Node* node);

Tree* tree_ctor(Node* root, size_t size);
int tree_dtor(Tree* tree);

MathExpression* read_data(FILE* file);
//Tree* read_data(FILE* file);
Node* read_node(FILE* file, MathExpression* exp);
int count_nodes(FILE* file);

int tree_calculate(Node* node);

MathExpression* expression_ctor(Tree* tree);
int expression_dtor(MathExpression* expression);

#endif // TREE_H_INCLUDED
