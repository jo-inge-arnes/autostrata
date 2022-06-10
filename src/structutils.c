#include <structutils.h>
#include <stdlib.h>

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

void sort_valueseq(valueseq_t *v) {
    qsort(v->vals, v->len, sizeof(value_t), compare_values);
}