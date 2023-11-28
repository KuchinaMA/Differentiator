#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "Tree.h"
#include "TreeDump.h"
#include "Diff.h"
#include "ReadData.h"

#define dL derivative(node->left)
#define dR derivative(node->right)

#define cL copy_node(node->left)
#define cR copy_node(node->right)

#define num_node(arg)         node_ctor(T_NUM, arg, NULL, NULL)
#define add_node(left, right) node_ctor(T_OP, ADD, left, right)
#define sub_node(left, right) node_ctor(T_OP, SUB, left, right)
#define mul_node(left, right) node_ctor(T_OP, MUL, left, right)
#define div_node(left, right) node_ctor(T_OP, DIV, left, right)
#define pow_node(left, right) node_ctor(T_OP, POW, left, right)
#define ln_node(arg)          node_ctor(T_OP, LN, NULL, arg)
#define sin_node(arg)         node_ctor(T_OP, SIN, NULL, arg)
#define cos_node(arg)         node_ctor(T_OP, COS, NULL, arg)


Node* derivative(const Node* node) {

    assert(node);

    switch (node->type) {

        case T_NUM:
            return num_node(0);

        case T_VAR:
            return num_node(1);

        case T_OP:
            return diff_operation(node);

    }

}


Node* diff_operation(const Node* node) {

    assert(node);

    switch(node->data) {

        case ADD:
            return add_node(dL, dR);

        case SUB:
            return sub_node(dL, dR);

        case MUL:
            return diff_mul(node);

        case DIV:
            return diff_div(node);

        case LN:
            return diff_ln(node);

        case POW:
            return diff_pow(node);

        case SIN:
            return diff_sin(node);

        case COS:
            return diff_cos(node);

        case TAN:
            return diff_tan(node);

        case CTG:
            return diff_ctg(node);
    }
}


Node* copy_node(const Node* node) {

    assert(node);

    Node* new_node = node_ctor(node->type, node->data, node->left, node->right);

    return new_node;
}


Node* diff_mul(const Node* node) {

    assert(node);

    Node* new_left = mul_node(dL, cR);
    Node* new_right = mul_node(cL, dR);

    return add_node(new_left, new_right);
}

Node* diff_div(const Node* node) {

    assert(node);

    Node* numerator_left = mul_node(dL, cR);
    Node* numerator_right = mul_node(cL, dR);

    Node* numerator = sub_node(numerator_left, numerator_right);
    Node* denominator = mul_node(cR, cR);

    return div_node(numerator, denominator);
}

Node* diff_ln(const Node* node) {

    assert(node);

    Node* numerator = num_node(1);
    Node* res = div_node(numerator, cR);

    return mul_node(res, dR);
}

Node* diff_pow(const Node* node) {

    assert(node);

    bool basis_var = find_var(node->left);
    bool indicator_var = find_var(node->right);

    if (!basis_var && !indicator_var)
        return node_ctor(T_NUM, 0, NULL, NULL);

    else if(basis_var && !indicator_var) {

        Node* multiplier = num_node(node->right->data);
        Node* new_indicator = num_node(node->right->data - 1);
        Node* new_degree = pow_node(cL, new_indicator);

        Node* res = mul_node(multiplier, new_degree);

        return mul_node(res, dL);
    }

    else if(!basis_var && indicator_var) {

        Node* multiplier = ln_node(cL);
        Node* new_node = copy_node(node);
        Node* res = mul_node(multiplier, new_node);

        return mul_node(res, dR);
    }

    else
        printf("I can't calculate this derivative yet:(\n");

}


Node* diff_sin(const Node* node) {

    assert(node);

    Node* res = cos_node(cR);

    return mul_node(res, dR);
}

Node* diff_cos(const Node* node) {

    assert(node);

    Node* new_node = sin_node(cR);
    Node* minus_mul = num_node(-1);
    Node* res = mul_node(minus_mul, new_node);

    return mul_node(res, dR);
}

Node* diff_tan(const Node* node) {

    assert(node);

    Node* numerator = num_node(1);

    Node* cos_node1 = cos_node(cR);
    Node* cos_node2 = cos_node(cR);
    Node* denominator = mul_node(cos_node1, cos_node2);

    Node* res = div_node(numerator, denominator);

    return mul_node(res, dR);
}

Node* diff_ctg(const Node* node) {

    assert(node);

    Node* numerator = num_node(-1);

    Node* sin_node1 = sin_node(cR);
    Node* sin_node2 = sin_node(cR);
    Node* denominator = mul_node(sin_node1, sin_node2);

    Node* res = div_node(numerator, denominator);

    return mul_node(res, dR);
}


MathExpression* diff_expression(MathExpression* expression) {

    assert(expression);

    Node* new_root = derivative(expression->tree->root);
    Tree* new_tree = tree_ctor(new_root, expression->tree->size);

    MathExpression* new_expression = expression_ctor(new_tree);

    copy_variables(new_expression, expression);

    return new_expression;
}

bool find_var(Node *node) {

    assert(node);

    if (node->type == T_VAR)
        return true;

    if (node->left != NULL) {
        if (find_var(node->left))
            return true;
    }

    if (node->right != NULL) {
        if (find_var(node->right))
            return true;
    }

    return false;
}

void remove_const_values(Node* node, bool* changes) {

    assert(node);

    if (node->left != NULL)
        remove_const_values(node->left, changes);

    if (node->right != NULL)
        remove_const_values(node->right, changes);

    if (node->type == T_OP && node->left != NULL && node->right != NULL) {

        if (!find_var(node->left) && !find_var(node->right)) {

            *changes = true;

            int subtree_value = tree_calculate(node);
            node->type = T_NUM;
            node->data = subtree_value;

            node_dtor(node->left);
            node_dtor(node->right);

            node->left = NULL;
            node->right = NULL;
        }

    }
}


void remove_neutral_elements(Node* node, bool* changes) {

    assert(node);

    if (node->left != NULL)
        remove_neutral_elements(node->left, changes);

    if (node->right != NULL)
        remove_neutral_elements(node->right, changes);

    if (node->type == T_OP && node->data == ADD) {

        if (node->left->type == T_NUM && node->left->data == 0) {

            *changes = true;

            node_dtor(node->left);
            *node = *node->right;

        }

        else if (node->right->type == T_NUM && node->right->data == 0) {

            *changes = true;

            node_dtor(node->right);
            *node = *node->left;
        }
    }

    else if (node->type == T_OP && node->data == MUL) {

        if ((node->left->type == T_NUM && node->left->data == 0) ||
            (node->right->type == T_NUM &&  node->right->data == 0)) {

            *changes = true;

            node->type = T_NUM;
            node->data = 0;

            node_dtor(node->left);
            node_dtor(node->right);

            node->left = NULL;
            node->right = NULL;
        }

        else if (node->left->type == T_NUM && node->left->data == 1) {

            *changes = true;

            node_dtor(node->left);
            *node = *node->right;
        }

        else if (node->right->type == T_NUM && node->right->data == 1) {

            *changes = true;

            node_dtor(node->right);
            *node = *node->left;
        }
    }

    else if (node->type == T_OP && node->data == POW) {

        if (node->right->type == T_NUM && node->right->data == 0) {

            *changes = true;

            node->type = T_NUM;
            node->data = 1;

            node_dtor(node->left);
            node_dtor(node->right);

            node->left = NULL;
            node->right = NULL;
        }

        else if (node->right->type == T_NUM && node->right->data == 1) {

            *changes = true;

            node_dtor(node->right);
            *node = *node->left;
        }
    }
}


void simplify_expression(MathExpression* expression, FILE* output) {

    assert(expression);

    TextData* phrases_data = (TextData*)calloc(1, sizeof(TextData));

    read_from_file("Phrases.txt", phrases_data);

    bool changes = false;

    do {

        changes = false;

        remove_const_values(expression->tree->root, &changes);

        if (changes) {
            int phrase_number = rand() % NUMBER_OF_STRINGS;
            fprintf(output, "%s", phrases_data->text[phrase_number].pointer);
            print_tree_tex(expression, output);
            fprintf(output, "\n");
            changes = false;
        }

        remove_neutral_elements(expression->tree->root, &changes);

        if (changes) {
            int phrase_number = rand() % NUMBER_OF_STRINGS;
            fprintf(output, "%s", phrases_data->text[phrase_number].pointer);
            print_tree_tex(expression, output);
            fprintf(output, "\n");
            changes = false;
        }

    }
    while(changes);

    free(phrases_data);
}





























