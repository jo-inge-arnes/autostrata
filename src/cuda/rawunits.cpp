#include <stdlib.h>
#include "rawunits.h"

size_t get_rawunit_colcnt(rawunits_t *units) {
    return units->covar_cnt + 1;
}

size_t get_rawunit_size(rawunits_t *units) {
    return sizeof(float) * get_rawunit_colcnt(units);
}

float *get_rawunit_start(rawunits_t *units, size_t unit_index) {
    return &units->data[unit_index * get_rawunit_colcnt(units)];
}

float get_rawunit_column_val(rawunits_t *units, size_t unit_index, size_t col_index) {
    size_t num_cols = get_rawunit_colcnt(units);
    return get_rawunit_start(units, unit_index)[col_index];
}

size_t get_rawunit_group_col_index(rawunits_t *units) {
    return units->covar_cnt + RAWUNITS_GROUP_COL_OFFSET;
}

size_t get_rawunit_group(rawunits_t *units, size_t unit_index) {
    return (size_t)get_rawunit_column_val(units, unit_index, get_rawunit_group_col_index(units));
}

rawunits_t *sort_rawunits(rawunits_t *units) {
    qsort_r(units->data,
        units->unit_cnt,
        get_rawunit_size(units),
        rawunits_comparator,
        &units->covar_cnt);
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
