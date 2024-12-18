#include <string.h>
#include <stdlib.h>

#include "tri.h"
#include "util.h"
#include "stats_util.h"
#include "global.h"

void initialize_tri_stats()
{
    int row0, col0, row1, col1, row2, col2;
    tri_stat *same_finger = (tri_stat *)malloc(sizeof(tri_stat));
    tri_head = same_finger;
    strcpy(same_finger->name, "Same Finger Trigram");
    same_finger->weight = 0;
    same_finger->length = 0;
    for (int i = 0; i < DIM3; i++)
    {
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

    tri_stat *alt = (tri_stat *)malloc(sizeof(tri_stat));
    same_finger->next = alt;
    strcpy(alt->name, "Alternation");
    alt->weight = 0;
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
    redirect->weight = 0;
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
    bad_redirect->weight = 0;
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
    onehand->weight = 0;
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
    onehand_in->weight = 0;
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
    onehand_out->weight = 0;
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
    same_row_onehand->weight = 0;
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
    same_row_onehand_in->weight = 0;
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
    same_row_onehand_out->weight = 0;
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
    adjacent_finger_onehand->weight = 0;
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
    adjacent_finger_onehand_in->weight = 0;
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
    adjacent_finger_onehand_out->weight = 0;
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
    same_row_adjacent_finger_onehand->weight = 0;
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
    same_row_adjacent_finger_onehand_in->weight = 0;
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
    same_row_adjacent_finger_onehand_out->weight = 0;
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
    roll->weight = 0;
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
    roll_in->weight = 0;
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
    roll_out->weight = 0;
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
    same_row_roll->weight = 0;
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
    same_row_roll_in->weight = 0;
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
    same_row_roll_out->weight = 0;
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
    adjacent_finger_roll->weight = 0;
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
    adjacent_finger_roll_in->weight = 0;
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
    adjacent_finger_roll_out->weight = 0;
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
    same_row_adjacent_finger_roll->weight = 0;
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
    same_row_adjacent_finger_roll_in->weight = 0;
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
    same_row_adjacent_finger_roll_out->weight = 0;
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

void trim_tri_stats()
{
    tri_stat *current = tri_head;

    while (current != NULL)
    {
        // Copy valid ngram entries into earliest free index
        if (current->length != 0)
        {
            int left = 0;             // Index for the front of the array
            int right = DIM3 - 1;    // Index for the back of the array

            // Use two pointers to efficiently partition the array
            while (left < right) {
                // Find the next -1 from the left
                while (left < right && current->ngrams[left] != -1) {
                    left++;
                }

                // Find the next non -1 from the right
                while (left < right && current->ngrams[right] == -1) {
                    right--;
                }

                // Swap the elements to move -1 to the back and non -1 to the front
                if (left < right) {
                    int temp = current->ngrams[left];
                    current->ngrams[left] = current->ngrams[right];
                    current->ngrams[right] = temp;
                    left++;
                    right--;
                }
            }
        }

        // Move to the next node
        current = current->next;
    }
}

void clean_tri_stats()
{
    if (tri_head == NULL) {return;}
    while (tri_head != NULL && (tri_head->length == 0)) {
        tri_stat *temp = tri_head;
        tri_head = tri_head->next;
        free(temp);
    }

    tri_stat *current = tri_head;
    while (current != NULL && current->next != NULL) {
        if (current->next->length == 0) {
            tri_stat *temp = current->next;
            current->next = current->next->next;
            free(temp);
        } else {
            current = current->next;
        }
    }

    current = tri_head;
    while (current != NULL) {
        TRI_END++;
        current = current->next;
    }
}

void tri_to_array()
{
    stats_tri = (tri_stat *)malloc(sizeof(tri_stat) * TRI_END);
    tri_stat *current_tri = tri_head;
    for (int i = 0; i < TRI_END; i++) {
        memcpy(&stats_tri[i], current_tri, sizeof(tri_stat));
        stats_tri[i].next = NULL; // Set next pointer to NULL
        current_tri = current_tri->next;
    }
}

void free_tri_stats()
{
    if (tri_head == NULL) {return;}
    while (tri_head != NULL)
    {
        tri_stat *temp = tri_head;
        tri_head = tri_head->next;
        free(temp);
    }
}
