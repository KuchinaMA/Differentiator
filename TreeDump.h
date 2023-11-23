#ifndef TREEDUMP_H_INCLUDED
#define TREEDUMP_H_INCLUDED

const int BEGIN_OP = 20;
enum Positions{
    LEFT  = -1,
    MID   =  0,
    RIGHT =  1,
};

void print_node_pre(const Node* node, FILE* output);
void print_data(const Node* node, FILE* output);

void print_tree_in(Tree* tree, FILE* output);
void print_node_in(const Node* node, FILE* output, int parent_data, int position);
void print_num(const Node* node, FILE* output);
void print_oper(const Node* node, FILE* output, int parent_data, int position);

char operation_to_sign(const Node* node);
int compare_operations(int parent_op, int cur_op, int position);

int graph_dump(const Tree* tree);
void node_graph_dump(Node* node, FILE* dotfile);
void edge_graph_dump(Node* node, FILE* dotfile);

#endif // TREEDUMP_H_INCLUDED
