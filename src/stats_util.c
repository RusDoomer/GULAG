#include "stats_util.h"
#include "global.h"

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
