#include <stdlib.h>
#include <stdio.h>
#include <autostrata.h>

#include "tests.h"
#include "structutilstests.h"

int main(int argc, char *argv[]) {
    printf("Autostrata Test Suite\n---------------------\n");
    printf("Note: To check memory, make sure to also run with 'make valgrind'.\n");

    int exit_code = EXIT_SUCCESS;

    if (test_sort_valueseq() < 0)
        exit_code = EXIT_FAILURE;

    if (test_unitseq_alloc() < 0)
        exit_code = EXIT_FAILURE;

    if (test_to_valueseq() < 0)
        exit_code = EXIT_FAILURE;

    if (test_to_valueseq_when_last_value_is_distinct() < 0)
        exit_code = EXIT_FAILURE;

    if (test_to_valueseq_when_all_values_equal() < 0)
        exit_code = EXIT_FAILURE;

    if (test_variablevals_alloc_and_free_when_vals_are_null() < 0)
        exit_code = EXIT_FAILURE;

    if (test_variablevals_alloc_and_free_when_vals_are_allocated() < 0)
        exit_code = EXIT_FAILURE;

    printf("Finished tests:\n");

    if (exit_code == EXIT_SUCCESS)
        printf("SUCCESS\n");
    else
        printf("FAILURE\n");

    exit(exit_code);
}