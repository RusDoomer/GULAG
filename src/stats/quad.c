#include <string.h>
#include <stdlib.h>

#include "quad.h"
#include "util.h"
#include "stats_util.h"
#include "global.h"

void initialize_quad_stats()
{
    int row0, col0, row1, col1, row2, col2, row3, col3;
    quad_stat *same_finger = (quad_stat *)malloc(sizeof(quad_stat));
    quad_head = same_finger;
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
    quad_stat *current = quad_head;

    while (current != NULL)
    {
        // Copy valid ngram entries into earliest free index
        if (current->length != 0)
        {
            int left = 0;             // Index for the front of the array
            int right = DIM4 - 1;    // Index for the back of the array

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

void clean_quad_stats()
{
    if (quad_head == NULL) {return;}
    while (quad_head != NULL && (quad_head->weight == 0 || quad_head->length == 0)) {
        quad_stat *temp = quad_head;
        quad_head = quad_head->next;
        free(temp);
    }

    quad_stat *current = quad_head;
    while (current != NULL && current->next != NULL) {
        if (current->next->weight == 0 || current->next->length == 0) {
            quad_stat *temp = current->next;
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

void quad_to_array()
{
    error("quad to array not implemented");
}

void free_quad_stats()
{
    if (quad_head == NULL) {return;}
    while (quad_head != NULL)
    {
        quad_stat *temp = quad_head;
        quad_head = quad_head->next;
        free(temp);
    }
}
