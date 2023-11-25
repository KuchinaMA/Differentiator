#ifndef DIFF_H_INCLUDED
#define DIFF_H_INCLUDED

Node* derivative(const Node* node);

Node* diff_operation(const Node* node);

Node* diff_mul(const Node* node);
Node* diff_div(const Node* node);

Node* diff_ln(const Node* node);

Node* copy_node(const Node* node);

MathExpression* diff_expression(MathExpression* expression);


#endif // DIFF_H_INCLUDED
