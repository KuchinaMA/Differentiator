#ifndef DIFF_H_INCLUDED
#define DIFF_H_INCLUDED

const size_t NUMBER_OF_STRINGS = 5;

Node* derivative(MathExpression* expression, const Node* node, FILE* output, LinesData* text);

Node* diff_operation(MathExpression* expression, const Node* node, FILE* output, LinesData* text);

Node* diff_mul(MathExpression* expression, const Node* node, FILE* output, LinesData* text);
Node* diff_div(MathExpression* expression, const Node* node, FILE* output, LinesData* text);

Node* diff_ln(MathExpression* expression, const Node* node, FILE* output, LinesData* text);
Node* diff_pow(MathExpression* expression, const Node* node, FILE* output, LinesData* text);

Node* diff_sin(MathExpression* expression, const Node* node, FILE* output, LinesData* text);
Node* diff_cos(MathExpression* expression, const Node* node, FILE* output, LinesData* text);
Node* diff_tan(MathExpression* expression, const Node* node, FILE* output, LinesData* text);
Node* diff_ctg(MathExpression* expression, const Node* node, FILE* output, LinesData* text);

Node* copy_node(const Node* node);

bool find_var(Node *node);

MathExpression* diff_expression(MathExpression* expression, FILE* output);

void remove_const_values(MathExpression* expression, Node* node, bool* changes, FILE* output, LinesData* text);
void remove_neutral_elements(MathExpression* expression, Node* node, bool* changes, FILE* output, LinesData* text);
void simplify_expression(MathExpression* expression, FILE* output);

void print_phrase(MathExpression* expression, FILE* output, LinesData* text);
void print_phrase_diff(MathExpression* expression, const Node* node, const Node* res, FILE* output, LinesData* text);


#endif // DIFF_H_INCLUDED
