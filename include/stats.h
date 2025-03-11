#ifndef STATS_H
#define STATS_H

/*
 * Initializes all statistic data structures for the GULAG. This involves
 * initializing arrays for each type of n-gram statistic as well as
 * meta-statistics. The function delegates the initialization of each statistic
 * type to its respective module.
 */
void initialize_stats();

/*
 * Cleans the statistics data by skipping entries with zero length or weight.
 * This function iterates through each category of statistics (monograms,
 * bigrams, trigrams, quadgrams, skipgrams, and meta-statistics) and marks skip
 * on any statistic that has a length of 0 or a weight of 0. This helps purge
 * irrelevant statistics for layout analysis.
 */
void clean_stats();

/*
 * Frees the memory allocated for all statistics data structures. This function
 * deallocates the memory used by the statistics arrays for each n-gram
 * category. It ensures that all dynamically allocated memory for statistics is
 * properly released.
 */
void free_stats();

#endif
