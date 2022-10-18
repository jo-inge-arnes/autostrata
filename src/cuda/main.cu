#include <cuda.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "structs.h"
#include "data.h"

void print_rawunits(rawunits_t *units) {
    size_t num_cols = units->covar_cnt + 1;

    for (size_t i = 0; i < units->unit_cnt; i++) {
        printf("%zu: [", i);
        for (size_t j = 0; j < num_cols; j++) {
            printf("%.2f%s", get_rawunit_column_val(units, i, j), j < num_cols - 1 ? ", ": "");
        }
        printf("]\n");
    }
}

void print_strata(strata *strata) {
    size_t num_cols = strata->data_colcnt;

    for (size_t i = 0; i < strata->stratum_cnt; i++) {
        printf("%zu: [", i);
        float *data_cols = get_stratum_start(strata, i);
        for (size_t j = 0; j < num_cols; j++) {
            printf("%.2f%s", data_cols[j], j < num_cols - 1 ? ", ": "");
        }
        printf("]\n");
    }
}

void print_stratamaps(stratamappings_t *strata_maps, strataindices_t *strata_ixs, strata_t *strata) {
    printf("Entries: %zu, allocated: %zu\n",
        strata_maps->entries_cnt,
        strata_maps->allocated_cnt);

    for (size_t i = 0; i < strata_maps->entries_cnt; i++) {
        printf("Entry: %zu\n", i);
        stratamapping_t *cur_mapping = &strata_maps->entries[i];
        printf("\tValue: %.2f, start: %zu, entries: %zu\n",
            cur_mapping->value,
            cur_mapping->start_index,
            cur_mapping->entries_cnt);
        printf("\tStratum indices:\n");

        for (size_t j = 0; j < cur_mapping->entries_cnt; j++) {
            printf("\t\t%zu\n",
                strata_ixs->indices[cur_mapping->start_index + j]);
        }
    }
}

int main(int argc, char **argv) {
    srand(time(NULL));

    size_t num_units = 20, num_covars = 3, num_groups = 2;

    rawunits_t *units = get_randomunits(num_units, num_covars, num_groups);
    sort_rawunits(units);

    printf("Raw units\n");
    print_rawunits(units);
    printf("\n");

    strata_t *strata = strata_from_sorted_rawunits(units);
    printf("Strata\n");
    print_strata(strata);
    printf("\n");

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

    printf("Covar maps indices:\n");
    for (size_t i = 0; i < strata->covar_cnt; i++)
        printf("\tCovar: %zu, Start index: %zu\n", i, covar_map_indices[i]);
    printf("\n");

    printf("Strata maps\n");
    print_stratamaps(strata_maps, strata_ixs, strata);
    printf("\n");

    free(strata_ixs);
    free(strata_maps);
    free(covar_map_indices);

    free(strata);
    free(units);

    return 0;
}