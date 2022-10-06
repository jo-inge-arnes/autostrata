#pragma once

typedef struct rawunits {
    size_t unit_cnt;
    size_t group_cnt;
    size_t covar_cnt;
    float data[0];
} rawunits_t;