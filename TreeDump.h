#ifndef TREEDUMP_H_INCLUDED
#define TREEDUMP_H_INCLUDED

void print_node_pre(Node* node, FILE* output);
void print_node_in(Node* node, FILE* output);
void print_data(Node* node, FILE* output);

#endif // TREEDUMP_H_INCLUDED
