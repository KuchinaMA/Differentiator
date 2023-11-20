#ifndef TREEDUMP_H_INCLUDED
#define TREEDUMP_H_INCLUDED

void print_node_pre(const Node* node, FILE* output);
void print_node_in(const Node* node, FILE* output);
void print_data(const Node* node, FILE* output);
char operation_to_sign(const Node* node);

int graph_dump(const Tree* tree);
void node_graph_dump(Node* node, FILE* dotfile);
void edge_graph_dump(Node* node, FILE* dotfile);

#endif // TREEDUMP_H_INCLUDED
