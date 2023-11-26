#include <stdio.h>

#include "Tree.h"
#include "TreeDump.h"
#include "Diff.h"

int main() {

    FILE* input = fopen("TestData4.txt", "r");

    MathExpression* exp = read_data(input);
    /*printf("%d    %s    %d", exp->vars_num, exp->variables[0].name, exp->variables[0].value);
    printf("      %s    %d", exp->variables[1].name, exp->variables[1].value);
    printf("      %s    %d", exp->variables[2].name, exp->variables[2].value);*/


    FILE* output5 = fopen("TreeDump4.txt", "w");
    print_tree_in(exp, output5);

    FILE* tex = fopen("TreeDumpTex4.txt", "w");
    print_tree_tex(exp, tex);

    graph_dump(exp);

    MathExpression* new_exp = diff_expression(exp);

    FILE* diff = fopen("TreeDumpDiff4.txt", "w");
    print_tree_tex(new_exp, diff);


    bool changes = false;
    remove_const_values(new_exp->tree->root, &changes);
    remove_neutral_elements(&(new_exp->tree->root), &changes);

    graph_dump(new_exp);

    fclose(output5);
    fclose(tex);
    fclose(diff);

    return 0;
}
