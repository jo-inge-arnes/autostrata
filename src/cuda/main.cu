#include <cuda.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "structs.h"
#include "data.h"

void print_rawunits(rawunits_t *units) {
    size_t num_cols = units->covar_cnt + 1;

    for (size_t i = 0; i < units->unit_cnt; i++) {
        printf("%zu: [", i);
        for (size_t j = 0; j < num_cols; j++) {
            printf("%.2f%s", get_rawunit_column_val(units, i, j), j < num_cols - 1 ? ", ": "");
        }
        printf("]\n");
    }
}

void print_strata(strata *strata) {
    size_t num_cols = strata->data_colcnt;

    for (size_t i = 0; i < strata->stratum_cnt; i++) {
        printf("%zu: [", i);
        float *data_cols = get_stratum_start(strata, i);
        for (size_t j = 0; j < num_cols; j++) {
            printf("%.2f%s", data_cols[j], j < num_cols - 1 ? ", ": "");
        }
        printf("]\n");
    }
}

int main(int argc, char **argv) {
    srand(time(NULL));

    size_t num_units = 20, num_covars = 3, num_groups = 2;

    rawunits_t *units = get_randomunits(num_units, num_covars, num_groups);
    sort_rawunits(units);

    printf("Raw units\n");
    print_rawunits(units);
    printf("\n");

    strata_t *strata = strata_from_sorted_rawunits(units);
    printf("Strata\n");
    print_strata(strata);

    free(strata);
    free(units);

    return 0;
}