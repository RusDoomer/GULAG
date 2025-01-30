/*
 * stats/meta.c - Meta statistic definitions for the GULAG.
 *
 * This file contains the implementation for initializing, cleaning, converting,
 * and freeing meta statistics used in the GULAG. Meta statistics are calculated
 * based on multiple types of ngram statistics, allowing for higher-level
 * analysis of keyboard layouts.
 *
 * Adding new stats:
 *     1. Incease META_LENGTH by as many stats as you are adding.
 *     2. Define its name, keep it a reasonable length.
 *     3. Set its weight to -INFINITY, and skip to 0 (to be changed later).
 *     4. Iterate the index.
 *     5. Add definition and set absv in define_meta_stats().
 *     6. Add the statistic to the weights files in data/weights/.
 */

#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "meta.h"
#include "stats_util.h"
#include "global.h"
#include "structs.h"


/*
 * Initializes the array of meta statistics. Each meta statistic represents a
 * composite metric derived from other statistics. The function allocates memory
 * for the stat array and sets default values, including a negative infinity
 * weight which will be later overwritten.
 */
void initialize_meta_stats()
{
    META_LENGTH = 1;
    stats_meta = (meta_stat *)malloc(sizeof(meta_stat) * META_LENGTH);
    int index = 0;

    /* Initialize hand balance. */
    strcpy(stats_meta[index].name, "Hand Balance");
    stats_meta[index].weight = -INFINITY;
    stats_meta[index].skip = 0;
    index++;
}

/*
 * Trims the ngrams in the array to move unused entries to the end.
 * This process ensures memory efficiency by eliminating gaps in the array.
 * Currently, this function does not perform any operations for meta stats,
 * as there are no unused entries to trim.
 */
void trim_meta_stats() {return;}

/*
 * Cleans the meta statistics array by removing statistics with zero length
 * or weight. This ensures that only relevant statistics are considered in the
 * analysis.
 */
void clean_meta_stats()
{
    /* Iterate through the array and remove those with zero length or weight */
    for (int i = 0; i < META_LENGTH; i++)
    {
        if (stats_meta[i].weight == 0) {stats_meta[i].skip = 1;}
    }
}

/*
 * Sets the actual definitions for each meta statistic. This involves choosing
 * which ngram statistics are relevant, finding and storing their type, indexes,
 * and the weight that should be applied to each. Finally we must set a stop
 * index and whether the meta statistics value should be absolute.
 */
void define_meta_stats()
{
    int index = 0;

    if (!stats_meta[index].skip)
    {
        int left_hand = find_stat_index("Left Hand Usage", 'm');
        int right_hand = find_stat_index("Right Hand Usage", 'm');
        /* type of stat m for mono, b for bi, 1 for skip 1, 2 for skip 2, etc.*/
        stats_meta[index].stat_types[0] = 'm';
        /* index in that ngram array */
        stats_meta[index].stat_indices[0] = left_hand;
        /* weight to multiply by */
        stats_meta[index].stat_weights[0] = 1;
        /* unset skip in case it was being skipped before */
        stats_mono[left_hand].skip = 0;

        stats_meta[index].stat_types[1] = 'm';
        stats_meta[index].stat_indices[1] = right_hand;
        stats_meta[index].stat_weights[1] = -1;
        stats_mono[right_hand].skip = 0;

        /* set stop index */
        stats_meta[index].stat_types[2] = 'x';
        /* Whether the value should be absolute */
        stats_meta[index].absv = 1;
    }
    index++;
}

/* Frees the memory allocated for the meta statistics array. */
void free_meta_stats()
{
    free(stats_meta);
}
