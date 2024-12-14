#include <string.h>
#include <stdlib.h>

#include "mono.h"
#include "util.h"
#include "stats_util.h"
#include "global.h"

// IF YOU WANT TO ADD STATS HERE'S WHERE YOU DO IT
void initialize_mono_stats()
{
    int row0, col0;

    mono_stat *left_outer = (mono_stat *)malloc(sizeof(mono_stat));
    mono_head = left_outer;
    strcpy(left_outer->name, "Left Outer Usage");
    left_outer->weight = 0;
    left_outer->length = 0;
    for (int i = 0; i < DIM1; i++)
    {
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
    left_pinky->weight = 0;
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
    left_ring->weight = 0;
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
    left_middle->weight = 0;
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
    left_index->weight = 0;
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
    left_inner->weight = 0;
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
    right_inner->weight = 0;
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
    right_index->weight = 0;
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
    right_middle->weight = 0;
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
    right_ring->weight = 0;
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
    right_pinky->weight = 0;
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
    right_outer->weight = 0;
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

    right_outer->next = NULL;
}

void trim_mono_stats()
{
    mono_stat *current = mono_head;

    while (current != NULL)
    {
        // Copy valid ngram entries into earliest free index
        if (current->length != 0)
        {
            int left = 0;             // Index for the front of the array
            int right = DIM1 - 1;    // Index for the back of the array

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

void clean_mono_stats()
{
    if (mono_head == NULL) {return;}
    while (mono_head != NULL && (mono_head->weight == 0 || mono_head->length == 0)) {
        mono_stat *temp = mono_head;
        mono_head = mono_head->next;
        free(temp);
    }

    mono_stat *current = mono_head;
    while (current != NULL && current->next != NULL) {
        if (current->next->weight == 0 || current->next->length == 0) {
            mono_stat *temp = current->next;
            current->next = current->next->next;
            free(temp);
        } else {
            current = current->next;
        }
    }

    current = mono_head;
    while (current != NULL) {
        MONO_END++;
        current = current->next;
    }
}

void mono_to_array()
{
    stats_mono = (mono_stat *)malloc(sizeof(mono_stat) * MONO_END);
    mono_stat *current_mono = mono_head;
    for (int i = 0; i < MONO_END; i++) {
        memcpy(&stats_mono[i], current_mono, sizeof(mono_stat));
        stats_mono[i].next = NULL; // Set next pointer to NULL
        current_mono = current_mono->next;
    }
}

void free_mono_stats()
{
    if (mono_head == NULL) {return;}
    while (mono_head != NULL)
    {
        mono_stat *temp = mono_head;
        mono_head = mono_head->next;
        free(temp);
    }
}
