#ifndef STATS_UTIL_H
#define STATS_UTIL_H

#include "global.h"

float find_stat_score(char *stat_name, char type, layout *lt);

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

int is_alt(int row0, int col0, int row1, int col1, int row2, int col2);
int is_redirect(int row0, int col0, int row1, int col1, int row2, int col2);
int is_bad_redirect(int row0, int col0, int row1, int col1, int row2, int col2);
#endif
