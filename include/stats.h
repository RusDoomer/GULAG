#ifndef STATS_H
#define STATS_H

/*
 * Initializes all statistical data structures for the GULAG.
 * This involves initializing linked lists for each type of n-gram statistic
 * (monograms, bigrams, trigrams, quadgrams, and skipgrams) as well as meta-statistics.
 * The function delegates the initialization of each statistic type to its respective module.
 * After initial setup, it trims each statistic list to optimize data storage.
 */
void initialize_stats();

/*
 * Cleans the statistics data by removing entries with zero length or weight.
 * This function iterates through each category of statistics (monograms, bigrams,
 * trigrams, quadgrams, skipgrams, and meta-statistics) and removes any statistic
 * that has a length of 0 or a weight of 0. This helps in maintaining a clean
 * and relevant set of statistics for layout analysis.
 */
void clean_stats();

/*
 * Converts the linked list statistics data structures into arrays.
 * This function transforms the data from linked lists to contiguous arrays for
 * each category of n-gram statistics (monograms, bigrams, trigrams, quadgrams,
 * skipgrams, and meta-statistics). This conversion facilitates quicker access
 * and streamlined processing of the statistics during layout analysis.
 */
void stats_to_array();

/*
 * Frees the memory allocated for all statistics data structures.
 * This function deallocates the memory used by the statistics arrays and their
 * corresponding linked lists for each n-gram category. It ensures that all
 * dynamically allocated memory for statistics is properly released.
 */
void free_stats();

#endif
