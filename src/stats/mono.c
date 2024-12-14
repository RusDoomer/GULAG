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

    stat *left_outer = (stat *)malloc(sizeof(stat) + DIM1 * sizeof(int));
    mono_head = left_outer;
    left_outer->type = 'm';
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

    stat *left_pinky = (stat *)malloc(sizeof(stat) + DIM1 * sizeof(int));
    left_outer->next = left_pinky;
    left_pinky->type = 'm';
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

    stat *left_ring = (stat *)malloc(sizeof(stat) + DIM1 * sizeof(int));
    left_pinky->next = left_ring;
    left_ring->type = 'm';
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

    stat *left_middle = (stat *)malloc(sizeof(stat) + DIM1 * sizeof(int));
    left_ring->next = left_middle;
    left_middle->type = 'm';
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

    stat *left_index = (stat *)malloc(sizeof(stat) + DIM1 * sizeof(int));
    left_middle->next = left_index;
    left_index->type = 'm';
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

    stat *left_inner = (stat *)malloc(sizeof(stat) + DIM1 * sizeof(int));
    left_index->next = left_inner;
    left_inner->type = 'm';
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


    stat *right_inner = (stat *)malloc(sizeof(stat) + DIM1 * sizeof(int));
    left_inner->next = right_inner;
    right_inner->type = 'm';
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

    stat *right_index = (stat *)malloc(sizeof(stat) + DIM1 * sizeof(int));
    right_inner->next = right_index;
    right_index->type = 'm';
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

    stat *right_middle = (stat *)malloc(sizeof(stat) + DIM1 * sizeof(int));
    right_index->next = right_middle;
    right_middle->type = 'm';
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

    stat *right_ring = (stat *)malloc(sizeof(stat) + DIM1 * sizeof(int));
    right_middle->next = right_ring;
    right_ring->type = 'm';
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

    stat *right_pinky = (stat *)malloc(sizeof(stat) + DIM1 * sizeof(int));
    right_ring->next = right_pinky;
    right_pinky->type = 'm';
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

    stat *right_outer = (stat *)malloc(sizeof(stat) + DIM1 * sizeof(int));
    right_pinky->next = right_outer;
    right_outer->type = 'm';
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
    stat *current = mono_head;
    stat *prev = NULL;

    while (current != NULL)
    {
        // Allocate memory for a new struct with the correct size
        stat *new_stat = (stat *)malloc(sizeof(stat) + current->length * sizeof(int));
        if (new_stat == NULL) {error("trim_mono_stats Memory allocation failed");}

        // Copy all relevant fields from current to new_stat
        strcpy(new_stat->name, current->name);
        new_stat->type = current->type;
        new_stat->length = current->length;
        new_stat->weight = current->weight;

        // Copy valid ngram entries into the new array
        if (current->length != 0)
        {
            int iter = 0;
            for (int i = 0; i < DIM1; i++)
            {
                if (current->ngrams[i] != -1)
                {
                    new_stat->ngrams[iter++] = current->ngrams[i];
                }
            }
        }

        // Free the old struct
        stat *next = current->next;
        free(current);

        // Update the pointer in the linked list to point to the new struct
        if (prev == NULL) {mono_head = new_stat;}
        else {prev->next = new_stat;}

        // Move to the next node
        current = next;
        prev = new_stat;
        if (prev != NULL)
        {
            prev->next = NULL;
        }
    }
}

void clean_mono_stats()
{
    if (mono_head == NULL) {return;}
    while (mono_head != NULL && (mono_head->weight == 0 || mono_head->length == 0)) {
        stat *temp = mono_head;
        mono_head = mono_head->next;
        free(temp);
    }

    stat *current = mono_head;
    while (current != NULL && current->next != NULL) {
        if (current->next->weight == 0 || current->next->length == 0) {
            stat *temp = current->next;
            current->next = current->next->next;
            free(temp);
        } else {
            current = current->next;
        }
    }
}

void free_mono_stats()
{
    if (mono_head == NULL) {return;}
    while (mono_head != NULL)
    {
        stat *temp = mono_head;
        mono_head = mono_head->next;
        free(temp);
    }
}
