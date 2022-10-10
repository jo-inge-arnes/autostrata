#pragma once
#include <stddef.h>
#include "structs.h"

rawunits_t *get_randomunits(size_t unit_cnt, size_t covar_cnt, size_t groups_cnt);
rawunits_t *sort_rawunits(rawunits_t *units);