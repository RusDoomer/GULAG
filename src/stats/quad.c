#include <string.h>
#include <stdlib.h>

#include "quad.h"
#include "util.h"
#include "stats_util.h"
#include "global.h"

void initialize_quad_stats()
{
    int row0, col0, row1, col1, row2, col2, row3, col3;
    stat *same_finger = (stat *)malloc(sizeof(stat) + DIM4 * sizeof(int));
    quad_head = same_finger;
    same_finger->type = 'q';
    strcpy(same_finger->name, "Same Finger Quadgram");
    same_finger->weight = 0;
    same_finger->length = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_same_finger_quad(row0, col0, row1, col1, row2, col2, row3, col3))
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

void trim_quad_stats()
{
    stat *current = quad_head;
    stat *prev = NULL;

    while (current != NULL)
    {
        // Allocate memory for a new struct with the correct size
        stat *new_stat = (stat *)malloc(sizeof(stat) + current->length * sizeof(int));
        if (new_stat == NULL) {error("trim_quad_stats Memory allocation failed");}

        // Copy all relevant fields from current to new_stat
        strcpy(new_stat->name, current->name);
        new_stat->type = current->type;
        new_stat->length = current->length;
        new_stat->weight = current->weight;

        // Copy valid ngram entries into the new array
        if (current->length != 0)
        {
            int iter = 0;
            for (int i = 0; i < DIM4; i++)
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
        if (prev == NULL) {quad_head = new_stat;}
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

void clean_quad_stats()
{
    if (quad_head == NULL) {return;}
    while (quad_head != NULL && (quad_head->weight == 0 || quad_head->length == 0)) {
        stat *temp = quad_head;
        quad_head = quad_head->next;
        free(temp);
    }

    stat *current = quad_head;
    while (current != NULL && current->next != NULL) {
        if (current->next->weight == 0 || current->next->length == 0) {
            stat *temp = current->next;
            current->next = current->next->next;
            free(temp);
        } else {
            current = current->next;
        }
    }

    current = quad_head;
    while (current != NULL) {
        QUAD_END++;
        current = current->next;
    }
}

void free_quad_stats()
{
    if (quad_head == NULL) {return;}
    while (quad_head != NULL)
    {
        stat *temp = quad_head;
        quad_head = quad_head->next;
        free(temp);
    }
}
