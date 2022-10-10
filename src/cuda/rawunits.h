#pragma once

#define RAWUNITS_GROUP_COL_OFFSET 0

typedef struct rawunits {
    size_t unit_cnt;
    size_t covar_cnt;
    size_t group_cnt;
    float data[0];
} rawunits_t;

size_t get_rawunit_colcnt(rawunits_t *units);
size_t get_rawunit_size(rawunits_t *units);
float *get_rawunit_start(rawunits_t *units, size_t unit_index);
float get_rawunit_column_val(rawunits_t *units, size_t unit_index, size_t col_index);
size_t get_rawunit_group(rawunits_t *units, size_t unit_index);
size_t get_rawunit_group_col_index(rawunits_t *units);
rawunits_t *sort_rawunits(rawunits_t *units);
int rawunits_comparator(const void *p, const void *q, void *arg);
