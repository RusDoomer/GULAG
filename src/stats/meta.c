/*
 * stats/meta.c - Meta statistic definitions for the GULAG project.
 *
 * Author: Rus Doomer
 *
 * Description: This file contains the implementation for initializing, cleaning,
 *              converting, and freeing meta statistics used in the GULAG.
 *              Meta statistics are calculated based on multiple types of
 *              ngram statistics, allowing for higher-level analysis of keyboard layouts.
 *
 * Adding new stats:
 *     1. In initialize_meta_stats(), add a new meta_stat structure.
 *     2. Define its name, and set its weight to -INFINITY (it will be changed later in io.c).
 *     3. Make sure to add it to the linked list by setting it as the next element of the last stat.
 *     4. Add the statistic to the weights files in data/weights/.
 *     5. In the meta_analysis() function, calculate and assign the statistic's value for a given layout.
 *
 * Note: Be cautious when adding new meta stats as each stat that it relies on
 *       will need to be set to a non-zero value for every weights file therefore
 *       increasing compute cost across the board.
 */

#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "meta.h"
#include "util.h"
#include "stats_util.h"
#include "global.h"


/*
 * Initializes the linked list of meta statistics.
 * Each meta statistic represents a composite metric derived from other statistics.
 * The function allocates memory for each stat and sets default values,
 * including a negative infinity weight which will be later updated.
 */
void initialize_meta_stats()
{
    /* Allocate and initialize hand balance. */
    meta_stat *hand_balance = (meta_stat *)malloc(sizeof(meta_stat));
    /* Ensure you set the head on the first stat of each type. */
    meta_head = hand_balance;
    strcpy(hand_balance->name, "Hand Balance");
    hand_balance->weight = -INFINITY;

    hand_balance->next = NULL;
}

/*
 * Trims the statistics in the linked list.
 * Currently, this function does not perform any operations for meta stats,
 * as there are no unused entries to trim.
 */
void trim_meta_stats()
{
    meta_stat *current = meta_head;

    while (current != NULL)
    {
        /* Do nothing */
        /* Move to the next node */
        current = current->next;
    }
}

/*
 * Cleans the meta statistics linked list by removing statistics with zero weight.
 * This function ensures that only relevant statistics are kept for analysis.
 * It updates the META_END global variable to reflect the new count of valid statistics.
 */
void clean_meta_stats()
{
    if (meta_head == NULL) {return;}
    /* Keep all stats */

    /* Iterate through the list. */
    meta_stat *current = meta_head;
    while (current != NULL && current->next != NULL) {
        /* do nothing */
        current = current->next;
    }

    /* Recount the number of valid statistics. */
    current = meta_head;
    while (current != NULL) {
        META_END++;
        current = current->next;
    }
}

/*
 * Converts the linked list of meta statistics to a contiguous array for easier access.
 * This function allocates memory for an array of meta_stat structures and copies
 * data from the linked list to the array, streamlining the data for analysis.
 */
void meta_to_array()
{
    stats_meta = (meta_stat *)malloc(sizeof(meta_stat) * META_END);
    meta_stat *current_meta = meta_head;
    for (int i = 0; i < META_END; i++) {
        memcpy(&stats_meta[i], current_meta, sizeof(meta_stat));
        /* Set next pointer to NULL. */
        stats_meta[i].next = NULL;
        current_meta = current_meta->next;
    }
}

/*
 * Frees the memory allocated for the meta statistics linked list.
 * This function iterates through the list, freeing each node's memory.
 */
void free_meta_stats()
{
    if (meta_head == NULL) {return;}
    while (meta_head != NULL)
    {
        meta_stat *temp = meta_head;
        meta_head = meta_head->next;
        free(temp);
    }
    free(stats_meta);
}

/*
 * Performs the meta-analysis on a given layout, calculating meta statistics.
 *
 * Parameters:
 *   lt: A pointer to the layout to analyze.
 *
 * Returns: void.
 */
void meta_analysis(layout *lt)
{
    /* Calculate hand balance. */
    lt->meta_score[0] = 0;
    /* stats_util.c - finds the score of a specific statistic in a given layout */
    lt->meta_score[0] += find_stat_score("Left Hand Usage", 'm', lt);
    lt->meta_score[0] -= find_stat_score("Right Hand Usage", 'm', lt);
    /* Take absolute value. */
    if (lt->meta_score[0] < 0) {lt->meta_score[0] *= -1;}
}
