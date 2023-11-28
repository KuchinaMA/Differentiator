#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include "Tree.h"
#include "TreeDump.h"

void print_node_pre(const Node* node, FILE* output) {

    if (node == 0) {
        fprintf(output, " .");
        return;
    }
    fprintf(output, " (");
    print_data(node, output);
    print_node_pre(node->left, output);
    print_node_pre(node->right, output);
    fprintf(output, " )");
}

void print_data(const Node* node, FILE* output) {

    switch (node->type) {
        case T_NUM:
            fprintf(output, " %d", node->data);
            break;
        case T_OP:
            fprintf(output, " %s", operation_to_sign(node));
            break;
    }
}

void print_tree_in(const MathExpression* expression, FILE* output) {
    print_node_in(expression, expression->tree->root, output, BEGIN_OP, MID);
}

void print_node_in(const MathExpression* expression, const Node* node, FILE* output, int parent_data, int position) {

    if (node == 0)
        return;

    if (node->type == T_NUM)
        print_num(expression, node, output);
    else if (node->type == T_OP)
        print_oper(expression, node, output, parent_data, position);

    else if (node->type == T_VAR)
        print_var(expression, node, output);
}

void print_num(const MathExpression* expression, const Node* node, FILE* output) {

    print_node_in(expression, node->left, output, node->data, LEFT);
    fprintf(output, " %d", node->data);
    print_node_in(expression, node->right, output, node->data, RIGHT);
}

void print_oper(const MathExpression* expression, const Node* node, FILE* output, int parent_data, int position) {

    int bracket = compare_operations(parent_data, node->data, position);
    if (bracket)
        fprintf(output, " (");
    print_node_in(expression, node->left, output, node->data, LEFT);
    fprintf(output, " %s", operation_to_sign(node));
    print_node_in(expression, node->right, output, node->data, RIGHT);
    if (bracket)
        fprintf(output, " )");
}

void print_var(const MathExpression* expression, const Node* node, FILE* output) {

    print_node_in(expression, node->left, output, node->data, LEFT);
    fprintf(output, " %s", expression->variables[node->data].name);
    print_node_in(expression, node->right, output, node->data, RIGHT);
}


char* operation_to_sign(const Node* node) {

    switch (node->data) {
        case ADD:
            return "+";
        case SUB:
            return "-";
        case MUL:
            return "*";
        case DIV:
            return "/";
        case LN:
            return "ln";
        case POW:
            return "^";
        case SIN:
            return "sin";
        case COS:
            return "cos";
        case TAN:
            return "tan";
        case CTG:
            return "ctg";
    }
}


int compare_operations(int parent_op, int cur_op, int position) {

    if (parent_op == ADD)
        return 0;

    else if (parent_op == SUB) {
        if (cur_op == ADD || cur_op == SUB) {
            if (position == RIGHT)
                return 1;
        }
        else
            return 0;
    }

    else if (parent_op == MUL) {
        if (cur_op == MUL || cur_op == DIV)
            return 0;

        else
            return 1;
    }

    else if (parent_op == DIV) {
        if (cur_op == MUL || cur_op == DIV) {
            if (position == LEFT)
                return 0;
        }
        else
            return 1;
    }

    else if (parent_op == BEGIN_OP)
        return 0;
}


int graph_dump(const MathExpression* expression) {

    FILE* dotfile = fopen("TreePicture4.dot", "w");

    fprintf(dotfile, "digraph {\n");
    fprintf(dotfile, "  rankdir = HR;\n");
    fprintf(dotfile, "  node [shape = Mrecord, color = \"#660066\", style = filled, fillcolor = \"#DFBFFF\"];\n");

    node_graph_dump(expression, expression->tree->root, dotfile);
    edge_graph_dump(expression->tree->root, dotfile);

    fprintf(dotfile, "}");

    fclose(dotfile);

    system("dot TreePicture4.dot -T png -o TreePicture4.png");

    return 0;
}


void node_graph_dump(const MathExpression* expression, const Node* node, FILE* dotfile) {

    if (node == 0)
        return;
    if (node->type == T_NUM)
        fprintf(dotfile, "  node_%p[label = \" %d \"]; \n", node, node->data);
    else if (node->type == T_OP)
        fprintf(dotfile, "  node_%p[label = \" %s \", color = \"#000066\", style = filled, fillcolor = \"#D5EAFF\"]; \n", node, operation_to_sign(node));
    else if (node->type == T_VAR)
        fprintf(dotfile, "  node_%p[label = \" %s \", color = \"#660033\", style = filled, fillcolor = \"#FFD5EA\"]; \n", node, expression->variables[node->data].name);
    node_graph_dump(expression, node->left, dotfile);
    node_graph_dump(expression, node->right, dotfile);

}


void edge_graph_dump(const Node* node, FILE* dotfile) {

    if (node->left != 0) {
        fprintf(dotfile, "  node_%p -> node_%p [weight = 1];\n", node, node->left);
        edge_graph_dump(node->left, dotfile);
    }

    if (node->right != 0) {
        fprintf(dotfile, "  node_%p -> node_%p [weight = 1];\n", node, node->right);
        edge_graph_dump(node->right, dotfile);
    }
}

void print_tree_tex(const MathExpression* expression, FILE* output) {
    fprintf(output, "\\[");
    print_node_tex(expression, expression->tree->root, output, BEGIN_OP, MID);
    fprintf(output, "\\]");
}


void print_node_tex(const MathExpression* expression, const Node* node, FILE* output, int parent_data, int position) {

    if (node == 0)
        return;

    if (node->type == T_NUM)
        print_num(expression, node, output);
    else if (node->type == T_OP)
        print_oper_tex(expression, node, output, parent_data, position);
    else if (node->type == T_VAR)
        print_var(expression, node, output);
}

void print_oper_tex(const MathExpression* expression, const Node* node, FILE* output, int parent_data, int position) {

    if (node->data == DIV) {
        fprintf(output, " \\frac{");
        print_node_tex(expression, node->left, output, node->data, LEFT);
        fprintf(output, "}{");
        print_node_tex(expression, node->right, output, node->data, RIGHT);
        fprintf(output, "}");
    }

    else if (node->data == POW) {

        print_node_tex(expression, node->left, output, node->data, LEFT);
        fprintf(output, " %s {", operation_to_sign(node));
        print_node_tex(expression, node->right, output, node->data, RIGHT);
        fprintf(output, "}");
    }

    else {
        int bracket = compare_operations_tex(parent_data, node->data, position);
        if (bracket)
            fprintf(output, " (");

        print_node_tex(expression, node->left, output, node->data, LEFT);

        if (node->data == MUL)
            fprintf(output, " \\cdot");
        else
            fprintf(output, " %s", operation_to_sign(node));

        print_node_tex(expression, node->right, output, node->data, RIGHT);

        if (bracket)
            fprintf(output, " )");
    }
}

int compare_operations_tex(int parent_op, int cur_op, int position) {

    switch(parent_op) {

        case ADD:
            return 0;

        case SUB:

            if (cur_op == ADD || cur_op == SUB) {
            if (position == RIGHT)
                return 1;
            }
            else
                return 0;

        case MUL:

            if (cur_op == ADD || cur_op == SUB)
                return 1;

            else
                return 0;

        case DIV:
            return 0;

        case POW:
            if (position == LEFT)
                return 1;
            else
                return 0;

        case LN:
            return 1;

        case SIN:
            return 1;

        case COS:
            return 1;

        case TAN:
            return 1;

        case CTG:
            return 1;

        case BEGIN_OP:
            return 0;
    }

}



