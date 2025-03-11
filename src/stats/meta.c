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
#include "util.h"
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
    META_LENGTH = 10;
    stats_meta = (meta_stat *)malloc(sizeof(meta_stat) * META_LENGTH);
    int index = 0;

    /* Initialize hand balance. */
    strcpy(stats_meta[index].name, "Hand Balance");
    stats_meta[index].weight = -INFINITY;
    stats_meta[index].skip = 0;
    index++;

    /* Initialize RuSpeed. */
    strcpy(stats_meta[index].name, "RuSpeed");
    stats_meta[index].weight = -INFINITY;
    stats_meta[index].skip = 0;
    index++;

    /* Initialize per finger RuSpeed. */
    strcpy(stats_meta[index].name, "Left Pinky RuSpeed");
    stats_meta[index].weight = -INFINITY;
    stats_meta[index].skip = 0;
    index++;

    strcpy(stats_meta[index].name, "Left Ring RuSpeed");
    stats_meta[index].weight = -INFINITY;
    stats_meta[index].skip = 0;
    index++;

    strcpy(stats_meta[index].name, "Left Middle RuSpeed");
    stats_meta[index].weight = -INFINITY;
    stats_meta[index].skip = 0;
    index++;

    strcpy(stats_meta[index].name, "Left Index RuSpeed");
    stats_meta[index].weight = -INFINITY;
    stats_meta[index].skip = 0;
    index++;

    strcpy(stats_meta[index].name, "Right Index RuSpeed");
    stats_meta[index].weight = -INFINITY;
    stats_meta[index].skip = 0;
    index++;

    strcpy(stats_meta[index].name, "Right Middle RuSpeed");
    stats_meta[index].weight = -INFINITY;
    stats_meta[index].skip = 0;
    index++;

    strcpy(stats_meta[index].name, "Right Ring RuSpeed");
    stats_meta[index].weight = -INFINITY;
    stats_meta[index].skip = 0;
    index++;

    strcpy(stats_meta[index].name, "Right Pinky RuSpeed");
    stats_meta[index].weight = -INFINITY;
    stats_meta[index].skip = 0;
    index++;

    if (index != META_LENGTH) { error("META_LENGTH incorrect for number of meta stats"); }
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

    /* HAND BALANCE */
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

    /* TOTAL RuSpeed */
    if (!stats_meta[index].skip)
    {
        int sub_index = 0;
        float current_weight = 1;
        float weight_multiplier = 0.25;

        int sfb = find_stat_index("Same Finger Bigram", 'b');
        stats_bi[sfb].skip = 0;
        int bad_sfb = find_stat_index("Bad Same Finger Bigram", 'b');
        stats_bi[bad_sfb].skip = 0;
        int lateral_sfb = find_stat_index("Lateral Same Finger Bigram", 'b');
        stats_bi[lateral_sfb].skip = 0;

        stats_meta[index].stat_types[sub_index] = 'b';
        stats_meta[index].stat_indices[sub_index] = sfb;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = 'b';
        stats_meta[index].stat_indices[sub_index] = bad_sfb;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = 'b';
        stats_meta[index].stat_indices[sub_index] = lateral_sfb;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;

        int sfs = find_stat_index("Same Finger Skipgram", '1');
        stats_skip[sfs].skip = 0;
        int bad_sfs = find_stat_index("Bad Same Finger Skipgram", '1');
        stats_skip[bad_sfs].skip = 0;
        int lateral_sfs = find_stat_index("Lateral Same Finger Skipgram", '1');
        stats_skip[lateral_sfs].skip = 0;

        current_weight *= weight_multiplier;
        stats_meta[index].stat_types[sub_index] = '1';
        stats_meta[index].stat_indices[sub_index] = sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '1';
        stats_meta[index].stat_indices[sub_index] = bad_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '1';
        stats_meta[index].stat_indices[sub_index] = lateral_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;

        current_weight *= weight_multiplier;
        stats_meta[index].stat_types[sub_index] = '2';
        stats_meta[index].stat_indices[sub_index] = sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '2';
        stats_meta[index].stat_indices[sub_index] = bad_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '2';
        stats_meta[index].stat_indices[sub_index] = lateral_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;

        current_weight *= weight_multiplier;
        stats_meta[index].stat_types[sub_index] = '3';
        stats_meta[index].stat_indices[sub_index] = sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '3';
        stats_meta[index].stat_indices[sub_index] = bad_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '3';
        stats_meta[index].stat_indices[sub_index] = lateral_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;

        current_weight *= weight_multiplier;
        stats_meta[index].stat_types[sub_index] = '4';
        stats_meta[index].stat_indices[sub_index] = sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '4';
        stats_meta[index].stat_indices[sub_index] = bad_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '4';
        stats_meta[index].stat_indices[sub_index] = lateral_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;

        current_weight *= weight_multiplier;
        stats_meta[index].stat_types[sub_index] = '5';
        stats_meta[index].stat_indices[sub_index] = sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '5';
        stats_meta[index].stat_indices[sub_index] = bad_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '5';
        stats_meta[index].stat_indices[sub_index] = lateral_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;

        current_weight *= weight_multiplier;
        stats_meta[index].stat_types[sub_index] = '6';
        stats_meta[index].stat_indices[sub_index] = sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '6';
        stats_meta[index].stat_indices[sub_index] = bad_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '6';
        stats_meta[index].stat_indices[sub_index] = lateral_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;

        current_weight *= weight_multiplier;
        stats_meta[index].stat_types[sub_index] = '7';
        stats_meta[index].stat_indices[sub_index] = sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '7';
        stats_meta[index].stat_indices[sub_index] = bad_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '7';
        stats_meta[index].stat_indices[sub_index] = lateral_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;

        current_weight *= weight_multiplier;
        stats_meta[index].stat_types[sub_index] = '8';
        stats_meta[index].stat_indices[sub_index] = sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '8';
        stats_meta[index].stat_indices[sub_index] = bad_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '8';
        stats_meta[index].stat_indices[sub_index] = lateral_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;

        current_weight *= weight_multiplier;
        stats_meta[index].stat_types[sub_index] = '9';
        stats_meta[index].stat_indices[sub_index] = sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '9';
        stats_meta[index].stat_indices[sub_index] = bad_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '9';
        stats_meta[index].stat_indices[sub_index] = lateral_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;

        stats_meta[index].stat_types[sub_index] = 'x';
        stats_meta[index].absv = 0;
    }
    index++;

    /* PER FINGER RuSpeed */
    if (!stats_meta[index].skip)
    {
        int sub_index = 0;
        float current_weight = 1;
        float weight_multiplier = 0.25;

        int sfb = find_stat_index("Left Pinky Bigram", 'b');
        stats_bi[sfb].skip = 0;
        int bad_sfb = find_stat_index("Bad Left Pinky Bigram", 'b');
        stats_bi[bad_sfb].skip = 0;
        int lateral_sfb = find_stat_index("Lateral Left Pinky Bigram", 'b');
        stats_bi[lateral_sfb].skip = 0;

        stats_meta[index].stat_types[sub_index] = 'b';
        stats_meta[index].stat_indices[sub_index] = sfb;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = 'b';
        stats_meta[index].stat_indices[sub_index] = bad_sfb;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = 'b';
        stats_meta[index].stat_indices[sub_index] = lateral_sfb;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;

        int sfs = find_stat_index("Left Pinky Skipgram", '1');
        stats_skip[sfs].skip = 0;
        int bad_sfs = find_stat_index("Bad Left Pinky Skipgram", '1');
        stats_skip[bad_sfs].skip = 0;
        int lateral_sfs = find_stat_index("Lateral Left Pinky Skipgram", '1');
        stats_skip[lateral_sfs].skip = 0;

        current_weight *= weight_multiplier;
        stats_meta[index].stat_types[sub_index] = '1';
        stats_meta[index].stat_indices[sub_index] = sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '1';
        stats_meta[index].stat_indices[sub_index] = bad_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '1';
        stats_meta[index].stat_indices[sub_index] = lateral_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;

        current_weight *= weight_multiplier;
        stats_meta[index].stat_types[sub_index] = '2';
        stats_meta[index].stat_indices[sub_index] = sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '2';
        stats_meta[index].stat_indices[sub_index] = bad_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '2';
        stats_meta[index].stat_indices[sub_index] = lateral_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;

        current_weight *= weight_multiplier;
        stats_meta[index].stat_types[sub_index] = '3';
        stats_meta[index].stat_indices[sub_index] = sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '3';
        stats_meta[index].stat_indices[sub_index] = bad_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '3';
        stats_meta[index].stat_indices[sub_index] = lateral_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;

        current_weight *= weight_multiplier;
        stats_meta[index].stat_types[sub_index] = '4';
        stats_meta[index].stat_indices[sub_index] = sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '4';
        stats_meta[index].stat_indices[sub_index] = bad_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '4';
        stats_meta[index].stat_indices[sub_index] = lateral_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;

        current_weight *= weight_multiplier;
        stats_meta[index].stat_types[sub_index] = '5';
        stats_meta[index].stat_indices[sub_index] = sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '5';
        stats_meta[index].stat_indices[sub_index] = bad_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '5';
        stats_meta[index].stat_indices[sub_index] = lateral_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;

        current_weight *= weight_multiplier;
        stats_meta[index].stat_types[sub_index] = '6';
        stats_meta[index].stat_indices[sub_index] = sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '6';
        stats_meta[index].stat_indices[sub_index] = bad_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '6';
        stats_meta[index].stat_indices[sub_index] = lateral_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;

        current_weight *= weight_multiplier;
        stats_meta[index].stat_types[sub_index] = '7';
        stats_meta[index].stat_indices[sub_index] = sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '7';
        stats_meta[index].stat_indices[sub_index] = bad_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '7';
        stats_meta[index].stat_indices[sub_index] = lateral_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;

        current_weight *= weight_multiplier;
        stats_meta[index].stat_types[sub_index] = '8';
        stats_meta[index].stat_indices[sub_index] = sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '8';
        stats_meta[index].stat_indices[sub_index] = bad_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '8';
        stats_meta[index].stat_indices[sub_index] = lateral_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;

        current_weight *= weight_multiplier;
        stats_meta[index].stat_types[sub_index] = '9';
        stats_meta[index].stat_indices[sub_index] = sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '9';
        stats_meta[index].stat_indices[sub_index] = bad_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '9';
        stats_meta[index].stat_indices[sub_index] = lateral_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;

        stats_meta[index].stat_types[sub_index] = 'x';
        stats_meta[index].absv = 0;
    }
    index++;

    if (!stats_meta[index].skip)
    {
        int sub_index = 0;
        float current_weight = 1;
        float weight_multiplier = 0.25;

        int sfb = find_stat_index("Left Ring Bigram", 'b');
        stats_bi[sfb].skip = 0;

        int bad_sfb = find_stat_index("Bad Left Ring Bigram", 'b');
        stats_bi[bad_sfb].skip = 0;

        stats_meta[index].stat_types[sub_index] = 'b';
        stats_meta[index].stat_indices[sub_index] = sfb;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = 'b';
        stats_meta[index].stat_indices[sub_index] = bad_sfb;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;

        int sfs = find_stat_index("Left Ring Skipgram", '1');
        stats_skip[sfs].skip = 0;
        int bad_sfs = find_stat_index("Bad Left Ring Skipgram", '1');
        stats_skip[bad_sfs].skip = 0;

        current_weight *= weight_multiplier;
        stats_meta[index].stat_types[sub_index] = '1';
        stats_meta[index].stat_indices[sub_index] = sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '1';
        stats_meta[index].stat_indices[sub_index] = bad_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;

        current_weight *= weight_multiplier;
        stats_meta[index].stat_types[sub_index] = '2';
        stats_meta[index].stat_indices[sub_index] = sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '2';
        stats_meta[index].stat_indices[sub_index] = bad_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;

        current_weight *= weight_multiplier;
        stats_meta[index].stat_types[sub_index] = '3';
        stats_meta[index].stat_indices[sub_index] = sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '3';
        stats_meta[index].stat_indices[sub_index] = bad_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;

        current_weight *= weight_multiplier;
        stats_meta[index].stat_types[sub_index] = '4';
        stats_meta[index].stat_indices[sub_index] = sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '4';
        stats_meta[index].stat_indices[sub_index] = bad_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;

        current_weight *= weight_multiplier;
        stats_meta[index].stat_types[sub_index] = '5';
        stats_meta[index].stat_indices[sub_index] = sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '5';
        stats_meta[index].stat_indices[sub_index] = bad_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;

        current_weight *= weight_multiplier;
        stats_meta[index].stat_types[sub_index] = '6';
        stats_meta[index].stat_indices[sub_index] = sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '6';
        stats_meta[index].stat_indices[sub_index] = bad_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;

        current_weight *= weight_multiplier;
        stats_meta[index].stat_types[sub_index] = '7';
        stats_meta[index].stat_indices[sub_index] = sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '7';
        stats_meta[index].stat_indices[sub_index] = bad_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;

        current_weight *= weight_multiplier;
        stats_meta[index].stat_types[sub_index] = '8';
        stats_meta[index].stat_indices[sub_index] = sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '8';
        stats_meta[index].stat_indices[sub_index] = bad_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;

        current_weight *= weight_multiplier;
        stats_meta[index].stat_types[sub_index] = '9';
        stats_meta[index].stat_indices[sub_index] = sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '9';
        stats_meta[index].stat_indices[sub_index] = bad_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;

        stats_meta[index].stat_types[sub_index] = 'x';
        stats_meta[index].absv = 0;
    }
    index++;

    if (!stats_meta[index].skip)
    {
        int sub_index = 0;
        float current_weight = 1;
        float weight_multiplier = 0.25;

        int sfb = find_stat_index("Left Middle Bigram", 'b');
        stats_bi[sfb].skip = 0;
        int bad_sfb = find_stat_index("Bad Left Middle Bigram", 'b');
        stats_bi[bad_sfb].skip = 0;

        stats_meta[index].stat_types[sub_index] = 'b';
        stats_meta[index].stat_indices[sub_index] = sfb;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = 'b';
        stats_meta[index].stat_indices[sub_index] = bad_sfb;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;

        int sfs = find_stat_index("Left Middle Skipgram", '1');
        stats_skip[sfs].skip = 0;
        int bad_sfs = find_stat_index("Bad Left Middle Skipgram", '1');
        stats_skip[bad_sfs].skip = 0;

        current_weight *= weight_multiplier;
        stats_meta[index].stat_types[sub_index] = '1';
        stats_meta[index].stat_indices[sub_index] = sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '1';
        stats_meta[index].stat_indices[sub_index] = bad_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;

        current_weight *= weight_multiplier;
        stats_meta[index].stat_types[sub_index] = '2';
        stats_meta[index].stat_indices[sub_index] = sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '2';
        stats_meta[index].stat_indices[sub_index] = bad_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;

        current_weight *= weight_multiplier;
        stats_meta[index].stat_types[sub_index] = '3';
        stats_meta[index].stat_indices[sub_index] = sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '3';
        stats_meta[index].stat_indices[sub_index] = bad_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;

        current_weight *= weight_multiplier;
        stats_meta[index].stat_types[sub_index] = '4';
        stats_meta[index].stat_indices[sub_index] = sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '4';
        stats_meta[index].stat_indices[sub_index] = bad_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;

        current_weight *= weight_multiplier;
        stats_meta[index].stat_types[sub_index] = '5';
        stats_meta[index].stat_indices[sub_index] = sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '5';
        stats_meta[index].stat_indices[sub_index] = bad_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;

        current_weight *= weight_multiplier;
        stats_meta[index].stat_types[sub_index] = '6';
        stats_meta[index].stat_indices[sub_index] = sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '6';
        stats_meta[index].stat_indices[sub_index] = bad_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;

        current_weight *= weight_multiplier;
        stats_meta[index].stat_types[sub_index] = '7';
        stats_meta[index].stat_indices[sub_index] = sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '7';
        stats_meta[index].stat_indices[sub_index] = bad_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;

        current_weight *= weight_multiplier;
        stats_meta[index].stat_types[sub_index] = '8';
        stats_meta[index].stat_indices[sub_index] = sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '8';
        stats_meta[index].stat_indices[sub_index] = bad_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;

        current_weight *= weight_multiplier;
        stats_meta[index].stat_types[sub_index] = '9';
        stats_meta[index].stat_indices[sub_index] = sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '9';
        stats_meta[index].stat_indices[sub_index] = bad_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;

        stats_meta[index].stat_types[sub_index] = 'x';
        stats_meta[index].absv = 0;
    }
    index++;

    if (!stats_meta[index].skip)
    {
        int sub_index = 0;
        float current_weight = 1;
        float weight_multiplier = 0.25;

        int sfb = find_stat_index("Left Index Bigram", 'b');
        stats_bi[sfb].skip = 0;
        int bad_sfb = find_stat_index("Bad Left Index Bigram", 'b');
        stats_bi[bad_sfb].skip = 0;
        int lateral_sfb = find_stat_index("Lateral Left Index Bigram", 'b');
        stats_bi[lateral_sfb].skip = 0;

        stats_meta[index].stat_types[sub_index] = 'b';
        stats_meta[index].stat_indices[sub_index] = sfb;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = 'b';
        stats_meta[index].stat_indices[sub_index] = bad_sfb;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = 'b';
        stats_meta[index].stat_indices[sub_index] = lateral_sfb;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;

        int sfs = find_stat_index("Left Index Skipgram", '1');
        stats_skip[sfs].skip = 0;
        int bad_sfs = find_stat_index("Bad Left Index Skipgram", '1');
        stats_skip[bad_sfs].skip = 0;
        int lateral_sfs = find_stat_index("Lateral Left Index Skipgram", '1');
        stats_skip[lateral_sfs].skip = 0;

        current_weight *= weight_multiplier;
        stats_meta[index].stat_types[sub_index] = '1';
        stats_meta[index].stat_indices[sub_index] = sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '1';
        stats_meta[index].stat_indices[sub_index] = bad_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '1';
        stats_meta[index].stat_indices[sub_index] = lateral_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;

        current_weight *= weight_multiplier;
        stats_meta[index].stat_types[sub_index] = '2';
        stats_meta[index].stat_indices[sub_index] = sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '2';
        stats_meta[index].stat_indices[sub_index] = bad_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '2';
        stats_meta[index].stat_indices[sub_index] = lateral_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;

        current_weight *= weight_multiplier;
        stats_meta[index].stat_types[sub_index] = '3';
        stats_meta[index].stat_indices[sub_index] = sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '3';
        stats_meta[index].stat_indices[sub_index] = bad_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '3';
        stats_meta[index].stat_indices[sub_index] = lateral_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;

        current_weight *= weight_multiplier;
        stats_meta[index].stat_types[sub_index] = '4';
        stats_meta[index].stat_indices[sub_index] = sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '4';
        stats_meta[index].stat_indices[sub_index] = bad_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '4';
        stats_meta[index].stat_indices[sub_index] = lateral_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;

        current_weight *= weight_multiplier;
        stats_meta[index].stat_types[sub_index] = '5';
        stats_meta[index].stat_indices[sub_index] = sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '5';
        stats_meta[index].stat_indices[sub_index] = bad_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '5';
        stats_meta[index].stat_indices[sub_index] = lateral_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;

        current_weight *= weight_multiplier;
        stats_meta[index].stat_types[sub_index] = '6';
        stats_meta[index].stat_indices[sub_index] = sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '6';
        stats_meta[index].stat_indices[sub_index] = bad_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '6';
        stats_meta[index].stat_indices[sub_index] = lateral_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;

        current_weight *= weight_multiplier;
        stats_meta[index].stat_types[sub_index] = '7';
        stats_meta[index].stat_indices[sub_index] = sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '7';
        stats_meta[index].stat_indices[sub_index] = bad_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '7';
        stats_meta[index].stat_indices[sub_index] = lateral_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;

        current_weight *= weight_multiplier;
        stats_meta[index].stat_types[sub_index] = '8';
        stats_meta[index].stat_indices[sub_index] = sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '8';
        stats_meta[index].stat_indices[sub_index] = bad_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '8';
        stats_meta[index].stat_indices[sub_index] = lateral_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;

        current_weight *= weight_multiplier;
        stats_meta[index].stat_types[sub_index] = '9';
        stats_meta[index].stat_indices[sub_index] = sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '9';
        stats_meta[index].stat_indices[sub_index] = bad_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '9';
        stats_meta[index].stat_indices[sub_index] = lateral_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;

        stats_meta[index].stat_types[sub_index] = 'x';
        stats_meta[index].absv = 0;
    }
    index++;

    if (!stats_meta[index].skip)
    {
        int sub_index = 0;
        float current_weight = 1;
        float weight_multiplier = 0.25;

        int sfb = find_stat_index("Right Index Bigram", 'b');
        stats_bi[sfb].skip = 0;
        int bad_sfb = find_stat_index("Bad Right Index Bigram", 'b');
        stats_bi[bad_sfb].skip = 0;
        int lateral_sfb = find_stat_index("Lateral Right Index Bigram", 'b');
        stats_bi[lateral_sfb].skip = 0;

        stats_meta[index].stat_types[sub_index] = 'b';
        stats_meta[index].stat_indices[sub_index] = sfb;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = 'b';
        stats_meta[index].stat_indices[sub_index] = bad_sfb;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = 'b';
        stats_meta[index].stat_indices[sub_index] = lateral_sfb;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;

        int sfs = find_stat_index("Right Index Skipgram", '1');
        stats_skip[sfs].skip = 0;
        int bad_sfs = find_stat_index("Bad Right Index Skipgram", '1');
        stats_skip[bad_sfs].skip = 0;
        int lateral_sfs = find_stat_index("Lateral Right Index Skipgram", '1');
        stats_skip[lateral_sfs].skip = 0;

        current_weight *= weight_multiplier;
        stats_meta[index].stat_types[sub_index] = '1';
        stats_meta[index].stat_indices[sub_index] = sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '1';
        stats_meta[index].stat_indices[sub_index] = bad_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '1';
        stats_meta[index].stat_indices[sub_index] = lateral_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;

        current_weight *= weight_multiplier;
        stats_meta[index].stat_types[sub_index] = '2';
        stats_meta[index].stat_indices[sub_index] = sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '2';
        stats_meta[index].stat_indices[sub_index] = bad_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '2';
        stats_meta[index].stat_indices[sub_index] = lateral_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;

        current_weight *= weight_multiplier;
        stats_meta[index].stat_types[sub_index] = '3';
        stats_meta[index].stat_indices[sub_index] = sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '3';
        stats_meta[index].stat_indices[sub_index] = bad_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '3';
        stats_meta[index].stat_indices[sub_index] = lateral_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;

        current_weight *= weight_multiplier;
        stats_meta[index].stat_types[sub_index] = '4';
        stats_meta[index].stat_indices[sub_index] = sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '4';
        stats_meta[index].stat_indices[sub_index] = bad_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '4';
        stats_meta[index].stat_indices[sub_index] = lateral_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;

        current_weight *= weight_multiplier;
        stats_meta[index].stat_types[sub_index] = '5';
        stats_meta[index].stat_indices[sub_index] = sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '5';
        stats_meta[index].stat_indices[sub_index] = bad_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '5';
        stats_meta[index].stat_indices[sub_index] = lateral_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;

        current_weight *= weight_multiplier;
        stats_meta[index].stat_types[sub_index] = '6';
        stats_meta[index].stat_indices[sub_index] = sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '6';
        stats_meta[index].stat_indices[sub_index] = bad_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '6';
        stats_meta[index].stat_indices[sub_index] = lateral_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;

        current_weight *= weight_multiplier;
        stats_meta[index].stat_types[sub_index] = '7';
        stats_meta[index].stat_indices[sub_index] = sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '7';
        stats_meta[index].stat_indices[sub_index] = bad_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '7';
        stats_meta[index].stat_indices[sub_index] = lateral_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;

        current_weight *= weight_multiplier;
        stats_meta[index].stat_types[sub_index] = '8';
        stats_meta[index].stat_indices[sub_index] = sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '8';
        stats_meta[index].stat_indices[sub_index] = bad_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '8';
        stats_meta[index].stat_indices[sub_index] = lateral_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;

        current_weight *= weight_multiplier;
        stats_meta[index].stat_types[sub_index] = '9';
        stats_meta[index].stat_indices[sub_index] = sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '9';
        stats_meta[index].stat_indices[sub_index] = bad_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '9';
        stats_meta[index].stat_indices[sub_index] = lateral_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;

        stats_meta[index].stat_types[sub_index] = 'x';
        stats_meta[index].absv = 0;
    }
    index++;

    if (!stats_meta[index].skip)
    {
        int sub_index = 0;
        float current_weight = 1;
        float weight_multiplier = 0.25;

        int sfb = find_stat_index("Right Middle Bigram", 'b');
        stats_bi[sfb].skip = 0;
        int bad_sfb = find_stat_index("Bad Right Middle Bigram", 'b');
        stats_bi[bad_sfb].skip = 0;

        stats_meta[index].stat_types[sub_index] = 'b';
        stats_meta[index].stat_indices[sub_index] = sfb;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = 'b';
        stats_meta[index].stat_indices[sub_index] = bad_sfb;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;

        int sfs = find_stat_index("Right Middle Skipgram", '1');
        stats_skip[sfs].skip = 0;
        int bad_sfs = find_stat_index("Bad Right Middle Skipgram", '1');
        stats_skip[bad_sfs].skip = 0;

        current_weight *= weight_multiplier;
        stats_meta[index].stat_types[sub_index] = '1';
        stats_meta[index].stat_indices[sub_index] = sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '1';
        stats_meta[index].stat_indices[sub_index] = bad_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;

        current_weight *= weight_multiplier;
        stats_meta[index].stat_types[sub_index] = '2';
        stats_meta[index].stat_indices[sub_index] = sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '2';
        stats_meta[index].stat_indices[sub_index] = bad_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;

        current_weight *= weight_multiplier;
        stats_meta[index].stat_types[sub_index] = '3';
        stats_meta[index].stat_indices[sub_index] = sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '3';
        stats_meta[index].stat_indices[sub_index] = bad_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;

        current_weight *= weight_multiplier;
        stats_meta[index].stat_types[sub_index] = '4';
        stats_meta[index].stat_indices[sub_index] = sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '4';
        stats_meta[index].stat_indices[sub_index] = bad_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;

        current_weight *= weight_multiplier;
        stats_meta[index].stat_types[sub_index] = '5';
        stats_meta[index].stat_indices[sub_index] = sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '5';
        stats_meta[index].stat_indices[sub_index] = bad_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;

        current_weight *= weight_multiplier;
        stats_meta[index].stat_types[sub_index] = '6';
        stats_meta[index].stat_indices[sub_index] = sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '6';
        stats_meta[index].stat_indices[sub_index] = bad_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;

        current_weight *= weight_multiplier;
        stats_meta[index].stat_types[sub_index] = '7';
        stats_meta[index].stat_indices[sub_index] = sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '7';
        stats_meta[index].stat_indices[sub_index] = bad_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;

        current_weight *= weight_multiplier;
        stats_meta[index].stat_types[sub_index] = '8';
        stats_meta[index].stat_indices[sub_index] = sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '8';
        stats_meta[index].stat_indices[sub_index] = bad_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;

        current_weight *= weight_multiplier;
        stats_meta[index].stat_types[sub_index] = '9';
        stats_meta[index].stat_indices[sub_index] = sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '9';
        stats_meta[index].stat_indices[sub_index] = bad_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;

        stats_meta[index].stat_types[sub_index] = 'x';
        stats_meta[index].absv = 0;
    }
    index++;

    if (!stats_meta[index].skip)
    {
        int sub_index = 0;
        float current_weight = 1;
        float weight_multiplier = 0.25;

        int sfb = find_stat_index("Right Ring Bigram", 'b');
        stats_bi[sfb].skip = 0;
        int bad_sfb = find_stat_index("Bad Right Ring Bigram", 'b');
        stats_bi[bad_sfb].skip = 0;

        stats_meta[index].stat_types[sub_index] = 'b';
        stats_meta[index].stat_indices[sub_index] = sfb;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = 'b';
        stats_meta[index].stat_indices[sub_index] = bad_sfb;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;

        int sfs = find_stat_index("Right Ring Skipgram", '1');
        stats_skip[sfs].skip = 0;
        int bad_sfs = find_stat_index("Bad Right Ring Skipgram", '1');
        stats_skip[bad_sfs].skip = 0;

        current_weight *= weight_multiplier;
        stats_meta[index].stat_types[sub_index] = '1';
        stats_meta[index].stat_indices[sub_index] = sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '1';
        stats_meta[index].stat_indices[sub_index] = bad_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;

        current_weight *= weight_multiplier;
        stats_meta[index].stat_types[sub_index] = '2';
        stats_meta[index].stat_indices[sub_index] = sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '2';
        stats_meta[index].stat_indices[sub_index] = bad_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;

        current_weight *= weight_multiplier;
        stats_meta[index].stat_types[sub_index] = '3';
        stats_meta[index].stat_indices[sub_index] = sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '3';
        stats_meta[index].stat_indices[sub_index] = bad_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;

        current_weight *= weight_multiplier;
        stats_meta[index].stat_types[sub_index] = '4';
        stats_meta[index].stat_indices[sub_index] = sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '4';
        stats_meta[index].stat_indices[sub_index] = bad_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;

        current_weight *= weight_multiplier;
        stats_meta[index].stat_types[sub_index] = '5';
        stats_meta[index].stat_indices[sub_index] = sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '5';
        stats_meta[index].stat_indices[sub_index] = bad_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;

        current_weight *= weight_multiplier;
        stats_meta[index].stat_types[sub_index] = '6';
        stats_meta[index].stat_indices[sub_index] = sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '6';
        stats_meta[index].stat_indices[sub_index] = bad_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;

        current_weight *= weight_multiplier;
        stats_meta[index].stat_types[sub_index] = '7';
        stats_meta[index].stat_indices[sub_index] = sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '7';
        stats_meta[index].stat_indices[sub_index] = bad_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;

        current_weight *= weight_multiplier;
        stats_meta[index].stat_types[sub_index] = '8';
        stats_meta[index].stat_indices[sub_index] = sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '8';
        stats_meta[index].stat_indices[sub_index] = bad_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;

        current_weight *= weight_multiplier;
        stats_meta[index].stat_types[sub_index] = '9';
        stats_meta[index].stat_indices[sub_index] = sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '9';
        stats_meta[index].stat_indices[sub_index] = bad_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;

        stats_meta[index].stat_types[sub_index] = 'x';
        stats_meta[index].absv = 0;
    }
    index++;

    if (!stats_meta[index].skip)
    {
        int sub_index = 0;
        float current_weight = 1;
        float weight_multiplier = 0.25;

        int sfb = find_stat_index("Right Pinky Bigram", 'b');
        stats_bi[sfb].skip = 0;
        int bad_sfb = find_stat_index("Bad Right Pinky Bigram", 'b');
        stats_bi[bad_sfb].skip = 0;
        int lateral_sfb = find_stat_index("Lateral Right Pinky Bigram", 'b');
        stats_bi[lateral_sfb].skip = 0;

        stats_meta[index].stat_types[sub_index] = 'b';
        stats_meta[index].stat_indices[sub_index] = sfb;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = 'b';
        stats_meta[index].stat_indices[sub_index] = bad_sfb;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = 'b';
        stats_meta[index].stat_indices[sub_index] = lateral_sfb;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;

        int sfs = find_stat_index("Right Pinky Skipgram", '1');
        stats_skip[sfs].skip = 0;
        int bad_sfs = find_stat_index("Bad Right Pinky Skipgram", '1');
        stats_skip[bad_sfs].skip = 0;
        int lateral_sfs = find_stat_index("Lateral Right Pinky Skipgram", '1');
        stats_skip[lateral_sfs].skip = 0;

        current_weight *= weight_multiplier;
        stats_meta[index].stat_types[sub_index] = '1';
        stats_meta[index].stat_indices[sub_index] = sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '1';
        stats_meta[index].stat_indices[sub_index] = bad_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '1';
        stats_meta[index].stat_indices[sub_index] = lateral_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;

        current_weight *= weight_multiplier;
        stats_meta[index].stat_types[sub_index] = '2';
        stats_meta[index].stat_indices[sub_index] = sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '2';
        stats_meta[index].stat_indices[sub_index] = bad_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '2';
        stats_meta[index].stat_indices[sub_index] = lateral_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;

        current_weight *= weight_multiplier;
        stats_meta[index].stat_types[sub_index] = '3';
        stats_meta[index].stat_indices[sub_index] = sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '3';
        stats_meta[index].stat_indices[sub_index] = bad_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '3';
        stats_meta[index].stat_indices[sub_index] = lateral_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;

        current_weight *= weight_multiplier;
        stats_meta[index].stat_types[sub_index] = '4';
        stats_meta[index].stat_indices[sub_index] = sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '4';
        stats_meta[index].stat_indices[sub_index] = bad_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '4';
        stats_meta[index].stat_indices[sub_index] = lateral_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;

        current_weight *= weight_multiplier;
        stats_meta[index].stat_types[sub_index] = '5';
        stats_meta[index].stat_indices[sub_index] = sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '5';
        stats_meta[index].stat_indices[sub_index] = bad_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '5';
        stats_meta[index].stat_indices[sub_index] = lateral_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;

        current_weight *= weight_multiplier;
        stats_meta[index].stat_types[sub_index] = '6';
        stats_meta[index].stat_indices[sub_index] = sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '6';
        stats_meta[index].stat_indices[sub_index] = bad_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '6';
        stats_meta[index].stat_indices[sub_index] = lateral_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;

        current_weight *= weight_multiplier;
        stats_meta[index].stat_types[sub_index] = '7';
        stats_meta[index].stat_indices[sub_index] = sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '7';
        stats_meta[index].stat_indices[sub_index] = bad_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '7';
        stats_meta[index].stat_indices[sub_index] = lateral_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;

        current_weight *= weight_multiplier;
        stats_meta[index].stat_types[sub_index] = '8';
        stats_meta[index].stat_indices[sub_index] = sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '8';
        stats_meta[index].stat_indices[sub_index] = bad_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '8';
        stats_meta[index].stat_indices[sub_index] = lateral_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;

        current_weight *= weight_multiplier;
        stats_meta[index].stat_types[sub_index] = '9';
        stats_meta[index].stat_indices[sub_index] = sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '9';
        stats_meta[index].stat_indices[sub_index] = bad_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;
        stats_meta[index].stat_types[sub_index] = '9';
        stats_meta[index].stat_indices[sub_index] = lateral_sfs;
        stats_meta[index].stat_weights[sub_index] = current_weight;
        sub_index++;

        stats_meta[index].stat_types[sub_index] = 'x';
        stats_meta[index].absv = 0;
    }
    index++;
}

/* Frees the memory allocated for the meta statistics array. */
void free_meta_stats()
{
    free(stats_meta);
}
