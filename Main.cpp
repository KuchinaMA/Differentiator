#include <stdio.h>

#include "Tree.h"
#include "TreeDump.h"

int main() {

    /*Node* node300 = node_ctor(T_NUM, 300, 0, 0);
    Node* node100 = node_ctor(T_NUM, 100, 0, 0);

    Node* node_sub1 = node_ctor(T_OPER, SUB, node300, node100);

    Node* node3 = node_ctor(T_NUM, 30, 0, 0);
    Node* node10 = node_ctor(T_NUM, 10, 0, 0);

    Node* node_sub2 = node_ctor(T_OPER, SUB, node3, node10);

    Node* node_div = node_ctor(T_OPER, DIV, node_sub1, node_sub2);

    Tree* tree1 = tree_ctor(node_div, 7);

    FILE* output = fopen("TreeDump.txt", "w");
    print_node_in(tree1->root, output);
    graph_dump(tree1);

    fclose(output);
    tree_dtor(tree1);*/

    FILE* input = fopen("TestData2.txt", "r");
    Tree* tree2 = read_data(input);

    FILE* output2 = fopen("TreeDump2.txt", "w");
    print_node_in(tree2->root, output2);
    graph_dump(tree2);

    int res = tree_calculate(tree2->root);
    printf("%d", res);

    fclose(output2);
    tree_dtor(tree2);


    return 0;
}
