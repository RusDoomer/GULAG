#ifndef MONO_H
#define MONO_H

/*
 * Initializes the linked list of monogram statistics.
 * Each monogram statistic tracks the usage of specific keys or key positions.
 * The function allocates memory for each stat and sets default values,
 * including a negative infinity weight which will be later updated.
 * Statistics are defined for various key positions and hand usage.
 */
void initialize_mono_stats();

/*
 * Trims the statistics in the linked list to remove unused entries.
 * This is done by iterating through each statistic's ngrams array and
 * reordering elements to place all valid entries at the beginning
 * of the array. This process ensures memory efficiency by eliminating
 * gaps in the array.
 */
void trim_mono_stats();

/*
 * Cleans the monogram statistics linked list by removing statistics with zero length or weight.
 * This ensures that only relevant statistics are considered in the analysis.
 */
void clean_mono_stats();

/*
 * Converts the linked list of monogram statistics to a contiguous array for easier access.
 * Allocates memory for an array of mono_stat structures and copies data from the linked list.
 */
void mono_to_array();

/*
 * Frees the memory allocated for the monogram statistics linked list.
 * Iterates through the list, freeing each node's memory.
 */
void free_mono_stats();

#endif
