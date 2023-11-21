#ifndef TREE_H_INCLUDED
#define TREE_H_INCLUDED

const int DATA_POISON = -555;
const int TYPE_POISON = 0;
const size_t MAX_LINE_LEN = 100;

enum Types {
    T_NUM  = 1,
    T_OPER = 2,
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


Node* node_ctor(int type, int data, Node* left, Node* right);
int node_dtor(Node* node);

Tree* tree_ctor(Node* root, size_t size);
int tree_dtor(Tree* tree);

Tree* read_data(FILE* file);
Node* read_node(FILE* file);
int count_nodes(FILE* file);

int tree_calculate(Node* node);

#endif // TREE_H_INCLUDED
