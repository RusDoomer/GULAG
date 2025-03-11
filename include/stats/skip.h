#ifndef SKIP_H
#define SKIP_H

/*
 * Initializes the array of skipgram statistics. The function allocates memory
 * for the stat array and sets default values, including a negative infinity
 * weight which will be later overwritten.
 */
void initialize_skip_stats();

/*
 * Trims the ngrams in the array to move unused entries to the end.
 * This process ensures memory efficiency by eliminating gaps in the array.
 */
void trim_skip_stats();

/*
 * Cleans the skipgram statistics array by removing statistics with zero length
 * or weight. This ensures that only relevant statistics are considered in the
 * analysis.
 */
void clean_skip_stats();

/* Frees the memory allocated for the skipgram statistics array. */
void free_skip_stats();

#endif
