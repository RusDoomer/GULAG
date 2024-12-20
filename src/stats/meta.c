#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "meta.h"
#include "util.h"
#include "stats_util.h"
#include "global.h"

void initialize_meta_stats()
{
    meta_stat *hand_balance = (meta_stat *)malloc(sizeof(meta_stat));
    meta_head = hand_balance;
    strcpy(hand_balance->name, "Hand Balance");
    hand_balance->weight = -INFINITY;

    hand_balance->next = NULL;
}
void trim_meta_stats()
{
    meta_stat *current = meta_head;

    while (current != NULL)
    {
        // Do nothing
        // Move to the next node
        current = current->next;
    }
}

void clean_meta_stats()
{
    if (meta_head == NULL) {return;}
    // Keep all stats

    meta_stat *current = meta_head;
    while (current != NULL && current->next != NULL) {
        // Move to the next node
        current = current->next;
    }

    current = meta_head;
    while (current != NULL) {
        META_END++;
        current = current->next;
    }
}

void meta_to_array()
{
    stats_meta = (meta_stat *)malloc(sizeof(meta_stat) * META_END);
    meta_stat *current_meta = meta_head;
    for (int i = 0; i < META_END; i++) {
        memcpy(&stats_meta[i], current_meta, sizeof(meta_stat));
        stats_meta[i].next = NULL; // Set next pointer to NULL
        current_meta = current_meta->next;
    }
}

void free_meta_stats()
{
    if (meta_head == NULL) {return;}
    while (meta_head != NULL)
    {
        meta_stat *temp = meta_head;
        meta_head = meta_head->next;
        free(temp);
    }
}

void meta_analysis(layout *lt)
{
    lt->meta_score[0] = 0;
    lt->meta_score[0] += find_stat_score("Left Hand Usage", 'm', lt);
    lt->meta_score[0] -= find_stat_score("Right Hand Usage", 'm', lt);
    if (lt->meta_score[0] < 0) {lt->meta_score[0] *= -1;}
}
