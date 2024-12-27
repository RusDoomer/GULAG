/*
 * stats/mono.c - Monogram statistic definitions for the GULAG.
 *
 * Author: Rus Doomer
 *
 * Description: This file contains the implementation for initializing, cleaning,
 *              converting, and freeing monogram statistics used in the GULAG.
 *              Monogram statistics track the frequency and positioning of single
 *              characters on a keyboard layout.
 *
 * Adding new stats:
 *     1. In initialize_mono_stats(), add a new mono_stat structure.
 *     2. Define its name, and set its weight to -INFINITY (it will be changed later in io.c).
 *     3. Make sure to add it to the linked list by setting it as the next element of the last stat.
 *     4. Define its length to 0, then loop through the DIM1 (36) possible positions on the
 *        layout.
 *     5. Use unflat_mono() to convert the 1D index to a 2D coordinate.
 *     6. Use an if statement and a stats_util.c function to check if the ngram falls under
 *        the stat.
 *     7. If it does, add it to the ngrams array and increment length.
 *     8. Add the statistic to the weights files in data/weights/.
 */

#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "mono.h"
#include "util.h"
#include "stats_util.h"
#include "global.h"
#include "structs.h"

/*
 * Initializes the linked list of monogram statistics.
 * Each monogram statistic tracks the usage of specific keys or key positions.
 * The function allocates memory for each stat and sets default values,
 * including a negative infinity weight which will be later updated.
 * Statistics are defined for various key positions and hand usage.
 */
void initialize_mono_stats()
{
    int row0, col0;

    /* Allocate and initialize a new stats for column/finger usage. */
    mono_stat *left_outer = (mono_stat *)malloc(sizeof(mono_stat));
    /* Ensure you set the head on the first stat of each type */
    mono_head = left_outer;
    strcpy(left_outer->name, "Left Outer Usage");
    left_outer->weight = -INFINITY;
    left_outer->length = 0;
    for (int i = 0; i < DIM1; i++)
    {
        /* util.c - convert a 1D index into a 2D matrix coordinate */
        unflat_mono(i, &row0, &col0);
        if (col0 == 0)
        {
            left_outer->ngrams[i] = i;
            left_outer->length++;
        }
        else
        {
            left_outer->ngrams[i] = -1;
        }
    }

    mono_stat *left_pinky = (mono_stat *)malloc(sizeof(mono_stat));
    left_outer->next = left_pinky;
    strcpy(left_pinky->name, "Left Pinky Usage");
    left_pinky->weight = -INFINITY;
    left_pinky->length = 0;
    for (int i = 0; i < DIM1; i++)
    {
        unflat_mono(i, &row0, &col0);
        if (finger(row0, col0) == 0)
        {
            left_pinky->ngrams[i] = i;
            left_pinky->length++;
        }
        else
        {
            left_pinky->ngrams[i] = -1;
        }
    }

    mono_stat *left_ring = (mono_stat *)malloc(sizeof(mono_stat));
    left_pinky->next = left_ring;
    strcpy(left_ring->name, "Left Ring Usage");
    left_ring->weight = -INFINITY;
    left_ring->length = 0;
    for (int i = 0; i < DIM1; i++)
    {
        unflat_mono(i, &row0, &col0);
        if (finger(row0, col0) == 1)
        {
            left_ring->ngrams[i] = i;
            left_ring->length++;
        }
        else
        {
            left_ring->ngrams[i] = -1;
        }
    }

    mono_stat *left_middle = (mono_stat *)malloc(sizeof(mono_stat));
    left_ring->next = left_middle;
    strcpy(left_middle->name, "Left Middle Usage");
    left_middle->weight = -INFINITY;
    left_middle->length = 0;
    for (int i = 0; i < DIM1; i++)
    {
        unflat_mono(i, &row0, &col0);
        if (finger(row0, col0) == 2)
        {
            left_middle->ngrams[i] = i;
            left_middle->length++;
        }
        else
        {
            left_middle->ngrams[i] = -1;
        }
    }

    mono_stat *left_index = (mono_stat *)malloc(sizeof(mono_stat));
    left_middle->next = left_index;
    strcpy(left_index->name, "Left Index Usage");
    left_index->weight = -INFINITY;
    left_index->length = 0;
    for (int i = 0; i < DIM1; i++)
    {
        unflat_mono(i, &row0, &col0);
        if (finger(row0, col0) == 3)
        {
            left_index->ngrams[i] = i;
            left_index->length++;
        }
        else
        {
            left_index->ngrams[i] = -1;
        }
    }

    mono_stat *left_inner = (mono_stat *)malloc(sizeof(mono_stat));
    left_index->next = left_inner;
    strcpy(left_inner->name, "Left Inner Usage");
    left_inner->weight = -INFINITY;
    left_inner->length = 0;
    for (int i = 0; i < DIM1; i++)
    {
        unflat_mono(i, &row0, &col0);
        if (col0 == 5)
        {
            left_inner->ngrams[i] = i;
            left_inner->length++;
        }
        else
        {
            left_inner->ngrams[i] = -1;
        }
    }


    mono_stat *right_inner = (mono_stat *)malloc(sizeof(mono_stat));
    left_inner->next = right_inner;
    strcpy(right_inner->name, "Right Inner Usage");
    right_inner->weight = -INFINITY;
    right_inner->length = 0;
    for (int i = 0; i < DIM1; i++)
    {
        unflat_mono(i, &row0, &col0);
        if (col0 == 6)
        {
            right_inner->ngrams[i] = i;
            right_inner->length++;
        }
        else
        {
            right_inner->ngrams[i] = -1;
        }
    }

    mono_stat *right_index = (mono_stat *)malloc(sizeof(mono_stat));
    right_inner->next = right_index;
    strcpy(right_index->name, "Right Index Usage");
    right_index->weight = -INFINITY;
    right_index->length = 0;
    for (int i = 0; i < DIM1; i++)
    {
        unflat_mono(i, &row0, &col0);
        if (finger(row0, col0) == 4)
        {
            right_index->ngrams[i] = i;
            right_index->length++;
        }
        else
        {
            right_index->ngrams[i] = -1;
        }
    }

    mono_stat *right_middle = (mono_stat *)malloc(sizeof(mono_stat));
    right_index->next = right_middle;
    strcpy(right_middle->name, "Right Middle Usage");
    right_middle->weight = -INFINITY;
    right_middle->length = 0;
    for (int i = 0; i < DIM1; i++)
    {
        unflat_mono(i, &row0, &col0);
        if (finger(row0, col0) == 5)
        {
            right_middle->ngrams[i] = i;
            right_middle->length++;
        }
        else
        {
            right_middle->ngrams[i] = -1;
        }
    }

    mono_stat *right_ring = (mono_stat *)malloc(sizeof(mono_stat));
    right_middle->next = right_ring;
    strcpy(right_ring->name, "Right Ring Usage");
    right_ring->weight = -INFINITY;
    right_ring->length = 0;
    for (int i = 0; i < DIM1; i++)
    {
        unflat_mono(i, &row0, &col0);
        if (finger(row0, col0) == 6)
        {
            right_ring->ngrams[i] = i;
            right_ring->length++;
        }
        else
        {
            right_ring->ngrams[i] = -1;
        }
    }

    mono_stat *right_pinky = (mono_stat *)malloc(sizeof(mono_stat));
    right_ring->next = right_pinky;
    strcpy(right_pinky->name, "Right Pinky Usage");
    right_pinky->weight = -INFINITY;
    right_pinky->length = 0;
    for (int i = 0; i < DIM1; i++)
    {
        unflat_mono(i, &row0, &col0);
        if (finger(row0, col0) == 7)
        {
            right_pinky->ngrams[i] = i;
            right_pinky->length++;
        }
        else
        {
            right_pinky->ngrams[i] = -1;
        }
    }

    mono_stat *right_outer = (mono_stat *)malloc(sizeof(mono_stat));
    right_pinky->next = right_outer;
    strcpy(right_outer->name, "Right Outer Usage");
    right_outer->weight = -INFINITY;
    right_outer->length = 0;
    for (int i = 0; i < DIM1; i++)
    {
        unflat_mono(i, &row0, &col0);
        if (col0 == 11)
        {
            right_outer->ngrams[i] = i;
            right_outer->length++;
        }
        else
        {
            right_outer->ngrams[i] = -1;
        }
    }

    /* Allocate and initialize a new stats for hand usage. */
    mono_stat *left_hand = (mono_stat *)malloc(sizeof(mono_stat));
    right_outer->next = left_hand;
    strcpy(left_hand->name, "Left Hand Usage");
    left_hand->weight = -INFINITY;
    left_hand->length = 0;
    for (int i = 0; i < DIM1; i++)
    {
        unflat_mono(i, &row0, &col0);
        if (hand(row0, col0) == 'l')
        {
            left_hand->ngrams[i] = i;
            left_hand->length++;
        }
        else
        {
            left_hand->ngrams[i] = -1;
        }
    }

    mono_stat *right_hand = (mono_stat *)malloc(sizeof(mono_stat));
    left_hand->next = right_hand;
    strcpy(right_hand->name, "Right Hand Usage");
    right_hand->weight = -INFINITY;
    right_hand->length = 0;
    for (int i = 0; i < DIM1; i++)
    {
        unflat_mono(i, &row0, &col0);
        if (hand(row0, col0) == 'r')
        {
            right_hand->ngrams[i] = i;
            right_hand->length++;
        }
        else
        {
            right_hand->ngrams[i] = -1;
        }
    }

    /* Allocate and initialize a new stats for row usage. */
    mono_stat *top_row = (mono_stat *)malloc(sizeof(mono_stat));
    right_hand->next = top_row;
    strcpy(top_row->name, "Top Row Usage");
    top_row->weight = -INFINITY;
    top_row->length = 0;
    for (int i = 0; i < DIM1; i++)
    {
        unflat_mono(i, &row0, &col0);
        if (row0 == 0)
        {
            top_row->ngrams[i] = i;
            top_row->length++;
        }
        else
        {
            top_row->ngrams[i] = -1;
        }
    }

    mono_stat *home_row = (mono_stat *)malloc(sizeof(mono_stat));
    top_row->next = home_row;
    strcpy(home_row->name, "Home Row Usage");
    home_row->weight = -INFINITY;
    home_row->length = 0;
    for (int i = 0; i < DIM1; i++)
    {
        unflat_mono(i, &row0, &col0);
        if (row0 == 1)
        {
            home_row->ngrams[i] = i;
            home_row->length++;
        }
        else
        {
            home_row->ngrams[i] = -1;
        }
    }

    mono_stat *bottom_row = (mono_stat *)malloc(sizeof(mono_stat));
    home_row->next = bottom_row;
    strcpy(bottom_row->name, "Bottom Row Usage");
    bottom_row->weight = -INFINITY;
    bottom_row->length = 0;
    for (int i = 0; i < DIM1; i++)
    {
        unflat_mono(i, &row0, &col0);
        if (row0 == 2)
        {
            bottom_row->ngrams[i] = i;
            bottom_row->length++;
        }
        else
        {
            bottom_row->ngrams[i] = -1;
        }
    }

    bottom_row->next = NULL;
}

/*
 * Trims the statistics in the linked list to move unused entries to the end.
 * This is done by iterating through each statistic's ngrams array and
 * reordering elements to place all valid entries at the beginning
 * of the array. This process ensures memory efficiency by eliminating
 * gaps in the array.
 */
void trim_mono_stats()
{
    mono_stat *current = mono_head;

    while (current != NULL)
    {
        /* Copy valid ngram entries into earliest free index */
        if (current->length != 0)
        {
            int left = 0;
            int right = DIM1 - 1;

            /* Use two pointers to partition the array */
            while (left < right) {
                /* Find the next -1 from the left */
                while (left < right && current->ngrams[left] != -1) {
                    left++;
                }

                /* Find the next non -1 from the right */
                while (left < right && current->ngrams[right] == -1) {
                    right--;
                }

                 /* Swap the elements to move -1 to the back and non -1 to the front */
                if (left < right) {
                    int temp = current->ngrams[left];
                    current->ngrams[left] = current->ngrams[right];
                    current->ngrams[right] = temp;
                    left++;
                    right--;
                }
            }
        }

        current = current->next;
    }
}

/*
 * Cleans the monogram statistics linked list by removing statistics with zero length or weight.
 * This ensures that only relevant statistics are considered in the analysis.
 */
void clean_mono_stats()
{
    /* Remove statistics from the beginning of the list if they have zero length or weight */
    if (mono_head == NULL) {return;}
    while (mono_head != NULL && (mono_head->length == 0 || mono_head->weight == 0)) {
        mono_stat *temp = mono_head;
        mono_head = mono_head->next;
        free(temp);
    }

    /* Iterate through the rest of the list and remove any node with zero length or weight */
    mono_stat *current = mono_head;
    while (current != NULL && current->next != NULL) {
        if (current->next->length == 0 || current->next->weight == 0) {
            mono_stat *temp = current->next;
            current->next = current->next->next;
            free(temp);
        } else {
            current = current->next;
        }
    }

    /* Recount the number of valid statistics */
    current = mono_head;
    while (current != NULL) {
        MONO_END++;
        current = current->next;
    }
}

/*
 * Converts the linked list of monogram statistics to a contiguous array for easier access.
 * Allocates memory for an array of mono_stat structures and copies data from the linked list.
 */
void mono_to_array()
{
    stats_mono = (mono_stat *)malloc(sizeof(mono_stat) * MONO_END);
    mono_stat *current_mono = mono_head;
    for (int i = 0; i < MONO_END; i++) {
        memcpy(&stats_mono[i], current_mono, sizeof(mono_stat));
        /* Set next pointer to NULL */
        stats_mono[i].next = NULL;
        current_mono = current_mono->next;
    }
}

/*
 * Frees the memory allocated for the monogram statistics linked list.
 * Iterates through the list, freeing each node's memory.
 */
void free_mono_stats()
{
    if (mono_head == NULL) {return;}
    while (mono_head != NULL)
    {
        mono_stat *temp = mono_head;
        mono_head = mono_head->next;
        free(temp);
    }
    free(stats_mono);
}
