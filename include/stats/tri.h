#ifndef TRI_H
#define TRI_H

/*
 * Initializes the linked list of trigram statistics.
 * Each trigram statistic tracks the usage of specific key sequences.
 * The function allocates memory for each stat and sets default values,
 * including a negative infinity weight which will be later updated.
 * Statistics are defined for various key sequences and combinations.
 */
void initialize_tri_stats();

/*
 * Trims the statistics in the linked list to move unused entries to the end.
 * This is done by iterating through each statistic's ngrams array and
 * reordering elements to place all valid entries at the beginning
 * of the array. This process ensures memory efficiency by eliminating
 * gaps in the array. It is somewhat analagous to a correctional officer
 * ordering prisoners to form an orderly line, ensuring no gaps or disorder.
 */
void trim_tri_stats();

/*
 * Cleans the trigram statistics linked list by removing statistics with zero length or weight.
 * This function ensures that only relevant statistics are kept for analysis,
 * similar to weeding out unnecessary files from a meticulously organized archive.
 * It updates the TRI_END global variable to reflect the new count of valid statistics.
 */
void clean_tri_stats();

/*
 * Converts the linked list of trigram statistics to a contiguous array for easier access.
 * This function allocates memory for an array of tri_stat structures and copies
 * data from the linked list to the array. This is akin to transferring data from
 * individual files to a consolidated ledger for streamlined processing.
 */
void tri_to_array();

/*
 * Frees the memory allocated for the trigram statistics linked list.
 * This function iterates through the list, freeing each node's memory,
 * similar to the careful archiving of documents that are no longer needed.
 */
void free_tri_stats();

#endif
