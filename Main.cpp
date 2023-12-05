#include <stdio.h>

#include "Tree.h"
#include "Descent.h"
#include "TreeDump.h"
#include "ReadData.h"
#include "Diff.h"

int main() {

    FILE* input = fopen("TestData8.txt", "rb");

    int size = size_of_file(input);
    char *line = (char*) calloc(size + 1, sizeof(char));
    size_t nRead = fread(line, sizeof(char), size, input);
    //printf("%s", line);

    FILE* diff = fopen("TreeDumpDiff8.tex", "w");

    //MathExpression* exp = read_data(input);
    MathExpression* exp = new_exp(line);

    make_tex_file(exp, diff);

    expression_dtor(exp);
    fclose(input);
    fclose(diff);

    return 0;
}
