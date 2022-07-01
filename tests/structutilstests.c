#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <structutils.h>

#include "structutilstests.h"

int test_sort_valueseq(void) {
    printf("%s\n", __func__);
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
    printf("%s\n", __func__);
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
    printf("%s\n", __func__);
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

int test_to_valueseq_when_last_value_is_distinct(void) {
    printf("%s\n", __func__);
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

int test_to_valueseq_when_all_values_equal(void) {
    printf("%s\n", __func__);
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

int test_index_of_value_with_existing_value(void) {
    printf("%s\n", __func__);
    int res = 0;

    valueseq_t *v = alloc_valueseq(10);
    for (int i = 0; i < v->len; i++)
        v->vals[i].val = v->len - i;

    sort_valueseq(v);

    value_t val_to_find;
    val_to_find.val = 8.0;
    int expected = (int)val_to_find.val - 1;

    int index = index_of_value(v, &val_to_find);

    if (index != expected) {
        printf("\texpected index %d, got %d\n", expected, index);
        res--;
    }

    free_valueseq(v);

    return res;
}

int test_variablevals_alloc_and_free_when_vals_are_null(void) {
    printf("%s\n", __func__);
    int res = 0;

    int num_vars = 3;
    variablevals_t *varvals = alloc_variablevals(num_vars);

    free_variablevals(varvals);

    return res;
}

int test_variablevals_alloc_and_free_when_vals_are_allocated(void) {
    printf("%s\n", __func__);
    int res = 0;

    int num_vars = 3;
    int num_vals = 10;
    variablevals_t *varvals = alloc_variablevals(num_vars);

    for (int i = 0; i < num_vars; i++)
        varvals->vars[i] = alloc_valueseq(num_vals);

    free_variablevals(varvals);

    return res;
}

int test_to_variablevals(void) {
    printf("%s\n", __func__);
    int res = 0;

    const int num_units = 5;
    const int num_vars = 3;

    unitseq_t *u = alloc_unitseq(num_units, num_vars);
    for (int i = 0; i < num_units; i++) {
        unit_t *unit = get_unit(u, i);
        unit->id = i;
        for (int j = 0; j < num_vars; j++)
            unit->vals[j] = pow(i + 1, j + 1);
    }

    variablevals_t *varvals = to_variablevals(u);

    if (varvals->num_vars != num_vars) {
        printf("\twrong number of variables");
        res--;
    } else {
        for (int i = 0; i < varvals->num_vars; i++) {
            if (varvals->vars[i]->len != num_units) {
                printf("\twrong number of values for vars-index %d", i);
                printf(" (expected %d, got %d)\n", num_units, varvals->vars[i]->len);
                res--;
            } else {
                for (int j = 0; j < varvals->vars[i]->len; j++) {
                    double expected = pow(j + 1, i + 1);
                    if (varvals->vars[i]->vals[j].val != expected) {
                        printf("\twrong value for vars-index %d, value-index %d. ", i, j);
                        printf(" (expected %.2f, got %.2f)\n", expected, varvals->vars[i]->vals[j].val);
                        res--;
                        break;
                    }
                }
            }
        }
    }

    free_unitseq(u);
    free_variablevals(varvals);

    return res;
}

int test_stata_stats_alloc_zero_group_counts(void) {
    printf("%s\n", __func__);
    int res = 0;
    int num_slots = 2;
    int num_groups = 3;

    stratastats_t *strata_stats = alloc_strata_stats(num_slots, num_groups);

    if (strata_stats->num_groups != num_groups) {
        printf("\twrong num groups, expected %d but got %d\n",
            num_groups,
            strata_stats->num_groups);
        res--;
    }

    if (strata_stats->num_slots != num_slots) {
        printf("\twrong num slots, expected %d but got %d\n",
            num_slots,
            strata_stats->num_slots);
        res--;
    }

    if (res >= 0) {
        stratumstats_t *stats;
        for (int i = 0; i < num_slots; i++) {
            stats = get_stratum_stats(strata_stats, i);
            if (stats->num_groups != num_groups) {
                printf("\texpected num groups to be %d but got %d\n",
                    num_groups, stats->num_groups);
                res--;
            } else {
                for (int j = 0; j < num_groups; j++) {
                    if (stats->group_unit_counts[j] != 0) {
                        printf("\tgroup count for stats index %d, group index %d, should be 0 but was %d\n",
                            i, j, stats->group_unit_counts[j]);
                        res--;
                        break;
                    }
                }
            }

            if (res < 0)
                break;
        }
    }

    free_strata_stats(strata_stats);

    return res;
}

int test_stata_stats_realloc_zero_group_counts(void) {
    printf("%s\n", __func__);
    int res = 0;

    return res;
}