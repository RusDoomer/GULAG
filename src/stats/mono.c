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

    stat *left_hand = (stat *)malloc(sizeof(stat));
    mono_head = left_hand;
    left_hand->type = 'm';
    strcpy(left_hand->name, "Left Hand Usage");
    left_hand->weight = 0;
    left_hand->ngrams = (int*)calloc(DIM1, sizeof(int));
    for (int i = 0; i < DIM1; i++)
    {
        unflat_mono(i, &row0, &col0);
        if (hand(row0, col0) == 'l')
        {
            left_hand->ngrams[i] = i;
            left_hand->length++;
        }
    }

    stat *right_hand = (stat *)malloc(sizeof(stat));
    left_hand->next = right_hand;
    right_hand->type = 'm';
    strcpy(right_hand->name, "Right Hand Usage");
    right_hand->weight = 0;
    right_hand->ngrams = (int*)calloc(DIM1, sizeof(int));
    for (int i = 0; i < DIM1; i++)
    {
        unflat_mono(i, &row0, &col0);
        if (hand(row0, col0) == 'r')
        {
            right_hand->ngrams[i] = i;
            right_hand->length++;
        }
    }
}

void trim_mono_stats()
{
    stat *current = mono_head;
    while (current != NULL)
    {
        int *new_ngram = (int*)calloc(current->length, sizeof(int));
        int iter = 0;
        for (int i = 0; i < DIM1; i++)
        {
            if(current->ngrams[i] != 0)
            {
                new_ngram[iter] = current->ngrams[i];
                iter++;
            }
        }
        free(current->ngrams);
        current->ngrams = new_ngram;
        current = current->next;
    }
}

void clean_mono_stats()
{
    if (mono_head == NULL) {return;}
    while (mono_head != NULL && (mono_head->weight == 0 || mono_head->length == 0)) {
        stat *temp = mono_head;
        mono_head = mono_head->next;
        free(temp->ngrams);
        free(temp);
    }

    stat *current = mono_head;
    while (current != NULL && current->next != NULL) {
        if (current->next->weight == 0 || current->next->length == 0) {
            stat *temp = current->next;
            current->next = current->next->next;
            free(temp->ngrams);
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
        free(temp->ngrams);
        free(temp);
    }
}
