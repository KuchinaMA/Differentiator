#include <stdio.h>

#include "Tree.h"
#include "TreeDump.h"
#include "ReadData.h"
#include "Diff.h"

int main() {

    FILE* input = fopen("TestData4.txt", "r");
    FILE* diff = fopen("TreeDumpDiff4.tex", "w");

    MathExpression* exp = read_data(input);

    fprintf(diff, "Продифференцируем выражение:\n");
    print_tree_tex(exp, diff);
    fprintf(diff, "\n\n");
    MathExpression* new_exp = diff_expression(exp);

    fprintf(diff, "После дифференцирования получим: \n");
    print_tree_tex(new_exp, diff);
    fprintf(diff, "\n");
    simplify_expression(new_exp, diff);

    graph_dump(new_exp);


    expression_dtor(exp);
    fclose(input);
    fclose(diff);

    return 0;
}
