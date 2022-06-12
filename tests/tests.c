#include <stdlib.h>
#include <stdio.h>
#include <autostrata.h>

#include "tests.h"
#include "structutilstests.h"

int main(int argc, char *argv[]) {
    printf("Autostrata Test Suite\n---------------------\n");

    int exit_code = EXIT_SUCCESS;

    if(test_sort_valueseq() < 0)
        exit_code = EXIT_FAILURE;

    if(test_unitseq_alloc() < 0)
        exit_code = EXIT_FAILURE;

    printf("finished testing!\n");
    exit(exit_code);
}