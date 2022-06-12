#include <stdlib.h>
#include <stdio.h>
#include <structutils.h>

#include "structutilstests.h"

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
