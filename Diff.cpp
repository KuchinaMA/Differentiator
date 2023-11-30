#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "Tree.h"
#include "TreeDump.h"
#include "ReadData.h"
#include "Diff.h"


#define dL derivative(expression, node->left, output, text)
#define dR derivative(expression, node->right, output, text)

#define cL copy_node(node->left)
#define cR copy_node(node->right)

#define NUM_NODE(arg)         node_ctor(T_NUM, arg, NULL, NULL)
#define ADD_NODE(left, right) node_ctor(T_OP, ADD, left, right)
#define SUB_NODE(left, right) node_ctor(T_OP, SUB, left, right)
#define MUL_NODE(left, right) node_ctor(T_OP, MUL, left, right)
#define DIV_NODE(left, right) node_ctor(T_OP, DIV, left, right)
#define POW_NODE(left, right) node_ctor(T_OP, POW, left, right)
#define LN_NODE(arg)          node_ctor(T_OP, LN, NULL, arg)
#define SIN_NODE(arg)         node_ctor(T_OP, SIN, NULL, arg)
#define COS_NODE(arg)         node_ctor(T_OP, COS, NULL, arg)


Node* derivative(MathExpression* expression, const Node* node, FILE* output, LinesData* text) {

    assert(node);

    switch (node->type) {

        case T_NUM: {
            Node* res = NUM_NODE(0);
            print_phrase_diff(expression, node, res, output, text);
            return res;
        }

        case T_VAR: {
            Node* res = NUM_NODE(1);
            print_phrase_diff(expression, node, res, output, text);
            return res;
        }

        case T_OP: {
            Node* res = diff_operation(expression, node, output, text);
            print_phrase_diff(expression, node, res, output, text);
            return res;
        }

    }

}


Node* diff_operation(MathExpression* expression, const Node* node, FILE* output, LinesData* text) {

    assert(node);

    switch(node->data) {

        case ADD:
            return ADD_NODE(dL, dR);

        case SUB:
            return SUB_NODE(dL, dR);

        case MUL:
            return diff_mul(expression, node, output, text);

        case DIV:
            return diff_div(expression, node, output, text);

        case LN:
            return diff_ln(expression, node, output, text);

        case POW:
            return diff_pow(expression, node, output, text);

        case SIN:
            return diff_sin(expression, node, output, text);

        case COS:
            return diff_cos(expression, node, output, text);

        case TAN:
            return diff_tan(expression, node, output, text);

        case CTG:
            return diff_ctg(expression, node, output, text);
    }
}


Node* copy_node(const Node* node) {

    assert(node);

    Node* new_node = node_ctor(node->type, node->data, node->left, node->right);

    return new_node;
}


Node* diff_mul(MathExpression* expression, const Node* node, FILE* output, LinesData* text) {

    assert(node);

    Node* new_left = MUL_NODE(dL, cR);
    Node* new_right = MUL_NODE(cL, dR);

    return ADD_NODE(new_left, new_right);
}

Node* diff_div(MathExpression* expression, const Node* node, FILE* output, LinesData* text) {

    assert(node);

    Node* numerator_left = MUL_NODE(dL, cR);
    Node* numerator_right = MUL_NODE(cL, dR);

    Node* numerator = SUB_NODE(numerator_left, numerator_right);
    Node* denominator = MUL_NODE(cR, cR);

    return DIV_NODE(numerator, denominator);
}

Node* diff_ln(MathExpression* expression, const Node* node, FILE* output, LinesData* text) {

    assert(node);

    Node* numerator = NUM_NODE(1);
    Node* res = DIV_NODE(numerator, cR);

    return MUL_NODE(res, dR);
}

Node* diff_pow(MathExpression* expression, const Node* node, FILE* output, LinesData* text) {

    assert(node);

    bool basis_var = find_var(node->left);
    bool indicator_var = find_var(node->right);

    if (!basis_var && !indicator_var)
        return NUM_NODE(0);

    else if(basis_var && !indicator_var) {

        Node* multiplier = NUM_NODE(node->right->data);
        Node* new_indicator = NUM_NODE(node->right->data - 1);
        Node* new_degree = POW_NODE(cL, new_indicator);

        Node* res = MUL_NODE(multiplier, new_degree);

        return MUL_NODE(res, dL);
    }

    else if(!basis_var && indicator_var) {

        Node* multiplier = LN_NODE(cL);
        Node* new_node = copy_node(node);
        Node* res = MUL_NODE(multiplier, new_node);

        return MUL_NODE(res, dR);
    }

    else
        printf("I can't calculate this derivative yet:(\n");

}


Node* diff_sin(MathExpression* expression, const Node* node, FILE* output, LinesData* text) {

    assert(node);

    Node* res = COS_NODE(cR);

    return MUL_NODE(res, dR);
}

Node* diff_cos(MathExpression* expression, const Node* node, FILE* output, LinesData* text) {

    assert(node);

    Node* new_node = SIN_NODE(cR);
    Node* minus_mul = NUM_NODE(-1);
    Node* res = MUL_NODE(minus_mul, new_node);

    return MUL_NODE(res, dR);
}

Node* diff_tan(MathExpression* expression, const Node* node, FILE* output, LinesData* text) {

    assert(node);

    Node* numerator = NUM_NODE(1);

    Node* cos_node1 = COS_NODE(cR);
    Node* cos_node2 = COS_NODE(cR);
    Node* denominator = MUL_NODE(cos_node1, cos_node2);

    Node* res = DIV_NODE(numerator, denominator);

    return MUL_NODE(res, dR);
}

Node* diff_ctg(MathExpression* expression, const Node* node, FILE* output, LinesData* text) {

    assert(node);

    Node* numerator = NUM_NODE(-1);

    Node* sin_node1 = SIN_NODE(cR);
    Node* sin_node2 = SIN_NODE(cR);
    Node* denominator = MUL_NODE(sin_node1, sin_node2);

    Node* res = DIV_NODE(numerator, denominator);

    return MUL_NODE(res, dR);
}


MathExpression* diff_expression(MathExpression* expression, FILE* output) {

    assert(expression);

    TextData* diff_phrases = (TextData*)calloc(1, sizeof(TextData));
    read_from_file("DiffPhrases.txt", diff_phrases);

    Node* new_root = derivative(expression, expression->tree->root, output, diff_phrases->text);
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

void remove_const_values(MathExpression* expression, Node* node, bool* changes, FILE* output, LinesData* text) {

    assert(node);

    if (node->left != NULL)
        remove_const_values(expression, node->left, changes, output, text);

    if (node->right != NULL)
        remove_const_values(expression, node->right, changes, output, text);

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

            print_phrase(expression, output, text);

        }

    }
}


void remove_neutral_elements(MathExpression* expression, Node* node, bool* changes, FILE* output, LinesData* text) {

    assert(node);

    if (node->left != NULL)
        remove_neutral_elements(expression, node->left, changes, output, text);

    if (node->right != NULL)
        remove_neutral_elements(expression, node->right, changes, output, text);

    if (node->type == T_OP && node->data == ADD) {

        if (node->left->type == T_NUM && node->left->data == 0) {

            *changes = true;

            node_dtor(node->left);
            *node = *node->right;

            print_phrase(expression, output, text);

        }

        else if (node->right->type == T_NUM && node->right->data == 0) {

            *changes = true;

            node_dtor(node->right);
            *node = *node->left;

            print_phrase(expression, output, text);
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

            print_phrase(expression, output, text);
        }

        else if (node->left->type == T_NUM && node->left->data == 1) {

            *changes = true;

            node_dtor(node->left);
            *node = *node->right;

            print_phrase(expression, output, text);
        }

        else if (node->right->type == T_NUM && node->right->data == 1) {

            *changes = true;

            node_dtor(node->right);
            *node = *node->left;

            print_phrase(expression, output, text);
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

            print_phrase(expression, output, text);
        }

        else if (node->right->type == T_NUM && node->right->data == 1) {

            *changes = true;

            node_dtor(node->right);
            *node = *node->left;

            print_phrase(expression, output, text);
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
        remove_const_values(expression, expression->tree->root, &changes, output, phrases_data->text);
        remove_neutral_elements(expression, expression->tree->root, &changes, output, phrases_data->text);

    }

    while(changes);

    free(phrases_data);
}


void print_phrase(MathExpression* expression, FILE* output, LinesData* text) {

    assert(expression);

    int phrase_number = rand() % NUMBER_OF_STRINGS;
    fprintf(output, "%s", text[phrase_number].pointer);
    print_tree_tex(expression, output);
    fprintf(output, "\n");
}

void print_phrase_diff(MathExpression* expression, const Node* node, const Node* res, FILE* output, LinesData* text) {

    assert(expression);

    int phrase_number = rand() % NUMBER_OF_STRINGS;
    fprintf(output, "%s", text[phrase_number].pointer);
    fprintf(output, "\\[(");
    print_node_tex(expression, node, output, BEGIN_OP, MID);
    fprintf(output, ")' = ");
    print_node_tex(expression, res, output, BEGIN_OP, MID);
    fprintf(output, "\\]");
    fprintf(output, "\n");
}






























