/*
 * stats/tri.c - Trigram statistic definitions.
 *
 * This file contains the implementation for initializing, cleaning, converting,
 * and freeing trigram statistics used in the GULAG. Trigram statistics track
 * the frequency and positioning of three character sequences.
 *
 * Adding new stats:
 *     1. Incease TRI_LENGTH by as many stats as you are adding.
 *     2. Define its name, keep it a reasonable length.
 *     3. Set its weight to -INFINITY, and skip to 0 (to be changed later).
 *     4. Set its length to 0, then loop through the DIM3 (36^3) sequences.
 *       4a. Use unflat_tri() to convert the 1D index to a set of 2D coordinates.
 *       4b. Check if the ngram falls under the stat.
 *       4c. If it does, add it to the ngrams array and increment length.
 *       4d. Otherwise set the ngram array element to -1.
 *     5. Iterate the index.
 *     6. Add the statistic to the weights files in data/weights/.
 */

#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "tri.h"
#include "util.h"
#include "stats_util.h"
#include "global.h"
#include "structs.h"


/*
 * Initializes the array of tripgram statistics. The function allocates memory
 * for the stat array and sets default values, including a negative infinity
 * weight which will be later overwritten.
 */
void initialize_tri_stats()
{
    TRI_LENGTH = 39;
    stats_tri = (tri_stat *)malloc(sizeof(tri_stat) * TRI_LENGTH);
    int row0, col0, row1, col1, row2, col2;
    int index = 0;

    /* Allocate and initialize SFTs. */
    strcpy(stats_tri[index].name, "Same Finger Trigram");
    stats_tri[index].weight = -INFINITY;
    stats_tri[index].length = 0;
    stats_tri[index].skip = 0;
    for (int i = 0; i < DIM3; i++)
    {
        /* convert a 1D index into a 6D matrix coordinate */
        unflat_tri(i, &row0, &col0, &row1, &col1, &row2, &col2); /* util.c */
        if (is_same_finger_tri(row0, col0, row1, col1, row2, col2))
        {
            stats_tri[index].ngrams[i] = i;
            stats_tri[index].length++;
        }
        else
        {
            stats_tri[index].ngrams[i] = -1;
        }
    }
    index++;

    /* standard trigram stats after this */
    strcpy(stats_tri[index].name, "Redirect");
    stats_tri[index].weight = -INFINITY;
    stats_tri[index].length = 0;
    stats_tri[index].skip = 0;
    for (int i = 0; i < DIM3; i++)
    {
        unflat_tri(i, &row0, &col0, &row1, &col1, &row2, &col2);
        if (is_redirect(row0, col0, row1, col1, row2, col2))
        {
            stats_tri[index].ngrams[i] = i;
            stats_tri[index].length++;
        }
        else
        {
            stats_tri[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_tri[index].name, "Bad Redirect");
    stats_tri[index].weight = -INFINITY;
    stats_tri[index].length = 0;
    stats_tri[index].skip = 0;
    for (int i = 0; i < DIM3; i++)
    {
        unflat_tri(i, &row0, &col0, &row1, &col1, &row2, &col2);
        if (is_bad_redirect(row0, col0, row1, col1, row2, col2))
        {
            stats_tri[index].ngrams[i] = i;
            stats_tri[index].length++;
        }
        else
        {
            stats_tri[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_tri[index].name, "Alternation");
    stats_tri[index].weight = -INFINITY;
    stats_tri[index].length = 0;
    stats_tri[index].skip = 0;
    for (int i = 0; i < DIM3; i++)
    {
        unflat_tri(i, &row0, &col0, &row1, &col1, &row2, &col2);
        if (is_alt(row0, col0, row1, col1, row2, col2))
        {
            stats_tri[index].ngrams[i] = i;
            stats_tri[index].length++;
        }
        else
        {
            stats_tri[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_tri[index].name, "Alternation In");
    stats_tri[index].weight = -INFINITY;
    stats_tri[index].length = 0;
    stats_tri[index].skip = 0;
    for (int i = 0; i < DIM3; i++)
    {
        unflat_tri(i, &row0, &col0, &row1, &col1, &row2, &col2);
        if (is_alt_in(row0, col0, row1, col1, row2, col2))
        {
            stats_tri[index].ngrams[i] = i;
            stats_tri[index].length++;
        }
        else
        {
            stats_tri[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_tri[index].name, "Alternation Out");
    stats_tri[index].weight = -INFINITY;
    stats_tri[index].length = 0;
    stats_tri[index].skip = 0;
    for (int i = 0; i < DIM3; i++)
    {
        unflat_tri(i, &row0, &col0, &row1, &col1, &row2, &col2);
        if (is_alt_out(row0, col0, row1, col1, row2, col2))
        {
            stats_tri[index].ngrams[i] = i;
            stats_tri[index].length++;
        }
        else
        {
            stats_tri[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_tri[index].name, "Same Row Alternation");
    stats_tri[index].weight = -INFINITY;
    stats_tri[index].length = 0;
    stats_tri[index].skip = 0;
    for (int i = 0; i < DIM3; i++)
    {
        unflat_tri(i, &row0, &col0, &row1, &col1, &row2, &col2);
        if (is_same_row_alt(row0, col0, row1, col1, row2, col2))
        {
            stats_tri[index].ngrams[i] = i;
            stats_tri[index].length++;
        }
        else
        {
            stats_tri[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_tri[index].name, "Same Row Alternation In");
    stats_tri[index].weight = -INFINITY;
    stats_tri[index].length = 0;
    stats_tri[index].skip = 0;
    for (int i = 0; i < DIM3; i++)
    {
        unflat_tri(i, &row0, &col0, &row1, &col1, &row2, &col2);
        if (is_same_row_alt_in(row0, col0, row1, col1, row2, col2))
        {
            stats_tri[index].ngrams[i] = i;
            stats_tri[index].length++;
        }
        else
        {
            stats_tri[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_tri[index].name, "Same Row Alternation Out");
    stats_tri[index].weight = -INFINITY;
    stats_tri[index].length = 0;
    stats_tri[index].skip = 0;
    for (int i = 0; i < DIM3; i++)
    {
        unflat_tri(i, &row0, &col0, &row1, &col1, &row2, &col2);
        if (is_same_row_alt_out(row0, col0, row1, col1, row2, col2))
        {
            stats_tri[index].ngrams[i] = i;
            stats_tri[index].length++;
        }
        else
        {
            stats_tri[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_tri[index].name, "Adjacent Finger Alternation");
    stats_tri[index].weight = -INFINITY;
    stats_tri[index].length = 0;
    stats_tri[index].skip = 0;
    for (int i = 0; i < DIM3; i++)
    {
        unflat_tri(i, &row0, &col0, &row1, &col1, &row2, &col2);
        if (is_adjacent_finger_alt(row0, col0, row1, col1, row2, col2))
        {
            stats_tri[index].ngrams[i] = i;
            stats_tri[index].length++;
        }
        else
        {
            stats_tri[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_tri[index].name, "Adjacent Finger Alternation In");
    stats_tri[index].weight = -INFINITY;
    stats_tri[index].length = 0;
    stats_tri[index].skip = 0;
    for (int i = 0; i < DIM3; i++)
    {
        unflat_tri(i, &row0, &col0, &row1, &col1, &row2, &col2);
        if (is_adjacent_finger_alt_in(row0, col0, row1, col1, row2, col2))
        {
            stats_tri[index].ngrams[i] = i;
            stats_tri[index].length++;
        }
        else
        {
            stats_tri[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_tri[index].name, "Adjacent Finger Alternation Out");
    stats_tri[index].weight = -INFINITY;
    stats_tri[index].length = 0;
    stats_tri[index].skip = 0;
    for (int i = 0; i < DIM3; i++)
    {
        unflat_tri(i, &row0, &col0, &row1, &col1, &row2, &col2);
        if (is_adjacent_finger_alt_out(row0, col0, row1, col1, row2, col2))
        {
            stats_tri[index].ngrams[i] = i;
            stats_tri[index].length++;
        }
        else
        {
            stats_tri[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_tri[index].name, "Same Row Adjacent Finger Alternation");
    stats_tri[index].weight = -INFINITY;
    stats_tri[index].length = 0;
    stats_tri[index].skip = 0;
    for (int i = 0; i < DIM3; i++)
    {
        unflat_tri(i, &row0, &col0, &row1, &col1, &row2, &col2);
        if (is_same_row_adjacent_finger_alt(row0, col0, row1, col1, row2, col2))
        {
            stats_tri[index].ngrams[i] = i;
            stats_tri[index].length++;
        }
        else
        {
            stats_tri[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_tri[index].name, "Same Row Adjacent Finger Alternation In");
    stats_tri[index].weight = -INFINITY;
    stats_tri[index].length = 0;
    stats_tri[index].skip = 0;
    for (int i = 0; i < DIM3; i++)
    {
        unflat_tri(i, &row0, &col0, &row1, &col1, &row2, &col2);
        if (is_same_row_adjacent_finger_alt_in(row0, col0, row1, col1, row2, col2))
        {
            stats_tri[index].ngrams[i] = i;
            stats_tri[index].length++;
        }
        else
        {
            stats_tri[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_tri[index].name, "Same Row Adjacent Finger Alternation Out");
    stats_tri[index].weight = -INFINITY;
    stats_tri[index].length = 0;
    stats_tri[index].skip = 0;
    for (int i = 0; i < DIM3; i++)
    {
        unflat_tri(i, &row0, &col0, &row1, &col1, &row2, &col2);
        if (is_same_row_adjacent_finger_alt_out(row0, col0, row1, col1, row2, col2))
        {
            stats_tri[index].ngrams[i] = i;
            stats_tri[index].length++;
        }
        else
        {
            stats_tri[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_tri[index].name, "One Hand");
    stats_tri[index].weight = -INFINITY;
    stats_tri[index].length = 0;
    stats_tri[index].skip = 0;
    for (int i = 0; i < DIM3; i++)
    {
        unflat_tri(i, &row0, &col0, &row1, &col1, &row2, &col2);
        if (is_onehand(row0, col0, row1, col1, row2, col2))
        {
            stats_tri[index].ngrams[i] = i;
            stats_tri[index].length++;
        }
        else
        {
            stats_tri[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_tri[index].name, "One Hand In");
    stats_tri[index].weight = -INFINITY;
    stats_tri[index].length = 0;
    stats_tri[index].skip = 0;
    for (int i = 0; i < DIM3; i++)
    {
        unflat_tri(i, &row0, &col0, &row1, &col1, &row2, &col2);
        if (is_onehand_in(row0, col0, row1, col1, row2, col2))
        {
            stats_tri[index].ngrams[i] = i;
            stats_tri[index].length++;
        }
        else
        {
            stats_tri[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_tri[index].name, "One Hand Out");
    stats_tri[index].weight = -INFINITY;
    stats_tri[index].length = 0;
    stats_tri[index].skip = 0;
    for (int i = 0; i < DIM3; i++)
    {
        unflat_tri(i, &row0, &col0, &row1, &col1, &row2, &col2);
        if (is_onehand_out(row0, col0, row1, col1, row2, col2))
        {
            stats_tri[index].ngrams[i] = i;
            stats_tri[index].length++;
        }
        else
        {
            stats_tri[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_tri[index].name, "Same Row One Hand");
    stats_tri[index].weight = -INFINITY;
    stats_tri[index].length = 0;
    stats_tri[index].skip = 0;
    for (int i = 0; i < DIM3; i++)
    {
        unflat_tri(i, &row0, &col0, &row1, &col1, &row2, &col2);
        if (is_same_row_onehand(row0, col0, row1, col1, row2, col2))
        {
            stats_tri[index].ngrams[i] = i;
            stats_tri[index].length++;
        }
        else
        {
            stats_tri[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_tri[index].name, "Same Row One Hand In");
    stats_tri[index].weight = -INFINITY;
    stats_tri[index].length = 0;
    stats_tri[index].skip = 0;
    for (int i = 0; i < DIM3; i++)
    {
        unflat_tri(i, &row0, &col0, &row1, &col1, &row2, &col2);
        if (is_same_row_onehand_in(row0, col0, row1, col1, row2, col2))
        {
            stats_tri[index].ngrams[i] = i;
            stats_tri[index].length++;
        }
        else
        {
            stats_tri[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_tri[index].name, "Same Row One Hand Out");
    stats_tri[index].weight = -INFINITY;
    stats_tri[index].length = 0;
    stats_tri[index].skip = 0;
    for (int i = 0; i < DIM3; i++)
    {
        unflat_tri(i, &row0, &col0, &row1, &col1, &row2, &col2);
        if (is_same_row_onehand_out(row0, col0, row1, col1, row2, col2))
        {
            stats_tri[index].ngrams[i] = i;
            stats_tri[index].length++;
        }
        else
        {
            stats_tri[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_tri[index].name, "Adjacent Finger One Hand");
    stats_tri[index].weight = -INFINITY;
    stats_tri[index].length = 0;
    stats_tri[index].skip = 0;
    for (int i = 0; i < DIM3; i++)
    {
        unflat_tri(i, &row0, &col0, &row1, &col1, &row2, &col2);
        if (is_adjacent_finger_onehand(row0, col0, row1, col1, row2, col2))
        {
            stats_tri[index].ngrams[i] = i;
            stats_tri[index].length++;
        }
        else
        {
            stats_tri[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_tri[index].name, "Adjacent Finger One Hand In");
    stats_tri[index].weight = -INFINITY;
    stats_tri[index].length = 0;
    stats_tri[index].skip = 0;
    for (int i = 0; i < DIM3; i++)
    {
        unflat_tri(i, &row0, &col0, &row1, &col1, &row2, &col2);
        if (is_adjacent_finger_onehand_in(row0, col0, row1, col1, row2, col2))
        {
            stats_tri[index].ngrams[i] = i;
            stats_tri[index].length++;
        }
        else
        {
            stats_tri[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_tri[index].name, "Adjacent Finger One Hand Out");
    stats_tri[index].weight = -INFINITY;
    stats_tri[index].length = 0;
    stats_tri[index].skip = 0;
    for (int i = 0; i < DIM3; i++)
    {
        unflat_tri(i, &row0, &col0, &row1, &col1, &row2, &col2);
        if (is_adjacent_finger_onehand_out(row0, col0, row1, col1, row2, col2))
        {
            stats_tri[index].ngrams[i] = i;
            stats_tri[index].length++;
        }
        else
        {
            stats_tri[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_tri[index].name, "Same Row Adjacent Finger One Hand");
    stats_tri[index].weight = -INFINITY;
    stats_tri[index].length = 0;
    stats_tri[index].skip = 0;
    for (int i = 0; i < DIM3; i++)
    {
        unflat_tri(i, &row0, &col0, &row1, &col1, &row2, &col2);
        if (is_same_row_adjacent_finger_onehand(row0, col0, row1, col1, row2, col2))
        {
            stats_tri[index].ngrams[i] = i;
            stats_tri[index].length++;
        }
        else
        {
            stats_tri[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_tri[index].name, "Same Row Adjacent Finger One Hand In");
    stats_tri[index].weight = -INFINITY;
    stats_tri[index].length = 0;
    stats_tri[index].skip = 0;
    for (int i = 0; i < DIM3; i++)
    {
        unflat_tri(i, &row0, &col0, &row1, &col1, &row2, &col2);
        if (is_same_row_adjacent_finger_onehand_in(row0, col0, row1, col1, row2, col2))
        {
            stats_tri[index].ngrams[i] = i;
            stats_tri[index].length++;
        }
        else
        {
            stats_tri[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_tri[index].name, "Same Row Adjacent Finger One Hand Out");
    stats_tri[index].weight = -INFINITY;
    stats_tri[index].length = 0;
    stats_tri[index].skip = 0;
    for (int i = 0; i < DIM3; i++)
    {
        unflat_tri(i, &row0, &col0, &row1, &col1, &row2, &col2);
        if (is_same_row_adjacent_finger_onehand_out(row0, col0, row1, col1, row2, col2))
        {
            stats_tri[index].ngrams[i] = i;
            stats_tri[index].length++;
        }
        else
        {
            stats_tri[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_tri[index].name, "Roll");
    stats_tri[index].weight = -INFINITY;
    stats_tri[index].length = 0;
    stats_tri[index].skip = 0;
    for (int i = 0; i < DIM3; i++)
    {
        unflat_tri(i, &row0, &col0, &row1, &col1, &row2, &col2);
        if (is_roll(row0, col0, row1, col1, row2, col2))
        {
            stats_tri[index].ngrams[i] = i;
            stats_tri[index].length++;
        }
        else
        {
            stats_tri[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_tri[index].name, "Roll In");
    stats_tri[index].weight = -INFINITY;
    stats_tri[index].length = 0;
    stats_tri[index].skip = 0;
    for (int i = 0; i < DIM3; i++)
    {
        unflat_tri(i, &row0, &col0, &row1, &col1, &row2, &col2);
        if (is_roll_in(row0, col0, row1, col1, row2, col2))
        {
            stats_tri[index].ngrams[i] = i;
            stats_tri[index].length++;
        }
        else
        {
            stats_tri[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_tri[index].name, "Roll Out");
    stats_tri[index].weight = -INFINITY;
    stats_tri[index].length = 0;
    stats_tri[index].skip = 0;
    for (int i = 0; i < DIM3; i++)
    {
        unflat_tri(i, &row0, &col0, &row1, &col1, &row2, &col2);
        if (is_roll_out(row0, col0, row1, col1, row2, col2))
        {
            stats_tri[index].ngrams[i] = i;
            stats_tri[index].length++;
        }
        else
        {
            stats_tri[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_tri[index].name, "Same Row Roll");
    stats_tri[index].weight = -INFINITY;
    stats_tri[index].length = 0;
    stats_tri[index].skip = 0;
    for (int i = 0; i < DIM3; i++)
    {
        unflat_tri(i, &row0, &col0, &row1, &col1, &row2, &col2);
        if (is_same_row_roll(row0, col0, row1, col1, row2, col2))
        {
            stats_tri[index].ngrams[i] = i;
            stats_tri[index].length++;
        }
        else
        {
            stats_tri[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_tri[index].name, "Same Row Roll In");
    stats_tri[index].weight = -INFINITY;
    stats_tri[index].length = 0;
    stats_tri[index].skip = 0;
    for (int i = 0; i < DIM3; i++)
    {
        unflat_tri(i, &row0, &col0, &row1, &col1, &row2, &col2);
        if (is_same_row_roll_in(row0, col0, row1, col1, row2, col2))
        {
            stats_tri[index].ngrams[i] = i;
            stats_tri[index].length++;
        }
        else
        {
            stats_tri[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_tri[index].name, "Same Row Roll Out");
    stats_tri[index].weight = -INFINITY;
    stats_tri[index].length = 0;
    stats_tri[index].skip = 0;
    for (int i = 0; i < DIM3; i++)
    {
        unflat_tri(i, &row0, &col0, &row1, &col1, &row2, &col2);
        if (is_same_row_roll_out(row0, col0, row1, col1, row2, col2))
        {
            stats_tri[index].ngrams[i] = i;
            stats_tri[index].length++;
        }
        else
        {
            stats_tri[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_tri[index].name, "Adjacent Finger Roll");
    stats_tri[index].weight = -INFINITY;
    stats_tri[index].length = 0;
    stats_tri[index].skip = 0;
    for (int i = 0; i < DIM3; i++)
    {
        unflat_tri(i, &row0, &col0, &row1, &col1, &row2, &col2);
        if (is_adjacent_finger_roll(row0, col0, row1, col1, row2, col2))
        {
            stats_tri[index].ngrams[i] = i;
            stats_tri[index].length++;
        }
        else
        {
            stats_tri[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_tri[index].name, "Adjacent Finger Roll In");
    stats_tri[index].weight = -INFINITY;
    stats_tri[index].length = 0;
    stats_tri[index].skip = 0;
    for (int i = 0; i < DIM3; i++)
    {
        unflat_tri(i, &row0, &col0, &row1, &col1, &row2, &col2);
        if (is_adjacent_finger_roll_in(row0, col0, row1, col1, row2, col2))
        {
            stats_tri[index].ngrams[i] = i;
            stats_tri[index].length++;
        }
        else
        {
            stats_tri[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_tri[index].name, "Adjacent Finger Roll Out");
    stats_tri[index].weight = -INFINITY;
    stats_tri[index].length = 0;
    stats_tri[index].skip = 0;
    for (int i = 0; i < DIM3; i++)
    {
        unflat_tri(i, &row0, &col0, &row1, &col1, &row2, &col2);
        if (is_adjacent_finger_roll_out(row0, col0, row1, col1, row2, col2))
        {
            stats_tri[index].ngrams[i] = i;
            stats_tri[index].length++;
        }
        else
        {
            stats_tri[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_tri[index].name, "Same Row Adjacent Finger Roll");
    stats_tri[index].weight = -INFINITY;
    stats_tri[index].length = 0;
    stats_tri[index].skip = 0;
    for (int i = 0; i < DIM3; i++)
    {
        unflat_tri(i, &row0, &col0, &row1, &col1, &row2, &col2);
        if (is_same_row_adjacent_finger_roll(row0, col0, row1, col1, row2, col2))
        {
            stats_tri[index].ngrams[i] = i;
            stats_tri[index].length++;
        }
        else
        {
            stats_tri[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_tri[index].name, "Same Row Adjacent Finger Roll In");
    stats_tri[index].weight = -INFINITY;
    stats_tri[index].length = 0;
    stats_tri[index].skip = 0;
    for (int i = 0; i < DIM3; i++)
    {
        unflat_tri(i, &row0, &col0, &row1, &col1, &row2, &col2);
        if (is_same_row_adjacent_finger_roll_in(row0, col0, row1, col1, row2, col2))
        {
            stats_tri[index].ngrams[i] = i;
            stats_tri[index].length++;
        }
        else
        {
            stats_tri[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_tri[index].name, "Same Row Adjacent Finger Roll Out");
    stats_tri[index].weight = -INFINITY;
    stats_tri[index].length = 0;
    stats_tri[index].skip = 0;
    for (int i = 0; i < DIM3; i++)
    {
        unflat_tri(i, &row0, &col0, &row1, &col1, &row2, &col2);
        if (is_same_row_adjacent_finger_roll_out(row0, col0, row1, col1, row2, col2))
        {
            stats_tri[index].ngrams[i] = i;
            stats_tri[index].length++;
        }
        else
        {
            stats_tri[index].ngrams[i] = -1;
        }
    }
    index++;

    if (index != TRI_LENGTH) {error("TRI_LENGTH incorrect for number of tri stats");}
}

/*
 * Trims the ngrams in the array to move unused entries to the end.
 * This process ensures memory efficiency by eliminating gaps in the array.
 */
void trim_tri_stats()
{
    for (int i = 0; i < TRI_LENGTH; i++)
    {
        if (stats_tri[i].length != 0)
        {
            int left = 0;
            int right = DIM3 - 1;

            /* Use two pointers to partition the array */
            while (left < right) {
                /* Find the next -1 from the left */
                while (left < right && stats_tri[i].ngrams[left] != -1) {
                    left++;
                }

                /* Find the next non -1 from the right */
                while (left < right && stats_tri[i].ngrams[right] == -1) {
                    right--;
                }

                 /* Swap the elements to move -1 to the back and non -1 to the front */
                if (left < right) {
                    int temp = stats_tri[i].ngrams[left];
                    stats_tri[i].ngrams[left] = stats_tri[i].ngrams[right];
                    stats_tri[i].ngrams[right] = temp;
                    left++;
                    right--;
                }
            }
        }
    }
}

/*
 * Cleans the trigram statistics array by removing statistics with zero length
 * or weight. This ensures that only relevant statistics are considered in the
 * analysis.
 */
void clean_tri_stats()
{
    /* Iterate through the array and remove those with zero length or weight */
    for (int i = 0; i < TRI_LENGTH; i++)
    {
        if (stats_tri[i].length == 0 || stats_tri[i].weight == 0) {stats_tri[i].skip = 1;}
    }
}

/* Frees the memory allocated for the trigram statistics array. */
void free_tri_stats()
{
    free(stats_tri);
}
