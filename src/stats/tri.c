#include <string.h>
#include <stdlib.h>

#include "tri.h"
#include "util.h"
#include "stats_util.h"
#include "global.h"

void initialize_tri_stats()
{
    int row0, col0, row1, col1, row2, col2;
    stat *same_finger = (stat *)malloc(sizeof(stat) + DIM3 * sizeof(int));
    tri_head = same_finger;
    same_finger->type = 't';
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

    same_finger->next = NULL;
}

void trim_tri_stats()
{
    stat *current = tri_head;
    stat *prev = NULL;

    while (current != NULL)
    {
        // Allocate memory for a new struct with the correct size
        stat *new_stat = (stat *)malloc(sizeof(stat) + current->length * sizeof(int));
        if (new_stat == NULL) {error("trim_tri_stats Memory allocation failed");}

        // Copy all relevant fields from current to new_stat
        strcpy(new_stat->name, current->name);
        new_stat->type = current->type;
        new_stat->length = current->length;
        new_stat->weight = current->weight;

        // Copy valid ngram entries into the new array
        if (current->length != 0)
        {
            int iter = 0;
            for (int i = 0; i < DIM3; i++)
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
        if (prev == NULL) {tri_head = new_stat;}
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

void clean_tri_stats()
{
    if (tri_head == NULL) {return;}
    while (tri_head != NULL && (tri_head->weight == 0 || tri_head->length == 0)) {
        stat *temp = tri_head;
        tri_head = tri_head->next;
        free(temp);
    }

    stat *current = tri_head;
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

void free_tri_stats()
{
    if (tri_head == NULL) {return;}
    while (tri_head != NULL)
    {
        stat *temp = tri_head;
        tri_head = tri_head->next;
        free(temp);
    }
}
