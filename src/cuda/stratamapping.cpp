#include <stdlib.h>
#include <string.h>
#include "stratamapping.h"

stratamappings_t *allocate_stratamappings(size_t num_entries) {
    stratamappings_t *sm = (stratamappings_t*)
        malloc(sizeof(stratamappings_t) + sizeof(stratamapping_t) * num_entries);
    sm->allocated_cnt = num_entries;
    sm->entries_cnt = 0;
    memset(sm->entries, 0, sizeof(stratamapping_t) * num_entries);
    return sm;
}

strataindices_t *allocate_strataindices(size_t num_indices) {
    strataindices_t *sp = (strataindices_t*)
        malloc(sizeof(strataindices_t) + sizeof(size_t) * num_indices);
    sp->allocated_cnt = num_indices;
    sp->indices_cnt = 0;
    memset(sp->indices, 0, sizeof(size_t) * num_indices);
    return sp;
}

int valueindex_comparator(const void *p, const void *q)
{
    const valueindex_t *l = (const valueindex_t*)p;
    const valueindex_t *r = (const valueindex_t*)q;
    int res;

    if (l->value > r->value)
        res = 1;
    else if (l->value < r->value)
        res = -1;
    else
        res = 0;

    return res;
}

valueindex_t *index_sort(strata_t *strata, size_t covar_index) {
    valueindex_t *sorted =
        (valueindex_t*)malloc(sizeof(valueindex_t) * strata->stratum_cnt);

    for (size_t i = 0; i < strata->stratum_cnt; i++) {
        sorted[i].index = i;
        sorted[i].value = get_stratum_start(strata, i)[covar_index];
    }

    qsort(sorted, strata->stratum_cnt, sizeof(valueindex_t), valueindex_comparator);

    return sorted;
}
