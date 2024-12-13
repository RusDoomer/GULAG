#include <string.h>
#include <stdlib.h>

#include "skip.h"
#include "util.h"
#include "stats_util.h"
#include "global.h"

void initialize_skip_stats()
{
    int row0, col0, row1, col1;
    stat *same_finger = (stat *)malloc(sizeof(stat) + DIM2 * sizeof(int));
    skip_head = same_finger;
    same_finger->type = 'b';
    strcpy(same_finger->name, "Same Finger Skipgram");
    same_finger->weight = 0;
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

    same_finger->next = NULL;
}

void trim_skip_stats()
{
    stat *current = skip_head;
    stat *prev = NULL;

    while (current != NULL)
    {
        // Allocate memory for a new struct with the correct size
        stat *new_stat = (stat *)malloc(sizeof(stat) + current->length * sizeof(int));
        if (new_stat == NULL) {error("trim_skip_stats Memory allocation failed");}

        // Copy all relevant fields from current to new_stat
        strcpy(new_stat->name, current->name);
        new_stat->type = current->type;
        new_stat->length = current->length;
        new_stat->weight = current->weight;

        // Copy valid ngram entries into the new array
        if (current->length != 0)
        {
            int iter = 0;
            for (int i = 0; i < DIM2; i++)
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
        if (prev == NULL) {skip_head = new_stat;}
        else {prev->next = new_stat;}

        // Move to the next node
        current = next;
        prev = new_stat;
    }
}

void clean_skip_stats()
{
    if (skip_head == NULL) {return;}
    while (skip_head != NULL && (skip_head->weight == 0 || skip_head->length == 0)) {
        stat *temp = skip_head;
        skip_head = skip_head->next;
        free(temp);
    }

    stat *current = skip_head;
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

void free_skip_stats()
{
    if (skip_head == NULL) {return;}
    while (skip_head != NULL)
    {
        stat *temp = skip_head;
        skip_head = skip_head->next;
        free(temp);
    }
}
