#include <stdlib.h>
#include <string.h>
#include "strata.h"

size_t get_stratum_size_from_units(rawunits_t *units) {
    size_t extra_fields = STRATUM_DATA_EXTRA_COLS; // The "stratum is active" field
    return sizeof(float) * (units->covar_cnt + units->group_cnt + extra_fields);
}

size_t get_stratum_size(strata_t *strata) {
    return sizeof(float) * strata->data_colcnt;
}

float *get_stratum_start(strata_t *strata, size_t stratum_index) {
    return &strata->data[stratum_index * strata->data_colcnt];
}

size_t get_group_fields_index(strata_t *strata) {
    return strata->covar_cnt;
}

size_t get_extra_fields_index(strata_t *strata) {
    return get_group_fields_index(strata) + strata->group_cnt;
}

size_t get_active_col_index(strata_t *strata) {
    return get_extra_fields_index(strata) + STRATUM_DATA_ACTIVE_OFFSET;
}

void set_stratum_active_val(strata_t *strata, size_t index, float is_active) {
    float *stratum_cols = get_stratum_start(strata, index);
    stratum_cols[get_active_col_index(strata)] = is_active;
}

void set_stratum_active(strata_t *strata, size_t index) {
    set_stratum_active_val(strata, index, STRATUM_ACTIVE_TRUE);
}

void set_stratum_inactive(strata_t *strata, size_t index) {
    set_stratum_active_val(strata, index, STRATUM_ACTIVE_FALSE);
}

void set_stratum_covars(strata_t *strata, size_t stratum_index, rawunits_t *units, size_t unit_index) {
    float *unit_covars = get_rawunit_start(units, unit_index);
    float *stratum_covars = get_stratum_start(strata, stratum_index);
    memcpy(stratum_covars, unit_covars, sizeof(float) * strata->covar_cnt);
}

void inc_stratum_group_cnt(strata_t *strata, size_t stratum_index, size_t group_index, int inc) {
    float *cols = get_stratum_start(strata, stratum_index);
    cols[get_group_fields_index(strata) + group_index] += (float) inc;
}

strata_t *strata_from_sorted_rawunits(rawunits_t *units) {
    size_t data_size = get_stratum_size_from_units(units) * units->unit_cnt;
    strata_t *strata = (strata_t*)malloc(
        sizeof(strata_t) + data_size);

    strata->stratum_cnt = 0;
    strata->allocated_cnt = units->unit_cnt;
    strata->covar_cnt = units->covar_cnt;
    strata->group_cnt = units->group_cnt;
    strata->data_colcnt =
        strata->covar_cnt + strata->group_cnt + STRATUM_DATA_EXTRA_COLS;

    memset(strata->data, 0, data_size);

    strata->stratum_cnt++;
    size_t cur_index = 0;
    set_stratum_active(strata, cur_index);
    set_stratum_covars(strata, cur_index, units, 0);
    size_t group = get_rawunit_group(units, 0);
    inc_stratum_group_cnt(strata, cur_index, group, 1);

    float *prev_unit = get_rawunit_start(units, 0);
    float *cur_unit = NULL;

    for (size_t i = 1; i < units->unit_cnt; i++) {
        cur_unit = get_rawunit_start(units, i);

        if (rawunits_comparator(cur_unit, prev_unit, &units->covar_cnt) != 0) {
            strata->stratum_cnt++;
            cur_index++;
            set_stratum_active(strata, cur_index);
            set_stratum_covars(strata, cur_index, units, i);
        }

        inc_stratum_group_cnt(strata, cur_index, group, 1);
        group = get_rawunit_group(units, i);
        prev_unit = cur_unit;
    }

    if (strata->stratum_cnt < strata->allocated_cnt) {
        strata = (strata_t*)realloc(strata,
            sizeof(strata_t) + get_stratum_size(strata) * strata->stratum_cnt);
    }

    return(strata);
}
