#include <string.h>
#include <stdlib.h>

#include "tri.h"
#include "util.h"
#include "stats_util.h"
#include "global.h"

void initialize_tri_stats()
{
    int row0, col0, row1, col1, row2, col2;
    tri_stat *same_finger = (tri_stat *)malloc(sizeof(tri_stat));
    tri_head = same_finger;
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
    tri_stat *current = tri_head;

    while (current != NULL)
    {
        // Copy valid ngram entries into earliest free index
        if (current->length != 0)
        {
            int left = 0;             // Index for the front of the array
            int right = DIM3 - 1;    // Index for the back of the array

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

void clean_tri_stats()
{
    if (tri_head == NULL) {return;}
    while (tri_head != NULL && (tri_head->length == 0)) {
        tri_stat *temp = tri_head;
        tri_head = tri_head->next;
        free(temp);
    }

    tri_stat *current = tri_head;
    while (current != NULL && current->next != NULL) {
        if (current->next->length == 0) {
            tri_stat *temp = current->next;
            current->next = current->next->next;
            free(temp);
        } else {
            current = current->next;
        }
    }

    current = tri_head;
    while (current != NULL) {
        TRI_END++;
        current = current->next;
    }
}

void tri_to_array()
{
    stats_tri = (tri_stat *)malloc(sizeof(tri_stat) * TRI_END);
    tri_stat *current_tri = tri_head;
    for (int i = 0; i < TRI_END; i++) {
        memcpy(&stats_tri[i], current_tri, sizeof(tri_stat));
        stats_tri[i].next = NULL; // Set next pointer to NULL
        current_tri = current_tri->next;
    }
}

void free_tri_stats()
{
    if (tri_head == NULL) {return;}
    while (tri_head != NULL)
    {
        tri_stat *temp = tri_head;
        tri_head = tri_head->next;
        free(temp);
    }
}
