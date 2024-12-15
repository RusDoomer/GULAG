#include <string.h>
#include <stdlib.h>

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
    same_finger->weight = 0;
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
    while (bi_head != NULL && (bi_head->length == 0)) {
        bi_stat *temp = bi_head;
        bi_head = bi_head->next;
        free(temp);
    }

    bi_stat *current = bi_head;
    while (current != NULL && current->next != NULL) {
        if (current->next->length == 0) {
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
