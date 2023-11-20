#include <stdio.h>

#include "Tree.h"
#include "TreeDump.h"

void print_node_pre(Node* node, FILE* output) {

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

void print_node_in(Node* node, FILE* output) {

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

void print_data(Node* node, FILE* output) {

    switch (node->type) {
        case T_NUM:
            fprintf(output, " %d", node->data);
            break;
        case T_OPER:
            switch (node->data) {
                case ADD:
                    fprintf(output, " +");
                    break;
                case SUB:
                    fprintf(output, " -");
                    break;
                case MUL:
                    fprintf(output, " *");
                    break;
                case DIV:
                    fprintf(output, " /");
                    break;
            }
            break;
    }
}

