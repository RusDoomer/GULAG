/*
 * stats/quad.c - Quadgram statistic definitions.
 *
 * This file contains the implementation for initializing, cleaning, converting,
 * and freeing quadgram statistics used in the GULAG. Quadgram statistics track
 * the frequency and positioning of four character sequences.
 *
 * Adding new stats:
 *     1. Incease QUAD_LENGTH by as many stats as you are adding.
 *     2. Define its name, keep it a reasonable length.
 *     3. Set its weight to -INFINITY, and skip to 0 (to be changed later).
 *     4. Set its length to 0, then loop through the DIM4 (36^4) sequences.
 *       4a. Use unflat_quad() to convert the 1D index to a set of 2D coordinates.
 *       4b. Check if the ngram falls under the stat.
 *       4c. If it does, add it to the ngrams array and increment length.
 *       4d. Otherwise set the ngram array element to -1.
 *     5. Iterate the index.
 *     6. Add the statistic to the weights files in data/weights/.
 */

#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "quad.h"
#include "util.h"
#include "stats_util.h"
#include "global.h"
#include "structs.h"

/*
 * Initializes the array of quadgram statistics. The function allocates memory
 * for the stat array and sets default values, including a negative infinity
 * weight which will be later overwritten.
 */
void initialize_quad_stats()
{
    QUAD_LENGTH = 71;
    stats_quad = (quad_stat *)malloc(sizeof(quad_stat) * QUAD_LENGTH);
    int row0, col0, row1, col1, row2, col2, row3, col3;
    int index = 0;

    /* Allocate and initialize SFQs. */
    strcpy(stats_quad[index].name, "Same Finger Quadgram");
    stats_quad[index].weight = -INFINITY;
    stats_quad[index].length = 0;
    stats_quad[index].skip = 0;
    for (int i = 0; i < DIM4; i++)
    {
        /* convert a 1D index into a 8D matrix coordinate */
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3); /* util.c */
        if (is_same_finger_quad(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            stats_quad[index].ngrams[i] = i;
            stats_quad[index].length++;
        }
        else
        {
            stats_quad[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_quad[index].name, "Chained Redirect");
    stats_quad[index].weight = -INFINITY;
    stats_quad[index].length = 0;
    stats_quad[index].skip = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_chained_redirect(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            stats_quad[index].ngrams[i] = i;
            stats_quad[index].length++;
        }
        else
        {
            stats_quad[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_quad[index].name, "Bad Chained Redirect");
    stats_quad[index].weight = -INFINITY;
    stats_quad[index].length = 0;
    stats_quad[index].skip = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_bad_chained_redirect(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            stats_quad[index].ngrams[i] = i;
            stats_quad[index].length++;
        }
        else
        {
            stats_quad[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_quad[index].name, "Chained Alternation");
    stats_quad[index].weight = -INFINITY;
    stats_quad[index].length = 0;
    stats_quad[index].skip = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_chained_alt(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            stats_quad[index].ngrams[i] = i;
            stats_quad[index].length++;
        }
        else
        {
            stats_quad[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_quad[index].name, "Chained Alternation In");
    stats_quad[index].weight = -INFINITY;
    stats_quad[index].length = 0;
    stats_quad[index].skip = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_chained_alt_in(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            stats_quad[index].ngrams[i] = i;
            stats_quad[index].length++;
        }
        else
        {
            stats_quad[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_quad[index].name, "Chained Alternation Out");
    stats_quad[index].weight = -INFINITY;
    stats_quad[index].length = 0;
    stats_quad[index].skip = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_chained_alt_out(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            stats_quad[index].ngrams[i] = i;
            stats_quad[index].length++;
        }
        else
        {
            stats_quad[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_quad[index].name, "Chained Alternation Mix");
    stats_quad[index].weight = -INFINITY;
    stats_quad[index].length = 0;
    stats_quad[index].skip = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_chained_alt_mix(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            stats_quad[index].ngrams[i] = i;
            stats_quad[index].length++;
        }
        else
        {
            stats_quad[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_quad[index].name, "Same Row Chained Alternation");
    stats_quad[index].weight = -INFINITY;
    stats_quad[index].length = 0;
    stats_quad[index].skip = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_chained_same_row_alt(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            stats_quad[index].ngrams[i] = i;
            stats_quad[index].length++;
        }
        else
        {
            stats_quad[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_quad[index].name, "Same Row Chained Alternation In");
    stats_quad[index].weight = -INFINITY;
    stats_quad[index].length = 0;
    stats_quad[index].skip = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_chained_same_row_alt_in(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            stats_quad[index].ngrams[i] = i;
            stats_quad[index].length++;
        }
        else
        {
            stats_quad[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_quad[index].name, "Same Row Chained Alternation Out");
    stats_quad[index].weight = -INFINITY;
    stats_quad[index].length = 0;
    stats_quad[index].skip = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_chained_same_row_alt_out(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            stats_quad[index].ngrams[i] = i;
            stats_quad[index].length++;
        }
        else
        {
            stats_quad[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_quad[index].name, "Same Row Chained Alternation Mix");
    stats_quad[index].weight = -INFINITY;
    stats_quad[index].length = 0;
    stats_quad[index].skip = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_chained_same_row_alt_mix(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            stats_quad[index].ngrams[i] = i;
            stats_quad[index].length++;
        }
        else
        {
            stats_quad[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_quad[index].name, "Adjacent Finger Chained Alternation");
    stats_quad[index].weight = -INFINITY;
    stats_quad[index].length = 0;
    stats_quad[index].skip = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_chained_adjacent_finger_alt(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            stats_quad[index].ngrams[i] = i;
            stats_quad[index].length++;
        }
        else
        {
            stats_quad[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_quad[index].name, "Adjacent Finger Chained Alternation In");
    stats_quad[index].weight = -INFINITY;
    stats_quad[index].length = 0;
    stats_quad[index].skip = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_chained_adjacent_finger_alt_in(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            stats_quad[index].ngrams[i] = i;
            stats_quad[index].length++;
        }
        else
        {
            stats_quad[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_quad[index].name, "Adjacent Finger Chained Alternation Out");
    stats_quad[index].weight = -INFINITY;
    stats_quad[index].length = 0;
    stats_quad[index].skip = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_chained_adjacent_finger_alt_out(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            stats_quad[index].ngrams[i] = i;
            stats_quad[index].length++;
        }
        else
        {
            stats_quad[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_quad[index].name, "Adjacent Finger Chained Alternation Mix");
    stats_quad[index].weight = -INFINITY;
    stats_quad[index].length = 0;
    stats_quad[index].skip = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_chained_adjacent_finger_alt_mix(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            stats_quad[index].ngrams[i] = i;
            stats_quad[index].length++;
        }
        else
        {
            stats_quad[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_quad[index].name, "Same Row Adjacent Finger Chained Alternation");
    stats_quad[index].weight = -INFINITY;
    stats_quad[index].length = 0;
    stats_quad[index].skip = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_chained_same_row_adjacent_finger_alt(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            stats_quad[index].ngrams[i] = i;
            stats_quad[index].length++;
        }
        else
        {
            stats_quad[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_quad[index].name, "Same Row Adjacent Finger Chained Alternation In");
    stats_quad[index].weight = -INFINITY;
    stats_quad[index].length = 0;
    stats_quad[index].skip = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_chained_same_row_adjacent_finger_alt_in(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            stats_quad[index].ngrams[i] = i;
            stats_quad[index].length++;
        }
        else
        {
            stats_quad[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_quad[index].name, "Same Row Adjacent Finger Chained Alternation Out");
    stats_quad[index].weight = -INFINITY;
    stats_quad[index].length = 0;
    stats_quad[index].skip = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_chained_same_row_adjacent_finger_alt_out(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            stats_quad[index].ngrams[i] = i;
            stats_quad[index].length++;
        }
        else
        {
            stats_quad[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_quad[index].name, "Same Row Adjacent Finger Chained Alternation Mix");
    stats_quad[index].weight = -INFINITY;
    stats_quad[index].length = 0;
    stats_quad[index].skip = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_chained_same_row_adjacent_finger_alt_mix(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            stats_quad[index].ngrams[i] = i;
            stats_quad[index].length++;
        }
        else
        {
            stats_quad[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_quad[index].name, "Quad One Hand");
    stats_quad[index].weight = -INFINITY;
    stats_quad[index].length = 0;
    stats_quad[index].skip = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_onehand_quad(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            stats_quad[index].ngrams[i] = i;
            stats_quad[index].length++;
        }
        else
        {
            stats_quad[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_quad[index].name, "Quad One Hand In");
    stats_quad[index].weight = -INFINITY;
    stats_quad[index].length = 0;
    stats_quad[index].skip = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_onehand_quad_in(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            stats_quad[index].ngrams[i] = i;
            stats_quad[index].length++;
        }
        else
        {
            stats_quad[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_quad[index].name, "Quad One Hand Out");
    stats_quad[index].weight = -INFINITY;
    stats_quad[index].length = 0;
    stats_quad[index].skip = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_onehand_quad_out(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            stats_quad[index].ngrams[i] = i;
            stats_quad[index].length++;
        }
        else
        {
            stats_quad[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_quad[index].name, "Quad Same Row One Hand");
    stats_quad[index].weight = -INFINITY;
    stats_quad[index].length = 0;
    stats_quad[index].skip = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_same_row_onehand_quad(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            stats_quad[index].ngrams[i] = i;
            stats_quad[index].length++;
        }
        else
        {
            stats_quad[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_quad[index].name, "Quad Same Row One Hand In");
    stats_quad[index].weight = -INFINITY;
    stats_quad[index].length = 0;
    stats_quad[index].skip = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_same_row_onehand_quad_in(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            stats_quad[index].ngrams[i] = i;
            stats_quad[index].length++;
        }
        else
        {
            stats_quad[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_quad[index].name, "Quad Same Row One Hand Out");
    stats_quad[index].weight = -INFINITY;
    stats_quad[index].length = 0;
    stats_quad[index].skip = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_same_row_onehand_quad_out(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            stats_quad[index].ngrams[i] = i;
            stats_quad[index].length++;
        }
        else
        {
            stats_quad[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_quad[index].name, "Quad Adjacent Finger One Hand");
    stats_quad[index].weight = -INFINITY;
    stats_quad[index].length = 0;
    stats_quad[index].skip = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_adjacent_finger_onehand_quad(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            stats_quad[index].ngrams[i] = i;
            stats_quad[index].length++;
        }
        else
        {
            stats_quad[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_quad[index].name, "Quad Adjacent Finger One Hand In");
    stats_quad[index].weight = -INFINITY;
    stats_quad[index].length = 0;
    stats_quad[index].skip = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_adjacent_finger_onehand_quad_in(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            stats_quad[index].ngrams[i] = i;
            stats_quad[index].length++;
        }
        else
        {
            stats_quad[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_quad[index].name, "Quad Adjacent Finger One Hand Out");
    stats_quad[index].weight = -INFINITY;
    stats_quad[index].length = 0;
    stats_quad[index].skip = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_adjacent_finger_onehand_quad_out(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            stats_quad[index].ngrams[i] = i;
            stats_quad[index].length++;
        }
        else
        {
            stats_quad[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_quad[index].name, "Quad Same Row Adjacent Finger One Hand");
    stats_quad[index].weight = -INFINITY;
    stats_quad[index].length = 0;
    stats_quad[index].skip = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_same_row_adjacent_finger_onehand_quad(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            stats_quad[index].ngrams[i] = i;
            stats_quad[index].length++;
        }
        else
        {
            stats_quad[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_quad[index].name, "Quad Same Row Adjacent Finger One Hand In");
    stats_quad[index].weight = -INFINITY;
    stats_quad[index].length = 0;
    stats_quad[index].skip = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_same_row_adjacent_finger_onehand_quad_in(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            stats_quad[index].ngrams[i] = i;
            stats_quad[index].length++;
        }
        else
        {
            stats_quad[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_quad[index].name, "Quad Same Row Adjacent Finger One Hand Out");
    stats_quad[index].weight = -INFINITY;
    stats_quad[index].length = 0;
    stats_quad[index].skip = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_same_row_adjacent_finger_onehand_quad_out(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            stats_quad[index].ngrams[i] = i;
            stats_quad[index].length++;
        }
        else
        {
            stats_quad[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_quad[index].name, "Quad Roll");
    stats_quad[index].weight = -INFINITY;
    stats_quad[index].length = 0;
    stats_quad[index].skip = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_roll_quad(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            stats_quad[index].ngrams[i] = i;
            stats_quad[index].length++;
        }
        else
        {
            stats_quad[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_quad[index].name, "Quad Roll In");
    stats_quad[index].weight = -INFINITY;
    stats_quad[index].length = 0;
    stats_quad[index].skip = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_roll_quad_in(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            stats_quad[index].ngrams[i] = i;
            stats_quad[index].length++;
        }
        else
        {
            stats_quad[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_quad[index].name, "Quad Roll Out");
    stats_quad[index].weight = -INFINITY;
    stats_quad[index].length = 0;
    stats_quad[index].skip = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_roll_quad_out(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            stats_quad[index].ngrams[i] = i;
            stats_quad[index].length++;
        }
        else
        {
            stats_quad[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_quad[index].name, "Quad Same Row Roll");
    stats_quad[index].weight = -INFINITY;
    stats_quad[index].length = 0;
    stats_quad[index].skip = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_same_row_roll_quad(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            stats_quad[index].ngrams[i] = i;
            stats_quad[index].length++;
        }
        else
        {
            stats_quad[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_quad[index].name, "Quad Same Row Roll In");
    stats_quad[index].weight = -INFINITY;
    stats_quad[index].length = 0;
    stats_quad[index].skip = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_same_row_roll_quad_in(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            stats_quad[index].ngrams[i] = i;
            stats_quad[index].length++;
        }
        else
        {
            stats_quad[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_quad[index].name, "Quad Same Row Roll Out");
    stats_quad[index].weight = -INFINITY;
    stats_quad[index].length = 0;
    stats_quad[index].skip = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_same_row_roll_quad_out(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            stats_quad[index].ngrams[i] = i;
            stats_quad[index].length++;
        }
        else
        {
            stats_quad[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_quad[index].name, "Quad Adjacent Finger Roll");
    stats_quad[index].weight = -INFINITY;
    stats_quad[index].length = 0;
    stats_quad[index].skip = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_adjacent_finger_roll_quad(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            stats_quad[index].ngrams[i] = i;
            stats_quad[index].length++;
        }
        else
        {
            stats_quad[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_quad[index].name, "Quad Adjacent Finger Roll In");
    stats_quad[index].weight = -INFINITY;
    stats_quad[index].length = 0;
    stats_quad[index].skip = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_adjacent_finger_roll_quad_in(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            stats_quad[index].ngrams[i] = i;
            stats_quad[index].length++;
        }
        else
        {
            stats_quad[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_quad[index].name, "Quad Adjacent Finger Roll Out");
    stats_quad[index].weight = -INFINITY;
    stats_quad[index].length = 0;
    stats_quad[index].skip = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_adjacent_finger_roll_quad_out(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            stats_quad[index].ngrams[i] = i;
            stats_quad[index].length++;
        }
        else
        {
            stats_quad[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_quad[index].name, "Quad Same Row Adjacent Finger Roll");
    stats_quad[index].weight = -INFINITY;
    stats_quad[index].length = 0;
    stats_quad[index].skip = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_same_row_adjacent_finger_roll_quad(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            stats_quad[index].ngrams[i] = i;
            stats_quad[index].length++;
        }
        else
        {
            stats_quad[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_quad[index].name, "Quad Same Row Adjacent Finger Roll In");
    stats_quad[index].weight = -INFINITY;
    stats_quad[index].length = 0;
    stats_quad[index].skip = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_same_row_adjacent_finger_roll_quad_in(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            stats_quad[index].ngrams[i] = i;
            stats_quad[index].length++;
        }
        else
        {
            stats_quad[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_quad[index].name, "Quad Same Row Adjacent Finger Roll Out");
    stats_quad[index].weight = -INFINITY;
    stats_quad[index].length = 0;
    stats_quad[index].skip = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_same_row_adjacent_finger_roll_quad_out(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            stats_quad[index].ngrams[i] = i;
            stats_quad[index].length++;
        }
        else
        {
            stats_quad[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_quad[index].name, "True Roll");
    stats_quad[index].weight = -INFINITY;
    stats_quad[index].length = 0;
    stats_quad[index].skip = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_true_roll(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            stats_quad[index].ngrams[i] = i;
            stats_quad[index].length++;
        }
        else
        {
            stats_quad[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_quad[index].name, "True Roll In");
    stats_quad[index].weight = -INFINITY;
    stats_quad[index].length = 0;
    stats_quad[index].skip = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_true_roll_in(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            stats_quad[index].ngrams[i] = i;
            stats_quad[index].length++;
        }
        else
        {
            stats_quad[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_quad[index].name, "True Roll Out");
    stats_quad[index].weight = -INFINITY;
    stats_quad[index].length = 0;
    stats_quad[index].skip = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_true_roll_out(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            stats_quad[index].ngrams[i] = i;
            stats_quad[index].length++;
        }
        else
        {
            stats_quad[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_quad[index].name, "Same Row True Roll");
    stats_quad[index].weight = -INFINITY;
    stats_quad[index].length = 0;
    stats_quad[index].skip = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_same_row_true_roll(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            stats_quad[index].ngrams[i] = i;
            stats_quad[index].length++;
        }
        else
        {
            stats_quad[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_quad[index].name, "Same Row True Roll In");
    stats_quad[index].weight = -INFINITY;
    stats_quad[index].length = 0;
    stats_quad[index].skip = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_same_row_true_roll_in(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            stats_quad[index].ngrams[i] = i;
            stats_quad[index].length++;
        }
        else
        {
            stats_quad[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_quad[index].name, "Same Row True Roll Out");
    stats_quad[index].weight = -INFINITY;
    stats_quad[index].length = 0;
    stats_quad[index].skip = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_same_row_true_roll_out(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            stats_quad[index].ngrams[i] = i;
            stats_quad[index].length++;
        }
        else
        {
            stats_quad[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_quad[index].name, "Adjacent Finger True Roll");
    stats_quad[index].weight = -INFINITY;
    stats_quad[index].length = 0;
    stats_quad[index].skip = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_adjacent_finger_true_roll(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            stats_quad[index].ngrams[i] = i;
            stats_quad[index].length++;
        }
        else
        {
            stats_quad[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_quad[index].name, "Adjacent Finger True Roll In");
    stats_quad[index].weight = -INFINITY;
    stats_quad[index].length = 0;
    stats_quad[index].skip = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_adjacent_finger_true_roll_in(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            stats_quad[index].ngrams[i] = i;
            stats_quad[index].length++;
        }
        else
        {
            stats_quad[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_quad[index].name, "Adjacent Finger True Roll Out");
    stats_quad[index].weight = -INFINITY;
    stats_quad[index].length = 0;
    stats_quad[index].skip = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_adjacent_finger_true_roll_out(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            stats_quad[index].ngrams[i] = i;
            stats_quad[index].length++;
        }
        else
        {
            stats_quad[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_quad[index].name, "Same Row Adjacent Finger True Roll");
    stats_quad[index].weight = -INFINITY;
    stats_quad[index].length = 0;
    stats_quad[index].skip = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_same_row_adjacent_finger_true_roll(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            stats_quad[index].ngrams[i] = i;
            stats_quad[index].length++;
        }
        else
        {
            stats_quad[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_quad[index].name, "Same Row Adjacent Finger True Roll In");
    stats_quad[index].weight = -INFINITY;
    stats_quad[index].length = 0;
    stats_quad[index].skip = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_same_row_adjacent_finger_true_roll_in(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            stats_quad[index].ngrams[i] = i;
            stats_quad[index].length++;
        }
        else
        {
            stats_quad[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_quad[index].name, "Same Row Adjacent Finger True Roll Out");
    stats_quad[index].weight = -INFINITY;
    stats_quad[index].length = 0;
    stats_quad[index].skip = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_same_row_adjacent_finger_true_roll_out(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            stats_quad[index].ngrams[i] = i;
            stats_quad[index].length++;
        }
        else
        {
            stats_quad[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_quad[index].name, "Chained Roll");
    stats_quad[index].weight = -INFINITY;
    stats_quad[index].length = 0;
    stats_quad[index].skip = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_chained_roll(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            stats_quad[index].ngrams[i] = i;
            stats_quad[index].length++;
        }
        else
        {
            stats_quad[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_quad[index].name, "Chained Roll In");
    stats_quad[index].weight = -INFINITY;
    stats_quad[index].length = 0;
    stats_quad[index].skip = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_chained_roll_in(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            stats_quad[index].ngrams[i] = i;
            stats_quad[index].length++;
        }
        else
        {
            stats_quad[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_quad[index].name, "Chained Roll Out");
    stats_quad[index].weight = -INFINITY;
    stats_quad[index].length = 0;
    stats_quad[index].skip = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_chained_roll_out(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            stats_quad[index].ngrams[i] = i;
            stats_quad[index].length++;
        }
        else
        {
            stats_quad[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_quad[index].name, "Chained Roll Mix");
    stats_quad[index].weight = -INFINITY;
    stats_quad[index].length = 0;
    stats_quad[index].skip = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_chained_roll_mix(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            stats_quad[index].ngrams[i] = i;
            stats_quad[index].length++;
        }
        else
        {
            stats_quad[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_quad[index].name, "Same Row Chained Roll");
    stats_quad[index].weight = -INFINITY;
    stats_quad[index].length = 0;
    stats_quad[index].skip = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_same_row_chained_roll(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            stats_quad[index].ngrams[i] = i;
            stats_quad[index].length++;
        }
        else
        {
            stats_quad[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_quad[index].name, "Same Row Chained Roll In");
    stats_quad[index].weight = -INFINITY;
    stats_quad[index].length = 0;
    stats_quad[index].skip = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_same_row_chained_roll_in(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            stats_quad[index].ngrams[i] = i;
            stats_quad[index].length++;
        }
        else
        {
            stats_quad[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_quad[index].name, "Same Row Chained Roll Out");
    stats_quad[index].weight = -INFINITY;
    stats_quad[index].length = 0;
    stats_quad[index].skip = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_same_row_chained_roll_out(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            stats_quad[index].ngrams[i] = i;
            stats_quad[index].length++;
        }
        else
        {
            stats_quad[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_quad[index].name, "Same Row Chained Roll Mix");
    stats_quad[index].weight = -INFINITY;
    stats_quad[index].length = 0;
    stats_quad[index].skip = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_same_row_chained_roll_mix(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            stats_quad[index].ngrams[i] = i;
            stats_quad[index].length++;
        }
        else
        {
            stats_quad[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_quad[index].name, "Adjacent Finger Chained Roll");
    stats_quad[index].weight = -INFINITY;
    stats_quad[index].length = 0;
    stats_quad[index].skip = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_adjacent_finger_chained_roll(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            stats_quad[index].ngrams[i] = i;
            stats_quad[index].length++;
        }
        else
        {
            stats_quad[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_quad[index].name, "Adjacent Finger Chained Roll In");
    stats_quad[index].weight = -INFINITY;
    stats_quad[index].length = 0;
    stats_quad[index].skip = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_adjacent_finger_chained_roll_in(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            stats_quad[index].ngrams[i] = i;
            stats_quad[index].length++;
        }
        else
        {
            stats_quad[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_quad[index].name, "Adjacent Finger Chained Roll Out");
    stats_quad[index].weight = -INFINITY;
    stats_quad[index].length = 0;
    stats_quad[index].skip = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_adjacent_finger_chained_roll_out(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            stats_quad[index].ngrams[i] = i;
            stats_quad[index].length++;
        }
        else
        {
            stats_quad[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_quad[index].name, "Adjacent Finger Chained Roll Mix");
    stats_quad[index].weight = -INFINITY;
    stats_quad[index].length = 0;
    stats_quad[index].skip = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_adjacent_finger_chained_roll_mix(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            stats_quad[index].ngrams[i] = i;
            stats_quad[index].length++;
        }
        else
        {
            stats_quad[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_quad[index].name, "Same Row Adjacent Finger Chained Roll");
    stats_quad[index].weight = -INFINITY;
    stats_quad[index].length = 0;
    stats_quad[index].skip = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_same_row_adjacent_finger_chained_roll(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            stats_quad[index].ngrams[i] = i;
            stats_quad[index].length++;
        }
        else
        {
            stats_quad[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_quad[index].name, "Same Row Adjacent Finger Chained Roll In");
    stats_quad[index].weight = -INFINITY;
    stats_quad[index].length = 0;
    stats_quad[index].skip = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_same_row_adjacent_finger_chained_roll_in(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            stats_quad[index].ngrams[i] = i;
            stats_quad[index].length++;
        }
        else
        {
            stats_quad[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_quad[index].name, "Same Row Adjacent Finger Chained Roll Out");
    stats_quad[index].weight = -INFINITY;
    stats_quad[index].length = 0;
    stats_quad[index].skip = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_same_row_adjacent_finger_chained_roll_out(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            stats_quad[index].ngrams[i] = i;
            stats_quad[index].length++;
        }
        else
        {
            stats_quad[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_quad[index].name, "Same Row Adjacent Finger Chained Roll Mix");
    stats_quad[index].weight = -INFINITY;
    stats_quad[index].length = 0;
    stats_quad[index].skip = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_same_row_adjacent_finger_chained_roll_mix(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            stats_quad[index].ngrams[i] = i;
            stats_quad[index].length++;
        }
        else
        {
            stats_quad[index].ngrams[i] = -1;
        }
    }
    index++;

    if (index != QUAD_LENGTH) { error("QUAD_LENGTH incorrect for number of quad stats"); }
}


/*
 * Trims the ngrams in the array to move unused entries to the end.
 * This process ensures memory efficiency by eliminating gaps in the array.
 */
void trim_quad_stats()
{
    for (int i = 0; i < QUAD_LENGTH; i++)
    {
        if (stats_quad[i].length != 0)
        {
            int left = 0;
            int right = DIM4 - 1;

            /* Use two pointers to partition the array */
            while (left < right) {
                /* Find the next -1 from the left */
                while (left < right && stats_quad[i].ngrams[left] != -1) {
                    left++;
                }

                /* Find the next non -1 from the right */
                while (left < right && stats_quad[i].ngrams[right] == -1) {
                    right--;
                }

                 /* Swap the elements to move -1 to the back and non -1 to the front */
                if (left < right) {
                    int temp = stats_quad[i].ngrams[left];
                    stats_quad[i].ngrams[left] = stats_quad[i].ngrams[right];
                    stats_quad[i].ngrams[right] = temp;
                    left++;
                    right--;
                }
            }
        }
    }
}

/*
 * Cleans the quadgram statistics array by removing statistics with zero length
 * or weight. This ensures that only relevant statistics are considered in the
 * analysis.
 */
void clean_quad_stats()
{
    /* Iterate through the array and remove those with zero length or weight */
    for (int i = 0; i < QUAD_LENGTH; i++)
    {
        if (stats_quad[i].length == 0 || stats_quad[i].weight == 0) {stats_quad[i].skip = 1;}
    }
}

/* Frees the memory allocated for the quadgram statistics array. */
void free_quad_stats()
{
    free(stats_quad);
}
