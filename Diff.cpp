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

#define COMPLEX_FUNC(inner_function)  MUL_NODE(res, inner_function)

#define LEFT     node->left
#define RIGHT    node->right
#define TYPE     node->type
#define DATA     node->data
#define LVALUE   node->left->data
#define RVALUE   node->right->data
#define LTYPE    node->left->type
#define RTYPE    node->right->type


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

    Node* new_node = node_ctor(TYPE, DATA, LEFT, RIGHT);

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

    return COMPLEX_FUNC(dR);
}

Node* diff_pow(MathExpression* expression, const Node* node, FILE* output, LinesData* text) {

    assert(node);

    bool basis_var = find_var(LEFT);
    bool indicator_var = find_var(RIGHT);

    if (!basis_var && !indicator_var)
        return NUM_NODE(0);

    else if(basis_var && !indicator_var) {

        Node* multiplier = NUM_NODE(RVALUE);
        Node* new_indicator = NUM_NODE(RVALUE - 1);
        Node* new_degree = POW_NODE(cL, new_indicator);

        Node* res = MUL_NODE(multiplier, new_degree);

        return COMPLEX_FUNC(dL);
    }

    else if(!basis_var && indicator_var) {

        Node* multiplier = LN_NODE(cL);
        Node* new_node = copy_node(node);
        Node* res = MUL_NODE(multiplier, new_node);

        return COMPLEX_FUNC(dR);
    }

    else
        printf("I can't calculate this derivative yet:(\n");

}


Node* diff_sin(MathExpression* expression, const Node* node, FILE* output, LinesData* text) {

    assert(node);

    Node* res = COS_NODE(cR);

    return COMPLEX_FUNC(dR);
}

Node* diff_cos(MathExpression* expression, const Node* node, FILE* output, LinesData* text) {

    assert(node);

    Node* new_node = SIN_NODE(cR);
    Node* minus_mul = NUM_NODE(-1);
    Node* res = MUL_NODE(minus_mul, new_node);

    return COMPLEX_FUNC(dR);
}

Node* diff_tan(MathExpression* expression, const Node* node, FILE* output, LinesData* text) {

    assert(node);

    Node* numerator = NUM_NODE(1);

    Node* cos_node1 = COS_NODE(cR);
    Node* cos_node2 = COS_NODE(cR);
    Node* denominator = MUL_NODE(cos_node1, cos_node2);

    Node* res = DIV_NODE(numerator, denominator);

    return COMPLEX_FUNC(dR);
}

Node* diff_ctg(MathExpression* expression, const Node* node, FILE* output, LinesData* text) {

    assert(node);

    Node* numerator = NUM_NODE(-1);

    Node* sin_node1 = SIN_NODE(cR);
    Node* sin_node2 = SIN_NODE(cR);
    Node* denominator = MUL_NODE(sin_node1, sin_node2);

    Node* res = DIV_NODE(numerator, denominator);

    return COMPLEX_FUNC(dR);
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

    if (TYPE == T_VAR)
        return true;

    if (LEFT != NULL) {
        if (find_var(LEFT))
            return true;
    }

    if (RIGHT != NULL) {
        if (find_var(RIGHT))
            return true;
    }

    return false;
}

void remove_const_values(MathExpression* expression, Node* node, bool* changes, FILE* output, LinesData* text) {

    assert(node);

    if (LEFT != NULL)
        remove_const_values(expression, LEFT, changes, output, text);

    if (RIGHT != NULL)
        remove_const_values(expression, RIGHT, changes, output, text);

    if (TYPE == T_OP && LEFT != NULL && RIGHT != NULL) {

        if (!find_var(LEFT) && !find_var(RIGHT)) {

            *changes = true;

            int subtree_value = tree_calculate(node);
            TYPE = T_NUM;
            DATA = subtree_value;

            node_dtor(LEFT);
            node_dtor(RIGHT);

            LEFT = NULL;
            RIGHT = NULL;

            print_phrase(expression, output, text);

        }

    }
}


void remove_neutral_elements(MathExpression* expression, Node* node, bool* changes, FILE* output, LinesData* text) {

    assert(node);

    if (LEFT != NULL)
        remove_neutral_elements(expression, LEFT, changes, output, text);

    if (RIGHT != NULL)
        remove_neutral_elements(expression, RIGHT, changes, output, text);

    if (TYPE == T_OP && DATA == ADD) {

        if (LTYPE == T_NUM && LVALUE == 0) {

            *changes = true;

            node_dtor(LEFT);
            *node = *RIGHT;

            print_phrase(expression, output, text);

        }

        else if (RTYPE == T_NUM && RVALUE == 0) {

            *changes = true;

            node_dtor(RIGHT);
            *node = *LEFT;

            print_phrase(expression, output, text);
        }
    }

    else if (TYPE == T_OP && DATA == MUL) {

        if ((LTYPE == T_NUM && LVALUE == 0) ||
            (RTYPE == T_NUM &&  RVALUE == 0)) {

            *changes = true;

            TYPE = T_NUM;
            DATA = 0;

            node_dtor(LEFT);
            node_dtor(RIGHT);

            LEFT = NULL;
            RIGHT = NULL;

            print_phrase(expression, output, text);
        }

        else if (LTYPE == T_NUM && LVALUE == 1) {

            *changes = true;

            node_dtor(LEFT);
            *node = *RIGHT;

            print_phrase(expression, output, text);
        }

        else if (RTYPE == T_NUM && RVALUE == 1) {

            *changes = true;

            node_dtor(RIGHT);
            *node = *LEFT;

            print_phrase(expression, output, text);
        }
    }

    else if (TYPE == T_OP && DATA == POW) {

        if (RTYPE == T_NUM && RVALUE == 0) {

            *changes = true;

            TYPE = T_NUM;
            DATA = 1;

            node_dtor(LEFT);
            node_dtor(RIGHT);

            LEFT = NULL;
            RIGHT = NULL;

            print_phrase(expression, output, text);
        }

        else if (RTYPE == T_NUM && RVALUE == 1) {

            *changes = true;

            node_dtor(RIGHT);
            *node = *LEFT;

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
    print_node_tex(expression, node, output, BEGIN_OP, MID_POS);
    fprintf(output, ")' = ");
    print_node_tex(expression, res, output, BEGIN_OP, MID_POS);
    fprintf(output, "\\]");
    fprintf(output, "\n");
}






























