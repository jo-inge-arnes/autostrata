#include <stdio.h>
#include <structutils.h>
#include <initialization.h>

#include "initializationtests.h"

void set_unit(unitseq_t *u, int index, double var1, double var2, double var3) {
    unit_t *unit = get_unit(u, index);
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

    set_unit(u, 0, 1, 1, 1);
    set_unit(u, 1, 1, 2, 2);
    set_unit(u, 2, 1, 1, 1);
    set_unit(u, 3, 1, 2, 31);
    set_unit(u, 4, 1, 2, 2);
    set_unit(u, 5, 1, 3, 1);
    set_unit(u, 6, 2, 1, 1);

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