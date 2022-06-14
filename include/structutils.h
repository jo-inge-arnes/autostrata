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


