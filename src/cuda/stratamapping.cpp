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

stratamappings_t *shrink_stratamappings(stratamappings_t *sm) {
    size_t new_cnt = sm->entries_cnt;

    if (new_cnt < sm->allocated_cnt) {
        sm = (stratamappings_t*)
            realloc(sm, sizeof(stratamappings_t) + sizeof(stratamapping_t) * new_cnt);
        sm->allocated_cnt = new_cnt;
    }

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

strataindices_t *shrink_strataindices(strataindices_t *sp) {
    size_t new_cnt = sp->indices_cnt;

    if (new_cnt < sp->allocated_cnt) {
        sp = (strataindices_t*)
            realloc(sp, sizeof(strataindices_t) + sizeof(size_t) * new_cnt);
        sp->allocated_cnt = new_cnt;
    }

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

covarstratamap_t *build_covarstratamap(strata_t *strata) {
    stratamappings_t *strata_maps = allocate_stratamappings(strata->stratum_cnt * strata->covar_cnt);
    strataindices_t *strata_ixs = allocate_strataindices(strata->stratum_cnt * strata->covar_cnt);

    float cur_val, prev_val;
    float *stratum_vals;
    stratamapping_t *cur_mapping;

    size_t *covar_map_indices =
        (size_t*)malloc(sizeof(size_t) * strata->covar_cnt);

    for (size_t covar_index = 0; covar_index < strata->covar_cnt; covar_index++) {
        covar_map_indices[covar_index] = strata_maps->entries_cnt;

        valueindex_t *sorted = index_sort(strata, covar_index);
        stratum_vals = get_stratum_start(strata, sorted[0].index);
        cur_val = stratum_vals[covar_index];
        cur_mapping = &strata_maps->entries[strata_maps->entries_cnt];
        strata_maps->entries_cnt++;
        cur_mapping->value = cur_val;
        cur_mapping->start_index = strata_ixs->indices_cnt;
        cur_mapping->entries_cnt++;
        strata_ixs->indices[strata_ixs->indices_cnt] = sorted[0].index;
        strata_ixs->indices_cnt++;
        prev_val = cur_val;

        for (size_t i = 1; i < strata->stratum_cnt; i++) {
            stratum_vals = get_stratum_start(strata, sorted[i].index);
            cur_val = stratum_vals[covar_index];

            if (cur_val != prev_val) {
                cur_mapping = &strata_maps->entries[strata_maps->entries_cnt];
                strata_maps->entries_cnt++;
                cur_mapping->value = cur_val;
                cur_mapping->start_index = strata_ixs->indices_cnt;
            }

            cur_mapping->entries_cnt++;
            strata_ixs->indices[strata_ixs->indices_cnt] = sorted[i].index;
            strata_ixs->indices_cnt++;

            prev_val = cur_val;
        }

        free(sorted);
    }

    strata_maps = shrink_stratamappings(strata_maps);
    strata_ixs = shrink_strataindices(strata_ixs);

    covarstratamap_t *res = (covarstratamap_t*)malloc(sizeof(covarstratamap_t));
    res->covar_map_indices = covar_map_indices;
    res->strata_maps = strata_maps;
    res->strata_ixs = strata_ixs;
    res->strata = strata;

    return res;
}

void free_covarstratamap(covarstratamap_t *covar_strata_maps) {
    free(covar_strata_maps->covar_map_indices);
    free(covar_strata_maps->strata_maps);
    free(covar_strata_maps->strata_ixs);
    free(covar_strata_maps);
}
