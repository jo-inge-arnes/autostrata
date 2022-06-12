#pragma once
#include <structs.h>

valueseq_t *alloc_valueseq(int num_values);
void free_valueseq(valueseq_t *v);
void sort_valueseq(valueseq_t *v);
int compare_values(const void *a, const void *b);

unitseq_t *alloc_unitseq(int num_units, int num_vals);
void free_unitseq(unitseq_t *u);
unit_t *get_unit(unitseq_t *unitseq, int index);
