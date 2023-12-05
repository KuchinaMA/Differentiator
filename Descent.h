#ifndef DESCENT_H_INCLUDED
#define DESCENT_H_INCLUDED

MathExpression* new_exp(const char* str);
Node* get_G(const char* str);
Node* get_N();
Node* get_E();
Node* get_T();
Node* get_P();
Node* get_Power();
int get_Id();
int find_variable(char* name, Variable* table);
void copy_variables(MathExpression* new_exp, const Variable* variables_table);

#endif // DESCENT_H_INCLUDED
