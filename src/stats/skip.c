#include <string.h>
#include <stdlib.h>

#include "skip.h"
#include "util.h"
#include "stats_util.h"
#include "global.h"

void initialize_skip_stats()
{
    int row0, col0, row1, col1;
    skip_stat *same_finger = (skip_stat *)malloc(sizeof(skip_stat));
    skip_head = same_finger;
    strcpy(same_finger->name, "Same Finger Skipgram");
    for (int i = 0; i < 10; i++) {
        same_finger->weight[i] = 0;
    }
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

    same_finger->next = NULL;
}

void trim_skip_stats()
{
    skip_stat *current = skip_head;

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

void clean_skip_stats()
{
    if (skip_head == NULL) {return;}
    while (skip_head != NULL && (skip_head->length == 0)) {
        skip_stat *temp = skip_head;
        skip_head = skip_head->next;
        free(temp);
    }

    skip_stat *current = skip_head;
    while (current != NULL && current->next != NULL) {
        if (current->next->length == 0) {
            skip_stat *temp = current->next;
            current->next = current->next->next;
            free(temp);
        } else {
            current = current->next;
        }
    }

    current = skip_head;
    while (current != NULL) {
        SKIP_END++;
        current = current->next;
    }
}

void skip_to_array()
{
    stats_skip = (skip_stat *)malloc(sizeof(skip_stat) * SKIP_END);
    skip_stat *current_skip = skip_head;
    for (int i = 0; i < SKIP_END; i++) {
        memcpy(&stats_skip[i], current_skip, sizeof(skip_stat));
        stats_skip[i].next = NULL; // Set next pointer to NULL
        current_skip = current_skip->next;
    }
}

void free_skip_stats()
{
    if (skip_head == NULL) {return;}
    while (skip_head != NULL)
    {
        skip_stat *temp = skip_head;
        skip_head = skip_head->next;
        free(temp);
    }
}
