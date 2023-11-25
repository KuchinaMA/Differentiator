#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "Tree.h"
#include "Diff.h"

#define dL derivative(node->left)
#define dR derivative(node->right)

#define cL copy_node(node->left)
#define cR copy_node(node->right)


Node* derivative(const Node* node) {

    assert(node);

    switch (node->type) {

        case T_NUM: {
            Node* new_node = node_ctor(T_NUM, 0, NULL, NULL);
            return new_node;
        }

        case T_VAR: {
            Node* new_node = node_ctor(T_NUM, 1, NULL, NULL);
            return new_node;
        }

        case T_OP: {
            Node* new_node = diff_operation(node);
            return new_node;
        }

    }

}


Node* diff_operation(const Node* node) {

    assert(node);

    switch(node->data) {

        case ADD:
            return node_ctor(T_OP, ADD, dL, dR);

        case SUB:
            return node_ctor(T_OP, SUB, dL, dR);

        case MUL:
            return diff_mul(node);

        case DIV:
            return diff_div(node);

        case LN:
            return diff_ln(node);
    }
}


Node* copy_node(const Node* node) {

    assert(node);

    Node* new_node = node_ctor(node->type, node->data, node->left, node->right);

    return new_node;
}


Node* diff_mul(const Node* node) {

    assert(node);

    Node* new_left = node_ctor(T_OP, MUL, dL, cR);
    Node* new_right = node_ctor(T_OP, MUL, cL, dR);

    return node_ctor(T_OP, ADD, new_left, new_right);
}

Node* diff_div(const Node* node) {

    assert(node);

    Node* numerator_left = node_ctor(T_OP, MUL, dL, cR);
    Node* numerator_right = node_ctor(T_OP, MUL, cL, dR);

    Node* numerator = node_ctor(T_OP, SUB, numerator_left, numerator_right);
    Node* denominator = node_ctor(T_OP, MUL, cR, cR);

    return node_ctor(T_OP, DIV, numerator, denominator);
}

Node* diff_ln(const Node* node) {

    assert(node);

    Node* numerator = node_ctor(T_NUM, 1, NULL, NULL);
    Node* denominator = copy_node(node->right);

    return node_ctor(T_OP, DIV, numerator, denominator);
}


MathExpression* diff_expression(MathExpression* expression) {

    assert(expression);

    Node* new_root = derivative(expression->tree->root);
    Tree* new_tree = tree_ctor(new_root, expression->tree->size);

    MathExpression* new_expression = expression_ctor(new_tree);

    copy_variables(new_expression, expression);

    return new_expression;
}

















