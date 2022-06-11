#include <stdlib.h>
#include <stdio.h>
#include <autostrata.h>
#include <structutils.h>

#include "tests.h"

int test_sort_valueseq() {
    printf("test_sort_valueseq\n");
    int res = 0;

    valueseq_t *v = alloc_valueseq(10);
    for (int i = 0; i < v->len; i++)
        v->vals[i].val = v->len - i;

    sort_valueseq(v);

    for (int i = 0; i < v->len; i++) {
        if (v->vals[i].val != i+1) {
            res = -1;
            printf("\tdidn't sort correctly\n");
            break;
        }
    }

    free_valueseq(v);
    return res;
}

int main(int argc, char *argv[]) {
    printf("Autostrata Test Suite\n---------------------\n");

    int exit_code = EXIT_SUCCESS;

    if(test_sort_valueseq() < 0)
        exit_code = EXIT_FAILURE;

    printf("finished testing!\n");
    exit(exit_code);
}