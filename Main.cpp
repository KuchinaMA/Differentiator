#include <stdio.h>

#include "Tree.h"
#include "TreeDump.h"
#include "Diff.h"
#include "ReadData.h"

int main() {

    FILE* input = fopen("TestData4.txt", "r");
    FILE* output5 = fopen("TreeDump4.txt", "w");
    FILE* tex = fopen("TreeDumpTex4.txt", "w");
    FILE* diff = fopen("TreeDumpDiff4.txt", "w");

    MathExpression* exp = read_data(input);

    print_tree_in(exp, output5);
    print_tree_tex(exp, tex);

    graph_dump(exp);


    fprintf(diff, "Продифференцируем выражение:\n");
    print_tree_tex(exp, diff);
    fprintf(diff, "\n\n");
    MathExpression* new_exp = diff_expression(exp);

    fprintf(diff, "После дифференцирования получим: \n");
    print_tree_tex(new_exp, diff);
    fprintf(diff, "\n");
    simplify_expression(new_exp, diff);

    graph_dump(new_exp);

    //print_tree_tex(new_exp, diff);

    expression_dtor(exp);
    fclose(input);
    fclose(output5);
    fclose(tex);
    fclose(diff);

    return 0;
}
