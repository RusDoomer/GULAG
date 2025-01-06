/*
 * stats_util.c - Utility functions for layout statistics.
 *
 * Author: Rus Doomer
 *
 * Description: This file contains utility functions for determining what
 *              stats each key sequences (n-grams) falls under, as well as
 *              other miscellaneous helpers.
 */

#include <string.h>

#include "stats_util.h"
#include "global.h"
#include "structs.h"
#include "util.h"

/*
 * Inserts a new monogram statistic into the monogram linked list
 */
void add_mono_stat(mono_stat *stat)
{
    stat->next = NULL;
    if (mono_head == NULL) {mono_head = stat;}
    else
    {
        mono_stat *ptr = mono_head;
        while (ptr->next != NULL) {ptr = ptr->next;}
        ptr->next = stat;
    }
}

/*
 * Inserts a new bigram statistic into the bigram linked list
 */
void add_bi_stat(bi_stat *stat)
{
    stat->next = NULL;
    if (bi_head == NULL) {bi_head = stat;}
    else
    {
        bi_stat *ptr = bi_head;
        while (ptr->next != NULL) {ptr = ptr->next;}
        ptr->next = stat;
    }
}

/*
 * Inserts a new trigram statistic into the trigram linked list
 */
void add_tri_stat(tri_stat *stat)
{
    stat->next = NULL;
    if (tri_head == NULL) {tri_head = stat;}
    else
    {
        tri_stat *ptr = tri_head;
        while (ptr->next != NULL) {ptr = ptr->next;}
        ptr->next = stat;
    }
}

/*
 * Inserts a new quadgram statistic into the quadgram linked list
 */
void add_quad_stat(quad_stat *stat)
{
    stat->next = NULL;
    if (quad_head == NULL) {quad_head = stat;}
    else
    {
        quad_stat *ptr = quad_head;
        while (ptr->next != NULL) {ptr = ptr->next;}
        ptr->next = stat;
    }
}

/*
 * Inserts a new skipgram statistic into the skipgram linked list
 */
void add_skip_stat(skip_stat *stat)
{
    stat->next = NULL;
    if (skip_head == NULL) {skip_head = stat;}
    else
    {
        skip_stat *ptr = skip_head;
        while (ptr->next != NULL) {ptr = ptr->next;}
        ptr->next = stat;
    }
}

/*
 * Inserts a new meta statistic into the meta linked list
 */
void add_meta_stat(meta_stat *stat)
{
    stat->next = NULL;
    if (meta_head == NULL) {meta_head = stat;}
    else
    {
        meta_stat *ptr = meta_head;
        while (ptr->next != NULL) {ptr = ptr->next;}
        ptr->next = stat;
    }
}


/*
 * Finds the index of a specific statistic in a given layout.
 * The function searches for the statistic by name and type within the layout's
 * statistical data.
 *
 * Parameters:
 *   stat_name: The name of the statistic to find.
 *   type: The type of the statistic ('m' for monogram, 'b' for bigram, etc.).
 *   lt: A pointer to the layout structure.
 *
 * Returns:
 *   The index of the found statistic. Returns -1 if the statistic is not found.
 */
int find_stat_index(char *stat_name, char type, layout *lt) {
    int i;

    switch (type) {
        case 'm':
            for (i = 0; i < MONO_END; i++) {
                if (strcmp(stats_mono[i].name, stat_name) == 0) {
                    return i;
                }
            }
            break;
        case 'b':
            for (i = 0; i < BI_END; i++) {
                if (strcmp(stats_bi[i].name, stat_name) == 0) {
                    return i;
                }
            }
            break;
        case 't':
            for (i = 0; i < TRI_END; i++) {
                if (strcmp(stats_tri[i].name, stat_name) == 0) {
                    return i;
                }
            }
            break;
        case 'q':
            for (i = 0; i < QUAD_END; i++) {
                if (strcmp(stats_quad[i].name, stat_name) == 0) {
                    return i;
                }
            }
            break;
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            for (i = 0; i < SKIP_END; i++) {
                if (strcmp(stats_skip[i].name, stat_name) == 0) {
                    return i;
                }
            }
            break;
        /* Meta stats */
        case 'e':
            for (i = 0; i < META_END; i++) {
                if (strcmp(stats_meta[i].name, stat_name) == 0) {
                    return i;
                }
            }
            break;
        default:
            error("Invalid type specified in find_stat_score");
    }

    /* If stat not found, return -1 to let user know this meta stat is unusable */
    return -1;
}

/* 'l' for left hand, 'r' for right hand. */
char hand(int row0, int col0)
{
    if (col0 < COL/2) {return 'l';}
    else {return 'r';}
}

/* An integer representing the finger used (0-7). */
int finger(int row0, int col0)
{
    switch(col0)
    {
        default:
        case 0:
        case 1:
            return 0;
        case 2:
            return 1;
        case 3:
            return 2;
        case 4:
        case 5:
            return 3;
        case 6:
        case 7:
            return 4;
        case 8:
            return 5;
        case 9:
            return 6;
        case 10:
        case 11:
            return 7;
    }
}

/* pinky and index stretch */
int is_stretch(int row0, int col0)
{
    return col0 == 0 || col0 == 5 || col0 == 6 || col0 == 11;
}

int is_same_hand_bi(int row0, int col0, int row1, int col1)
{
    return hand(row0, col0) == hand(row1, col1);
}

int is_same_hand_tri(int row0, int col0, int row1, int col1, int row2, int col2)
{
    return hand(row0, col0) == hand(row1, col1) && hand(row1, col1) == hand(row2, col2);
}

int is_same_hand_quad(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3)
{
    return hand(row0, col0) == hand(row1, col1)
        && hand(row1, col1) == hand(row2, col2)
        && hand(row2, col2) == hand(row3, col3);
}

int is_same_col_bi(int row0, int col0, int row1, int col1)
{
    return col0 == col1;
}

int is_same_col_tri(int row0, int col0, int row1, int col1, int row2, int col2)
{
    return col0 == col1 && col1 == col2;
}

int is_same_col_quad(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3)
{
    return col0 == col1 && col1 == col2 && col2 == col3;
}

/* literal same row */
int is_same_row_bi(int row0, int col0, int row1, int col1)
{
    return row0 == row1;
}

int is_same_row_tri(int row0, int col0, int row1, int col1, int row2, int col2)
{
    return row0 == row1 && row1 == row2;
}

int is_same_row_quad(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3)
{
    return row0 == row1 && row1 == row2 && row2 == row3;
}

/* same row without stretch columns for stats */
int is_same_row_mod_bi(int row0, int col0, int row1, int col1)
{
    return row0 == row1
        && !is_stretch(row0, col0)
        && !is_stretch(row1, col1);
}

int is_same_row_mod_tri(int row0, int col0, int row1, int col1, int row2, int col2)
{
    return row0 == row1 && row1 == row2
        && !is_stretch(row0, col0)
        && !is_stretch(row1, col1)
        && !is_stretch(row2, col2);
}

int is_same_row_mod_quad(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3)
{
    return row0 == row1 && row1 == row2 && row2 == row3
        && !is_stretch(row0, col0)
        && !is_stretch(row1, col1)
        && !is_stretch(row2, col2)
        && !is_stretch(row3, col3);
}

/* doesn't include stretch columns */
int is_adjacent_finger_bi(int row0, int col0, int row1, int col1)
{
    return !is_stretch(row0, col0) && !is_stretch(row1, col1)
        && (finger(row0, col0) - finger(row1, col1) == 1
            || finger(row0, col0) - finger(row1, col1) == -1);
}

int is_adjacent_finger_tri(int row0, int col0, int row1, int col1, int row2, int col2)
{
    return !is_stretch(row0, col0) && !is_stretch(row1, col1) && !is_stretch(row2, col2)
        && (finger(row0, col0) - finger(row1, col1) == 1
            || finger(row0, col0) - finger(row1, col1) == -1)
        && (finger(row1, col1) - finger(row2, col2) == 1
            || finger(row1, col1) - finger(row2, col2) == -1)
        && (finger(row0, col0) != finger(row1, col1));
}

int is_adjacent_finger_quad(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3)
{
    return !is_stretch(row0, col0) && !is_stretch(row1, col1)
        && !is_stretch(row2, col2) && !is_stretch(row3, col3)
        && (finger(row0, col0) - finger(row1, col1) == 1
            || finger(row0, col0) - finger(row1, col1) == -1)
        && (finger(row1, col1) - finger(row2, col2) == 1
            || finger(row1, col1) - finger(row2, col2) == -1)
        && (finger(row2, col2) - finger(row3, col3) == 1
            || finger(row2, col2) - finger(row3, col3) == -1)
        && (finger(row0, col0) != finger(row1, col1))
        && (finger(row1, col1) != finger(row2, col2))
        && (finger(row0, col0) != finger(row2, col2));
}

int is_same_pos_bi(int row0, int col0, int row1, int col1)
{
    return is_same_col_bi(row0, col0, row1, col1)
        && is_same_row_bi(row0, col0, row1, col1);
}

int is_same_pos_tri(int row0, int col0, int row1, int col1, int row2, int col2)
{
    return is_same_col_tri(row0, col0, row1, col1, row2, col2)
        && is_same_row_tri(row0, col0, row1, col1, row2, col2);
}

int is_same_pos_quad(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3)
{
    return is_same_col_quad(row0, col0, row1, col1, row2, col2, row3, col3)
        && is_same_row_quad(row0, col0, row1, col1, row2, col2, row3, col3);
}

int row_diff(int row0, int col0, int row1, int col1)
{
    if (row0 - row1 < 0) {return row1 - row0;}
    else {return row0 - row1;}
}

/* doesn't include repeats for stats */
int is_same_finger_bi(int row0, int col0, int row1, int col1)
{
    return (finger(row0, col0) == finger(row1, col1))
        && !is_same_pos_bi(row0, col0, row1, col1);
}

int is_same_finger_tri(int row0, int col0, int row1, int col1, int row2, int col2)
{
    return finger(row0, col0) == finger(row1, col1)
        && finger(row1, col1) == finger(row2, col2)
        && !is_same_pos_bi(row0, col0, row1, col1)
        && !is_same_pos_bi(row1, col1, row2, col2);
}

int is_same_finger_quad(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3)
{
    return finger(row0, col0) == finger(row1, col1)
        && finger(row1, col1) == finger(row2, col2)
        && finger(row2, col2) == finger(row3, col3)
        && !is_same_pos_bi(row0, col0, row1, col1)
        && !is_same_pos_bi(row1, col1, row2, col2)
        && !is_same_pos_bi(row2, col2, row3, col3);
}

/* 2u sfb */
int is_bad_same_finger_bi(int row0, int col0, int row1, int col1)
{
    return is_same_finger_bi(row0, col0, row1, col1) && (row0 - row1 == 2 || row1 - row0 == 2);
}

/* separate fingers but not index + pinky combo */
int is_russor_fingers(int row0, int col0, int row1, int col1)
{
    return !is_same_finger_bi(row0, col0, row1, col1) && !is_same_pos_bi(row0, col0, row1, col1)
        && is_same_hand_bi(row0, col0, row1, col1)
        && !(finger(row0, col0) == 0 && finger(row1, col1) == 3)
        && !(finger(row0, col0) == 3 && finger(row1, col1) == 0)
        && !(finger(row0, col0) == 4 && finger(row1, col1) == 7)
        && !(finger(row0, col0) == 7 && finger(row1, col1) == 4);
}

/* 2u is full */
int is_full_russor(int row0, int col0, int row1, int col1)
{
    return row_diff(row0, col0, row1, col1) == 2
        && is_russor_fingers(row0, col0, row1, col1);
}

/* 1u is half */
int is_half_russor(int row0, int col0, int row1, int col1)
{
    return row_diff(row0, col0, row1, col1) == 1
        && is_russor_fingers(row0, col0, row1, col1);
}

int is_index_stretch_bi(int row0, int col0, int row1, int col1)
{
    return (finger(row0, col0) == 2 && col1 == 5)
        || (finger(row1, col1) == 2 && col0 == 5)
        || (finger(row0, col0) == 5 && col1 == 6)
        || (finger(row1, col1) == 5 && col0 == 6);
}

int is_pinky_stretch_bi(int row0, int col0, int row1, int col1)
{
    return (finger(row0, col0) == 1 && col1 == 0)
        || (finger(row1, col1) == 1 && col0 == 0)
        || (finger(row0, col0) == 6 && col1 == 11)
        || (finger(row1, col1) == 6 && col0 == 11);
}

/*                                                   */
/* not explaining all of these this is too much work */
/*                                                   */

int is_redirect(int row0, int col0, int row1, int col1, int row2, int col2)
{
    return is_same_hand_tri(row0, col0, row1, col1, row2, col2)
        && !is_same_finger_bi(row0, col0, row2, col2)
        && !is_same_pos_bi(row0, col0, row2, col2)
        && (
            (finger(row0, col0) < finger(row1, col1)
            && finger(row1, col1) > finger(row2, col2))
           ||
            (finger(row0, col0) > finger(row1, col1)
            && finger(row1, col1) < finger(row2, col2))
           );
}

int is_bad_redirect(int row0, int col0, int row1, int col1, int row2, int col2)
{
    return is_redirect(row0, col0, row1, col1, row2, col2)
        && finger(row0, col0) != 3 && finger(row0, col0) != 4
        && finger(row1, col1) != 3 && finger(row1, col1) != 4
        && finger(row2, col2) != 3 && finger(row2, col2) != 4;
}

int is_alt(int row0, int col0, int row1, int col1, int row2, int col2)
{
    return !is_same_hand_bi(row0, col0, row1, col1)
        && !is_same_hand_bi(row1, col1, row2, col2)
        && !is_same_finger_bi(row0, col0, row2, col2)
        && !is_same_pos_bi(row0, col0, row2, col2);
}

int is_alt_in(int row0, int col0, int row1, int col1, int row2, int col2)
{
    return is_alt(row0, col0, row1, col1, row2, col2)
        && is_roll_in(row0, col0, row2, col2, row1, col1);
}

int is_alt_out(int row0, int col0, int row1, int col1, int row2, int col2)
{
    return is_alt(row0, col0, row1, col1, row2, col2)
        && !is_alt_in(row0, col0, row1, col1, row2, col2);
}


int is_same_row_alt(int row0, int col0, int row1, int col1, int row2, int col2)
{
    return is_alt(row0, col0, row1, col1, row2, col2)
        && is_same_row_bi(row0, col0, row2, col2);
}

int is_same_row_alt_in(int row0, int col0, int row1, int col1, int row2, int col2)
{
    return is_same_row_alt(row0, col0, row1, col1, row2, col2)
        && is_roll_in(row0, col0, row2, col2, row1, col1);
}

int is_same_row_alt_out(int row0, int col0, int row1, int col1, int row2, int col2)
{
    return is_same_row_alt(row0, col0, row1, col1, row2, col2)
        && !is_same_row_alt_in(row0, col0, row1, col1, row2, col2);
}


int is_adjacent_finger_alt(int row0, int col0, int row1, int col1, int row2, int col2)
{
    return is_alt(row0, col0, row1, col1, row2, col2)
        && is_adjacent_finger_bi(row0, col0, row2, col2);
}

int is_adjacent_finger_alt_in(int row0, int col0, int row1, int col1, int row2, int col2)
{
    return is_adjacent_finger_alt(row0, col0, row1, col1, row2, col2)
        && is_roll_in(row0, col0, row2, col2, row1, col1);
}

int is_adjacent_finger_alt_out(int row0, int col0, int row1, int col1, int row2, int col2)
{
    return is_adjacent_finger_alt(row0, col0, row1, col1, row2, col2)
        && !is_adjacent_finger_alt_in(row0, col0, row1, col1, row2, col2);
}


int is_same_row_adjacent_finger_alt(int row0, int col0, int row1, int col1, int row2, int col2)
{
    return is_alt(row0, col0, row1, col1, row2, col2)
        && is_same_row_bi(row0, col0, row2, col2)
        && is_adjacent_finger_bi(row0, col0, row2, col2);
}

int is_same_row_adjacent_finger_alt_in(int row0, int col0, int row1, int col1, int row2, int col2)
{
    return is_same_row_adjacent_finger_alt(row0, col0, row1, col1, row2, col2)
        && is_roll_in(row0, col0, row2, col2, row1, col1);
}

int is_same_row_adjacent_finger_alt_out(int row0, int col0, int row1, int col1, int row2, int col2)
{
    return is_same_row_adjacent_finger_alt(row0, col0, row1, col1, row2, col2)
        && !is_same_row_adjacent_finger_alt_in(row0, col0, row1, col1, row2, col2);
}


int is_onehand(int row0, int col0, int row1, int col1, int row2, int col2)
{
    return is_same_hand_tri(row0, col0, row1, col1, row2, col2)
        && (
             (finger(row0, col0) < finger(row1, col1)
             && finger(row1, col1) < finger(row2, col2))
            ||
             (finger(row0, col0) > finger(row1, col1)
             && finger(row1, col1) > finger(row2, col2))
           );
}

int is_onehand_in(int row0, int col0, int row1, int col1, int row2, int col2)
{
    return is_onehand(row0, col0, row1, col1, row2, col2)
        && (
             (hand(row0, col0) == 'l'
              && finger(row0, col0) < finger(row1, col1)
              && finger(row1, col1) < finger(row2, col2)
             )
            ||
             (hand(row0, col0) == 'r'
              && finger(row0, col0) > finger(row1, col1)
              && finger(row1, col1) > finger(row2, col2)
             )
           );
}

int is_onehand_out(int row0, int col0, int row1, int col1, int row2, int col2)
{
    return is_onehand(row0, col0, row1, col1, row2, col2)
        && !is_onehand_in(row0, col0, row1, col1, row2, col2);
}

int is_same_row_onehand(int row0, int col0, int row1, int col1, int row2, int col2)
{
    return is_onehand(row0, col0, row1, col1, row2, col2)
        && is_same_row_mod_tri(row0, col0, row1, col1, row2, col2);
}

int is_same_row_onehand_in(int row0, int col0, int row1, int col1, int row2, int col2)
{
    return is_onehand_in(row0, col0, row1, col1, row2, col2)
        && is_same_row_mod_tri(row0, col0, row1, col1, row2, col2);
}

int is_same_row_onehand_out(int row0, int col0, int row1, int col1, int row2, int col2)
{
    return is_onehand_out(row0, col0, row1, col1, row2, col2)
        && is_same_row_mod_tri(row0, col0, row1, col1, row2, col2);
}

int is_adjacent_finger_onehand(int row0, int col0, int row1, int col1, int row2, int col2)
{
    return is_onehand(row0, col0, row1, col1, row2, col2)
        && is_adjacent_finger_tri(row0, col0, row1, col1, row2, col2);
}

int is_adjacent_finger_onehand_in(int row0, int col0, int row1, int col1, int row2, int col2)
{
    return is_onehand_in(row0, col0, row1, col1, row2, col2)
        && is_adjacent_finger_tri(row0, col0, row1, col1, row2, col2);
}

int is_adjacent_finger_onehand_out(int row0, int col0, int row1, int col1, int row2, int col2)
{
    return is_onehand_out(row0, col0, row1, col1, row2, col2)
        && is_adjacent_finger_tri(row0, col0, row1, col1, row2, col2);
}

int is_same_row_adjacent_finger_onehand(int row0, int col0, int row1, int col1, int row2, int col2)
{
    return is_onehand(row0, col0, row1, col1, row2, col2)
        && is_same_row_mod_tri(row0, col0, row1, col1, row2, col2)
        && is_adjacent_finger_tri(row0, col0, row1, col1, row2, col2);
}

int is_same_row_adjacent_finger_onehand_in(int row0, int col0, int row1, int col1, int row2, int col2)
{
    return is_onehand_in(row0, col0, row1, col1, row2, col2)
        && is_same_row_mod_tri(row0, col0, row1, col1, row2, col2)
        && is_adjacent_finger_tri(row0, col0, row1, col1, row2, col2);
}

int is_same_row_adjacent_finger_onehand_out(int row0, int col0, int row1, int col1, int row2, int col2)
{
    return is_onehand_out(row0, col0, row1, col1, row2, col2)
        && is_same_row_mod_tri(row0, col0, row1, col1, row2, col2)
        && is_adjacent_finger_tri(row0, col0, row1, col1, row2, col2);
}



int is_roll(int row0, int col0, int row1, int col1, int row2, int col2)
{
    return (
            is_same_hand_bi(row0, col0, row1, col1) && !is_same_hand_bi(row1, col1, row2, col2)
            && !is_same_finger_bi(row0, col0, row1, col1) && !is_same_pos_bi(row0, col0, row1, col1)
           )
        ||
           (
            !is_same_hand_bi(row0, col0, row1, col1) && is_same_hand_bi(row1, col1, row2, col2)
            && !is_same_finger_bi(row1, col1, row2, col2) && !is_same_pos_bi(row1, col1, row2, col2)
           );
}

int is_roll_in(int row0, int col0, int row1, int col1, int row2, int col2)
{
    return is_roll(row0, col0, row1, col1, row2, col2)
        && (
               (is_same_hand_bi(row0, col0, row1, col1) && hand(row1, col1) == 'l' && finger(row0, col0) < finger(row1,col1))
            || (is_same_hand_bi(row1, col1, row2, col2) && hand(row1, col1) == 'l' && finger(row1, col1) < finger(row2,col2))
            || (is_same_hand_bi(row0, col0, row1, col1) && hand(row1, col1) == 'r' && finger(row0, col0) > finger(row1,col1))
            || (is_same_hand_bi(row1, col1, row2, col2) && hand(row1, col1) == 'r' && finger(row1, col1) > finger(row2,col2))
           );
}

int is_roll_out(int row0, int col0, int row1, int col1, int row2, int col2)
{
    return is_roll(row0, col0, row1, col1, row2, col2)
        && !is_roll_in(row0, col0, row1, col1, row2, col2);
}

int is_same_row_roll(int row0, int col0, int row1, int col1, int row2, int col2)
{
    return is_roll(row0, col0, row1, col1, row2, col2)
        && (
               (is_same_hand_bi(row0, col0, row1, col1) && is_same_row_mod_bi(row0, col0, row1, col1))
            || (is_same_hand_bi(row1, col1, row2, col2) && is_same_row_mod_bi(row1, col1, row2, col2))
           );
}

int is_same_row_roll_in(int row0, int col0, int row1, int col1, int row2, int col2)
{
    return is_roll_in(row0, col0, row1, col1, row2, col2)
        && is_same_row_roll(row0, col0, row1, col1, row2, col2);
}

int is_same_row_roll_out(int row0, int col0, int row1, int col1, int row2, int col2)
{
    return is_roll_out(row0, col0, row1, col1, row2, col2)
        && is_same_row_roll(row0, col0, row1, col1, row2, col2);
}

int is_adjacent_finger_roll(int row0, int col0, int row1, int col1, int row2, int col2)
{
    return is_roll(row0, col0, row1, col1, row2, col2)
        && (
               (is_same_hand_bi(row0, col0, row1, col1) && is_adjacent_finger_bi(row0, col0, row1, col1))
            || (is_same_hand_bi(row1, col1, row2, col2) && is_adjacent_finger_bi(row1, col1, row2, col2))
           );
}

int is_adjacent_finger_roll_in(int row0, int col0, int row1, int col1, int row2, int col2)
{
    return is_roll_in(row0, col0, row1, col1, row2, col2)
        && is_adjacent_finger_roll(row0, col0, row1, col1, row2, col2);
}

int is_adjacent_finger_roll_out(int row0, int col0, int row1, int col1, int row2, int col2)
{
    return is_roll_out(row0, col0, row1, col1, row2, col2)
        && is_adjacent_finger_roll(row0, col0, row1, col1, row2, col2);
}

int is_same_row_adjacent_finger_roll(int row0, int col0, int row1, int col1, int row2, int col2)
{
    return is_same_row_roll(row0, col0, row1, col1, row2, col2)
        && is_adjacent_finger_roll(row0, col0, row1, col1, row2, col2);
}

int is_same_row_adjacent_finger_roll_in(int row0, int col0, int row1, int col1, int row2, int col2)
{
    return is_roll_in(row0, col0, row1, col1, row2, col2)
        && is_same_row_adjacent_finger_roll(row0, col0, row1, col1, row2, col2);
}

int is_same_row_adjacent_finger_roll_out(int row0, int col0, int row1, int col1, int row2, int col2)
{
    return is_roll_out(row0, col0, row1, col1, row2, col2)
        && is_same_row_adjacent_finger_roll(row0, col0, row1, col1, row2, col2);
}

int is_chained_redirect(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3)
{
    return is_redirect(row0, col0, row1, col1, row2, col2) && is_redirect(row1, col1, row2, col2, row3, col3);
}

int is_bad_chained_redirect(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3)
{
    return is_bad_redirect(row0, col0, row1, col1, row2, col2) && is_bad_redirect(row1, col1, row2, col2, row3, col3);
}

int is_chained_alt(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3)
{
    return is_alt(row0, col0, row1, col1, row2, col2) && is_alt(row1, col1, row2, col2, row3, col3);
}

int is_chained_alt_in(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3)
{
    return is_alt_in(row0, col0, row1, col1, row2, col2) && is_alt_in(row1, col1, row2, col2, row3, col3);
}

int is_chained_alt_out(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3)
{
    return is_alt_out(row0, col0, row1, col1, row2, col2) && is_alt_out(row1, col1, row2, col2, row3, col3);
}

int is_chained_alt_mix(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3)
{
    return (is_alt_in(row0, col0, row1, col1, row2, col2) && is_alt_out(row1, col1, row2, col2, row3, col3))
        || (is_alt_out(row0, col0, row1, col1, row2, col2) && is_alt_in(row1, col1, row2, col2, row3, col3));
}


int is_chained_same_row_alt(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3)
{
    return is_same_row_alt(row0, col0, row1, col1, row2, col2) && is_same_row_alt(row1, col1, row2, col2, row3, col3);
}

int is_chained_same_row_alt_in(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3)
{
    return is_same_row_alt_in(row0, col0, row1, col1, row2, col2) && is_same_row_alt_in(row1, col1, row2, col2, row3, col3);
}

int is_chained_same_row_alt_out(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3)
{
    return is_same_row_alt_out(row0, col0, row1, col1, row2, col2) && is_same_row_alt_out(row1, col1, row2, col2, row3, col3);
}

int is_chained_same_row_alt_mix(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3)
{
    return (is_same_row_alt_in(row0, col0, row1, col1, row2, col2) && is_same_row_alt_out(row1, col1, row2, col2, row3, col3))
        || (is_same_row_alt_out(row0, col0, row1, col1, row2, col2) && is_same_row_alt_in(row1, col1, row2, col2, row3, col3));
}


int is_chained_adjacent_finger_alt(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3)
{
    return is_adjacent_finger_alt(row0, col0, row1, col1, row2, col2) && is_adjacent_finger_alt(row1, col1, row2, col2, row3, col3);
}

int is_chained_adjacent_finger_alt_in(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3)
{
    return is_adjacent_finger_alt_in(row0, col0, row1, col1, row2, col2) && is_adjacent_finger_alt_in(row1, col1, row2, col2, row3, col3);
}

int is_chained_adjacent_finger_alt_out(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3)
{
    return is_adjacent_finger_alt_out(row0, col0, row1, col1, row2, col2) && is_adjacent_finger_alt_out(row1, col1, row2, col2, row3, col3);
}

int is_chained_adjacent_finger_alt_mix(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3)
{
    return (is_adjacent_finger_alt_in(row0, col0, row1, col1, row2, col2) && is_adjacent_finger_alt_out(row1, col1, row2, col2, row3, col3))
        || (is_adjacent_finger_alt_out(row0, col0, row1, col1, row2, col2) && is_adjacent_finger_alt_in(row1, col1, row2, col2, row3, col3));
}


int is_chained_same_row_adjacent_finger_alt(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3)
{
    return is_same_row_adjacent_finger_alt(row0, col0, row1, col1, row2, col2) && is_same_row_adjacent_finger_alt(row1, col1, row2, col2, row3, col3);
}

int is_chained_same_row_adjacent_finger_alt_in(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3)
{
    return is_same_row_adjacent_finger_alt_in(row0, col0, row1, col1, row2, col2) && is_same_row_adjacent_finger_alt_in(row1, col1, row2, col2, row3, col3);
}

int is_chained_same_row_adjacent_finger_alt_out(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3)
{
    return is_same_row_adjacent_finger_alt_out(row0, col0, row1, col1, row2, col2) && is_same_row_adjacent_finger_alt_out(row1, col1, row2, col2, row3, col3);
}

int is_chained_same_row_adjacent_finger_alt_mix(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3)
{
    return (is_same_row_adjacent_finger_alt_in(row0, col0, row1, col1, row2, col2) && is_same_row_adjacent_finger_alt_out(row1, col1, row2, col2, row3, col3))
        || (is_same_row_adjacent_finger_alt_out(row0, col0, row1, col1, row2, col2) && is_same_row_adjacent_finger_alt_in(row1, col1, row2, col2, row3, col3));
}


int is_onehand_quad(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3)
{
    return is_same_hand_quad(row0, col0, row1, col1, row2, col2, row3, col3)
        && (
             (
                finger(row0, col0) < finger(row1, col1)
             && finger(row1, col1) < finger(row2, col2)
             && finger(row2, col2) < finger(row3, col3)
             )
            ||
             (
                finger(row0, col0) > finger(row1, col1)
             && finger(row1, col1) > finger(row2, col2)
             && finger(row2, col2) > finger(row3, col3)
             )
           );
}

int is_onehand_quad_in(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3)
{
    return is_onehand_quad(row0, col0, row1, col1, row2, col2, row3, col3)
        && (
             (hand(row0, col0) == 'l'
              && finger(row0, col0) < finger(row1, col1)
              && finger(row1, col1) < finger(row2, col2)
              && finger(row2, col2) < finger(row3, col3)
             )
            ||
             (hand(row0, col0) == 'r'
              && finger(row0, col0) > finger(row1, col1)
              && finger(row1, col1) > finger(row2, col2)
              && finger(row2, col2) > finger(row3, col3)
             )
           );
}

int is_onehand_quad_out(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3)
{
    return is_onehand_quad(row0, col0, row1, col1, row2, col2, row3, col3)
        && !is_onehand_quad_in(row0, col0, row1, col1, row2, col2, row3, col3);
}

int is_same_row_onehand_quad(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3)
{
    return is_onehand_quad(row0, col0, row1, col1, row2, col2, row3, col3)
        && is_same_row_mod_quad(row0, col0, row1, col1, row2, col2, row3, col3);
}

int is_same_row_onehand_quad_in(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3)
{
    return is_onehand_quad_in(row0, col0, row1, col1, row2, col2, row3, col3)
        && is_same_row_mod_quad(row0, col0, row1, col1, row2, col2, row3, col3);
}

int is_same_row_onehand_quad_out(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3)
{
    return is_onehand_quad_out(row0, col0, row1, col1, row2, col2, row3, col3)
        && is_same_row_mod_quad(row0, col0, row1, col1, row2, col2, row3, col3);
}

int is_adjacent_finger_onehand_quad(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3)
{
    return is_onehand_quad(row0, col0, row1, col1, row2, col2, row3, col3)
        && is_adjacent_finger_quad(row0, col0, row1, col1, row2, col2, row3, col3);
}

int is_adjacent_finger_onehand_quad_in(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3)
{
    return is_onehand_quad_in(row0, col0, row1, col1, row2, col2, row3, col3)
        && is_adjacent_finger_quad(row0, col0, row1, col1, row2, col2, row3, col3);
}

int is_adjacent_finger_onehand_quad_out(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3)
{
    return is_onehand_quad_out(row0, col0, row1, col1, row2, col2, row3, col3)
        && is_adjacent_finger_quad(row0, col0, row1, col1, row2, col2, row3, col3);
}

int is_same_row_adjacent_finger_onehand_quad(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3)
{
    return is_onehand_quad(row0, col0, row1, col1, row2, col2, row3, col3)
        && is_same_row_mod_quad(row0, col0, row1, col1, row2, col2, row3, col3)
        && is_adjacent_finger_quad(row0, col0, row1, col1, row2, col2, row3, col3);
}

int is_same_row_adjacent_finger_onehand_quad_in(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3)
{
    return is_onehand_quad_in(row0, col0, row1, col1, row2, col2, row3, col3)
        && is_same_row_mod_quad(row0, col0, row1, col1, row2, col2, row3, col3)
        && is_adjacent_finger_quad(row0, col0, row1, col1, row2, col2, row3, col3);
}

int is_same_row_adjacent_finger_onehand_quad_out(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3)
{
    return is_onehand_quad_out(row0, col0, row1, col1, row2, col2, row3, col3)
        && is_same_row_mod_quad(row0, col0, row1, col1, row2, col2, row3, col3)
        && is_adjacent_finger_quad(row0, col0, row1, col1, row2, col2, row3, col3);
}

int is_roll_quad(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3)
{
    return (
            is_onehand(row0, col0, row1, col1, row2, col2) && !is_same_hand_bi(row2, col2, row3, col3)
           )
        ||
           (
            !is_same_hand_bi(row0, col0, row1, col1) && is_onehand(row1, col1, row2, col2, row3, col3)
           );
}

int is_roll_quad_in(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3)
{
    return (
            is_onehand_in(row0, col0, row1, col1, row2, col2) && !is_same_hand_bi(row2, col2, row3, col3)
           )
        ||
           (
            !is_same_hand_bi(row0, col0, row1, col1) && is_onehand_in(row1, col1, row2, col2, row3, col3)
           );
}

int is_roll_quad_out(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3)
{
    return (
            is_onehand_out(row0, col0, row1, col1, row2, col2) && !is_same_hand_bi(row2, col2, row3, col3)
           )
        ||
           (
            !is_same_hand_bi(row0, col0, row1, col1) && is_onehand_out(row1, col1, row2, col2, row3, col3)
           );
}

int is_same_row_roll_quad(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3)
{
    return is_roll_quad(row0, col0, row1, col1, row2, col2, row3, col3)
        && (
               is_same_row_onehand(row0, col0, row1, col1, row2, col2)
            || is_same_row_onehand(row1, col1, row2, col2, row3, col3)
           );
}

int is_same_row_roll_quad_in(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3)
{
    return is_same_row_roll_quad(row0, col0, row1, col1, row2, col2, row3, col3)
        && is_roll_quad_in(row0, col0, row1, col1, row2, col2, row3, col3);
}

int is_same_row_roll_quad_out(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3)
{
    return is_same_row_roll_quad(row0, col0, row1, col1, row2, col2, row3, col3)
        && is_roll_quad_out(row0, col0, row1, col1, row2, col2, row3, col3);
}

int is_adjacent_finger_roll_quad(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3)
{
    return is_roll_quad(row0, col0, row1, col1, row2, col2, row3, col3)
        && (
               is_adjacent_finger_onehand(row0, col0, row1, col1, row2, col2)
            || is_adjacent_finger_onehand(row1, col1, row2, col2, row3, col3)
           );
}

int is_adjacent_finger_roll_quad_in(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3)
{
    return is_adjacent_finger_roll_quad(row0, col0, row1, col1, row2, col2, row3, col3)
        && is_roll_quad_in(row0, col0, row1, col1, row2, col2, row3, col3);
}

int is_adjacent_finger_roll_quad_out(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3)
{
    return is_adjacent_finger_roll_quad(row0, col0, row1, col1, row2, col2, row3, col3)
        && is_roll_quad_out(row0, col0, row1, col1, row2, col2, row3, col3);
}

int is_same_row_adjacent_finger_roll_quad(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3)
{
    return is_roll_quad(row0, col0, row1, col1, row2, col2, row3, col3)
        && (
               is_same_row_adjacent_finger_onehand(row0, col0, row1, col1, row2, col2)
            || is_same_row_adjacent_finger_onehand(row1, col1, row2, col2, row3, col3)
           );
}

int is_same_row_adjacent_finger_roll_quad_in(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3)
{
    return is_same_row_adjacent_finger_roll_quad(row0, col0, row1, col1, row2, col2, row3, col3)
        && is_roll_quad_in(row0, col0, row1, col1, row2, col2, row3, col3);
}

int is_same_row_adjacent_finger_roll_quad_out(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3)
{
    return is_same_row_adjacent_finger_roll_quad(row0, col0, row1, col1, row2, col2, row3, col3)
        && is_roll_quad_out(row0, col0, row1, col1, row2, col2, row3, col3);
}

int is_true_roll(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3)
{
    return !is_same_hand_bi(row0, col0, row1, col1)
        && is_same_hand_bi(row1, col1, row2, col2)
        && !is_same_hand_bi(row2, col2, row3, col3)
        && !is_same_finger_bi(row1, col1, row2, col2)
        && !is_same_pos_bi(row1, col1, row2, col2);
}

int is_true_roll_in(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3)
{
    return is_true_roll(row0, col0, row1, col1, row2, col2, row3, col3)
        && is_roll_in(row0, col0, row1, col1, row2, col2);
}

int is_true_roll_out(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3)
{
    return is_true_roll(row0, col0, row1, col1, row2, col2, row3, col3)
        && is_roll_out(row0, col0, row1, col1, row2, col2);
}

int is_same_row_true_roll(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3)
{
    return is_true_roll(row0, col0, row1, col1, row2, col2, row3, col3)
        && is_same_row_mod_bi(row1, col1, row2, col2);
}

int is_same_row_true_roll_in(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3)
{
    return is_true_roll_in(row0, col0, row1, col1, row2, col2, row3, col3)
        && is_same_row_mod_bi(row1, col1, row2, col2);
}

int is_same_row_true_roll_out(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3)
{
    return is_true_roll_out(row0, col0, row1, col1, row2, col2, row3, col3)
        && is_same_row_mod_bi(row1, col1, row2, col2);
}

int is_adjacent_finger_true_roll(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3)
{
    return is_true_roll(row0, col0, row1, col1, row2, col2, row3, col3)
        && is_adjacent_finger_bi(row1, col1, row2, col2);
}

int is_adjacent_finger_true_roll_in(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3)
{
    return is_true_roll_in(row0, col0, row1, col1, row2, col2, row3, col3)
        && is_adjacent_finger_bi(row1, col1, row2, col2);
}

int is_adjacent_finger_true_roll_out(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3)
{
    return is_true_roll_out(row0, col0, row1, col1, row2, col2, row3, col3)
        && is_adjacent_finger_bi(row1, col1, row2, col2);
}

int is_same_row_adjacent_finger_true_roll(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3)
{
    return is_true_roll(row0, col0, row1, col1, row2, col2, row3, col3)
        && is_same_row_mod_bi(row1, col1, row2, col2)
        && is_adjacent_finger_bi(row1, col1, row2, col2);
}

int is_same_row_adjacent_finger_true_roll_in(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3)
{
    return is_true_roll_in(row0, col0, row1, col1, row2, col2, row3, col3)
        && is_same_row_mod_bi(row1, col1, row2, col2)
        && is_adjacent_finger_bi(row1, col1, row2, col2);
}

int is_same_row_adjacent_finger_true_roll_out(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3)
{
    return is_true_roll_out(row0, col0, row1, col1, row2, col2, row3, col3)
        && is_same_row_mod_bi(row1, col1, row2, col2)
        && is_adjacent_finger_bi(row1, col1, row2, col2);
}

int is_chained_roll(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3)
{
    return is_roll(row0, col0, row1, col1, row2, col2) && is_roll(row1, col1, row2, col2, row3, col3)
        && !is_same_hand_bi(row1, col1, row2, col2);
}

int is_chained_roll_in(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3)
{
    return is_chained_roll(row0, col0, row1, col1, row2, col2, row3, col3)
        && is_roll_in(row0, col0, row1, col1, row2, col2)
        && is_roll_in(row1, col1, row2, col2, row3, col3);
}

int is_chained_roll_out(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3)
{
    return is_chained_roll(row0, col0, row1, col1, row2, col2, row3, col3)
        && is_roll_out(row0, col0, row1, col1, row2, col2)
        && is_roll_out(row1, col1, row2, col2, row3, col3);
}

int is_chained_roll_mix(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3)
{
    return is_chained_roll(row0, col0, row1, col1, row2, col2, row3, col3)
        && (
            (is_roll_in(row0, col0, row1, col1, row2, col2) && is_roll_out(row1, col1, row2, col2, row3, col3))
            ||
            (is_roll_out(row0, col0, row1, col1, row2, col2) && is_roll_in(row1, col1, row2, col2, row3, col3))
           );
}

int is_same_row_chained_roll(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3)
{
    return is_chained_roll(row0, col0, row1, col1, row2, col2, row3, col3)
        && is_same_row_mod_bi(row0, col0, row1, col1)
        && is_same_row_mod_bi(row2, col2, row3, col3);
}

int is_same_row_chained_roll_in(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3)
{
    return is_chained_roll_in(row0, col0, row1, col1, row2, col2, row3, col3)
        && is_same_row_mod_bi(row0, col0, row1, col1)
        && is_same_row_mod_bi(row2, col2, row3, col3);
}

int is_same_row_chained_roll_out(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3)
{
    return is_chained_roll_out(row0, col0, row1, col1, row2, col2, row3, col3)
        && is_same_row_mod_bi(row0, col0, row1, col1)
        && is_same_row_mod_bi(row2, col2, row3, col3);
}

int is_same_row_chained_roll_mix(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3)
{
    return is_chained_roll_mix(row0, col0, row1, col1, row2, col2, row3, col3)
        && is_same_row_mod_bi(row0, col0, row1, col1)
        && is_same_row_mod_bi(row2, col2, row3, col3);
}

int is_adjacent_finger_chained_roll(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3)
{
    return is_chained_roll(row0, col0, row1, col1, row2, col2, row3, col3)
        && is_adjacent_finger_bi(row0, col0, row1, col1)
        && is_adjacent_finger_bi(row2, col2, row3, col3);
}

int is_adjacent_finger_chained_roll_in(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3)
{
    return is_chained_roll_in(row0, col0, row1, col1, row2, col2, row3, col3)
        && is_adjacent_finger_bi(row0, col0, row1, col1)
        && is_adjacent_finger_bi(row2, col2, row3, col3);
}

int is_adjacent_finger_chained_roll_out(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3)
{
    return is_chained_roll_out(row0, col0, row1, col1, row2, col2, row3, col3)
        && is_adjacent_finger_bi(row0, col0, row1, col1)
        && is_adjacent_finger_bi(row2, col2, row3, col3);
}

int is_adjacent_finger_chained_roll_mix(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3)
{
    return is_chained_roll_mix(row0, col0, row1, col1, row2, col2, row3, col3)
        && is_adjacent_finger_bi(row0, col0, row1, col1)
        && is_adjacent_finger_bi(row2, col2, row3, col3);
}

int is_same_row_adjacent_finger_chained_roll(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3)
{
    return is_chained_roll(row0, col0, row1, col1, row2, col2, row3, col3)
        && is_same_row_mod_bi(row0, col0, row1, col1)
        && is_same_row_mod_bi(row2, col2, row3, col3)
        && is_adjacent_finger_bi(row0, col0, row1, col1)
        && is_adjacent_finger_bi(row2, col2, row3, col3);
}

int is_same_row_adjacent_finger_chained_roll_in(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3)
{
    return is_chained_roll_in(row0, col0, row1, col1, row2, col2, row3, col3)
        && is_same_row_mod_bi(row0, col0, row1, col1)
        && is_same_row_mod_bi(row2, col2, row3, col3)
        && is_adjacent_finger_bi(row0, col0, row1, col1)
        && is_adjacent_finger_bi(row2, col2, row3, col3);
}

int is_same_row_adjacent_finger_chained_roll_out(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3)
{
    return is_chained_roll_out(row0, col0, row1, col1, row2, col2, row3, col3)
        && is_same_row_mod_bi(row0, col0, row1, col1)
        && is_same_row_mod_bi(row2, col2, row3, col3)
        && is_adjacent_finger_bi(row0, col0, row1, col1)
        && is_adjacent_finger_bi(row2, col2, row3, col3);
}

int is_same_row_adjacent_finger_chained_roll_mix(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3)
{
    return is_chained_roll_mix(row0, col0, row1, col1, row2, col2, row3, col3)
        && is_same_row_mod_bi(row0, col0, row1, col1)
        && is_same_row_mod_bi(row2, col2, row3, col3)
        && is_adjacent_finger_bi(row0, col0, row1, col1)
        && is_adjacent_finger_bi(row2, col2, row3, col3);
}

