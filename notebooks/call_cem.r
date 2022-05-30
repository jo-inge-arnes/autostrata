library(tidyverse)
require(cem)

call_cem_autostrata <- function(d, treated_col, dropcols, cps) {
    d <- data.frame(na.omit(d))

    # L1.breaks instead of cutpoints? What is most correct?
    mat <- cem(
        treatment = treated_col, data = d, drop = dropcols,
        cutpoints = cps,
        eval.imbalance = TRUE,
        # L1.breaks = cps,
        keep.all = TRUE
    )

    return(mat)
}

call_plain_cem <- function(d, treated_col, dropcols) {
    d <- data.frame(na.omit(d))

    mat <- cem(
        treatment = treated_col, data = d,
        cutpoints = list("scott"),
        # L1.breaks = NULL,
        drop = dropcols, eval.imbalance = TRUE, keep.all = TRUE
    )

    return(mat)
}

overall_imbalance <- function(d, treated_col, vars) {
    mat <- imbalance(
        group = d[[treated_col]],
        data = d[vars]
    )
    return(mat)
}
