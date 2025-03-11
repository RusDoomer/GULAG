/*
 * stats/skip.c - Skipgram statistic definitions.
 *
 * This file contains the implementation for initializing, cleaning, converting,
 * and freeing skipgram statistics used in the GULAG. Bigram statistics track
 * the frequency and positioning of two character sequences with
 * 1 to 9 skipped characters in the middle.
 *
 * Adding new stats:
 *     1. Incease SKIP_LENGTH by as many stats as you are adding.
 *     2. Define its name, keep it a reasonable length.
 *     3. Set its weight to -INFINITY, and skip to 0 (to be changed later).
 *     4. Set its length to 0, then loop through the DIM2 (36^2) sequences.
 *       4a. Use unflat_bi() to convert the 1D index to a set of 2D coordinates.
 *       4b. Check if the ngram falls under the stat.
 *       4c. If it does, add it to the ngrams array and increment length.
 *       4d. Otherwise set the ngram array element to -1.
 *     5. Iterate the index.
 *     6. Add the statistic to the weights files in data/weights/.
 */

#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "skip.h"
#include "util.h"
#include "stats_util.h"
#include "global.h"
#include "structs.h"

/*
 * Initializes the array of skipgram statistics. The function allocates memory
 * for the stat array and sets default values, including a negative infinity
 * weight which will be later overwritten.
 */
void initialize_skip_stats()
{
    SKIP_LENGTH = 23;
    stats_skip = (skip_stat *)malloc(sizeof(skip_stat) * SKIP_LENGTH);
    int row0, col0, row1, col1;
    int index = 0;

    /* Initialize SFS. */
    strcpy(stats_skip[index].name, "Same Finger Skipgram");
    /* Set all skip weights to -INFINITY. */
    for (int i = 0; i < 10; i++) {stats_skip[index].weight[i] = -INFINITY;}
    stats_skip[index].length = 0;
    stats_skip[index].skip = 0;
    for (int i = 0; i < DIM2; i++)
    {
        /* convert a 1D index into a 4D matrix coordinate */
        unflat_bi(i, &row0, &col0, &row1, &col1); /* util.c */
        if (is_same_finger_bi(row0, col0, row1, col1))
        {
            stats_skip[index].ngrams[i] = i;
            stats_skip[index].length++;
        }
        else
        {
            stats_skip[index].ngrams[i] = -1;
        }
    }
    index++;

    /* per finger SFS */
    strcpy(stats_skip[index].name, "Left Pinky Skipgram");
    for (int i = 0; i < 10; i++) {stats_skip[index].weight[i] = -INFINITY;}
    stats_skip[index].length = 0;
    stats_skip[index].skip = 0;
    for (int i = 0; i < DIM2; i++)
    {
        unflat_bi(i, &row0, &col0, &row1, &col1);
        if (is_same_finger_bi(row0, col0, row1, col1) && finger(row0, col0) == 0)
        {
            stats_skip[index].ngrams[i] = i;
            stats_skip[index].length++;
        }
        else
        {
            stats_skip[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_skip[index].name, "Left Ring Skipgram");
    for (int i = 0; i < 10; i++) {stats_skip[index].weight[i] = -INFINITY;}
    stats_skip[index].length = 0;
    stats_skip[index].skip = 0;
    for (int i = 0; i < DIM2; i++)
    {
        unflat_bi(i, &row0, &col0, &row1, &col1);
        if (is_same_finger_bi(row0, col0, row1, col1) && finger(row0, col0) == 1)
        {
            stats_skip[index].ngrams[i] = i;
            stats_skip[index].length++;
        }
        else
        {
            stats_skip[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_skip[index].name, "Left Middle Skipgram");
    for (int i = 0; i < 10; i++) {stats_skip[index].weight[i] = -INFINITY;}
    stats_skip[index].length = 0;
    stats_skip[index].skip = 0;
    for (int i = 0; i < DIM2; i++)
    {
        unflat_bi(i, &row0, &col0, &row1, &col1);
        if (is_same_finger_bi(row0, col0, row1, col1) && finger(row0, col0) == 2)
        {
            stats_skip[index].ngrams[i] = i;
            stats_skip[index].length++;
        }
        else
        {
            stats_skip[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_skip[index].name, "Left Index Skipgram");
    for (int i = 0; i < 10; i++) {stats_skip[index].weight[i] = -INFINITY;}
    stats_skip[index].length = 0;
    stats_skip[index].skip = 0;
    for (int i = 0; i < DIM2; i++)
    {
        unflat_bi(i, &row0, &col0, &row1, &col1);
        if (is_same_finger_bi(row0, col0, row1, col1) && finger(row0, col0) == 3)
        {
            stats_skip[index].ngrams[i] = i;
            stats_skip[index].length++;
        }
        else
        {
            stats_skip[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_skip[index].name, "Right Index Skipgram");
    for (int i = 0; i < 10; i++) {stats_skip[index].weight[i] = -INFINITY;}
    stats_skip[index].length = 0;
    stats_skip[index].skip = 0;
    for (int i = 0; i < DIM2; i++)
    {
        unflat_bi(i, &row0, &col0, &row1, &col1);
        if (is_same_finger_bi(row0, col0, row1, col1) && finger(row0, col0) == 4)
        {
            stats_skip[index].ngrams[i] = i;
            stats_skip[index].length++;
        }
        else
        {
            stats_skip[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_skip[index].name, "Right Middle Skipgram");
    for (int i = 0; i < 10; i++) {stats_skip[index].weight[i] = -INFINITY;}
    stats_skip[index].length = 0;
    stats_skip[index].skip = 0;
    for (int i = 0; i < DIM2; i++)
    {
        unflat_bi(i, &row0, &col0, &row1, &col1);
        if (is_same_finger_bi(row0, col0, row1, col1) && finger(row0, col0) == 5)
        {
            stats_skip[index].ngrams[i] = i;
            stats_skip[index].length++;
        }
        else
        {
            stats_skip[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_skip[index].name, "Right Ring Skipgram");
    for (int i = 0; i < 10; i++) {stats_skip[index].weight[i] = -INFINITY;}
    stats_skip[index].length = 0;
    stats_skip[index].skip = 0;
    for (int i = 0; i < DIM2; i++)
    {
        unflat_bi(i, &row0, &col0, &row1, &col1);
        if (is_same_finger_bi(row0, col0, row1, col1) && finger(row0, col0) == 6)
        {
            stats_skip[index].ngrams[i] = i;
            stats_skip[index].length++;
        }
        else
        {
            stats_skip[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_skip[index].name, "Right Pinky Skipgram");
    for (int i = 0; i < 10; i++) {stats_skip[index].weight[i] = -INFINITY;}
    stats_skip[index].length = 0;
    stats_skip[index].skip = 0;
    for (int i = 0; i < DIM2; i++)
    {
        unflat_bi(i, &row0, &col0, &row1, &col1);
        if (is_same_finger_bi(row0, col0, row1, col1) && finger(row0, col0) == 7)
        {
            stats_skip[index].ngrams[i] = i;
            stats_skip[index].length++;
        }
        else
        {
            stats_skip[index].ngrams[i] = -1;
        }
    }
    index++;

    /* 2U SFS */
    strcpy(stats_skip[index].name, "Bad Same Finger Skipgram");
    for (int i = 0; i < 10; i++) {stats_skip[index].weight[i] = -INFINITY;}
    stats_skip[index].length = 0;
    stats_skip[index].skip = 0;
    for (int i = 0; i < DIM2; i++)
    {
        unflat_bi(i, &row0, &col0, &row1, &col1);
        if (is_bad_same_finger_bi(row0, col0, row1, col1))
        {
            stats_skip[index].ngrams[i] = i;
            stats_skip[index].length++;
        }
        else
        {
            stats_skip[index].ngrams[i] = -1;
        }
    }
    index++;

    /* per finger 2U SFS */
    strcpy(stats_skip[index].name, "Bad Left Pinky Skipgram");
    for (int i = 0; i < 10; i++) {stats_skip[index].weight[i] = -INFINITY;}
    stats_skip[index].length = 0;
    stats_skip[index].skip = 0;
    for (int i = 0; i < DIM2; i++)
    {
        unflat_bi(i, &row0, &col0, &row1, &col1);
        if (is_bad_same_finger_bi(row0, col0, row1, col1) && finger(row0, col0) == 0)
        {
            stats_skip[index].ngrams[i] = i;
            stats_skip[index].length++;
        }
        else
        {
            stats_skip[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_skip[index].name, "Bad Left Ring Skipgram");
    for (int i = 0; i < 10; i++) {stats_skip[index].weight[i] = -INFINITY;}
    stats_skip[index].length = 0;
    stats_skip[index].skip = 0;
    for (int i = 0; i < DIM2; i++)
    {
        unflat_bi(i, &row0, &col0, &row1, &col1);
        if (is_bad_same_finger_bi(row0, col0, row1, col1) && finger(row0, col0) == 1)
        {
            stats_skip[index].ngrams[i] = i;
            stats_skip[index].length++;
        }
        else
        {
            stats_skip[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_skip[index].name, "Bad Left Middle Skipgram");
    for (int i = 0; i < 10; i++) {stats_skip[index].weight[i] = -INFINITY;}
    stats_skip[index].length = 0;
    stats_skip[index].skip = 0;
    for (int i = 0; i < DIM2; i++)
    {
        unflat_bi(i, &row0, &col0, &row1, &col1);
        if (is_bad_same_finger_bi(row0, col0, row1, col1) && finger(row0, col0) == 2)
        {
            stats_skip[index].ngrams[i] = i;
            stats_skip[index].length++;
        }
        else
        {
            stats_skip[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_skip[index].name, "Bad Left Index Skipgram");
    for (int i = 0; i < 10; i++) {stats_skip[index].weight[i] = -INFINITY;}
    stats_skip[index].length = 0;
    stats_skip[index].skip = 0;
    for (int i = 0; i < DIM2; i++)
    {
        unflat_bi(i, &row0, &col0, &row1, &col1);
        if (is_bad_same_finger_bi(row0, col0, row1, col1) && finger(row0, col0) == 3)
        {
            stats_skip[index].ngrams[i] = i;
            stats_skip[index].length++;
        }
        else
        {
            stats_skip[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_skip[index].name, "Bad Right Index Skipgram");
    for (int i = 0; i < 10; i++) {stats_skip[index].weight[i] = -INFINITY;}
    stats_skip[index].length = 0;
    stats_skip[index].skip = 0;
    for (int i = 0; i < DIM2; i++)
    {
        unflat_bi(i, &row0, &col0, &row1, &col1);
        if (is_bad_same_finger_bi(row0, col0, row1, col1) && finger(row0, col0) == 4)
        {
            stats_skip[index].ngrams[i] = i;
            stats_skip[index].length++;
        }
        else
        {
            stats_skip[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_skip[index].name, "Bad Right Middle Skipgram");
    for (int i = 0; i < 10; i++) {stats_skip[index].weight[i] = -INFINITY;}
    stats_skip[index].length = 0;
    stats_skip[index].skip = 0;
    for (int i = 0; i < DIM2; i++)
    {
        unflat_bi(i, &row0, &col0, &row1, &col1);
        if (is_bad_same_finger_bi(row0, col0, row1, col1) && finger(row0, col0) == 5)
        {
            stats_skip[index].ngrams[i] = i;
            stats_skip[index].length++;
        }
        else
        {
            stats_skip[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_skip[index].name, "Bad Right Ring Skipgram");
    for (int i = 0; i < 10; i++) {stats_skip[index].weight[i] = -INFINITY;}
    stats_skip[index].length = 0;
    stats_skip[index].skip = 0;
    for (int i = 0; i < DIM2; i++)
    {
        unflat_bi(i, &row0, &col0, &row1, &col1);
        if (is_bad_same_finger_bi(row0, col0, row1, col1) && finger(row0, col0) == 6)
        {
            stats_skip[index].ngrams[i] = i;
            stats_skip[index].length++;
        }
        else
        {
            stats_skip[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_skip[index].name, "Bad Right Pinky Skipgram");
    for (int i = 0; i < 10; i++) {stats_skip[index].weight[i] = -INFINITY;}
    stats_skip[index].length = 0;
    stats_skip[index].skip = 0;
    for (int i = 0; i < DIM2; i++)
    {
        unflat_bi(i, &row0, &col0, &row1, &col1);
        if (is_bad_same_finger_bi(row0, col0, row1, col1) && finger(row0, col0) == 7)
        {
            stats_skip[index].ngrams[i] = i;
            stats_skip[index].length++;
        }
        else
        {
            stats_skip[index].ngrams[i] = -1;
        }
    }
    index++;


    /* initialize lateral SFS*/
    strcpy(stats_skip[index].name, "Lateral Same Finger Skipgram");
    for (int i = 0; i < 10; i++) {stats_skip[index].weight[i] = -INFINITY;}
    stats_skip[index].length = 0;
    stats_skip[index].skip = 0;
    for (int i = 0; i < DIM2; i++)
    {
        unflat_bi(i, &row0, &col0, &row1, &col1);
        if (is_lateral_same_finger_bi(row0, col0, row1, col1))
        {
            stats_skip[index].ngrams[i] = i;
            stats_skip[index].length++;
        }
        else
        {
            stats_skip[index].ngrams[i] = -1;
        }
    }
    index++;

    /* initialize per finger lateral skipgram stats */
    strcpy(stats_skip[index].name, "Lateral Left Pinky Skipgram");
    for (int i = 0; i < 10; i++) {stats_skip[index].weight[i] = -INFINITY;}
    stats_skip[index].length = 0;
    stats_skip[index].skip = 0;
    for (int i = 0; i < DIM2; i++)
    {
        unflat_bi(i, &row0, &col0, &row1, &col1);
        if (is_lateral_same_finger_bi(row0, col0, row1, col1) && finger(row0, col0) == 0)
        {
            stats_skip[index].ngrams[i] = i;
            stats_skip[index].length++;
        }
        else
        {
            stats_skip[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_skip[index].name, "Lateral Left Index Skipgram");
    for (int i = 0; i < 10; i++) {stats_skip[index].weight[i] = -INFINITY;}
    stats_skip[index].length = 0;
    stats_skip[index].skip = 0;
    for (int i = 0; i < DIM2; i++)
    {
        unflat_bi(i, &row0, &col0, &row1, &col1);
        if (is_lateral_same_finger_bi(row0, col0, row1, col1) && finger(row0, col0) == 3)
        {
            stats_skip[index].ngrams[i] = i;
            stats_skip[index].length++;
        }
        else
        {
            stats_skip[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_skip[index].name, "Lateral Right Index Skipgram");
    for (int i = 0; i < 10; i++) {stats_skip[index].weight[i] = -INFINITY;}
    stats_skip[index].length = 0;
    stats_skip[index].skip = 0;
    for (int i = 0; i < DIM2; i++)
    {
        unflat_bi(i, &row0, &col0, &row1, &col1);
        if (is_lateral_same_finger_bi(row0, col0, row1, col1) && finger(row0, col0) == 4)
        {
            stats_skip[index].ngrams[i] = i;
            stats_skip[index].length++;
        }
        else
        {
            stats_skip[index].ngrams[i] = -1;
        }
    }
    index++;


    strcpy(stats_skip[index].name, "Lateral Right Pinky Skipgram");
    for (int i = 0; i < 10; i++) {stats_skip[index].weight[i] = -INFINITY;}
    stats_skip[index].length = 0;
    stats_skip[index].skip = 0;
    for (int i = 0; i < DIM2; i++)
    {
        unflat_bi(i, &row0, &col0, &row1, &col1);
        if (is_lateral_same_finger_bi(row0, col0, row1, col1) && finger(row0, col0) == 7)
        {
            stats_skip[index].ngrams[i] = i;
            stats_skip[index].length++;
        }
        else
        {
            stats_skip[index].ngrams[i] = -1;
        }
    }
    index++;

    if (index != SKIP_LENGTH) {error("SKIP_LENGTH incorrect for number of skip stats");}
}

/*
 * Trims the ngrams in the array to move unused entries to the end.
 * This process ensures memory efficiency by eliminating gaps in the array.
 */
void trim_skip_stats()
{
    for (int i = 0; i < SKIP_LENGTH; i++)
    {
        if (stats_skip[i].length != 0)
        {
            int left = 0;
            int right = DIM2 - 1;

            /* Use two pointers to partition the array */
            while (left < right) {
                /* Find the next -1 from the left */
                while (left < right && stats_skip[i].ngrams[left] != -1) {
                    left++;
                }

                /* Find the next non -1 from the right */
                while (left < right && stats_skip[i].ngrams[right] == -1) {
                    right--;
                }

                 /* Swap the elements to move -1 to the back and non -1 to the front */
                if (left < right) {
                    int temp = stats_skip[i].ngrams[left];
                    stats_skip[i].ngrams[left] = stats_skip[i].ngrams[right];
                    stats_skip[i].ngrams[right] = temp;
                    left++;
                    right--;
                }
            }
        }
    }
}

/*
 * Checks if all weights of a skip_stat are zero.
 * This is a helper function for clean_skip_stats.
 *
 * Parameters:
 *   stat: Pointer to the skip_stat structure.
 * Returns: 1 if all weights are zero, 0 otherwise.
 */
int all_zeroes(skip_stat *stat)
{
    for (int i = 1; i <= 9; i++)
    {
        if (stat->weight[i] != 0) {return 0;}
    }
    return 1;
}

/*
 * Cleans the skipgram statistics array by removing statistics with zero length
 * or weight. This ensures that only relevant statistics are considered in the
 * analysis.
 */
void clean_skip_stats()
{
    /* Iterate through the array and remove those with zero length or weight */
    for (int i = 0; i < SKIP_LENGTH; i++)
    {
        if (stats_skip[i].length == 0 || all_zeroes(&stats_skip[i])) {stats_skip[i].skip = 1;}
    }
}

/* Frees the memory allocated for the skipgram statistics array. */
void free_skip_stats()
{
    free(stats_skip);
}
