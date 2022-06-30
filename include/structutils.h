#pragma once
#include <structs.h>

unitseq_t *alloc_unitseq(const int num_units, const int num_vals);
void free_unitseq(unitseq_t *u);
unit_t *get_unit(const unitseq_t *const unitseq, const int index);

valueseq_t *alloc_valueseq(const int num_values);
valueseq_t *realloc_valueseq(valueseq_t *v, int num_values);
void free_valueseq(valueseq_t *v);
void sort_valueseq(valueseq_t *const v);
int compare_values(const void *const a, const void *const b);
void alloc_unit_ids(value_t *const v, const int num_units);
valueseq_t *to_valueseq(const unitseq_t *const u, const int val_index);
int index_of_value(const valueseq_t *const v, const value_t *const value);

variablevals_t *alloc_variablevals(const int num_vars);
void free_variablevals(variablevals_t *varvals);
variablevals_t *to_variablevals(const unitseq_t *const u);

strata_t *alloc_strata(int num_slots, int num_groups);
strata_t *realloc_strata(strata_t *strata, int num_slots);
void alloc_stratum_unit_ids(stratum_t *s, int num_units);
void shrink_stratum_unit_ids(stratum_t *s, int num_units);
void free_stratum_unit_ids(stratum_t *s);
void alloc_group_counts(stratum_t *s, int num_groups);
void free_group_counts(stratum_t *s);
void free_strata(strata_t *strata);