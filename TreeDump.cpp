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

void print_node_in(const Node* node, FILE* output) {

    if (node == 0) {
        fprintf(output, " .");
        return;
    }
    if (node->type != T_NUM)
        fprintf(output, " (");
    print_node_in(node->left, output);
    print_data(node, output);
    print_node_in(node->right, output);
    if (node->type != T_NUM)
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



