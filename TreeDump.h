#ifndef TREEDUMP_H_INCLUDED
#define TREEDUMP_H_INCLUDED

const int BEGIN_OP = 20;

void print_node_pre(const Node* node, FILE* output);
void print_data(const Node* node, FILE* output);

void print_node_in(const Node* node, FILE* output, int parent_data);
void print_num(const Node* node, FILE* output);
void print_oper(const Node* node, FILE* output, int parent_data);

char operation_to_sign(const Node* node);
int operation_priority(int oper);

int graph_dump(const Tree* tree);
void node_graph_dump(Node* node, FILE* dotfile);
void edge_graph_dump(Node* node, FILE* dotfile);

#endif // TREEDUMP_H_INCLUDED
