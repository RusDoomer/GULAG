#ifndef META_H
#define META_H

#include "global.h"

/*
 * Initializes the linked list of meta statistics.
 * Each meta statistic represents a composite metric derived from other statistics.
 * The function allocates memory for each stat and sets default values,
 * including a negative infinity weight which will be later updated.
 */
void initialize_meta_stats();

/*
 * Trims the statistics in the linked list.
 * Currently, this function does not perform any operations for meta stats,
 * as there are no unused entries to trim.
 */
void trim_meta_stats();

/*
 * Cleans the meta statistics linked list by removing statistics with zero weight.
 * This function ensures that only relevant statistics are kept for analysis.
 * It updates the META_END global variable to reflect the new count of valid statistics.
 */
void clean_meta_stats();

/*
 * Converts the linked list of meta statistics to a contiguous array for easier access.
 * This function allocates memory for an array of meta_stat structures and copies
 * data from the linked list to the array, streamlining the data for analysis.
 */
void meta_to_array();

/*
 * Frees the memory allocated for the meta statistics linked list.
 * This function iterates through the list, freeing each node's memory.
 */
void free_meta_stats();

/*
 * Performs the meta-analysis on a given layout, calculating meta statistics.
 *
 * Parameters:
 *   lt: A pointer to the layout to analyze.
 *
 * Returns: void.
 */
void meta_analysis(layout *lt);

#endif
