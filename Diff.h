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

void remove_const_values(Node* node, bool* changes);
void remove_neutral_elements(Node** node, bool* changes);
void simplify_expression(MathExpression* expression, FILE* output);


#endif // DIFF_H_INCLUDED
