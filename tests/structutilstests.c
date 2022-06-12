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
            res--;
            printf("\tdidn't sort correctly\n");
            break;
        }
    }

    free_valueseq(v);
    return res;
}

int test_unitseq_alloc() {
    printf("test_unitseq_alloc\n");
    int res = 0;

    int num_units = 10;
    int num_vals = 2;

    unitseq_t *u = alloc_unitseq(num_units, num_vals);
    if (u->num_units != num_units) {
        res--;
        printf("\tnum_units expected %d, got %d\n", num_units, u->num_units);
    }

    if (u->num_vals != num_vals) {
        res--;
        printf("\tnum_vals expected %d, got %d\n", num_vals, u->num_vals);
    }

    if (res >= 0) {
        // Check that all units can be accessed and their values written to
        for (int i = 0; i < u->num_units; i++) {
            unit_t *u_i = get_unit(u, i);
            for (int j = 0; j < u->num_vals; j++)
            u_i->vals[j] = i * u->num_vals + j;
        }
    }

    free_unitseq(u);

    return res;
}
