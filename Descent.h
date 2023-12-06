#ifndef DESCENT_H_INCLUDED
#define DESCENT_H_INCLUDED

const int MAX_TOKENS_NUMB = 100;

MathExpression* new_exp(const char* str);
MathExpression* get_expression(const char* str, int size);

Node* get_G(const char* str);
Node* get_N();
Node* get_E();
Node* get_T();
Node* get_P();
Node* get_Power();

Node* get_Id();
Node* get_Number();

int find_variable(char* name, Variable* table);
void copy_vars(MathExpression* new_exp, const Variable* variables_table);

void skip_spaces();

void string_to_tokens(const char* str, int size);

Node* connect_nodes(Node* node, Node* left, Node* right);

#endif // DESCENT_H_INCLUDED
