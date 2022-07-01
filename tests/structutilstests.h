#pragma once

int test_unitseq_alloc(void);

int test_to_valueseq(void);
int test_to_valueseq_when_last_value_is_distinct(void);
int test_to_valueseq_when_all_values_equal(void);
int test_sort_valueseq(void);
int test_index_of_value_with_existing_value(void);

int test_variablevals_alloc_and_free_when_vals_are_null(void);
int test_variablevals_alloc_and_free_when_vals_are_allocated(void);

int test_to_variablevals(void);

int test_stata_stats_alloc_zero_group_counts(void);
int test_stata_stats_realloc_zero_group_counts(void);


