#ifndef TRI_H
#define TRI_H

/*
 * Initializes the array of tripgram statistics. The function allocates memory
 * for the stat array and sets default values, including a negative infinity
 * weight which will be later overwritten.
 */
void initialize_tri_stats();

/*
 * Trims the ngrams in the array to move unused entries to the end.
 * This process ensures memory efficiency by eliminating gaps in the array.
 */
void trim_tri_stats();

/*
 * Cleans the trigram statistics array by removing statistics with zero length
 * or weight. This ensures that only relevant statistics are considered in the
 * analysis.
 */
void clean_tri_stats();

/* Frees the memory allocated for the trigram statistics array. */
void free_tri_stats();

#endif
