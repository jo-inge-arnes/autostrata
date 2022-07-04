#pragma once
#include <structs.h>

unitseq_t *alloc_unitseq(const int num_units, const int num_vals);
void free_unitseq(unitseq_t *u);
unit_t *get_unit(const unitseq_t *const unitseq, const int index);
void clear_stratum_ids(unitseq_t *u);

valueseq_t *alloc_valueseq(const int num_values);
valueseq_t *realloc_valueseq(valueseq_t *v, int num_values);
void free_valueseq(valueseq_t *v);
void sort_valueseq(valueseq_t *const v);
int compare_values(const void *const a, const void *const b);
void alloc_unit_ids(value_t *const v, const int num_units, intpool_t *id_pool);
valueseq_t *to_valueseq(const unitseq_t *const u, const int val_index, intpool_t *id_pool);
int index_of_value(const valueseq_t *const v, const value_t *const value);

variablevals_t *alloc_variablevals(const int num_vars, const int num_units);
void free_variablevals(variablevals_t *varvals);
variablevals_t *to_variablevals(const unitseq_t *const u);

size_t stratumstats_size(int num_groups);
stratumstats_t *get_stratum_stats_total(const stratastats_t *const strata_stats);
stratumstats_t *get_stratum_stats(const stratastats_t *const strata_stats, int index);

void update_group_counts(stratumstats_t *stats, unit_t *unit);

stratastats_t *alloc_strata_stats(int num_slots, int num_groups);
stratastats_t *realloc_strata_stats(stratastats_t *strata_stats, int num_slots);
void free_strata_stats(stratastats_t *strata_stats);

strata_t *alloc_strata(int num_slots, int num_groups);
strata_t *realloc_strata(strata_t *strata, int num_slots);
void alloc_stratum_unit_ids(stratum_t *s, int num_units);
void shrink_stratum_unit_ids(stratum_t *s, int num_units);
void free_stratum_unit_ids(stratum_t *s);
void free_strata(strata_t *strata);

intpool_t *allocate_intpool(int num_slots);
intpool_t *rellocate_intpool(intpool_t *pool, int num_slots);
void free_intpool(intpool_t *pool);
int *intpool_get_pointer(intpool_t *pool, int index);