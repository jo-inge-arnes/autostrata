#include <cuda.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "structs.h"
#include "data.h"

void print_rawunits(rawunits_t *units) {
    size_t num_cols = units->covar_cnt + 1;

    for (int i = 0; i < units->unit_cnt; i++) {
        printf("%d: [", i);
        for (int j = 0; j < num_cols; j++) {
            printf("%.2f%s", units->data[i * num_cols + j], j < num_cols - 1 ? ", ": "");
        }
        printf("]\n");
    }
}

int main(int argc, char **argv) {
    srand(time(NULL));

    size_t num_units = 20, num_covars = 3, num_groups = 2;

    rawunits_t *units = get_randomunits(num_units, num_covars, num_groups);
    print_rawunits(units);

    sort_rawunits(units);

    printf("\nAfter sort\n");
    print_rawunits(units);

    free(units);

    return 0;
}