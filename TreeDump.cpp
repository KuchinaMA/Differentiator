#include <stdio.h>
#include <stdlib.h>

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
        case T_OPER:
            fprintf(output, " %c", operation_to_sign(node));
            break;
    }
}

void print_tree_in(Tree* tree, FILE* output) {
    print_node_in(tree->root, output, BEGIN_OP, MID);
}

void print_node_in(const Node* node, FILE* output, int parent_data, int position) {

    if (node == 0) {
        //fprintf(output, " .");
        return;
    }
    if (node->type == T_NUM)
        print_num(node, output);
    else if (node->type == T_OPER)
        print_oper(node, output, parent_data, position);
}

void print_num(const Node* node, FILE* output) {

    print_node_in(node->left, output, node->data, LEFT);
    fprintf(output, " %d", node->data);
    print_node_in(node->right, output, node->data, RIGHT);
}

void print_oper(const Node* node, FILE* output, int parent_data, int position) {

    int bracket = compare_operations(parent_data, node->data, position);
    if (bracket)
        fprintf(output, " (");
    print_node_in(node->left, output, node->data, LEFT);
    fprintf(output, " %c", operation_to_sign(node));
    print_node_in(node->right, output, node->data, RIGHT);
    if (bracket)
        fprintf(output, " )");
}

char operation_to_sign(const Node* node) {
    switch (node->data) {
        case ADD:
            return '+';
            break;
        case SUB:
            return '-';
            break;
        case MUL:
            return '*';
            break;
        case DIV:
            return '/';
            break;
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

    else if (parent_op == MUL)
        if (cur_op == MUL || cur_op == DIV)
            return 0;

        else
            return 1;

    else if (parent_op == DIV)
        if (cur_op == MUL || cur_op == DIV) {
            if (position == LEFT)
                return 0;
        }
        else
            return 1;

    else if (parent_op == BEGIN_OP)
        return 0;
}


int graph_dump(const Tree* tree) {

    FILE* dotfile = fopen("TreePicture.dot", "w");

    fprintf(dotfile, "digraph {\n");
    fprintf(dotfile, "  rankdir = HR;\n");
    fprintf(dotfile, "  node [shape = Mrecord, color = \"#660066\", style = filled, fillcolor = \"#DFBFFF\"];\n");

    node_graph_dump(tree->root, dotfile);
    edge_graph_dump(tree->root, dotfile);

    fprintf(dotfile, "}");

    fclose(dotfile);

    system("dot TreePicture.dot -T png -o TreePicture.png");

    return 0;
}


void node_graph_dump(Node* node, FILE* dotfile) {

    if (node == 0)
        return;
    if (node->type == T_NUM)
        fprintf(dotfile, "  node_%p[label = \" %d \"]; \n", node, node->data);
    else if (node->type == T_OPER)
        fprintf(dotfile, "  node_%p[label = \" %c \", color = \"#000066\", style = filled, fillcolor = \"#D5EAFF\"]; \n", node, operation_to_sign(node));
    node_graph_dump(node->left, dotfile);
    node_graph_dump(node->right, dotfile);

}


void edge_graph_dump(Node* node, FILE* dotfile) {

    if (node->left != 0) {
        fprintf(dotfile, "  node_%p -> node_%p [weight = 1];\n", node, node->left);
        edge_graph_dump(node->left, dotfile);
    }

    if (node->right != 0) {
        fprintf(dotfile, "  node_%p -> node_%p [weight = 1];\n", node, node->right);
        edge_graph_dump(node->right, dotfile);
    }
}



