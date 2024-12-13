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

    stat *left_hand = (stat *)malloc(sizeof(stat) + DIM1 * sizeof(int));
    mono_head = left_hand;
    left_hand->type = 'm';
    strcpy(left_hand->name, "Left Hand Usage");
    left_hand->weight = 0;
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

    stat *right_hand = (stat *)malloc(sizeof(stat) + DIM1 * sizeof(int));
    left_hand->next = right_hand;
    right_hand->type = 'm';
    strcpy(right_hand->name, "Right Hand Usage");
    right_hand->weight = 0;
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
            left_hand->ngrams[i] = -1;
        }
    }
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
        int iter = 0;
        for (int i = 0; i < DIM1; i++)
        {
            if (current->ngrams[i] != -1)
            {
                new_stat->ngrams[iter++] = current->ngrams[i];
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
