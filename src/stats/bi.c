/*
 * stats/bi.c - Bigram statistic definitions.
 *
 * This file contains the implementation for initializing, cleaning, converting,
 * and freeing bigram statistics used in the GULAG. Bigram statistics track
 * the frequency and positioning of two character sequences.
 *
 * Adding new stats:
 *     1. Incease BI_LENGTH by as many stats as you are adding.
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

#include "bi.h"
#include "util.h"
#include "stats_util.h"
#include "global.h"
#include "structs.h"

/*
 * Initializes the array of bigram statistics. The function allocates memory
 * for the stat array and sets default values, including a negative infinity
 * weight which will be later overwritten.
 */
void initialize_bi_stats()
{
    BI_LENGTH = 22;
    stats_bi = (bi_stat *)malloc(sizeof(bi_stat) * BI_LENGTH);
    int row0, col0, row1, col1;
    int index = 0;

    /* Initialize SFBs */
    strcpy(stats_bi[index].name, "Same Finger Bigram");
    stats_bi[index].weight = -INFINITY;
    stats_bi[index].length = 0;
    stats_bi[index].skip = 0;
    for (int i = 0; i < DIM2; i++)
    {
        unflat_bi(i, &row0, &col0, &row1, &col1);
        if (is_same_finger_bi(row0, col0, row1, col1))
        {
            stats_bi[index].ngrams[i] = i;
            stats_bi[index].length++;
        }
        else
        {
            stats_bi[index].ngrams[i] = -1;
        }
    }
    index++;

    /* initialize per finger bigram stats */
    strcpy(stats_bi[index].name, "Left Pinky Bigram");
    stats_bi[index].weight = -INFINITY;
    stats_bi[index].length = 0;
    stats_bi[index].skip = 0;
    for (int i = 0; i < DIM2; i++)
    {
        unflat_bi(i, &row0, &col0, &row1, &col1);
        if (is_same_finger_bi(row0, col0, row1, col1) && finger(row0, col0) == 0)
        {
            stats_bi[index].ngrams[i] = i;
            stats_bi[index].length++;
        }
        else
        {
            stats_bi[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_bi[index].name, "Left Ring Bigram");
    stats_bi[index].weight = -INFINITY;
    stats_bi[index].length = 0;
    stats_bi[index].skip = 0;
    for (int i = 0; i < DIM2; i++)
    {
        unflat_bi(i, &row0, &col0, &row1, &col1);
        if (is_same_finger_bi(row0, col0, row1, col1) && finger(row0, col0) == 1)
        {
            stats_bi[index].ngrams[i] = i;
            stats_bi[index].length++;
        }
        else
        {
            stats_bi[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_bi[index].name, "Left Middle Bigram");
    stats_bi[index].weight = -INFINITY;
    stats_bi[index].length = 0;
    stats_bi[index].skip = 0;
    for (int i = 0; i < DIM2; i++)
    {
        unflat_bi(i, &row0, &col0, &row1, &col1);
        if (is_same_finger_bi(row0, col0, row1, col1) && finger(row0, col0) == 2)
        {
            stats_bi[index].ngrams[i] = i;
            stats_bi[index].length++;
        }
        else
        {
            stats_bi[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_bi[index].name, "Left Index Bigram");
    stats_bi[index].weight = -INFINITY;
    stats_bi[index].length = 0;
    stats_bi[index].skip = 0;
    for (int i = 0; i < DIM2; i++)
    {
        unflat_bi(i, &row0, &col0, &row1, &col1);
        if (is_same_finger_bi(row0, col0, row1, col1) && finger(row0, col0) == 3)
        {
            stats_bi[index].ngrams[i] = i;
            stats_bi[index].length++;
        }
        else
        {
            stats_bi[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_bi[index].name, "Right Index Bigram");
    stats_bi[index].weight = -INFINITY;
    stats_bi[index].length = 0;
    stats_bi[index].skip = 0;
    for (int i = 0; i < DIM2; i++)
    {
        unflat_bi(i, &row0, &col0, &row1, &col1);
        if (is_same_finger_bi(row0, col0, row1, col1) && finger(row0, col0) == 4)
        {
            stats_bi[index].ngrams[i] = i;
            stats_bi[index].length++;
        }
        else
        {
            stats_bi[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_bi[index].name, "Right Middle Bigram");
    stats_bi[index].weight = -INFINITY;
    stats_bi[index].length = 0;
    stats_bi[index].skip = 0;
    for (int i = 0; i < DIM2; i++)
    {
        unflat_bi(i, &row0, &col0, &row1, &col1);
        if (is_same_finger_bi(row0, col0, row1, col1) && finger(row0, col0) == 5)
        {
            stats_bi[index].ngrams[i] = i;
            stats_bi[index].length++;
        }
        else
        {
            stats_bi[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_bi[index].name, "Right Ring Bigram");
    stats_bi[index].weight = -INFINITY;
    stats_bi[index].length = 0;
    stats_bi[index].skip = 0;
    for (int i = 0; i < DIM2; i++)
    {
        unflat_bi(i, &row0, &col0, &row1, &col1);
        if (is_same_finger_bi(row0, col0, row1, col1) && finger(row0, col0) == 6)
        {
            stats_bi[index].ngrams[i] = i;
            stats_bi[index].length++;
        }
        else
        {
            stats_bi[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_bi[index].name, "Right Pinky Bigram");
    stats_bi[index].weight = -INFINITY;
    stats_bi[index].length = 0;
    stats_bi[index].skip = 0;
    for (int i = 0; i < DIM2; i++)
    {
        unflat_bi(i, &row0, &col0, &row1, &col1);
        if (is_same_finger_bi(row0, col0, row1, col1) && finger(row0, col0) == 7)
        {
            stats_bi[index].ngrams[i] = i;
            stats_bi[index].length++;
        }
        else
        {
            stats_bi[index].ngrams[i] = -1;
        }
    }
    index++;

    /* initialize 2U SFBs */
    strcpy(stats_bi[index].name, "Bad Same Finger Bigram");
    stats_bi[index].weight = -INFINITY;
    stats_bi[index].length = 0;
    stats_bi[index].skip = 0;
    for (int i = 0; i < DIM2; i++)
    {
        unflat_bi(i, &row0, &col0, &row1, &col1);
        if (is_bad_same_finger_bi(row0, col0, row1, col1))
        {
            stats_bi[index].ngrams[i] = i;
            stats_bi[index].length++;
        }
        else
        {
            stats_bi[index].ngrams[i] = -1;
        }
    }
    index++;

    /* initialize per finger 2U bigram stats */
    strcpy(stats_bi[index].name, "Bad Left Pinky Bigram");
    stats_bi[index].weight = -INFINITY;
    stats_bi[index].length = 0;
    stats_bi[index].skip = 0;
    for (int i = 0; i < DIM2; i++)
    {
        unflat_bi(i, &row0, &col0, &row1, &col1);
        if (is_bad_same_finger_bi(row0, col0, row1, col1) && finger(row0, col0) == 0)
        {
            stats_bi[index].ngrams[i] = i;
            stats_bi[index].length++;
        }
        else
        {
            stats_bi[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_bi[index].name, "Bad Left Ring Bigram");
    stats_bi[index].weight = -INFINITY;
    stats_bi[index].length = 0;
    stats_bi[index].skip = 0;
    for (int i = 0; i < DIM2; i++)
    {
        unflat_bi(i, &row0, &col0, &row1, &col1);
        if (is_bad_same_finger_bi(row0, col0, row1, col1) && finger(row0, col0) == 1)
        {
            stats_bi[index].ngrams[i] = i;
            stats_bi[index].length++;
        }
        else
        {
            stats_bi[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_bi[index].name, "Bad Left Middle Bigram");
    stats_bi[index].weight = -INFINITY;
    stats_bi[index].length = 0;
    stats_bi[index].skip = 0;
    for (int i = 0; i < DIM2; i++)
    {
        unflat_bi(i, &row0, &col0, &row1, &col1);
        if (is_bad_same_finger_bi(row0, col0, row1, col1) && finger(row0, col0) == 2)
        {
            stats_bi[index].ngrams[i] = i;
            stats_bi[index].length++;
        }
        else
        {
            stats_bi[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_bi[index].name, "Bad Left Index Bigram");
    stats_bi[index].weight = -INFINITY;
    stats_bi[index].length = 0;
    stats_bi[index].skip = 0;
    for (int i = 0; i < DIM2; i++)
    {
        unflat_bi(i, &row0, &col0, &row1, &col1);
        if (is_bad_same_finger_bi(row0, col0, row1, col1) && finger(row0, col0) == 3)
        {
            stats_bi[index].ngrams[i] = i;
            stats_bi[index].length++;
        }
        else
        {
            stats_bi[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_bi[index].name, "Bad Right Index Bigram");
    stats_bi[index].weight = -INFINITY;
    stats_bi[index].length = 0;
    stats_bi[index].skip = 0;
    for (int i = 0; i < DIM2; i++)
    {
        unflat_bi(i, &row0, &col0, &row1, &col1);
        if (is_bad_same_finger_bi(row0, col0, row1, col1) && finger(row0, col0) == 4)
        {
            stats_bi[index].ngrams[i] = i;
            stats_bi[index].length++;
        }
        else
        {
            stats_bi[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_bi[index].name, "Bad Right Middle Bigram");
    stats_bi[index].weight = -INFINITY;
    stats_bi[index].length = 0;
    stats_bi[index].skip = 0;
    for (int i = 0; i < DIM2; i++)
    {
        unflat_bi(i, &row0, &col0, &row1, &col1);
        if (is_bad_same_finger_bi(row0, col0, row1, col1) && finger(row0, col0) == 5)
        {
            stats_bi[index].ngrams[i] = i;
            stats_bi[index].length++;
        }
        else
        {
            stats_bi[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_bi[index].name, "Bad Right Ring Bigram");
    stats_bi[index].weight = -INFINITY;
    stats_bi[index].length = 0;
    stats_bi[index].skip = 0;
    for (int i = 0; i < DIM2; i++)
    {
        unflat_bi(i, &row0, &col0, &row1, &col1);
        if (is_bad_same_finger_bi(row0, col0, row1, col1) && finger(row0, col0) == 6)
        {
            stats_bi[index].ngrams[i] = i;
            stats_bi[index].length++;
        }
        else
        {
            stats_bi[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_bi[index].name, "Bad Right Pinky Bigram");
    stats_bi[index].weight = -INFINITY;
    stats_bi[index].length = 0;
    stats_bi[index].skip = 0;
    for (int i = 0; i < DIM2; i++)
    {
        unflat_bi(i, &row0, &col0, &row1, &col1);
        if (is_bad_same_finger_bi(row0, col0, row1, col1) && finger(row0, col0) == 7)
        {
            stats_bi[index].ngrams[i] = i;
            stats_bi[index].length++;
        }
        else
        {
            stats_bi[index].ngrams[i] = -1;
        }
    }
    index++;

    /* initialize russor stats */
    strcpy(stats_bi[index].name, "Full Russor Bigram");
    stats_bi[index].weight = -INFINITY;
    stats_bi[index].length = 0;
    stats_bi[index].skip = 0;
    for (int i = 0; i < DIM2; i++)
    {
        unflat_bi(i, &row0, &col0, &row1, &col1);
        if (is_full_russor(row0, col0, row1, col1))
        {
            stats_bi[index].ngrams[i] = i;
            stats_bi[index].length++;
        }
        else
        {
            stats_bi[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_bi[index].name, "Half Russor Bigram");
    stats_bi[index].weight = -INFINITY;
    stats_bi[index].length = 0;
    stats_bi[index].skip = 0;
    for (int i = 0; i < DIM2; i++)
    {
        unflat_bi(i, &row0, &col0, &row1, &col1);
        if (is_half_russor(row0, col0, row1, col1))
        {
            stats_bi[index].ngrams[i] = i;
            stats_bi[index].length++;
        }
        else
        {
            stats_bi[index].ngrams[i] = -1;
        }
    }
    index++;

    /* initialize LSBs */
    strcpy(stats_bi[index].name, "Index Stretch Bigram");
    stats_bi[index].weight = -INFINITY;
    stats_bi[index].length = 0;
    stats_bi[index].skip = 0;
    for (int i = 0; i < DIM2; i++)
    {
        unflat_bi(i, &row0, &col0, &row1, &col1);
        if (is_index_stretch_bi(row0, col0, row1, col1))
        {
            stats_bi[index].ngrams[i] = i;
            stats_bi[index].length++;
        }
        else
        {
            stats_bi[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_bi[index].name, "Pinky Stretch Bigram");
    stats_bi[index].weight = -INFINITY;
    stats_bi[index].length = 0;
    stats_bi[index].skip = 0;
    for (int i = 0; i < DIM2; i++)
    {
        unflat_bi(i, &row0, &col0, &row1, &col1);
        if (is_pinky_stretch_bi(row0, col0, row1, col1))
        {
            stats_bi[index].ngrams[i] = i;
            stats_bi[index].length++;
        }
        else
        {
            stats_bi[index].ngrams[i] = -1;
        }
    }
    index++;
    if (index != BI_LENGTH) {error("BI_LENGTH incorrect for number of bi stats");}
}

/*
 * Trims the ngrams in the array to move unused entries to the end.
 * This process ensures memory efficiency by eliminating gaps in the array.
 */
void trim_bi_stats()
{
    for (int i = 0; i < BI_LENGTH; i++)
    {
        if (stats_bi[i].length != 0)
        {
            int left = 0;
            int right = DIM2 - 1;

            /* Use two pointers to partition the array */
            while (left < right) {
                /* Find the next -1 from the left */
                while (left < right && stats_bi[i].ngrams[left] != -1) {
                    left++;
                }

                /* Find the next non -1 from the right */
                while (left < right && stats_bi[i].ngrams[right] == -1) {
                    right--;
                }

                /* Swap the elements to move -1 to the back and non -1 to the front */
                if (left < right) {
                    int temp = stats_bi[i].ngrams[left];
                    stats_bi[i].ngrams[left] = stats_bi[i].ngrams[right];
                    stats_bi[i].ngrams[right] = temp;
                    left++;
                    right--;
                }
            }
        }
    }
}

/*
 * Cleans the bigram statistics array by removing statistics with zero length or
 * weight. This ensures that only relevant statistics are considered in the
 * analysis.
 */
void clean_bi_stats()
{
    /* Iterate through the array and remove those with zero length or weight */
    for (int i = 0; i < BI_LENGTH; i++)
    {
        if (stats_bi[i].length == 0 || stats_bi[i].weight == 0) {stats_bi[i].skip = 1;}
    }
}

/* Frees the memory allocated for the bigram statistics array. */
void free_bi_stats()
{
    free(stats_bi);
}
