#include <stdlib.h>
#include "structs.h"
#include "data.h"

rawunits_t *get_randomunits(size_t unit_cnt, size_t covar_cnt, size_t group_cnt) {
    size_t col_cnt = covar_cnt + 1;
    rawunits_t *units = (rawunits_t*)malloc(sizeof(rawunits_t) + unit_cnt * col_cnt * sizeof(float));

    units->unit_cnt = unit_cnt;
    units->covar_cnt = covar_cnt;
    units->group_cnt = group_cnt;

    int max_val = unit_cnt / 3;
    float *row;
    for (size_t i = 0; i < unit_cnt; i++) {
        row = &units->data[i * col_cnt];
        for (size_t j = 0; j < covar_cnt; j++) {
            row[j] = rand() % max_val;
        }
        // Group label
        row[col_cnt - 1] = rand() % group_cnt;
    }

    return units;
}
