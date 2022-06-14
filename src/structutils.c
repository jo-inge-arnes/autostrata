#include <structutils.h>
#include <stdlib.h>
#include <stdio.h>

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
    valueseq_t *v = malloc(sizeof(valueseq_t) + num_values * sizeof(value_t));
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

        v = (valueseq_t *)realloc(v, sizeof(valueseq_t) + num_values * sizeof(value_t));
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
    for (int i = 0; i < v->len; i++) {
        if (v->vals[i].num_units > 0) {
            free(v->vals[i].unit_ids);
            v->vals[i].num_units = 0;
        }
    }
    free(v);
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
