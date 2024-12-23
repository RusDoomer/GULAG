#ifndef ANALYZE_H
#define ANALYZE_H

#include "global.h"

/*
 * Performs analysis on a single layout, calculating statistics for monograms,
 * bigrams, trigrams, quadgrams, and skipgrams. It then delegates to meta_analysis
 * for the calculation of meta-statistics.
 *
 * Parameters:
 *   lt: A pointer to the layout to analyze.
 *
 * Returns: void.
 */
void single_analyze(layout *lt);

/*
 * Placeholder for OpenCL-accelerated layout analysis.
 * Currently, this function is not implemented and will throw an error if called.
 *
 * Parameters:
 *   lt: A pointer to the layout to analyze.
 *
 * Returns: void.
 */
void cl_single_analyze(layout *lt);

#endif
