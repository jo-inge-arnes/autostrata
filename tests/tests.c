#include <stdlib.h>
#include <stdio.h>
#include <autostrata.h>

#include "tests.h"
#include "structutilstests.h"
#include "initializationtests.h"

int main(int argc, char *argv[]) {
    printf("Autostrata Test Suite\n---------------------\n");
    printf("Note: To check memory, make sure to also run with 'make valgrind'.\n");

    int failures = 0;

    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunused-value"
    test_sort_valueseq() && failures++;
    test_unitseq_alloc() && failures++;

    test_to_valueseq() && failures++;
    test_to_valueseq_when_last_value_is_distinct() && failures++;
    test_to_valueseq_when_all_values_equal() && failures++;
    test_index_of_value_with_existing_value() && failures++;

    test_variablevals_alloc_and_free_when_vals_are_null() && failures++;
    test_variablevals_alloc_and_free_when_vals_are_allocated() && failures++;
    test_to_variablevals() && failures++;

    test_init_strata() && failures++;
    test_init_strata_unit_stratum_ids() && failures++;
    test_init_strata_stats() && failures++;

    test_stata_stats_alloc_zero_group_counts() && failures++;
    test_stata_stats_realloc_zero_group_counts() && failures++;
    #pragma GCC diagnostic pop

    int exit_code = failures ? EXIT_FAILURE : EXIT_SUCCESS;

    printf("Finished tests: ");
    if (exit_code == EXIT_FAILURE) {
        printf("%d tests failed\n", failures);
        printf("FAILURE\n");
    } else {
        printf("SUCCESS\n");
    }

    exit(exit_code);
}