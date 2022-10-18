#pragma once
#include <stddef.h>
#include "strata.h"

typedef struct stratamapping {
    float value;
    size_t start_index;
    size_t entries_cnt;
} stratamapping_t;

typedef struct stratamappings {
    size_t allocated_cnt;
    size_t entries_cnt;
    stratamapping_t entries[0];
} stratamappings_t;

typedef struct strataindices {
    size_t allocated_cnt;
    size_t indices_cnt;
    size_t indices[0];
} strataindices_t;

typedef struct valueindex {
    size_t index;
    float value;
} valueindex_t;

stratamappings_t *allocate_stratamappings(size_t num_mappings);
stratamappings_t *shrink_stratamappings(stratamappings_t *sm);

strataindices_t *allocate_strataindices(size_t num_indices);
strataindices_t *shrink_strataindices(strataindices_t *sp);

valueindex_t *index_sort(strata_t *strata, size_t covar_index);
