#include <string.h>

#include "stats_util.h"
#include "global.h"
#include "util.h"


float find_stat_score(char *stat_name, char type, layout *lt) {
    int i;

    switch (type) {
        case 'm':
            for (i = 0; i < MONO_END; i++) {
                if (strcmp(stats_mono[i].name, stat_name) == 0) {
                    return lt->mono_score[i];
                }
            }
            break;
        case 'b':
            for (i = 0; i < BI_END; i++) {
                if (strcmp(stats_bi[i].name, stat_name) == 0) {
                    return lt->bi_score[i];
                }
            }
            break;
        case 't':
            for (i = 0; i < TRI_END; i++) {
                if (strcmp(stats_tri[i].name, stat_name) == 0) {
                    return lt->tri_score[i];
                }
            }
            break;
        case 'q':
            for (i = 0; i < QUAD_END; i++) {
                if (strcmp(stats_quad[i].name, stat_name) == 0) {
                    return lt->quad_score[i];
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
                    // Convert char type to int for skipgram index
                    int skip_index = type - '0';
                    return lt->skip_score[skip_index][i];
                }
            }
            break;
        case 'e': // Meta stats
            for (i = 0; i < META_END; i++) {
                if (strcmp(stats_meta[i].name, stat_name) == 0) {
                    return lt->meta_score[i];
                }
            }
            break;
        default:
            error("Invalid type specified in find_stat_score");
    }

    // If stat not found, return 0.0
    return 0.0;
}

char hand(int row0, int col0)
{
    if (col0 < COL/2) {return 'l';}
    else {return 'r';}
}

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

int is_stretch(int row0, int col0)
{
    return col0 == 0 || col0 == 5 || col0 == 6 || col0 == 11;
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
