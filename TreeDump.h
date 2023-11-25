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

void print_tree_in(const MathExpression* expression, FILE* output);
void print_node_in(const MathExpression* expression, const Node* node, FILE* output, int parent_data, int position);
void print_num(const MathExpression* expression, const Node* node, FILE* output);
void print_oper(const MathExpression* expression, const Node* node, FILE* output, int parent_data, int position);
void print_var(const MathExpression* expression, const Node* node, FILE* output);

char* operation_to_sign(const Node* node);
int compare_operations(int parent_op, int cur_op, int position);

int graph_dump(const MathExpression* expression);
void node_graph_dump(const MathExpression* expression, const Node* node, FILE* dotfile);
void edge_graph_dump(const Node* node, FILE* dotfile);

void print_tree_tex(const MathExpression* expression, FILE* output);
void print_node_tex(const MathExpression* expression, const Node* node, FILE* output, int parent_data, int position);
void print_oper_tex(const MathExpression* expression, const Node* node, FILE* output, int parent_data, int position);
int compare_operations_tex(int parent_op, int cur_op, int position);

#endif // TREEDUMP_H_INCLUDED
