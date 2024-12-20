#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "bi.h"
#include "util.h"
#include "stats_util.h"
#include "global.h"

void initialize_bi_stats()
{
    int row0, col0, row1, col1;
    bi_stat *same_finger = (bi_stat *)malloc(sizeof(bi_stat));
    bi_head = same_finger;
    strcpy(same_finger->name, "Same Finger Bigram");
    same_finger->weight = -INFINITY;
    same_finger->length = 0;
    for (int i = 0; i < DIM2; i++)
    {
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

    bi_stat *left_pinky = (bi_stat *)malloc(sizeof(bi_stat));
    same_finger->next = left_pinky;
    strcpy(left_pinky->name, "Left Pinky Bigram");
    left_pinky->weight = -INFINITY;
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

    bi_stat *left_ring = (bi_stat *)malloc(sizeof(bi_stat));
    left_pinky->next = left_ring;
    strcpy(left_ring->name, "Left Ring Bigram");
    left_ring->weight = -INFINITY;
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

    bi_stat *left_middle = (bi_stat *)malloc(sizeof(bi_stat));
    left_ring->next = left_middle;
    strcpy(left_middle->name, "Left Middle Bigram");
    left_middle->weight = -INFINITY;
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

    bi_stat *left_index = (bi_stat *)malloc(sizeof(bi_stat));
    left_middle->next = left_index;
    strcpy(left_index->name, "Left Index Bigram");
    left_index->weight = -INFINITY;
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

    bi_stat *right_index = (bi_stat *)malloc(sizeof(bi_stat));
    left_index->next = right_index;
    strcpy(right_index->name, "Right Index Bigram");
    right_index->weight = -INFINITY;
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

    bi_stat *right_middle = (bi_stat *)malloc(sizeof(bi_stat));
    right_index->next = right_middle;
    strcpy(right_middle->name, "Right Middle Bigram");
    right_middle->weight = -INFINITY;
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

    bi_stat *right_ring = (bi_stat *)malloc(sizeof(bi_stat));
    right_middle->next = right_ring;
    strcpy(right_ring->name, "Right Ring Bigram");
    right_ring->weight = -INFINITY;
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

    bi_stat *right_pinky = (bi_stat *)malloc(sizeof(bi_stat));
    right_ring->next = right_pinky;
    strcpy(right_pinky->name, "Right Pinky Bigram");
    right_pinky->weight = -INFINITY;
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

    bi_stat *bad_same_finger = (bi_stat *)malloc(sizeof(bi_stat));
    right_pinky->next = bad_same_finger;
    strcpy(bad_same_finger->name, "Bad Same Finger Bigram");
    bad_same_finger->weight = -INFINITY;
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

    bi_stat *bad_left_pinky = (bi_stat *)malloc(sizeof(bi_stat));
    bad_same_finger->next = bad_left_pinky;
    strcpy(bad_left_pinky->name, "Bad Left Pinky Bigram");
    bad_left_pinky->weight = -INFINITY;
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

    bi_stat *bad_left_ring = (bi_stat *)malloc(sizeof(bi_stat));
    bad_left_pinky->next = bad_left_ring;
    strcpy(bad_left_ring->name, "Bad Left Ring Bigram");
    bad_left_ring->weight = -INFINITY;
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

    bi_stat *bad_left_middle = (bi_stat *)malloc(sizeof(bi_stat));
    bad_left_ring->next = bad_left_middle;
    strcpy(bad_left_middle->name, "Bad Left Middle Bigram");
    bad_left_middle->weight = -INFINITY;
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

    bi_stat *bad_left_index = (bi_stat *)malloc(sizeof(bi_stat));
    bad_left_middle->next = bad_left_index;
    strcpy(bad_left_index->name, "Bad Left Index Bigram");
    bad_left_index->weight = -INFINITY;
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

    bi_stat *bad_right_index = (bi_stat *)malloc(sizeof(bi_stat));
    bad_left_index->next = bad_right_index;
    strcpy(bad_right_index->name, "Bad Right Index Bigram");
    bad_right_index->weight = -INFINITY;
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

    bi_stat *bad_right_middle = (bi_stat *)malloc(sizeof(bi_stat));
    bad_right_index->next = bad_right_middle;
    strcpy(bad_right_middle->name, "Bad Right Middle Bigram");
    bad_right_middle->weight = -INFINITY;
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

    bi_stat *bad_right_ring = (bi_stat *)malloc(sizeof(bi_stat));
    bad_right_middle->next = bad_right_ring;
    strcpy(bad_right_ring->name, "Bad Right Ring Bigram");
    bad_right_ring->weight = -INFINITY;
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

    bi_stat *bad_right_pinky = (bi_stat *)malloc(sizeof(bi_stat));
    bad_right_ring->next = bad_right_pinky;
    strcpy(bad_right_pinky->name, "Bad Right Pinky Bigram");
    bad_right_pinky->weight = -INFINITY;
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

    bi_stat *full_russor = (bi_stat *)malloc(sizeof(bi_stat));
    bad_right_pinky->next = full_russor;
    strcpy(full_russor->name, "Full Russor Bigram");
    full_russor->weight = -INFINITY;
    full_russor->length = 0;
    for (int i = 0; i < DIM2; i++)
    {
        unflat_bi(i, &row0, &col0, &row1, &col1);
        if (is_full_russor(row0, col0, row1, col1))
        {
            full_russor->ngrams[i] = i;
            full_russor->length++;
        }
        else
        {
            full_russor->ngrams[i] = -1;
        }
    }

    bi_stat *half_russor = (bi_stat *)malloc(sizeof(bi_stat));
    full_russor->next = half_russor;
    strcpy(half_russor->name, "Half Russor Bigram");
    half_russor->weight = -INFINITY;
    half_russor->length = 0;
    for (int i = 0; i < DIM2; i++)
    {
        unflat_bi(i, &row0, &col0, &row1, &col1);
        if (is_half_russor(row0, col0, row1, col1))
        {
            half_russor->ngrams[i] = i;
            half_russor->length++;
        }
        else
        {
            half_russor->ngrams[i] = -1;
        }
    }

    bi_stat *index_lsb = (bi_stat *)malloc(sizeof(bi_stat));
    half_russor->next = index_lsb;
    strcpy(index_lsb->name, "Index Stretch Bigram");
    index_lsb->weight = -INFINITY;
    index_lsb->length = 0;
    for (int i = 0; i < DIM2; i++)
    {
        unflat_bi(i, &row0, &col0, &row1, &col1);
        if (is_index_stretch_bi(row0, col0, row1, col1))
        {
            index_lsb->ngrams[i] = i;
            index_lsb->length++;
        }
        else
        {
            index_lsb->ngrams[i] = -1;
        }
    }

    bi_stat *pinky_lsb = (bi_stat *)malloc(sizeof(bi_stat));
    index_lsb->next = pinky_lsb;
    strcpy(pinky_lsb->name, "Pinky Stretch Bigram");
    pinky_lsb->weight = -INFINITY;
    pinky_lsb->length = 0;
    for (int i = 0; i < DIM2; i++)
    {
        unflat_bi(i, &row0, &col0, &row1, &col1);
        if (is_pinky_stretch_bi(row0, col0, row1, col1))
        {
            pinky_lsb->ngrams[i] = i;
            pinky_lsb->length++;
        }
        else
        {
            pinky_lsb->ngrams[i] = -1;
        }
    }

    pinky_lsb->next = NULL;
}

void trim_bi_stats()
{
    bi_stat *current = bi_head;

    while (current != NULL)
    {
        // Copy valid ngram entries into earliest free index
        if (current->length != 0)
        {
            int left = 0;             // Index for the front of the array
            int right = DIM2 - 1;    // Index for the back of the array

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

void clean_bi_stats()
{
    if (bi_head == NULL) {return;}
    while (bi_head != NULL && (bi_head->length == 0 || mono_head->weight == 0)) {
        bi_stat *temp = bi_head;
        bi_head = bi_head->next;
        free(temp);
    }

    bi_stat *current = bi_head;
    while (current != NULL && current->next != NULL) {
        if (current->next->length == 0 || current->next->weight == 0) {
            bi_stat *temp = current->next;
            current->next = current->next->next;
            free(temp);
        } else {
            current = current->next;
        }
    }

    current = bi_head;
    while (current != NULL) {
        BI_END++;
        current = current->next;
    }
}

void bi_to_array()
{
    stats_bi = (bi_stat *)malloc(sizeof(bi_stat) * BI_END);
    bi_stat *current_bi = bi_head;
    for (int i = 0; i < BI_END; i++) {
        memcpy(&stats_bi[i], current_bi, sizeof(bi_stat));
        stats_bi[i].next = NULL; // Set next pointer to NULL
        current_bi = current_bi->next;
    }
}

void free_bi_stats()
{
    if (bi_head == NULL) {return;}
    while (bi_head != NULL)
    {
        bi_stat *temp = bi_head;
        bi_head = bi_head->next;
        free(temp);
    }
}
