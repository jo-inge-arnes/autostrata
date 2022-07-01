#pragma once

#include <stddef.h>
#include <stdbool.h>

typedef struct unit {
    int id;
    int group_id;
    int stratum_id;
    double vals[0]; // Same number of values for all instances
} unit_t;

typedef struct unitseq {
    int num_units;
    int num_vals;
    size_t unit_size;
    char units[0];
} unitseq_t;

typedef struct stratumstats {
    int num_groups;
    int group_unit_counts[0];
} stratumstats_t;

typedef struct stratastats {
    int num_slots;
    int num_groups;
    stratumstats_t *stats_total;
    char slots[0];
} stratastats_t;

typedef struct stratum {
    int id;
    int num_units; // not equal for all instances, but doesn't change
    bool in_use;
    stratumstats_t *stats;
    int *unit_ids;
} stratum_t;

typedef struct strata {
    int num_slots;
    stratastats_t *strata_stats;
    stratum_t slots[0];
} strata_t;

typedef struct value {
    double val;
    int num_units; // not equal for all instances, but doesn't change
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
