#include <initialization.h>
#include <structutils.h>
#include <stdio.h>
#include <stdbool.h>

#define EMPTY_ID -1

void clear_stratum_ids(unitseq_t *u) {
    for (int i = 0; i < u->num_units; i++)
        get_unit(u, i)->stratum_id = EMPTY_ID;
}

void update_group_counts(stratumstats_t *stats, unit_t *unit) {
    if (unit->group_id < 0 || unit->group_id >= stats->num_groups) {
        fprintf(stderr,
            "[ERROR]: The range of valid groups ids are [0 , %d], "
            "but a unit was found that had a group id of %d.\n",
            stats->num_groups,
            unit->group_id);
    } else {
        stats->group_unit_counts[unit->group_id]++;
    }
}


/**
 * @brief Assigns initial stratum IDs to all units and creates strata-structure
 *
 * The stratum ID assignment algorithm is described here:
 * https://docs.google.com/document/d/1lNqy7jY1BKA5fNJ7K0LWWJFWATFyBXPQK9ESEFF8kh8/edit?usp=sharing
 *
 * @param u The units
 * @param v The sorted distinct values for the units' variables and their associated unit_ids
 * @param num_groups The number of different groups in the sample
 */
strata_t *init_strata(unitseq_t *u, variablevals_t *v, const int num_groups) {
    clear_stratum_ids(u);
    strata_t *strata = alloc_strata(u->num_units, num_groups);

    int next_stratum_id = 0;
    valueseq_t *first_var = v->vars[0];

    for (int value_i = 0; value_i < first_var->len; value_i++) {
        value_t *cur_val = &first_var->vals[value_i];
        int cur_num_units = cur_val->num_units;
        if (cur_num_units == 1) {
            int unit_id = cur_val->unit_ids[0];
            unit_t *unit = get_unit(u, unit_id);
            unit->stratum_id = next_stratum_id;

            stratum_t *stratum = &strata->slots[next_stratum_id];
            alloc_stratum_unit_ids(stratum, 1);
            stratum->unit_ids[0] = unit_id;
            update_group_counts(stratum->stats, unit);
            stratum->in_use = true;
            stratum->id = next_stratum_id;

            next_stratum_id++;
        } else {
            for (int base_unit_i = 0; base_unit_i < cur_num_units; base_unit_i++) {
                int base_unit_id = cur_val->unit_ids[base_unit_i];
                unit_t *base_unit = get_unit(u, base_unit_id);

                if (base_unit->stratum_id != EMPTY_ID)
                    continue;

                base_unit->stratum_id = next_stratum_id;

                stratum_t *stratum = &strata->slots[next_stratum_id];
                alloc_stratum_unit_ids(stratum, cur_num_units - base_unit_i);
                int unit_ids_added = 0;
                stratum->unit_ids[unit_ids_added] = base_unit_id;
                update_group_counts(stratum->stats, base_unit);
                unit_ids_added++;
                stratum->in_use = true;
                stratum->id = next_stratum_id;

                next_stratum_id++;

                for (int cur_unit_i = base_unit_i + 1; cur_unit_i < cur_num_units; cur_unit_i++) {
                    int cur_unit_id = cur_val->unit_ids[cur_unit_i];
                    unit_t *cur_unit = get_unit(u, cur_unit_id);

                    if (cur_unit->stratum_id != EMPTY_ID)
                        continue;

                    bool only_identical_vals = true;
                    for (int var_i = 1; var_i < v->num_vars; var_i++) {
                        if (cur_unit->vals[var_i] != base_unit->vals[var_i]) {
                            only_identical_vals = false;
                            break;
                        }
                    }

                    if (only_identical_vals) {
                        cur_unit->stratum_id = base_unit->stratum_id;
                        stratum->unit_ids[unit_ids_added] = cur_unit->id;
                        update_group_counts(stratum->stats, cur_unit);
                        unit_ids_added++;
                    }
                }

                shrink_stratum_unit_ids(stratum, unit_ids_added);
            }
        }
    }

    strata = realloc_strata(strata, next_stratum_id);
    return strata;
}
