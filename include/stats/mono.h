#ifndef MONO_H
#define MONO_H

/*
 * Initializes the array of monogram statistics. The function allocates memory
 * for the stat array and sets default values, including a negative infinity
 * weight which will be later overwritten.
 */
void initialize_mono_stats();

/*
 * Trims the ngrams in the array to move unused entries to the end.
 * This process ensures memory efficiency by eliminating gaps in the array.
 */
void trim_mono_stats();

/*
 * Cleans the monogram statistics array by removing statistics with zero length
 * or weight. This ensures that only relevant statistics are considered in the
 * analysis.
 */
void clean_mono_stats();

/* Frees the memory allocated for the monogram statistics array. */
void free_mono_stats();

#endif
