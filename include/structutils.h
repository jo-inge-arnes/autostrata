#pragma once
#include <structs.h>

valueseq_t *alloc_valueseq(int num_values);
void free_valueseq(valueseq_t *v);
void sort_valueseq(valueseq_t *v);

int compare_values(const void *a, const void *b);
