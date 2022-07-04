#include <stdio.h>
#include <structutils.h>
#include <initialization.h>

#include "initializationtests.h"

void set_unit(unitseq_t *u, int index, int group, double var1, double var2, double var3) {
    unit_t *unit = get_unit(u, index);
    unit->group_id = group;
    unit->vals[0] = var1;
    unit->vals[1] = var2;
    unit->vals[2] = var3;
}

unitseq_t *setup_unitseq(void) {
    const int num_units = 7;
    const int num_vars = 3;

    unitseq_t *u = alloc_unitseq(num_units, num_vars);
    for (int i = 0; i < num_units; i++)
        get_unit(u, i)->id = i;

    set_unit(u, 0, 0, 1.0, 1.0, 1.0);
    set_unit(u, 1, 1, 1.0, 2.0, 2.0);
    set_unit(u, 2, 1, 1.0, 1.0, 1.0);
    set_unit(u, 3, 0, 1.0, 2.0, 31.0);
    set_unit(u, 4, 1, 1.0, 2.0, 2.0);
    set_unit(u, 5, 0, 1.0, 3.0, 1.0);
    set_unit(u, 6, 1, 2.0, 1.0, 1.0);

    return u;
}

int test_init_strata(void) {
    printf("%s\n", __func__);
    int res = 0;

    const int num_groups = 2;
    unitseq_t *u = setup_unitseq();
    variablevals_t *v = to_variablevals(u);
    strata_t *s = init_strata(u, v, num_groups);

    if (s->num_slots != 5) {
        printf("\twrong number of slots allocated (expected: %d, got: %d)\n",
            5, s->num_slots);
        res--;
    } else {
        for (int i = 0; i < s->num_slots; i++) {
            if (!s->slots[i].in_use) {
                printf("\tslot %d not marked as in use\n", i);
                res--;
                break;
            } else if (s->slots[i].id != i) {
                printf("\texpected id to be %d, but got %d\n", i, s->slots[i].id);
                res--;
                break;
            }
        }
    }

    if (s->slots[0].num_units != 2) {
        printf("\texpected slot 0 num_units to be 2, but got %d\n", s->slots[0].num_units);
        res--;
    } else {
        if (s->slots[0].unit_ids[0] != 0) {
            printf("\texpected slot 0, index 0 to be 0, but got %d\n", s->slots[0].unit_ids[0]);
            res--;
        }

        if (s->slots[0].unit_ids[1] != 2) {
            printf("\texpected slot 0, index 1 to be 2, but got %d\n", s->slots[0].unit_ids[1]);
            res--;
        }
    }

    if (s->slots[1].num_units != 2) {
        printf("\texpected slot 1 num_units to be 2, but got %d\n", s->slots[1].num_units);
        res--;
    } else {
        if (s->slots[1].unit_ids[0] != 1) {
            printf("\texpected slot 1, index 0 to be 1, but got %d\n", s->slots[1].unit_ids[0]);
            res--;
        }

        if (s->slots[1].unit_ids[1] != 4) {
            printf("\texpected slot 1, index 1 to be 2, but got %d\n", s->slots[1].unit_ids[1]);
            res--;
        }
    }

    if (s->slots[2].num_units != 1) {
        printf("\texpected slot 2 num_units to be 1, but got %d\n", s->slots[2].num_units);
        res--;
    } else if (s->slots[2].unit_ids[0] != 3) {
        printf("\texpected slot 2, index 0 to be 3, but got %d\n", s->slots[2].unit_ids[0]);
        res--;
    }

    if (s->slots[3].num_units != 1) {
        printf("\texpected slot 3 num_units to be 1, but got %d\n", s->slots[3].num_units);
        res--;
    } else if (s->slots[3].unit_ids[0] != 5) {
        printf("\texpected slot 3, index 0 to be 5, but got %d\n", s->slots[3].unit_ids[0]);
        res--;
    }

    if (s->slots[4].num_units != 1) {
        printf("\texpected slot 4 num_units to be 1, but got %d\n", s->slots[4].num_units);
        res--;
    } else if (s->slots[4].unit_ids[0] != 6) {
        printf("\texpected slot 4, index 0 to be 6, but got %d\n", s->slots[4].unit_ids[0]);
        res--;
    }

    free_strata(s);
    free_variablevals(v);
    free_unitseq(u);

    return res;
}

int test_init_strata_unit_stratum_ids(void) {
    printf("%s\n", __func__);
    int res = 0;

    const int num_groups = 2;
    unitseq_t *u = setup_unitseq();
    variablevals_t *v = to_variablevals(u);
    strata_t *s = init_strata(u, v, num_groups);

    int expected[] = { 0, 1, 0, 2, 1, 3, 4 };
    for (int i = 0; i < u->num_units; i++) {
        if (get_unit(u, i)->stratum_id != expected[i]) {
            printf("\texpected unit %d to have stratum id %d, but got %d\n",
                i, expected[i], get_unit(u, i)->stratum_id);
            res--;
        }
    }

    free_strata(s);
    free_variablevals(v);
    free_unitseq(u);

    return res;
}

int test_init_strata_stats(void) {
    printf("%s\n", __func__);
    int res = 0;

    const int num_groups = 2;
    unitseq_t *u = setup_unitseq();
    variablevals_t *v = to_variablevals(u);
    strata_t *s = init_strata(u, v, num_groups);

    if (s->strata_stats->num_groups != num_groups) {
        printf("\texpected num groups for strata stats to %d, but got %d\n",
            num_groups,
            s->strata_stats->num_groups);
        res--;
    }

    if (s->strata_stats->num_slots != s->num_slots) {
        printf("\texpected num slots for strata stats to %d, but got %d\n",
            s->num_slots,
            s->strata_stats->num_slots);
        res--;
    } else {
        for (int i = 0; i < s->strata_stats->num_slots; i++) {
            if (get_stratum_stats(s->strata_stats, i)->num_groups != num_groups) {
                printf("\texpected num groups for each stratum stats instance to be %d, but got %d for index %d\n",
                    num_groups,
                    get_stratum_stats(s->strata_stats, i)->num_groups,
                    i);
                res--;
                break;
            }
        }
    }

    if (res >= 0) {
        for (int i = 0; i < s->num_slots; i++) {
            stratumstats_t *stats = get_stratum_stats(s->strata_stats, i);
            if (s->slots[i].stats != stats) {
                printf("\texpected stats pointer for stratum with index %d to be %p, but got %p\n",
                    i, stats, s->slots[i].stats);
                res--;
                break;
            }
        }
    }

    free_strata(s);
    free_variablevals(v);
    free_unitseq(u);

    return res;
    }

int test_init_strata_stats_group_counts(void) {
    printf("%s\n", __func__);
    int res = 0;

    const int num_groups = 2;
    unitseq_t *u = setup_unitseq();
    variablevals_t *v = to_variablevals(u);
    strata_t *s = init_strata(u, v, num_groups);

    int expected[] = {1, 1, 0, 2, 1, 0, 1, 0, 0, 1};
    for (int i = 0; i < s->num_slots; i++) {
        stratumstats_t *stats = s->slots[i].stats;
        for (int j = 0; j < stats->num_groups; j++)
        {
            int expected_i = i * stats->num_groups + j;
            if (stats->group_unit_counts[j] != expected[expected_i]) {
                printf("\tgroup count for stratum %d, group %d, was %d, expected %d\n",
                    i, j, stats->group_unit_counts[j], expected[expected_i]);
                res--;
            }
        }
    }

    int expected_total[] = {3, 4};
    for (int i = 0; i < num_groups; i++) {
        int group_total = s->strata_stats->stats_total->group_unit_counts[i];
        if(group_total != expected_total[i]) {
            printf("\ttotal group count for group %d was %d, expected %d\n",
                i, group_total, expected_total[i]);
            res--;
        }
    }

    free_strata(s);
    free_variablevals(v);
    free_unitseq(u);

    return res;
}
