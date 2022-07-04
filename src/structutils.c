#include <structutils.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

unitseq_t *alloc_unitseq(const int num_units, const int num_vals) {
    const size_t unit_size = sizeof(unit_t) + num_vals * sizeof(double);
    unitseq_t *u = malloc(sizeof(unitseq_t) + num_units * unit_size);
    u->unit_size = unit_size;
    u->num_units = num_units;
    u->num_vals = num_vals;
    return u;
}

void free_unitseq(unitseq_t *u) {
    free(u);
}

unit_t *get_unit(const unitseq_t *const unitseq, const int index) {
    return (unit_t *) &unitseq->units[index * unitseq->unit_size];
}

valueseq_t *alloc_valueseq(const int num_values) {
    size_t size = sizeof(valueseq_t) + num_values * sizeof(value_t);
    valueseq_t *v = malloc(size);
    v->memsize = size;
    v->len = num_values;
    for (int i = 0; i < num_values; i++)
        v->vals[i].num_units = 0;
    return v;
}

valueseq_t *realloc_valueseq(valueseq_t *v, int num_values) {
    if (v->len != num_values) {
        int num_added = num_values - v->len;

        if (num_added < 0) {
            // Shrinking. Free unit_ids for instances that will be deleted.
            for (int i = num_values; i < v->len; i++) {
                if (v->vals[i].num_units > 0) {
                    free(v->vals[i].unit_ids);
                    v->vals[i].num_units = 0;
                }
            }
        }

        size_t new_size = sizeof(valueseq_t) + num_values * sizeof(value_t);
        v = (valueseq_t *)realloc(v, new_size);
        v->memsize = new_size;
        v->len = num_values;

        if (num_added > 0) {
            for (int i = num_values - num_added; i < num_values; i++) {
                v->vals[i].num_units = 0;
            }
        }
    }

    return v;
}

void alloc_unit_ids(value_t *const v, const int num_units) {
    if (v->num_units > 0)
        free(v->unit_ids);
    v->unit_ids = malloc(num_units * sizeof(int));
    v->num_units = num_units;
}

void free_valueseq(valueseq_t *v) {
    if (v != NULL) {
        for (int i = 0; i < v->len; i++) {
            if (v->vals[i].num_units > 0) {
                free(v->vals[i].unit_ids);
                v->vals[i].num_units = 0;
            }
        }
        free(v);
    }
}

void sort_valueseq(valueseq_t *const v) {
    qsort(v->vals, v->len, sizeof(value_t), compare_values);
}

int compare_values(const void *const a, const void *const b) {
    double a_val = (*(value_t *)a).val;
    double b_val = (*(value_t *)b).val;

    // Can't just subtract because return is int and vals are doubles
    if (a_val > b_val)
        return 1;
    else if (a_val < b_val)
        return -1;
    else
        return 0;
}

valueseq_t *to_valueseq(const unitseq_t *const u, const int val_index) {
    const int num_units = u->num_units;

    if (num_units < 1) {
        fprintf(stderr,
            "ERROR: to_valueseq() expects at least 1 units as input. Got %d\n",
            num_units);
        return NULL;
    }

    unit_t *u_i;
    value_t *v_i;

    // First, allocate a value_t instance per unit_t instance, allowing
    // non-unique values.
    valueseq_t *v = alloc_valueseq(num_units);

    for (int i = 0; i < num_units; i++) {
        u_i = get_unit(u, i);
        v_i = &v->vals[i];
        v_i->val = u_i->vals[val_index];
        alloc_unit_ids(v_i, 1);
        v_i->unit_ids[0] = i;
    }

    if (num_units > 1) {
        // Second, sort the valueseq.
        // After sorting, equal values will be in successive order.
        sort_valueseq(v);

        // Third, copy all distinct values to new sequence, where each value
        // has a reference to all unit ids having the given value.

        valueseq_t *v2 = alloc_valueseq(num_units); // Copy target

        int num_distinct_vals = 0;
        int start = 0;
        int end = 1;
        double prev = v->vals[0].val;
        double cur;

        for (int i = 1; i < num_units; i++) {
            cur = v->vals[i].val;
            if (cur == prev) {
                end++;
            } else {
                value_t *distinct = &v2->vals[num_distinct_vals];
                distinct->val = prev;
                int num_repeats = end - start;
                alloc_unit_ids(distinct, num_repeats);
                for (int j = 0; j < num_repeats; j++) {
                    distinct->unit_ids[j] = v->vals[start + j].unit_ids[0];
                }
                prev = cur;
                start = end;
                end++;
                num_distinct_vals++;
            }
        }

        value_t *distinct = &v2->vals[num_distinct_vals];
        distinct->val = prev;
        int num_repeats = end - start;
        alloc_unit_ids(distinct, num_repeats);
        for (int j = 0; j < num_repeats; j++) {
            distinct->unit_ids[j] = v->vals[start + j].unit_ids[0];
        }
        num_distinct_vals++;

        // Lastly, shrink space allocated to valueseq.
        free_valueseq(v);
        v = realloc_valueseq(v2, num_distinct_vals);
    }

    return v;
}

int index_of_value(const valueseq_t *const v, const value_t *const value) {
    int res = -1;

    value_t *found =
        bsearch(value, v->vals, v->len, sizeof(value_t), compare_values);

    if (found != NULL)
        res = (found - v->vals);

    return res;
}

variablevals_t *alloc_variablevals(const int num_vars) {
    size_t size = sizeof(variablevals_t) + num_vars * sizeof(valueseq_t*);
    variablevals_t *varvals = malloc(size);
    varvals->memsize = size;
    varvals->num_vars = num_vars;
    memset(varvals->vars, 0,  num_vars * sizeof(valueseq_t*));
    return varvals;
}

void free_variablevals(variablevals_t *varvals) {
    for (int i = 0; i < varvals->num_vars; i++)
        free_valueseq(varvals->vars[i]);
    free(varvals);
}

variablevals_t *to_variablevals(const unitseq_t *const u) {
    variablevals_t *varvals = alloc_variablevals(u->num_vals);

    for (int i = 0; i < varvals->num_vars; i++)
        varvals->vars[i] = to_valueseq(u, i);

    return varvals;
}

size_t stratumstats_size(int num_groups) {
    return sizeof(stratumstats_t) + num_groups * sizeof(int);
}

stratumstats_t *get_stratum_stats_total(const stratastats_t *const strata_stats) {
    return (stratumstats_t *)strata_stats->slots;
}

stratumstats_t *get_stratum_stats(const stratastats_t *const strata_stats, int index) {
    // Note that the actual index is 1 + index, because index 0 is used for
    // total counts
    return (stratumstats_t *) &strata_stats->slots[(1 + index) * stratumstats_size(strata_stats->num_groups)];
}

stratastats_t *alloc_strata_stats(int num_slots, int num_groups) {
    // Note that 1 more slot than requested is allocated.
    // This is because the first one is used for the total counts.
    size_t size = sizeof(stratastats_t) +
            (num_slots + 1) * stratumstats_size(num_groups);
    stratastats_t *strata_stats = malloc(size);
    memset(strata_stats, 0, size);

    strata_stats->memsize = size;
    strata_stats->num_slots = num_slots;
    strata_stats->num_groups = num_groups;
    strata_stats->stats_total = get_stratum_stats_total(strata_stats);
    strata_stats->stats_total->num_groups = num_groups;

    stratumstats_t *stats;
    for (int i = 0; i < num_slots; i++) {
        stats = get_stratum_stats(strata_stats, i);
        stats->num_groups = num_groups;
    }

    return strata_stats;
}

stratastats_t *realloc_strata_stats(stratastats_t *strata_stats, int num_slots) {
    size_t size_stratumstats = stratumstats_size(strata_stats->num_groups);
    // One more slot that requested is allocated, because the first is used for
    // the total count.
    size_t new_size = sizeof(stratastats_t) + (num_slots + 1) * size_stratumstats;

    strata_stats = realloc(strata_stats, new_size);
    strata_stats->memsize = new_size;
    strata_stats->stats_total = get_stratum_stats_total(strata_stats);

    if (num_slots > strata_stats->num_slots) {
        int added_start = strata_stats->num_slots;
        int num_added = num_slots - strata_stats->num_slots;
        memset(get_stratum_stats(strata_stats, added_start),
            0, num_added * size_stratumstats);

        stratumstats_t *stats;
        for (int i = added_start; i < num_slots; i++) {
            stats = get_stratum_stats(strata_stats, i);
            stats->num_groups = strata_stats->num_groups;
        }
    }

    strata_stats->num_slots = num_slots;

    return strata_stats;
}

void free_strata_stats(stratastats_t *strata_stats) {
    free(strata_stats);
}

strata_t *alloc_strata(int num_slots, int num_groups) {
    size_t size = sizeof(strata_t) + num_slots * sizeof(stratum_t);
    strata_t *strata = malloc(size);
    strata->memsize = size;
    strata->num_slots = num_slots;

    strata->strata_stats = alloc_strata_stats(num_slots, num_groups);

    for (int i = 0; i < strata->num_slots; i++) {
        strata->slots[i].num_units = 0;
        strata->slots[i].unit_ids = NULL;
        strata->slots[i].stats = get_stratum_stats(strata->strata_stats, i);
        strata->slots[i].in_use = false;
    }

    return strata;
}

strata_t *realloc_strata(strata_t *strata, int num_slots) {
    if (strata->num_slots != num_slots) {
        int slots_added = num_slots - strata->num_slots;

        if (slots_added < 0) {
            // Shrinking. Free unit_ids for instances that will be deleted.
            for (int i = num_slots; i < strata->num_slots; i++) {
                free_stratum_unit_ids(&strata->slots[i]);
            }
        }

        size_t new_size = sizeof(strata_t) + num_slots * sizeof(stratum_t);
        strata = (strata_t *)realloc(strata, new_size);
        strata->memsize = new_size;
        strata->num_slots = num_slots;

        stratastats_t *old_strata_stats = strata->strata_stats;
        strata->strata_stats =
            realloc_strata_stats(strata->strata_stats, num_slots);

        if (slots_added > 0) {
            for (int i = num_slots - slots_added; i < num_slots; i++) {
                strata->slots[i].num_units = 0;
                strata->slots[i].unit_ids = NULL;
                strata->slots[i].in_use = false;
            }
        }

        if (old_strata_stats != strata->strata_stats) {
            // The strata_stats address has changed due to realloc
            for (int i = 0; i < strata->num_slots; i++) {
                strata->slots[i].stats =
                    get_stratum_stats(strata->strata_stats, i);
            }
        }
    }

    return strata;
}

void alloc_stratum_unit_ids(stratum_t *s, int num_units) {
    free_stratum_unit_ids(s);

    if (num_units > 0) {
        s->unit_ids = malloc(num_units * sizeof(int));
        s->num_units = num_units;
    } else {
        s->unit_ids = NULL;
        s->num_units = 0;
    }
}

void shrink_stratum_unit_ids(stratum_t *s, int num_units) {
    if (num_units < 1) {
        free_stratum_unit_ids(s);
    } else if (num_units < s->num_units) {
        s->unit_ids = realloc(s->unit_ids, num_units * sizeof(int));
        s->num_units = num_units;
    }
}

void free_stratum_unit_ids(stratum_t *s) {
    if (s->num_units > 0) {
        free(s->unit_ids);
        s->unit_ids = NULL;
        s->num_units = 0;
    }
}

void free_strata(strata_t *strata) {
    for (int i = 0; i < strata->num_slots; i++) {
        free_stratum_unit_ids(&strata->slots[i]);
    }

    free_strata_stats(strata->strata_stats);
    strata->strata_stats = NULL;
    free(strata);
}

intpool_t *allocate_intpool(int num_slots) {
    size_t size = sizeof(intpool_t) + num_slots * sizeof(int);
    intpool_t *res = malloc(size);
    res->memsize = size;
    res->num_slots = num_slots;
    res->next_slot = 0;
    return res;
}

intpool_t *rellocate_intpool(intpool_t *pool, int num_slots) {
    size_t new_size = sizeof(intpool_t) + num_slots * sizeof(int);
    intpool_t *res = malloc(new_size);
    res->memsize = new_size;
    res->num_slots = num_slots;
    if (res->next_slot > num_slots)
        res->next_slot = num_slots;

    return res;
}

void free_intpool(intpool_t *pool) {
    free(pool);
}

int *intpool_get_pointer(intpool_t *pool, int index) {
    return &pool->slots[index];
}
