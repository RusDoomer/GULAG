#ifndef BI_H
#define BI_H

/*
 * Initializes the array of bigram statistics. The function allocates memory
 * for the stat array and sets default values, including a negative infinity
 * weight which will be later overwritten.
 */
void initialize_bi_stats();

/*
 * Trims the ngrams in the array to move unused entries to the end.
 * This process ensures memory efficiency by eliminating gaps in the array.
 */
void trim_bi_stats();

/*
 * Cleans the bigram statistics array by removing statistics with zero length or
 * weight. This ensures that only relevant statistics are considered in the
 * analysis.
 */
void clean_bi_stats();

/* Frees the memory allocated for the bigram statistics array. */
void free_bi_stats();

#endif
