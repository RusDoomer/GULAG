#ifndef BI_H
#define BI_H

/*
 * Initializes the linked list of bigram statistics.
 * Allocates and initializes each bi_stat structure, setting default values
 * for name, weight, and ngrams. The weight is initially set to negative infinity,
 * indicating that it will be updated later.
 */
void initialize_bi_stats();

/*
 * Trims the statistics in the linked list to move unused entries to the end.
 * This is done by iterating through each statistic's ngrams array and
 * reordering elements to place all valid entries at the beginning
 * of the array. This process ensures memory efficiency by eliminating
 * gaps in the array.
 */
void trim_bi_stats();

/*
 * Removes all stats that have a length of 0, or a weight of 0.
 * Iterates through the linked list, freeing the memory
 * of each deleted node, and updates the BI_END global variable.
 */
void clean_bi_stats();

/*
 * Converts the linked list of bigram statistics to a contiguous array.
 * This function allocates memory for an array of bi_stat structures
 * and copies the data from the linked list to the array, allowing for
 * more efficient access and manipulation of the statistics.
 */
void bi_to_array();

/*
 * Frees the memory allocated for the bigram statistics linked list.
 * Iterates through the list, freeing each node's memory.
 */
void free_bi_stats();

#endif
