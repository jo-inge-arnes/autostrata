#pragma once
#include "rawunits.h"

#define STRATUM_DATA_EXTRA_COLS 1
#define STRATUM_DATA_ACTIVE_OFFSET 0
#define STRATUM_ACTIVE_TRUE 1.0f
#define STRATUM_ACTIVE_FALSE 0.0f

typedef struct strata {
    size_t allocated_cnt;
    size_t stratum_cnt;
    size_t covar_cnt;
    size_t group_cnt;
    size_t data_colcnt;
    float data[0];
} strata_t;

void set_stratum_active_val(strata_t *strata, size_t index, float val);
void set_stratum_active(strata_t *strata, size_t index);
void set_stratum_inactive(strata_t *strata, size_t index);
void set_stratum_covars(strata_t *strata, size_t stratum_index, rawunits_t *units, size_t unit_index);
void inc_stratum_group_cnt(strata_t *strata, size_t stratum_index, size_t group_index, int inc);
float *get_stratum_start(strata_t *strata, size_t stratum_index);
size_t get_group_fields_index(strata_t *strata);
size_t get_extra_fields_index(strata_t *strata);
size_t get_active_col_index(strata_t *strata);
size_t get_stratum_size(strata_t *strata);
size_t get_stratum_size_from_units(rawunits_t *units);
strata_t *strata_from_sorted_rawunits(rawunits_t *units);
bool are_equal_except(strata_t *strata, size_t a_index, size_t b_index, size_t except_covar);