#pragma once

#include <stddef.h>

typedef struct unit {
    int id;
    int group_id;
    int stratum_id;
    double vals[0]; // Same size for all instances
} unit_t;

typedef struct unitseq {
    int num_units;
    int num_vals;
    size_t unit_size;
    char units[0];
} unitseq_t;

typedef struct stratum {
    int id;
    int num_units; // not equal for all instances, but is immutable
    int *unit_ids;
} stratum_t;

typedef struct value {
    double val;
    int num_units; // not equal for all instances, but is immutable
    int *unit_ids;
} value_t;

typedef struct valueseq {
    int len;
    value_t vals[0];
} valueseq_t;

typedef struct variablevals {
    int num_vars;
    valueseq_t *vars[0];
} variablevals_t;
