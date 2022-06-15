#include <stdlib.h>
#include <stdio.h>
#include <autostrata.h>

#include "tests.h"
#include "structutilstests.h"

int main(int argc, char *argv[]) {
    printf("Autostrata Test Suite\n---------------------\n");

    int exit_code = EXIT_SUCCESS;

    if (test_sort_valueseq() < 0)
        exit_code = EXIT_FAILURE;

    if (test_unitseq_alloc() < 0)
        exit_code = EXIT_FAILURE;

    if (test_to_valueseq() < 0)
        exit_code = EXIT_FAILURE;

    if (test_to_valueseq_last_value_is_distinct() < 0)
        exit_code = EXIT_FAILURE;

    if (test_to_valueseq_all_values_equal() < 0)
        exit_code = EXIT_FAILURE;

    printf("Finished tests:\n");

    if (exit_code == EXIT_SUCCESS)
        printf("SUCCESS\n");
    else
        printf("FAILURE\n");

    exit(exit_code);
}