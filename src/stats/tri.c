/*
 * stats/tri.c - Trigram statistic definitions for the GULAG.
 *
 * Author: Rus Doomer
 *
 * Description: This file contains the implementation for initializing, cleaning,
 *              converting, and freeing trigram statistics used in the GULAG.
 *              Trigram statistics track the frequency and positioning of three
 *              character sequences on a keyboard layout.
 *
 * Adding new stats:
 *     1. In initialize_tri_stats(), add a new tri_stat structure.
 *     2. Define its name, and set its weight to -INFINITY (it will be changed later in io.c).
 *     3. Make sure to add it to the linked list by setting it as the next element of the last stat.
 *     4. Define its length to 0, then loop through the DIM3 (46656) possible trigrams on the
 *        layout.
 *     5. Use unflat_tri() to convert the 1D index to a 6D coordinate.
 *     6. Use an if statement and a stats_util.c function to check if the ngram falls under
 *        the stat.
 *     7. If it does, add it to the ngrams array and increment length.
 *     8. Add the statistic to the weights files in data/weights/.
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
 * Initializes the linked list of trigram statistics.
 * Each trigram statistic tracks the usage of specific key sequences.
 * The function allocates memory for each stat and sets default values,
 * including a negative infinity weight which will be later updated.
 * Statistics are defined for various key sequences and combinations.
 */
void initialize_tri_stats()
{
    int row0, col0, row1, col1, row2, col2;

    /* Allocate and initialize SFTs. */
    tri_stat *same_finger = (tri_stat *)malloc(sizeof(tri_stat));
    /* Ensure you set the head on the first stat of each type. */
    tri_head = same_finger;
    strcpy(same_finger->name, "Same Finger Trigram");
    same_finger->weight = -INFINITY;
    same_finger->length = 0;
    for (int i = 0; i < DIM3; i++)
    {
        /* util.c - convert a 1D index into a 6D matrix coordinate */
        unflat_tri(i, &row0, &col0, &row1, &col1, &row2, &col2);
        if (is_same_finger_tri(row0, col0, row1, col1, row2, col2))
        {
            same_finger->ngrams[i] = i;
            same_finger->length++;
        }
        else
        {
            same_finger->ngrams[i] = -1;
        }
    }

    /* standard trigram stats after this */
    tri_stat *alt = (tri_stat *)malloc(sizeof(tri_stat));
    same_finger->next = alt;
    strcpy(alt->name, "Alternation");
    alt->weight = -INFINITY;
    alt->length = 0;
    for (int i = 0; i < DIM3; i++)
    {
        unflat_tri(i, &row0, &col0, &row1, &col1, &row2, &col2);
        if (is_alt(row0, col0, row1, col1, row2, col2))
        {
            alt->ngrams[i] = i;
            alt->length++;
        }
        else
        {
            alt->ngrams[i] = -1;
        }
    }

    tri_stat *redirect = (tri_stat *)malloc(sizeof(tri_stat));
    alt->next = redirect;
    strcpy(redirect->name, "Redirect");
    redirect->weight = -INFINITY;
    redirect->length = 0;
    for (int i = 0; i < DIM3; i++)
    {
        unflat_tri(i, &row0, &col0, &row1, &col1, &row2, &col2);
        if (is_redirect(row0, col0, row1, col1, row2, col2))
        {
            redirect->ngrams[i] = i;
            redirect->length++;
        }
        else
        {
            redirect->ngrams[i] = -1;
        }
    }

    tri_stat *bad_redirect = (tri_stat *)malloc(sizeof(tri_stat));
    redirect->next = bad_redirect;
    strcpy(bad_redirect->name, "Bad Redirect");
    bad_redirect->weight = -INFINITY;
    bad_redirect->length = 0;
    for (int i = 0; i < DIM3; i++)
    {
        unflat_tri(i, &row0, &col0, &row1, &col1, &row2, &col2);
        if (is_bad_redirect(row0, col0, row1, col1, row2, col2))
        {
            bad_redirect->ngrams[i] = i;
            bad_redirect->length++;
        }
        else
        {
            bad_redirect->ngrams[i] = -1;
        }
    }

    tri_stat *onehand = (tri_stat *)malloc(sizeof(tri_stat));
    bad_redirect->next = onehand;
    strcpy(onehand->name, "One Hand");
    onehand->weight = -INFINITY;
    onehand->length = 0;
    for (int i = 0; i < DIM3; i++)
    {
        unflat_tri(i, &row0, &col0, &row1, &col1, &row2, &col2);
        if (is_onehand(row0, col0, row1, col1, row2, col2))
        {
            onehand->ngrams[i] = i;
            onehand->length++;
        }
        else
        {
            onehand->ngrams[i] = -1;
        }
    }

    tri_stat *onehand_in = (tri_stat *)malloc(sizeof(tri_stat));
    onehand->next = onehand_in;
    strcpy(onehand_in->name, "One Hand In");
    onehand_in->weight = -INFINITY;
    onehand_in->length = 0;
    for (int i = 0; i < DIM3; i++)
    {
        unflat_tri(i, &row0, &col0, &row1, &col1, &row2, &col2);
        if (is_onehand_in(row0, col0, row1, col1, row2, col2))
        {
            onehand_in->ngrams[i] = i;
            onehand_in->length++;
        }
        else
        {
            onehand_in->ngrams[i] = -1;
        }
    }

    tri_stat *onehand_out = (tri_stat *)malloc(sizeof(tri_stat));
    onehand_in->next = onehand_out;
    strcpy(onehand_out->name, "One Hand Out");
    onehand_out->weight = -INFINITY;
    onehand_out->length = 0;
    for (int i = 0; i < DIM3; i++)
    {
        unflat_tri(i, &row0, &col0, &row1, &col1, &row2, &col2);
        if (is_onehand_out(row0, col0, row1, col1, row2, col2))
        {
            onehand_out->ngrams[i] = i;
            onehand_out->length++;
        }
        else
        {
            onehand_out->ngrams[i] = -1;
        }
    }

    tri_stat *same_row_onehand = (tri_stat *)malloc(sizeof(tri_stat));
    onehand_out->next = same_row_onehand;
    strcpy(same_row_onehand->name, "Same Row One Hand");
    same_row_onehand->weight = -INFINITY;
    same_row_onehand->length = 0;
    for (int i = 0; i < DIM3; i++)
    {
        unflat_tri(i, &row0, &col0, &row1, &col1, &row2, &col2);
        if (is_same_row_onehand(row0, col0, row1, col1, row2, col2))
        {
            same_row_onehand->ngrams[i] = i;
            same_row_onehand->length++;
        }
        else
        {
            same_row_onehand->ngrams[i] = -1;
        }
    }

    tri_stat *same_row_onehand_in = (tri_stat *)malloc(sizeof(tri_stat));
    same_row_onehand->next = same_row_onehand_in;
    strcpy(same_row_onehand_in->name, "Same Row One Hand In");
    same_row_onehand_in->weight = -INFINITY;
    same_row_onehand_in->length = 0;
    for (int i = 0; i < DIM3; i++)
    {
        unflat_tri(i, &row0, &col0, &row1, &col1, &row2, &col2);
        if (is_same_row_onehand_in(row0, col0, row1, col1, row2, col2))
        {
            same_row_onehand_in->ngrams[i] = i;
            same_row_onehand_in->length++;
        }
        else
        {
            same_row_onehand_in->ngrams[i] = -1;
        }
    }

    tri_stat *same_row_onehand_out = (tri_stat *)malloc(sizeof(tri_stat));
    same_row_onehand_in->next = same_row_onehand_out;
    strcpy(same_row_onehand_out->name, "Same Row One Hand Out");
    same_row_onehand_out->weight = -INFINITY;
    same_row_onehand_out->length = 0;
    for (int i = 0; i < DIM3; i++)
    {
        unflat_tri(i, &row0, &col0, &row1, &col1, &row2, &col2);
        if (is_same_row_onehand_out(row0, col0, row1, col1, row2, col2))
        {
            same_row_onehand_out->ngrams[i] = i;
            same_row_onehand_out->length++;
        }
        else
        {
            same_row_onehand_out->ngrams[i] = -1;
        }
    }

    tri_stat *adjacent_finger_onehand = (tri_stat *)malloc(sizeof(tri_stat));
    same_row_onehand_out->next = adjacent_finger_onehand;
    strcpy(adjacent_finger_onehand->name, "Adjacent Finger One Hand");
    adjacent_finger_onehand->weight = -INFINITY;
    adjacent_finger_onehand->length = 0;
    for (int i = 0; i < DIM3; i++)
    {
        unflat_tri(i, &row0, &col0, &row1, &col1, &row2, &col2);
        if (is_adjacent_finger_onehand(row0, col0, row1, col1, row2, col2))
        {
            adjacent_finger_onehand->ngrams[i] = i;
            adjacent_finger_onehand->length++;
        }
        else
        {
            adjacent_finger_onehand->ngrams[i] = -1;
        }
    }

    tri_stat *adjacent_finger_onehand_in = (tri_stat *)malloc(sizeof(tri_stat));
    adjacent_finger_onehand->next = adjacent_finger_onehand_in;
    strcpy(adjacent_finger_onehand_in->name, "Adjacent Finger One Hand In");
    adjacent_finger_onehand_in->weight = -INFINITY;
    adjacent_finger_onehand_in->length = 0;
    for (int i = 0; i < DIM3; i++)
    {
        unflat_tri(i, &row0, &col0, &row1, &col1, &row2, &col2);
        if (is_adjacent_finger_onehand_in(row0, col0, row1, col1, row2, col2))
        {
            adjacent_finger_onehand_in->ngrams[i] = i;
            adjacent_finger_onehand_in->length++;
        }
        else
        {
            adjacent_finger_onehand_in->ngrams[i] = -1;
        }
    }

    tri_stat *adjacent_finger_onehand_out = (tri_stat *)malloc(sizeof(tri_stat));
    adjacent_finger_onehand_in->next = adjacent_finger_onehand_out;
    strcpy(adjacent_finger_onehand_out->name, "Adjacent Finger One Hand Out");
    adjacent_finger_onehand_out->weight = -INFINITY;
    adjacent_finger_onehand_out->length = 0;
    for (int i = 0; i < DIM3; i++)
    {
        unflat_tri(i, &row0, &col0, &row1, &col1, &row2, &col2);
        if (is_adjacent_finger_onehand_out(row0, col0, row1, col1, row2, col2))
        {
            adjacent_finger_onehand_out->ngrams[i] = i;
            adjacent_finger_onehand_out->length++;
        }
        else
        {
            adjacent_finger_onehand_out->ngrams[i] = -1;
        }
    }

    tri_stat *same_row_adjacent_finger_onehand = (tri_stat *)malloc(sizeof(tri_stat));
    adjacent_finger_onehand_out->next = same_row_adjacent_finger_onehand;
    strcpy(same_row_adjacent_finger_onehand->name, "Same Row Adjacent Finger One Hand");
    same_row_adjacent_finger_onehand->weight = -INFINITY;
    same_row_adjacent_finger_onehand->length = 0;
    for (int i = 0; i < DIM3; i++)
    {
        unflat_tri(i, &row0, &col0, &row1, &col1, &row2, &col2);
        if (is_same_row_adjacent_finger_onehand(row0, col0, row1, col1, row2, col2))
        {
            same_row_adjacent_finger_onehand->ngrams[i] = i;
            same_row_adjacent_finger_onehand->length++;
        }
        else
        {
            same_row_adjacent_finger_onehand->ngrams[i] = -1;
        }
    }

    tri_stat *same_row_adjacent_finger_onehand_in = (tri_stat *)malloc(sizeof(tri_stat));
    same_row_adjacent_finger_onehand->next = same_row_adjacent_finger_onehand_in;
    strcpy(same_row_adjacent_finger_onehand_in->name, "Same Row Adjacent Finger One Hand In");
    same_row_adjacent_finger_onehand_in->weight = -INFINITY;
    same_row_adjacent_finger_onehand_in->length = 0;
    for (int i = 0; i < DIM3; i++)
    {
        unflat_tri(i, &row0, &col0, &row1, &col1, &row2, &col2);
        if (is_same_row_adjacent_finger_onehand_in(row0, col0, row1, col1, row2, col2))
        {
            same_row_adjacent_finger_onehand_in->ngrams[i] = i;
            same_row_adjacent_finger_onehand_in->length++;
        }
        else
        {
            same_row_adjacent_finger_onehand_in->ngrams[i] = -1;
        }
    }

    tri_stat *same_row_adjacent_finger_onehand_out = (tri_stat *)malloc(sizeof(tri_stat));
    same_row_adjacent_finger_onehand_in->next = same_row_adjacent_finger_onehand_out;
    strcpy(same_row_adjacent_finger_onehand_out->name, "Same Row Adjacent Finger One Hand Out");
    same_row_adjacent_finger_onehand_out->weight = -INFINITY;
    same_row_adjacent_finger_onehand_out->length = 0;
    for (int i = 0; i < DIM3; i++)
    {
        unflat_tri(i, &row0, &col0, &row1, &col1, &row2, &col2);
        if (is_same_row_adjacent_finger_onehand_out(row0, col0, row1, col1, row2, col2))
        {
            same_row_adjacent_finger_onehand_out->ngrams[i] = i;
            same_row_adjacent_finger_onehand_out->length++;
        }
        else
        {
            same_row_adjacent_finger_onehand_out->ngrams[i] = -1;
        }
    }

    tri_stat *roll = (tri_stat *)malloc(sizeof(tri_stat));
    same_row_adjacent_finger_onehand_out->next = roll;
    strcpy(roll->name, "Roll");
    roll->weight = -INFINITY;
    roll->length = 0;
    for (int i = 0; i < DIM3; i++)
    {
        unflat_tri(i, &row0, &col0, &row1, &col1, &row2, &col2);
        if (is_roll(row0, col0, row1, col1, row2, col2))
        {
            roll->ngrams[i] = i;
            roll->length++;
        }
        else
        {
            roll->ngrams[i] = -1;
        }
    }

    tri_stat *roll_in = (tri_stat *)malloc(sizeof(tri_stat));
    roll->next = roll_in;
    strcpy(roll_in->name, "Roll In");
    roll_in->weight = -INFINITY;
    roll_in->length = 0;
    for (int i = 0; i < DIM3; i++)
    {
        unflat_tri(i, &row0, &col0, &row1, &col1, &row2, &col2);
        if (is_roll_in(row0, col0, row1, col1, row2, col2))
        {
            roll_in->ngrams[i] = i;
            roll_in->length++;
        }
        else
        {
            roll_in->ngrams[i] = -1;
        }
    }

    tri_stat *roll_out = (tri_stat *)malloc(sizeof(tri_stat));
    roll_in->next = roll_out;
    strcpy(roll_out->name, "Roll Out");
    roll_out->weight = -INFINITY;
    roll_out->length = 0;
    for (int i = 0; i < DIM3; i++)
    {
        unflat_tri(i, &row0, &col0, &row1, &col1, &row2, &col2);
        if (is_roll_out(row0, col0, row1, col1, row2, col2))
        {
            roll_out->ngrams[i] = i;
            roll_out->length++;
        }
        else
        {
            roll_out->ngrams[i] = -1;
        }
    }

    tri_stat *same_row_roll = (tri_stat *)malloc(sizeof(tri_stat));
    roll_out->next = same_row_roll;
    strcpy(same_row_roll->name, "Same Row Roll");
    same_row_roll->weight = -INFINITY;
    same_row_roll->length = 0;
    for (int i = 0; i < DIM3; i++)
    {
        unflat_tri(i, &row0, &col0, &row1, &col1, &row2, &col2);
        if (is_same_row_roll(row0, col0, row1, col1, row2, col2))
        {
            same_row_roll->ngrams[i] = i;
            same_row_roll->length++;
        }
        else
        {
            same_row_roll->ngrams[i] = -1;
        }
    }

    tri_stat *same_row_roll_in = (tri_stat *)malloc(sizeof(tri_stat));
    same_row_roll->next = same_row_roll_in;
    strcpy(same_row_roll_in->name, "Same Row Roll In");
    same_row_roll_in->weight = -INFINITY;
    same_row_roll_in->length = 0;
    for (int i = 0; i < DIM3; i++)
    {
        unflat_tri(i, &row0, &col0, &row1, &col1, &row2, &col2);
        if (is_same_row_roll_in(row0, col0, row1, col1, row2, col2))
        {
            same_row_roll_in->ngrams[i] = i;
            same_row_roll_in->length++;
        }
        else
        {
            same_row_roll_in->ngrams[i] = -1;
        }
    }

    tri_stat *same_row_roll_out = (tri_stat *)malloc(sizeof(tri_stat));
    same_row_roll_in->next = same_row_roll_out;
    strcpy(same_row_roll_out->name, "Same Row Roll Out");
    same_row_roll_out->weight = -INFINITY;
    same_row_roll_out->length = 0;
    for (int i = 0; i < DIM3; i++)
    {
        unflat_tri(i, &row0, &col0, &row1, &col1, &row2, &col2);
        if (is_same_row_roll_out(row0, col0, row1, col1, row2, col2))
        {
            same_row_roll_out->ngrams[i] = i;
            same_row_roll_out->length++;
        }
        else
        {
            same_row_roll_out->ngrams[i] = -1;
        }
    }

    tri_stat *adjacent_finger_roll = (tri_stat *)malloc(sizeof(tri_stat));
    same_row_roll_out->next = adjacent_finger_roll;
    strcpy(adjacent_finger_roll->name, "Adjacent Finger Roll");
    adjacent_finger_roll->weight = -INFINITY;
    adjacent_finger_roll->length = 0;
    for (int i = 0; i < DIM3; i++)
    {
        unflat_tri(i, &row0, &col0, &row1, &col1, &row2, &col2);
        if (is_adjacent_finger_roll(row0, col0, row1, col1, row2, col2))
        {
            adjacent_finger_roll->ngrams[i] = i;
            adjacent_finger_roll->length++;
        }
        else
        {
            adjacent_finger_roll->ngrams[i] = -1;
        }
    }

    tri_stat *adjacent_finger_roll_in = (tri_stat *)malloc(sizeof(tri_stat));
    adjacent_finger_roll->next = adjacent_finger_roll_in;
    strcpy(adjacent_finger_roll_in->name, "Adjacent Finger Roll In");
    adjacent_finger_roll_in->weight = -INFINITY;
    adjacent_finger_roll_in->length = 0;
    for (int i = 0; i < DIM3; i++)
    {
        unflat_tri(i, &row0, &col0, &row1, &col1, &row2, &col2);
        if (is_adjacent_finger_roll_in(row0, col0, row1, col1, row2, col2))
        {
            adjacent_finger_roll_in->ngrams[i] = i;
            adjacent_finger_roll_in->length++;
        }
        else
        {
            adjacent_finger_roll_in->ngrams[i] = -1;
        }
    }

    tri_stat *adjacent_finger_roll_out = (tri_stat *)malloc(sizeof(tri_stat));
    adjacent_finger_roll_in->next = adjacent_finger_roll_out;
    strcpy(adjacent_finger_roll_out->name, "Adjacent Finger Roll Out");
    adjacent_finger_roll_out->weight = -INFINITY;
    adjacent_finger_roll_out->length = 0;
    for (int i = 0; i < DIM3; i++)
    {
        unflat_tri(i, &row0, &col0, &row1, &col1, &row2, &col2);
        if (is_adjacent_finger_roll_out(row0, col0, row1, col1, row2, col2))
        {
            adjacent_finger_roll_out->ngrams[i] = i;
            adjacent_finger_roll_out->length++;
        }
        else
        {
            adjacent_finger_roll_out->ngrams[i] = -1;
        }
    }

    tri_stat *same_row_adjacent_finger_roll = (tri_stat *)malloc(sizeof(tri_stat));
    adjacent_finger_roll_out->next = same_row_adjacent_finger_roll;
    strcpy(same_row_adjacent_finger_roll->name, "Same Row Adjacent Finger Roll");
    same_row_adjacent_finger_roll->weight = -INFINITY;
    same_row_adjacent_finger_roll->length = 0;
    for (int i = 0; i < DIM3; i++)
    {
        unflat_tri(i, &row0, &col0, &row1, &col1, &row2, &col2);
        if (is_same_row_adjacent_finger_roll(row0, col0, row1, col1, row2, col2))
        {
            same_row_adjacent_finger_roll->ngrams[i] = i;
            same_row_adjacent_finger_roll->length++;
        }
        else
        {
            same_row_adjacent_finger_roll->ngrams[i] = -1;
        }
    }

    tri_stat *same_row_adjacent_finger_roll_in = (tri_stat *)malloc(sizeof(tri_stat));
    same_row_adjacent_finger_roll->next = same_row_adjacent_finger_roll_in;
    strcpy(same_row_adjacent_finger_roll_in->name, "Same Row Adjacent Finger Roll In");
    same_row_adjacent_finger_roll_in->weight = -INFINITY;
    same_row_adjacent_finger_roll_in->length = 0;
    for (int i = 0; i < DIM3; i++)
    {
        unflat_tri(i, &row0, &col0, &row1, &col1, &row2, &col2);
        if (is_same_row_adjacent_finger_roll_in(row0, col0, row1, col1, row2, col2))
        {
            same_row_adjacent_finger_roll_in->ngrams[i] = i;
            same_row_adjacent_finger_roll_in->length++;
        }
        else
        {
            same_row_adjacent_finger_roll_in->ngrams[i] = -1;
        }
    }

    tri_stat *same_row_adjacent_finger_roll_out = (tri_stat *)malloc(sizeof(tri_stat));
    same_row_adjacent_finger_roll_in->next = same_row_adjacent_finger_roll_out;
    strcpy(same_row_adjacent_finger_roll_out->name, "Same Row Adjacent Finger Roll Out");
    same_row_adjacent_finger_roll_out->weight = -INFINITY;
    same_row_adjacent_finger_roll_out->length = 0;
    for (int i = 0; i < DIM3; i++)
    {
        unflat_tri(i, &row0, &col0, &row1, &col1, &row2, &col2);
        if (is_same_row_adjacent_finger_roll_out(row0, col0, row1, col1, row2, col2))
        {
            same_row_adjacent_finger_roll_out->ngrams[i] = i;
            same_row_adjacent_finger_roll_out->length++;
        }
        else
        {
            same_row_adjacent_finger_roll_out->ngrams[i] = -1;
        }
    }

    same_row_adjacent_finger_roll_out->next = NULL;
}

/*
 * Trims the statistics in the linked list to move unused entries to the end.
 * This is done by iterating through each statistic's ngrams array and
 * reordering elements to place all valid entries at the beginning
 * of the array. This process ensures memory efficiency by eliminating
 * gaps in the array. It is somewhat analagous to a correctional officer
 * ordering prisoners to form an orderly line, ensuring no gaps or disorder.
 */
void trim_tri_stats()
{
    tri_stat *current = tri_head;

    while (current != NULL)
    {
        /* Copy valid ngram entries into earliest free index */
        if (current->length != 0)
        {
            int left = 0;
            int right = DIM3 - 1;

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
 * Cleans the trigram statistics linked list by removing statistics with zero length or weight.
 * This function ensures that only relevant statistics are kept for analysis,
 * similar to weeding out unnecessary files from a meticulously organized archive.
 * It updates the TRI_END global variable to reflect the new count of valid statistics.
 */
void clean_tri_stats()
{
    /* Remove statistics from the beginning of the list if they have zero length or weight */
    if (tri_head == NULL) {return;}
    while (tri_head != NULL && (tri_head->length == 0 || tri_head->weight == 0)) {
        tri_stat *temp = tri_head;
        tri_head = tri_head->next;
        free(temp);
    }

    /* Iterate through the rest of the list and remove any node with zero length or weight */
    tri_stat *current = tri_head;
    while (current != NULL && current->next != NULL) {
        if (current->next->length == 0 || current->next->weight == 0) {
            tri_stat *temp = current->next;
            current->next = current->next->next;
            free(temp);
        } else {
            current = current->next;
        }
    }

    /* Recount the number of valid statistics */
    current = tri_head;
    while (current != NULL) {
        TRI_END++;
        current = current->next;
    }
}

/*
 * Converts the linked list of trigram statistics to a contiguous array for easier access.
 * This function allocates memory for an array of tri_stat structures and copies
 * data from the linked list to the array. This is akin to transferring data from
 * individual files to a consolidated ledger for streamlined processing.
 */
void tri_to_array()
{
    stats_tri = (tri_stat *)malloc(sizeof(tri_stat) * TRI_END);
    tri_stat *current_tri = tri_head;
    for (int i = 0; i < TRI_END; i++) {
        memcpy(&stats_tri[i], current_tri, sizeof(tri_stat));
        /* Set next pointer to NULL */
        stats_tri[i].next = NULL;
        current_tri = current_tri->next;
    }
}

/*
 * Frees the memory allocated for the trigram statistics linked list.
 * This function iterates through the list, freeing each node's memory,
 * similar to the careful archiving of documents that are no longer needed.
 */
void free_tri_stats()
{
    if (tri_head == NULL) {return;}
    while (tri_head != NULL)
    {
        tri_stat *temp = tri_head;
        tri_head = tri_head->next;
        free(temp);
    }
    free(stats_tri);
}
