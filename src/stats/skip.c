/*
 * stats/skip.c - Skipgram statistic definitions for the GULAG.
 *
 * Author: Rus Doomer
 *
 * Description: This file contains the implementation for initializing, cleaning,
 *              converting, and freeing skipgram statistics used in the GULAG.
 *              Skipgram statistics track the frequency and positioning of two
 *              character sequences on a keyboard layout with a certain number
 *              of characters (the skip) between them.
 *
 * Adding new stats:
 *     1. In initialize_skip_stats(), add a new skip_stat structure.
 *     2. Define its name, and set its weight to -INFINITY for each skip (it will be changed later in io.c).
 *     3. Make sure to add it to the linked list by setting it as the next element of the last stat.
 *     4. Define its length to 0, then loop through the DIM2 (1296) possible skipgrams on the
 *        layout.
 *     5. Use unflat_bi() to convert the 1D index to a 4D coordinate.
 *     6. Use an if statement and a stats_util.c function to check if the ngram falls under
 *        the stat.
 *     7. If it does, add it to the ngrams array and increment length.
 *     8. Add the statistic to the weights files in data/weights/.
 */


#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "skip.h"
#include "util.h"
#include "stats_util.h"
#include "global.h"

/*
 * Initializes the linked list of skipgram statistics.
 * Each skipgram statistic tracks the usage of specific key sequences
 * with a certain skip. The function allocates memory for each stat
 * and sets default values, including a negative infinity weight which will be later updated.
 * Statistics are defined for various key sequences and combinations.
 */
void initialize_skip_stats()
{
    int row0, col0, row1, col1;
    /* Allocate and initialize SFS. */
    skip_stat *same_finger = (skip_stat *)malloc(sizeof(skip_stat));
    /* Ensure you set the head on the first stat of each type. */
    skip_head = same_finger;
    strcpy(same_finger->name, "Same Finger Skipgram");
    /* Set all skip weights to -INFINITY. */
    for (int i = 0; i < 10; i++) {
        same_finger->weight[i] = -INFINITY;
    }
    same_finger->length = 0;
    for (int i = 0; i < DIM2; i++)
    {
        /* util.c - convert a 1D index into a 4D matrix coordinate */
        unflat_bi(i, &row0, &col0, &row1, &col1);
        if (is_same_finger_bi(row0, col0, row1, col1))
        {
            same_finger->ngrams[i] = i;
            same_finger->length++;
        }
        else
        {
            same_finger->ngrams[i] = -1;
        }
    }

    /* per finger SFS */
    skip_stat *left_pinky = (skip_stat *)malloc(sizeof(skip_stat));
    same_finger->next = left_pinky;
    strcpy(left_pinky->name, "Left Pinky Skipgram");
    for (int i = 0; i < 10; i++) {
        left_pinky->weight[i] = -INFINITY;
    }
    left_pinky->length = 0;
    for (int i = 0; i < DIM2; i++)
    {
        unflat_bi(i, &row0, &col0, &row1, &col1);
        if (is_same_finger_bi(row0, col0, row1, col1) && finger(row0, col0) == 0)
        {
            left_pinky->ngrams[i] = i;
            left_pinky->length++;
        }
        else
        {
            left_pinky->ngrams[i] = -1;
        }
    }

    skip_stat *left_ring = (skip_stat *)malloc(sizeof(skip_stat));
    left_pinky->next = left_ring;
    strcpy(left_ring->name, "Left Ring Skipgram");
    for (int i = 0; i < 10; i++) {
        left_ring->weight[i] = -INFINITY;
    }
    left_ring->length = 0;
    for (int i = 0; i < DIM2; i++)
    {
        unflat_bi(i, &row0, &col0, &row1, &col1);
        if (is_same_finger_bi(row0, col0, row1, col1) && finger(row0, col0) == 1)
        {
            left_ring->ngrams[i] = i;
            left_ring->length++;
        }
        else
        {
            left_ring->ngrams[i] = -1;
        }
    }

    skip_stat *left_middle = (skip_stat *)malloc(sizeof(skip_stat));
    left_ring->next = left_middle;
    strcpy(left_middle->name, "Left Middle Skipgram");
    for (int i = 0; i < 10; i++) {
        left_middle->weight[i] = -INFINITY;
    }
    left_middle->length = 0;
    for (int i = 0; i < DIM2; i++)
    {
        unflat_bi(i, &row0, &col0, &row1, &col1);
        if (is_same_finger_bi(row0, col0, row1, col1) && finger(row0, col0) == 2)
        {
            left_middle->ngrams[i] = i;
            left_middle->length++;
        }
        else
        {
            left_middle->ngrams[i] = -1;
        }
    }

    skip_stat *left_index = (skip_stat *)malloc(sizeof(skip_stat));
    left_middle->next = left_index;
    strcpy(left_index->name, "Left Index Skipgram");
    for (int i = 0; i < 10; i++) {
        left_index->weight[i] = -INFINITY;
    }
    left_index->length = 0;
    for (int i = 0; i < DIM2; i++)
    {
        unflat_bi(i, &row0, &col0, &row1, &col1);
        if (is_same_finger_bi(row0, col0, row1, col1) && finger(row0, col0) == 3)
        {
            left_index->ngrams[i] = i;
            left_index->length++;
        }
        else
        {
            left_index->ngrams[i] = -1;
        }
    }

    skip_stat *right_index = (skip_stat *)malloc(sizeof(skip_stat));
    left_index->next = right_index;
    strcpy(right_index->name, "Right Index Skipgram");
    for (int i = 0; i < 10; i++) {
        right_index->weight[i] = -INFINITY;
    }
    right_index->length = 0;
    for (int i = 0; i < DIM2; i++)
    {
        unflat_bi(i, &row0, &col0, &row1, &col1);
        if (is_same_finger_bi(row0, col0, row1, col1) && finger(row0, col0) == 4)
        {
            right_index->ngrams[i] = i;
            right_index->length++;
        }
        else
        {
            right_index->ngrams[i] = -1;
        }
    }

    skip_stat *right_middle = (skip_stat *)malloc(sizeof(skip_stat));
    right_index->next = right_middle;
    strcpy(right_middle->name, "Right Middle Skipgram");
    for (int i = 0; i < 10; i++) {
        right_middle->weight[i] = -INFINITY;
    }
    right_middle->length = 0;
    for (int i = 0; i < DIM2; i++)
    {
        unflat_bi(i, &row0, &col0, &row1, &col1);
        if (is_same_finger_bi(row0, col0, row1, col1) && finger(row0, col0) == 5)
        {
            right_middle->ngrams[i] = i;
            right_middle->length++;
        }
        else
        {
            right_middle->ngrams[i] = -1;
        }
    }

    skip_stat *right_ring = (skip_stat *)malloc(sizeof(skip_stat));
    right_middle->next = right_ring;
    strcpy(right_ring->name, "Right Ring Skipgram");
    for (int i = 0; i < 10; i++) {
        right_ring->weight[i] = -INFINITY;
    }
    right_ring->length = 0;
    for (int i = 0; i < DIM2; i++)
    {
        unflat_bi(i, &row0, &col0, &row1, &col1);
        if (is_same_finger_bi(row0, col0, row1, col1) && finger(row0, col0) == 6)
        {
            right_ring->ngrams[i] = i;
            right_ring->length++;
        }
        else
        {
            right_ring->ngrams[i] = -1;
        }
    }

    skip_stat *right_pinky = (skip_stat *)malloc(sizeof(skip_stat));
    right_ring->next = right_pinky;
    strcpy(right_pinky->name, "Right Pinky Skipgram");
    for (int i = 0; i < 10; i++) {
        right_pinky->weight[i] = -INFINITY;
    }
    right_pinky->length = 0;
    for (int i = 0; i < DIM2; i++)
    {
        unflat_bi(i, &row0, &col0, &row1, &col1);
        if (is_same_finger_bi(row0, col0, row1, col1) && finger(row0, col0) == 7)
        {
            right_pinky->ngrams[i] = i;
            right_pinky->length++;
        }
        else
        {
            right_pinky->ngrams[i] = -1;
        }
    }

    /* 2U SFS */
    skip_stat *bad_same_finger = (skip_stat *)malloc(sizeof(skip_stat));
    right_pinky->next = bad_same_finger;
    strcpy(bad_same_finger->name, "Bad Same Finger Skipgram");
    for (int i = 0; i < 10; i++) {
        bad_same_finger->weight[i] = -INFINITY;
    }
    bad_same_finger->length = 0;
    for (int i = 0; i < DIM2; i++)
    {
        unflat_bi(i, &row0, &col0, &row1, &col1);
        if (is_bad_same_finger_bi(row0, col0, row1, col1))
        {
            bad_same_finger->ngrams[i] = i;
            bad_same_finger->length++;
        }
        else
        {
            bad_same_finger->ngrams[i] = -1;
        }
    }

    /* per finger 2U SFS */
    skip_stat *bad_left_pinky = (skip_stat *)malloc(sizeof(skip_stat));
    bad_same_finger->next = bad_left_pinky;
    strcpy(bad_left_pinky->name, "Bad Left Pinky Skipgram");
    for (int i = 0; i < 10; i++) {
        bad_left_pinky->weight[i] = -INFINITY;
    }
    bad_left_pinky->length = 0;
    for (int i = 0; i < DIM2; i++)
    {
        unflat_bi(i, &row0, &col0, &row1, &col1);
        if (is_bad_same_finger_bi(row0, col0, row1, col1) && finger(row0, col0) == 0)
        {
            bad_left_pinky->ngrams[i] = i;
            bad_left_pinky->length++;
        }
        else
        {
            bad_left_pinky->ngrams[i] = -1;
        }
    }

    skip_stat *bad_left_ring = (skip_stat *)malloc(sizeof(skip_stat));
    bad_left_pinky->next = bad_left_ring;
    strcpy(bad_left_ring->name, "Bad Left Ring Skipgram");
    for (int i = 0; i < 10; i++) {
        bad_left_ring->weight[i] = -INFINITY;
    }
    bad_left_ring->length = 0;
    for (int i = 0; i < DIM2; i++)
    {
        unflat_bi(i, &row0, &col0, &row1, &col1);
        if (is_bad_same_finger_bi(row0, col0, row1, col1) && finger(row0, col0) == 1)
        {
            bad_left_ring->ngrams[i] = i;
            bad_left_ring->length++;
        }
        else
        {
            bad_left_ring->ngrams[i] = -1;
        }
    }

    skip_stat *bad_left_middle = (skip_stat *)malloc(sizeof(skip_stat));
    bad_left_ring->next = bad_left_middle;
    strcpy(bad_left_middle->name, "Bad Left Middle Skipgram");
    for (int i = 0; i < 10; i++) {
        bad_left_middle->weight[i] = -INFINITY;
    }
    bad_left_middle->length = 0;
    for (int i = 0; i < DIM2; i++)
    {
        unflat_bi(i, &row0, &col0, &row1, &col1);
        if (is_bad_same_finger_bi(row0, col0, row1, col1) && finger(row0, col0) == 2)
        {
            bad_left_middle->ngrams[i] = i;
            bad_left_middle->length++;
        }
        else
        {
            bad_left_middle->ngrams[i] = -1;
        }
    }

    skip_stat *bad_left_index = (skip_stat *)malloc(sizeof(skip_stat));
    bad_left_middle->next = bad_left_index;
    strcpy(bad_left_index->name, "Bad Left Index Skipgram");
    for (int i = 0; i < 10; i++) {
        bad_left_index->weight[i] = -INFINITY;
    }
    bad_left_index->length = 0;
    for (int i = 0; i < DIM2; i++)
    {
        unflat_bi(i, &row0, &col0, &row1, &col1);
        if (is_bad_same_finger_bi(row0, col0, row1, col1) && finger(row0, col0) == 3)
        {
            bad_left_index->ngrams[i] = i;
            bad_left_index->length++;
        }
        else
        {
            bad_left_index->ngrams[i] = -1;
        }
    }

    skip_stat *bad_right_index = (skip_stat *)malloc(sizeof(skip_stat));
    bad_left_index->next = bad_right_index;
    strcpy(bad_right_index->name, "Bad Right Index Skipgram");
    for (int i = 0; i < 10; i++) {
        bad_right_index->weight[i] = -INFINITY;
    }
    bad_right_index->length = 0;
    for (int i = 0; i < DIM2; i++)
    {
        unflat_bi(i, &row0, &col0, &row1, &col1);
        if (is_bad_same_finger_bi(row0, col0, row1, col1) && finger(row0, col0) == 4)
        {
            bad_right_index->ngrams[i] = i;
            bad_right_index->length++;
        }
        else
        {
            bad_right_index->ngrams[i] = -1;
        }
    }

    skip_stat *bad_right_middle = (skip_stat *)malloc(sizeof(skip_stat));
    bad_right_index->next = bad_right_middle;
    strcpy(bad_right_middle->name, "Bad Right Middle Skipgram");
    for (int i = 0; i < 10; i++) {
        bad_right_middle->weight[i] = -INFINITY;
    }
    bad_right_middle->length = 0;
    for (int i = 0; i < DIM2; i++)
    {
        unflat_bi(i, &row0, &col0, &row1, &col1);
        if (is_bad_same_finger_bi(row0, col0, row1, col1) && finger(row0, col0) == 5)
        {
            bad_right_middle->ngrams[i] = i;
            bad_right_middle->length++;
        }
        else
        {
            bad_right_middle->ngrams[i] = -1;
        }
    }

    skip_stat *bad_right_ring = (skip_stat *)malloc(sizeof(skip_stat));
    bad_right_middle->next = bad_right_ring;
    strcpy(bad_right_ring->name, "Bad Right Ring Skipgram");
    for (int i = 0; i < 10; i++) {
        bad_right_ring->weight[i] = -INFINITY;
    }
    bad_right_ring->length = 0;
    for (int i = 0; i < DIM2; i++)
    {
        unflat_bi(i, &row0, &col0, &row1, &col1);
        if (is_bad_same_finger_bi(row0, col0, row1, col1) && finger(row0, col0) == 6)
        {
            bad_right_ring->ngrams[i] = i;
            bad_right_ring->length++;
        }
        else
        {
            bad_right_ring->ngrams[i] = -1;
        }
    }

    skip_stat *bad_right_pinky = (skip_stat *)malloc(sizeof(skip_stat));
    bad_right_ring->next = bad_right_pinky;
    strcpy(bad_right_pinky->name, "Bad Right Pinky Skipgram");
    for (int i = 0; i < 10; i++) {
        bad_right_pinky->weight[i] = -INFINITY;
    }
    bad_right_pinky->length = 0;
    for (int i = 0; i < DIM2; i++)
    {
        unflat_bi(i, &row0, &col0, &row1, &col1);
        if (is_bad_same_finger_bi(row0, col0, row1, col1) && finger(row0, col0) == 7)
        {
            bad_right_pinky->ngrams[i] = i;
            bad_right_pinky->length++;
        }
        else
        {
            bad_right_pinky->ngrams[i] = -1;
        }
    }

    bad_right_pinky->next = NULL;
}

/*
 * Trims the statistics in the linked list to move unused entries to the end.
 * This is done by iterating through each statistic's ngrams array and
 * reordering elements to place all valid entries at the beginning
 * of the array. This process ensures memory efficiency by eliminating
 * gaps in the array. It is somewhat analagous to a correctional officer
 * ordering prisoners to form an orderly line, ensuring no gaps or disorder.
 */
void trim_skip_stats()
{
    skip_stat *current = skip_head;

    while (current != NULL)
    {
        /* Copy valid ngram entries into earliest free index */
        if (current->length != 0)
        {
            int left = 0;
            int right = DIM2 - 1;

            /* Use two pointers to efficiently partition the array */
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
 * Cleans the skipgram statistics linked list by removing statistics with zero length or zero weight.
 * This function ensures that only relevant statistics are kept for analysis,
 * similar to weeding out unnecessary files from a meticulously organized archive.
 * It updates the SKIP_END global variable to reflect the new count of valid statistics.
 */
void clean_skip_stats()
{
    /* Remove statistics from the beginning of the list if they have zero length or all zero weights */
    if (skip_head == NULL) {return;}
    while (skip_head != NULL && (skip_head->length == 0 || all_zeroes(skip_head))) {
        skip_stat *temp = skip_head;
        skip_head = skip_head->next;
        free(temp);
    }

    /* Iterate through the rest of the list and remove any node with zero length or all zero weights */
    skip_stat *current = skip_head;
    while (current != NULL && current->next != NULL) {
        if (current->next->length == 0 || all_zeroes(current->next)) {
            skip_stat *temp = current->next;
            current->next = current->next->next;
            free(temp);
        } else {
            current = current->next;
        }
    }

    /* Recount the number of valid statistics */
    current = skip_head;
    while (current != NULL) {
        SKIP_END++;
        current = current->next;
    }
}

/*
 * Converts the linked list of skipgram statistics to a contiguous array for easier access.
 * This function allocates memory for an array of skip_stat structures and copies
 * data from the linked list to the array. This is akin to transferring data from
 * individual files to a consolidated ledger for streamlined processing.
 */
void skip_to_array()
{
    stats_skip = (skip_stat *)malloc(sizeof(skip_stat) * SKIP_END);
    skip_stat *current_skip = skip_head;
    for (int i = 0; i < SKIP_END; i++) {
        memcpy(&stats_skip[i], current_skip, sizeof(skip_stat));
        /* Set next pointer to NULL */
        stats_skip[i].next = NULL;
        current_skip = current_skip->next;
    }
}

/*
 * Frees the memory allocated for the skipgram statistics linked list.
 * This function iterates through the list, freeing each node's memory,
 * similar to the careful archiving of documents that are no longer needed.
 */
void free_skip_stats()
{
    if (skip_head == NULL) {return;}
    while (skip_head != NULL)
    {
        skip_stat *temp = skip_head;
        skip_head = skip_head->next;
        free(temp);
    }
    free(stats_skip);
}
