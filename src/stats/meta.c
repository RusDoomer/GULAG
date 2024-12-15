#include <string.h>
#include <stdlib.h>

#include "meta.h"
#include "util.h"
#include "stats_util.h"
#include "global.h"

void initialize_meta_stats()
{
    meta_stat *left_hand = (meta_stat *)malloc(sizeof(meta_stat));
    meta_head = left_hand;
    strcpy(left_hand->name, "Left Hand Usage");
    left_hand->weight = 0;

    meta_stat *right_hand = (meta_stat *)malloc(sizeof(meta_stat));
    left_hand->next = right_hand;
    strcpy(right_hand->name, "Right Hand Usage");
    right_hand->weight = 0;

    meta_stat *hand_balance = (meta_stat *)malloc(sizeof(meta_stat));
    right_hand->next = hand_balance;
    strcpy(hand_balance->name, "Hand Balance");
    hand_balance->weight = 0;

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
    lt->meta_score[0] += find_stat_score("Left Pinky Usage", 'm', lt);
    lt->meta_score[0] += find_stat_score("Left Ring Usage", 'm', lt);
    lt->meta_score[0] += find_stat_score("Left Middle Usage", 'm', lt);
    lt->meta_score[0] += find_stat_score("Left Index Usage", 'm', lt);

    lt->meta_score[1] = 0;
    lt->meta_score[1] += find_stat_score("Right Index Usage", 'm', lt);
    lt->meta_score[1] += find_stat_score("Right Middle Usage", 'm', lt);
    lt->meta_score[1] += find_stat_score("Right Ring Usage", 'm', lt);
    lt->meta_score[1] += find_stat_score("Right Pinky Usage", 'm', lt);

    lt->meta_score[2] = 0;
    lt->meta_score[2] = lt->meta_score[0] - lt->meta_score[1];
    if (lt->meta_score[2] < 0) {lt->meta_score[2] *= -1;}
}
