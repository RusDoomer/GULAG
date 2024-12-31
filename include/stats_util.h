#ifndef STATS_UTIL_H
#define STATS_UTIL_H

#include "global.h"
#include "structs.h"

/*
 * Finds the score of a specific statistic in a given layout.
 * The function searches for the statistic by name and type within the layout's
 * statistical data.
 *
 * Parameters:
 *   stat_name: The name of the statistic to find.
 *   type: The type of the statistic ('m' for monogram, 'b' for bigram, etc.).
 *   lt: A pointer to the layout structure.
 *
 * Returns:
 *   The score of the found statistic. Returns NaN if the statistic is not found.
 */
int find_stat_index(char *stat_name, char type, layout *lt);

char hand(int row0, int col0);
int finger(int row0, int col0);
int is_stretch(int row0, int col0);

int is_same_col_bi(int row0, int col0, int row1, int col1);
int is_same_col_tri(int row0, int col0, int row1, int col1, int row2, int col2);
int is_same_col_quad(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3);

int is_same_row_bi(int row0, int col0, int row1, int col1);
int is_same_row_tri(int row0, int col0, int row1, int col1, int row2, int col2);
int is_same_row_quad(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3);

int is_same_pos_bi(int row0, int col0, int row1, int col1);
int is_same_pos_tri(int row0, int col0, int row1, int col1, int row2, int col2);
int is_same_pos_quad(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3);

int is_same_finger_bi(int row0, int col0, int row1, int col1);
int is_same_finger_tri(int row0, int col0, int row1, int col1, int row2, int col2);
int is_same_finger_quad(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3);

int is_bad_same_finger_bi(int row0, int col0, int row1, int col1);

int is_full_russor(int row0, int col0, int row1, int col1);
int is_half_russor(int row0, int col0, int row1, int col1);

int is_index_stretch_bi(int row0, int col0, int row1, int col1);
int is_pinky_stretch_bi(int row0, int col0, int row1, int col1);

/*                                                   */
/* not explaining all of these this is too much work */
/*                                                   */

int is_redirect(int row0, int col0, int row1, int col1, int row2, int col2);
int is_bad_redirect(int row0, int col0, int row1, int col1, int row2, int col2);


int is_alt(int row0, int col0, int row1, int col1, int row2, int col2);
int is_alt_in(int row0, int col0, int row1, int col1, int row2, int col2);
int is_alt_out(int row0, int col0, int row1, int col1, int row2, int col2);

int is_same_row_alt(int row0, int col0, int row1, int col1, int row2, int col2);
int is_same_row_alt_in(int row0, int col0, int row1, int col1, int row2, int col2);
int is_same_row_alt_out(int row0, int col0, int row1, int col1, int row2, int col2);

int is_adjacent_finger_alt(int row0, int col0, int row1, int col1, int row2, int col2);
int is_adjacent_finger_alt_in(int row0, int col0, int row1, int col1, int row2, int col2);
int is_adjacent_finger_alt_out(int row0, int col0, int row1, int col1, int row2, int col2);

int is_same_row_adjacent_finger_alt(int row0, int col0, int row1, int col1, int row2, int col2);
int is_same_row_adjacent_finger_alt_in(int row0, int col0, int row1, int col1, int row2, int col2);
int is_same_row_adjacent_finger_alt_out(int row0, int col0, int row1, int col1, int row2, int col2);


int is_onehand(int row0, int col0, int row1, int col1, int row2, int col2);
int is_onehand_in(int row0, int col0, int row1, int col1, int row2, int col2);
int is_onehand_out(int row0, int col0, int row1, int col1, int row2, int col2);

int is_same_row_onehand(int row0, int col0, int row1, int col1, int row2, int col2);
int is_same_row_onehand_in(int row0, int col0, int row1, int col1, int row2, int col2);
int is_same_row_onehand_out(int row0, int col0, int row1, int col1, int row2, int col2);

int is_adjacent_finger_onehand(int row0, int col0, int row1, int col1, int row2, int col2);
int is_adjacent_finger_onehand_in(int row0, int col0, int row1, int col1, int row2, int col2);
int is_adjacent_finger_onehand_out(int row0, int col0, int row1, int col1, int row2, int col2);

int is_same_row_adjacent_finger_onehand(int row0, int col0, int row1, int col1, int row2, int col2);
int is_same_row_adjacent_finger_onehand_in(int row0, int col0, int row1, int col1, int row2, int col2);
int is_same_row_adjacent_finger_onehand_out(int row0, int col0, int row1, int col1, int row2, int col2);


int is_roll(int row0, int col0, int row1, int col1, int row2, int col2);
int is_roll_in(int row0, int col0, int row1, int col1, int row2, int col2);
int is_roll_out(int row0, int col0, int row1, int col1, int row2, int col2);

int is_same_row_roll(int row0, int col0, int row1, int col1, int row2, int col2);
int is_same_row_roll_in(int row0, int col0, int row1, int col1, int row2, int col2);
int is_same_row_roll_out(int row0, int col0, int row1, int col1, int row2, int col2);

int is_adjacent_finger_roll(int row0, int col0, int row1, int col1, int row2, int col2);
int is_adjacent_finger_roll_in(int row0, int col0, int row1, int col1, int row2, int col2);
int is_adjacent_finger_roll_out(int row0, int col0, int row1, int col1, int row2, int col2);

int is_same_row_adjacent_finger_roll(int row0, int col0, int row1, int col1, int row2, int col2);
int is_same_row_adjacent_finger_roll_in(int row0, int col0, int row1, int col1, int row2, int col2);
int is_same_row_adjacent_finger_roll_out(int row0, int col0, int row1, int col1, int row2, int col2);


int is_chained_redirect(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3);
int is_bad_chained_redirect(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3);

int is_chained_alt(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3);
int is_chained_alt_in(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3);
int is_chained_alt_out(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3);
int is_chained_alt_mix(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3);

int is_chained_same_row_alt(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3);
int is_chained_same_row_alt_in(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3);
int is_chained_same_row_alt_out(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3);
int is_chained_same_row_alt_mix(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3);

int is_chained_adjacent_finger_alt(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3);
int is_chained_adjacent_finger_alt_in(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3);
int is_chained_adjacent_finger_alt_out(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3);
int is_chained_adjacent_finger_alt_mix(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3);

int is_chained_same_row_adjacent_finger_alt(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3);
int is_chained_same_row_adjacent_finger_alt_in(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3);
int is_chained_same_row_adjacent_finger_alt_out(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3);
int is_chained_same_row_adjacent_finger_alt_mix(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3);

int is_onehand_quad(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3);
int is_onehand_quad_in(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3);
int is_onehand_quad_out(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3);

int is_same_row_onehand_quad(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3);
int is_same_row_onehand_quad_in(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3);
int is_same_row_onehand_quad_out(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3);

int is_adjacent_finger_onehand_quad(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3);
int is_adjacent_finger_onehand_quad_in(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3);
int is_adjacent_finger_onehand_quad_out(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3);

int is_same_row_adjacent_finger_onehand_quad(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3);
int is_same_row_adjacent_finger_onehand_quad_in(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3);
int is_same_row_adjacent_finger_onehand_quad_out(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3);


int is_roll_quad(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3);
int is_roll_quad_in(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3);
int is_roll_quad_out(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3);

int is_same_row_roll_quad(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3);
int is_same_row_roll_quad_in(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3);
int is_same_row_roll_quad_out(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3);

int is_adjacent_finger_roll_quad(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3);
int is_adjacent_finger_roll_quad_in(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3);
int is_adjacent_finger_roll_quad_out(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3);

int is_same_row_adjacent_finger_roll_quad(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3);
int is_same_row_adjacent_finger_roll_quad_in(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3);
int is_same_row_adjacent_finger_roll_quad_out(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3);


int is_true_roll(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3);
int is_true_roll_in(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3);
int is_true_roll_out(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3);

int is_same_row_true_roll(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3);
int is_same_row_true_roll_in(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3);
int is_same_row_true_roll_out(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3);

int is_adjacent_finger_true_roll(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3);
int is_adjacent_finger_true_roll_in(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3);
int is_adjacent_finger_true_roll_out(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3);

int is_same_row_adjacent_finger_true_roll(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3);
int is_same_row_adjacent_finger_true_roll_in(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3);
int is_same_row_adjacent_finger_true_roll_out(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3);


int is_chained_roll(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3);
int is_chained_roll_in(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3);
int is_chained_roll_out(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3);
int is_chained_roll_mix(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3);

int is_same_row_chained_roll(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3);
int is_same_row_chained_roll_in(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3);
int is_same_row_chained_roll_out(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3);
int is_same_row_chained_roll_mix(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3);

int is_adjacent_finger_chained_roll(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3);
int is_adjacent_finger_chained_roll_in(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3);
int is_adjacent_finger_chained_roll_out(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3);
int is_adjacent_finger_chained_roll_mix(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3);

int is_same_row_adjacent_finger_chained_roll(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3);
int is_same_row_adjacent_finger_chained_roll_in(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3);
int is_same_row_adjacent_finger_chained_roll_out(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3);
int is_same_row_adjacent_finger_chained_roll_mix(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3);
#endif
