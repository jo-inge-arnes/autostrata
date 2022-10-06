#include <stdlib.h>

#include "structs.h"
#include "data.h"

rawunits_t *get_randomunits(size_t unit_cnt, size_t covar_cnt, size_t group_cnt) {
    size_t col_cnt = covar_cnt + 1;
    rawunits_t *units = (rawunits_t*)malloc(sizeof(rawunits_t) + unit_cnt * col_cnt * sizeof(float));

    units->unit_cnt = unit_cnt;
    units->covar_cnt = covar_cnt;
    units->group_cnt = group_cnt;

    int max_val = unit_cnt / 2;
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

int rawunits_comparator(const void *p, const void *q, void *arg)
{
    const float *l = (const float*)p;
    const float *r = (const float*)q;
    size_t covar_cnt = *((size_t*)arg);
    int res = 0;

    for (size_t i = 0; i < covar_cnt; i++) {
        if (l[i] != r[i]) {
            res = l[i] < r[i] ? -1 : 1;
            break;
        }
    }

    return res;
}

rawunits_t *sort_rawunits(rawunits_t *units) {
    qsort_r(units->data,
        units->unit_cnt,
        sizeof(float) * (units->covar_cnt + 1),
        rawunits_comparator,
        &units->covar_cnt);
    return units;
}