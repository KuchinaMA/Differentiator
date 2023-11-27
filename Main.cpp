#include <stdio.h>

#include "Tree.h"
#include "TreeDump.h"
#include "Diff.h"

int main() {

    FILE* input = fopen("TestData5.txt", "r");
    FILE* output5 = fopen("TreeDump5.txt", "w");
    FILE* tex = fopen("TreeDumpTex5.txt", "w");
    FILE* diff = fopen("TreeDumpDiff5.txt", "w");

    MathExpression* exp = read_data(input);

    print_tree_in(exp, output5);
    print_tree_tex(exp, tex);

    graph_dump(exp);

    MathExpression* new_exp = diff_expression(exp);
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
