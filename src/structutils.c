#include <structutils.h>
#include <stdlib.h>

valueseq_t *alloc_valueseq(int num_values) {
    valueseq_t *v = malloc(sizeof(valueseq_t) + num_values * sizeof(value_t));
    v->len = num_values;
    return v;
}

void free_valueseq(valueseq_t *v) {
    free(v);
}

void sort_valueseq(valueseq_t *v) {
    qsort(v->vals, v->len, sizeof(value_t), compare_values);
}

int compare_values(const void *a, const void *b) {
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

unitseq_t *alloc_unitseq(int num_units, int num_vals) {
    size_t unit_size = sizeof(unit_t) + num_vals * sizeof(double);
    unitseq_t *u = malloc(sizeof(unitseq_t) + num_units * unit_size);
    u->unit_size = unit_size;
    u->num_units = num_units;
    u->num_vals = num_vals;
}

void free_unitseq(unitseq_t *u) {
    free(u);
}

unit_t *get_unit(unitseq_t *unitseq, int index) {
    return (unit_t *) &unitseq->units[index * unitseq->unit_size];
}
