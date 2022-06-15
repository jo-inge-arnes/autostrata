#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <structutils.h>

#include "structutilstests.h"

int test_sort_valueseq(void) {
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

int test_unitseq_alloc(void) {
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

int test_to_valueseq(void) {
    printf("test_to_valueseq\n");
    int res = 0;

    const int num_units = 5;
    const int num_vars = 3;
    const int selected_var = 1;

    unitseq_t *u = alloc_unitseq(num_units, num_vars);
    for (int i = 0; i < num_units; i++)
        get_unit(u, i)->id = i;

    get_unit(u, 0)->vals[selected_var] = 5.0;
    get_unit(u, 1)->vals[selected_var] = 3.0;
    get_unit(u, 2)->vals[selected_var] = 5.0;
    get_unit(u, 3)->vals[selected_var] = 1.0;
    get_unit(u, 4)->vals[selected_var] = 1.0;

    valueseq_t *v = to_valueseq(u, selected_var);

    if (v->len != 3) {
        res--;
        printf("\twrong number of distinct values\n");
    }

    if (v->vals[0].num_units != 2) {
        res--;
        printf("\twrong number of units for index 0\n");
    }

    if (v->vals[1].num_units != 1) {
        res--;
        printf("\twrong number of units for index 1\n");
    }

    if (v->vals[2].num_units != 2) {
        res--;
        printf("\twrong number of units for index 2\n");
    }

    if (v->vals[0].val != 1.0) {
        res--;
        printf("\twrong value for index 0\n");
    }

    if (v->vals[1].val != 3.0) {
        res--;
        printf("\twrong value for index 1\n");
    }

    if (v->vals[2].val != 5.0) {
        res--;
        printf("\twrong value for index 2\n");
    }

    if (!((v->vals[0].unit_ids[0] == 3 && v->vals[0].unit_ids[1] == 4) ||
         (v->vals[0].unit_ids[0] == 4 && v->vals[0].unit_ids[1] == 3))) {
        res--;
        printf("\twrong unit_ids for index 0\n");
    }

    if (v->vals[1].unit_ids[0] != 1) {
        res--;
        printf("\twrong unit_ids for index 1\n");
    }

    if (!((v->vals[2].unit_ids[0] == 0 && v->vals[2].unit_ids[1] == 2) ||
         (v->vals[2].unit_ids[0] == 2 && v->vals[2].unit_ids[1] == 0))) {
        res--;
        printf("\twrong unit_ids for index 2\n");
    }

    free_unitseq(u);
    free_valueseq(v);

    return res;
}

int test_to_valueseq_last_value_is_distinct(void) {
    printf("test_to_valueseq_last_value_is_distinct\n");
    int res = 0;

    const int num_units = 2;
    const int num_vars = 1;
    const int selected_var = 0;

    unitseq_t *u = alloc_unitseq(num_units, num_vars);
    for (int i = 0; i < num_units; i++)
        get_unit(u, i)->id = i;

    get_unit(u, 0)->vals[selected_var] = 1.0;
    get_unit(u, 1)->vals[selected_var] = 2.0;

    valueseq_t *v = to_valueseq(u, selected_var);

    if (v->len != 2) {
        res--;
        printf("\twrong number of distinct values\n");
    }

    if (v->vals[0].num_units != 1) {
        res--;
        printf("\twrong number of units for index 0\n");
    }

    if (v->vals[1].num_units != 1) {
        res--;
        printf("\twrong number of units for index 1\n");
    }

    if (v->vals[0].unit_ids[0] != 0) {
        res--;
        printf("\twrong unit_ids for index 0\n");
    }

    if (v->vals[1].unit_ids[0] != 1) {
        res--;
        printf("\twrong unit_ids for index 1\n");
    }

    free_unitseq(u);
    free_valueseq(v);

    return res;
}

int test_to_valueseq_all_values_equal(void) {
    printf("test_to_valueseq_all_values_equal\n");
    int res = 0;

    const int num_units = 4;
    const int num_vars = 1;
    const int selected_var = 0;

    unitseq_t *u = alloc_unitseq(num_units, num_vars);
    for (int i = 0; i < num_units; i++) {
        unit_t *unit = get_unit(u, i);
        unit->id = i;
        unit->vals[selected_var] = 10.0;
    }

    valueseq_t *v = to_valueseq(u, selected_var);

    if (v->len != 1) {
        res--;
        printf("\twrong number of distinct values\n");
    }

    if (v->vals[0].num_units != num_units) {
        res--;
        printf("\twrong number of units\n");
    }

    // Ids can be in any order, just check that all are present.
    bool found0 = false, found1 = false, found2 = false, found3 = false;
    for (int i = 0; i < num_units; i++) {
        int unit_id = v->vals[0].unit_ids[i];
        switch (unit_id) {
            case 0:
                found0 = true;
                break;
            case 1:
                found1 = true;
                break;
            case 2:
                found2 = true;
                break;
            case 3:
                found3 = true;
                break;
            default:
                break;
        }
    }

    if (!(found0 && found1 && found2 && found3)) {
        res--;
        printf("\tone or more unit_ids missing\n");
    }

    free_unitseq(u);
    free_valueseq(v);

    return res;
}