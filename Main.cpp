#include <stdio.h>

#include "Tree.h"
#include "TreeDump.h"
#include "ReadData.h"
#include "Diff.h"

int main() {

    FILE* input = fopen("TestData4.txt", "r");
    FILE* diff = fopen("TreeDumpDiff4.tex", "w");

    MathExpression* exp = read_data(input);

    make_tex_file(exp, diff);

    expression_dtor(exp);
    fclose(input);
    fclose(diff);

    return 0;
}
