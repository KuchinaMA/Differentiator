#ifndef DIFF_H_INCLUDED
#define DIFF_H_INCLUDED

const size_t NUMBER_OF_STRINGS = 5;

Node* derivative(const Node* node);

Node* diff_operation(const Node* node);

Node* diff_mul(const Node* node);
Node* diff_div(const Node* node);

Node* diff_ln(const Node* node);
Node* diff_pow(const Node* node);

Node* diff_sin(const Node* node);
Node* diff_cos(const Node* node);
Node* diff_tan(const Node* node);
Node* diff_ctg(const Node* node);

Node* copy_node(const Node* node);

bool find_var(Node *node);

MathExpression* diff_expression(MathExpression* expression);

void remove_const_values(MathExpression* expression, Node* node, bool* changes, FILE* output, LinesData* text);
void remove_neutral_elements(MathExpression* expression, Node* node, bool* changes, FILE* output, LinesData* text);
void simplify_expression(MathExpression* expression, FILE* output);
void print_phrase(MathExpression* expression, FILE* output, LinesData* text);


#endif // DIFF_H_INCLUDED
