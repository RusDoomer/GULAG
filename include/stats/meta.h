#ifndef META_H
#define META_H

#include "global.h"
#include "structs.h"

/*
 * Initializes the array of meta statistics. Each meta statistic represents a
 * composite metric derived from other statistics. The function allocates memory
 * for the stat array and sets default values, including a negative infinity
 * weight which will be later overwritten.
 */
void initialize_meta_stats();

/*
 * Trims the ngrams in the array to move unused entries to the end.
 * This process ensures memory efficiency by eliminating gaps in the array.
 * Currently, this function does not perform any operations for meta stats,
 * as there are no unused entries to trim.
 */
void trim_meta_stats();

/*
 * Cleans the meta statistics array by removing statistics with zero length
 * or weight. This ensures that only relevant statistics are considered in the
 * analysis.
 */
void clean_meta_stats();

/*
 * Sets the actual definitions for each meta statistic. This involves choosing
 * which ngram statistics are relevant, finding and storing their type, indexes,
 * and the weight that should be applied to each. Finally we must set a stop
 * index and whether the meta statistics value should be absolute.
 */
void define_meta_stats();

/* Frees the memory allocated for the meta statistics array. */
void free_meta_stats();

#endif
