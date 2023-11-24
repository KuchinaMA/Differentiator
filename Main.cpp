#include <stdio.h>

#include "Tree.h"
#include "TreeDump.h"
#include "Diff.h"

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

    FILE* input = fopen("TestData3.txt", "r");
    /*Tree* tree2 = read_data(input);

    MathExpression* exp = expression_ctor(tree2);
    exp->vars_num = 1;
    exp->variables[0].name = "x";
    exp->variables[0].value = 0;*/


    MathExpression* exp = read_data(input);
    printf("%d    %s    %d", exp->vars_num, exp->variables[0].name, exp->variables[0].value);
    printf("      %s    %d", exp->variables[1].name, exp->variables[1].value);
    printf("      %s    %d", exp->variables[2].name, exp->variables[2].value);


    FILE* output3 = fopen("TreeDump3.txt", "w");
    //print_tree_in(tree2, output2);

    FILE* tex = fopen("TreeDumpTex3.txt", "w");
    //print_tree_tex(tree2, tex);

    //graph_dump(tree2);

    //int res = tree_calculate(tree2->root);
    //printf("%d", res);

    fclose(output3);
    fclose(tex);
    //tree_dtor(tree2);

    return 0;
}
