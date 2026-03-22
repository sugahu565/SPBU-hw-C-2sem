#include "fileFunctions.h"
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    FILE* f1 = fopen("input.csv", "r");
    FILE* f2 = fopen("output.txt", "w");
    fwriteTable(f1, f2);
    return 0;
}
