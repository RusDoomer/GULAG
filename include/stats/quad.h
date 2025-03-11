#ifndef QUAD_H
#define QUAD_H

/*
 * Initializes the array of quadgram statistics. The function allocates memory
 * for the stat array and sets default values, including a negative infinity
 * weight which will be later overwritten.
 */
void initialize_quad_stats();

/*
 * Trims the ngrams in the array to move unused entries to the end.
 * This process ensures memory efficiency by eliminating gaps in the array.
 */
void trim_quad_stats();

/*
 * Cleans the quadgram statistics array by removing statistics with zero length
 * or weight. This ensures that only relevant statistics are considered in the
 * analysis.
 */
void clean_quad_stats();

/* Frees the memory allocated for the quadgram statistics array. */
void free_quad_stats();

#endif
