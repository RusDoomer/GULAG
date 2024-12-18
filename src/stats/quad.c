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

    quad_stat *alt = (quad_stat *)malloc(sizeof(quad_stat));
    same_finger->next = alt;
    strcpy(alt->name, "Chained Alternation");
    alt->weight = 0;
    alt->length = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_chained_alt(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            alt->ngrams[i] = i;
            alt->length++;
        }
        else
        {
            alt->ngrams[i] = -1;
        }
    }

    quad_stat *redirect = (quad_stat *)malloc(sizeof(quad_stat));
    alt->next = redirect;
    strcpy(redirect->name, "Chained Redirect");
    redirect->weight = 0;
    redirect->length = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_chained_redirect(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            redirect->ngrams[i] = i;
            redirect->length++;
        }
        else
        {
            redirect->ngrams[i] = -1;
        }
    }

    quad_stat *bad_redirect = (quad_stat *)malloc(sizeof(quad_stat));
    redirect->next = bad_redirect;
    strcpy(bad_redirect->name, "Bad Chained Redirect");
    bad_redirect->weight = 0;
    bad_redirect->length = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_bad_chained_redirect(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            bad_redirect->ngrams[i] = i;
            bad_redirect->length++;
        }
        else
        {
            bad_redirect->ngrams[i] = -1;
        }
    }

    quad_stat *onehand = (quad_stat *)malloc(sizeof(quad_stat));
    bad_redirect->next = onehand;
    strcpy(onehand->name, "Quad One Hand");
    onehand->weight = 0;
    onehand->length = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_onehand_quad(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            onehand->ngrams[i] = i;
            onehand->length++;
        }
        else
        {
            onehand->ngrams[i] = -1;
        }
    }

    quad_stat *onehand_in = (quad_stat *)malloc(sizeof(quad_stat));
    onehand->next = onehand_in;
    strcpy(onehand_in->name, "Quad One Hand In");
    onehand_in->weight = 0;
    onehand_in->length = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_onehand_quad_in(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            onehand_in->ngrams[i] = i;
            onehand_in->length++;
        }
        else
        {
            onehand_in->ngrams[i] = -1;
        }
    }

    quad_stat *onehand_out = (quad_stat *)malloc(sizeof(quad_stat));
    onehand_in->next = onehand_out;
    strcpy(onehand_out->name, "Quad One Hand Out");
    onehand_out->weight = 0;
    onehand_out->length = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_onehand_quad_out(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            onehand_out->ngrams[i] = i;
            onehand_out->length++;
        }
        else
        {
            onehand_out->ngrams[i] = -1;
        }
    }

    quad_stat *same_row_onehand = (quad_stat *)malloc(sizeof(quad_stat));
    onehand_out->next = same_row_onehand;
    strcpy(same_row_onehand->name, "Quad Same Row One Hand");
    same_row_onehand->weight = 0;
    same_row_onehand->length = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_same_row_onehand_quad(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            same_row_onehand->ngrams[i] = i;
            same_row_onehand->length++;
        }
        else
        {
            same_row_onehand->ngrams[i] = -1;
        }
    }

    quad_stat *same_row_onehand_in = (quad_stat *)malloc(sizeof(quad_stat));
    same_row_onehand->next = same_row_onehand_in;
    strcpy(same_row_onehand_in->name, "Quad Same Row One Hand In");
    same_row_onehand_in->weight = 0;
    same_row_onehand_in->length = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_same_row_onehand_quad_in(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            same_row_onehand_in->ngrams[i] = i;
            same_row_onehand_in->length++;
        }
        else
        {
            same_row_onehand_in->ngrams[i] = -1;
        }
    }

    quad_stat *same_row_onehand_out = (quad_stat *)malloc(sizeof(quad_stat));
    same_row_onehand_in->next = same_row_onehand_out;
    strcpy(same_row_onehand_out->name, "Quad Same Row One Hand Out");
    same_row_onehand_out->weight = 0;
    same_row_onehand_out->length = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_same_row_onehand_quad_out(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            same_row_onehand_out->ngrams[i] = i;
            same_row_onehand_out->length++;
        }
        else
        {
            same_row_onehand_out->ngrams[i] = -1;
        }
    }

    quad_stat *adjacent_finger_onehand = (quad_stat *)malloc(sizeof(quad_stat));
    same_row_onehand_out->next = adjacent_finger_onehand;
    strcpy(adjacent_finger_onehand->name, "Quad Adjacent Finger One Hand");
    adjacent_finger_onehand->weight = 0;
    adjacent_finger_onehand->length = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_adjacent_finger_onehand_quad(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            adjacent_finger_onehand->ngrams[i] = i;
            adjacent_finger_onehand->length++;
        }
        else
        {
            adjacent_finger_onehand->ngrams[i] = -1;
        }
    }

    quad_stat *adjacent_finger_onehand_in = (quad_stat *)malloc(sizeof(quad_stat));
    adjacent_finger_onehand->next = adjacent_finger_onehand_in;
    strcpy(adjacent_finger_onehand_in->name, "Quad Adjacent Finger One Hand In");
    adjacent_finger_onehand_in->weight = 0;
    adjacent_finger_onehand_in->length = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_adjacent_finger_onehand_quad_in(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            adjacent_finger_onehand_in->ngrams[i] = i;
            adjacent_finger_onehand_in->length++;
        }
        else
        {
            adjacent_finger_onehand_in->ngrams[i] = -1;
        }
    }

    quad_stat *adjacent_finger_onehand_out = (quad_stat *)malloc(sizeof(quad_stat));
    adjacent_finger_onehand_in->next = adjacent_finger_onehand_out;
    strcpy(adjacent_finger_onehand_out->name, "Quad Adjacent Finger One Hand Out");
    adjacent_finger_onehand_out->weight = 0;
    adjacent_finger_onehand_out->length = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_adjacent_finger_onehand_quad_out(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            adjacent_finger_onehand_out->ngrams[i] = i;
            adjacent_finger_onehand_out->length++;
        }
        else
        {
            adjacent_finger_onehand_out->ngrams[i] = -1;
        }
    }

    quad_stat *same_row_adjacent_finger_onehand = (quad_stat *)malloc(sizeof(quad_stat));
    adjacent_finger_onehand_out->next = same_row_adjacent_finger_onehand;
    strcpy(same_row_adjacent_finger_onehand->name, "Quad Same Row Adjacent Finger One Hand");
    same_row_adjacent_finger_onehand->weight = 0;
    same_row_adjacent_finger_onehand->length = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_same_row_adjacent_finger_onehand_quad(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            same_row_adjacent_finger_onehand->ngrams[i] = i;
            same_row_adjacent_finger_onehand->length++;
        }
        else
        {
            same_row_adjacent_finger_onehand->ngrams[i] = -1;
        }
    }

    quad_stat *same_row_adjacent_finger_onehand_in = (quad_stat *)malloc(sizeof(quad_stat));
    same_row_adjacent_finger_onehand->next = same_row_adjacent_finger_onehand_in;
    strcpy(same_row_adjacent_finger_onehand_in->name, "Quad Same Row Adjacent Finger One Hand In");
    same_row_adjacent_finger_onehand_in->weight = 0;
    same_row_adjacent_finger_onehand_in->length = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_same_row_adjacent_finger_onehand_quad_in(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            same_row_adjacent_finger_onehand_in->ngrams[i] = i;
            same_row_adjacent_finger_onehand_in->length++;
        }
        else
        {
            same_row_adjacent_finger_onehand_in->ngrams[i] = -1;
        }
    }

    quad_stat *same_row_adjacent_finger_onehand_out = (quad_stat *)malloc(sizeof(quad_stat));
    same_row_adjacent_finger_onehand_in->next = same_row_adjacent_finger_onehand_out;
    strcpy(same_row_adjacent_finger_onehand_out->name, "Quad Same Row Adjacent Finger One Hand Out");
    same_row_adjacent_finger_onehand_out->weight = 0;
    same_row_adjacent_finger_onehand_out->length = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_same_row_adjacent_finger_onehand_quad_out(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            same_row_adjacent_finger_onehand_out->ngrams[i] = i;
            same_row_adjacent_finger_onehand_out->length++;
        }
        else
        {
            same_row_adjacent_finger_onehand_out->ngrams[i] = -1;
        }
    }

    quad_stat *roll = (quad_stat *)malloc(sizeof(quad_stat));
    same_row_adjacent_finger_onehand_out->next = roll;
    strcpy(roll->name, "Quad Roll");
    roll->weight = 0;
    roll->length = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_roll_quad(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            roll->ngrams[i] = i;
            roll->length++;
        }
        else
        {
            roll->ngrams[i] = -1;
        }
    }

    quad_stat *roll_in = (quad_stat *)malloc(sizeof(quad_stat));
    roll->next = roll_in;
    strcpy(roll_in->name, "Quad Roll In");
    roll_in->weight = 0;
    roll_in->length = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_roll_quad_in(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            roll_in->ngrams[i] = i;
            roll_in->length++;
        }
        else
        {
            roll_in->ngrams[i] = -1;
        }
    }

    quad_stat *roll_out = (quad_stat *)malloc(sizeof(quad_stat));
    roll_in->next = roll_out;
    strcpy(roll_out->name, "Quad Roll Out");
    roll_out->weight = 0;
    roll_out->length = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_roll_quad_out(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            roll_out->ngrams[i] = i;
            roll_out->length++;
        }
        else
        {
            roll_out->ngrams[i] = -1;
        }
    }

    quad_stat *same_row_roll = (quad_stat *)malloc(sizeof(quad_stat));
    roll_out->next = same_row_roll;
    strcpy(same_row_roll->name, "Quad Same Row Roll");
    same_row_roll->weight = 0;
    same_row_roll->length = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_same_row_roll_quad(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            same_row_roll->ngrams[i] = i;
            same_row_roll->length++;
        }
        else
        {
            same_row_roll->ngrams[i] = -1;
        }
    }

    quad_stat *same_row_roll_in = (quad_stat *)malloc(sizeof(quad_stat));
    same_row_roll->next = same_row_roll_in;
    strcpy(same_row_roll_in->name, "Quad Same Row Roll In");
    same_row_roll_in->weight = 0;
    same_row_roll_in->length = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_same_row_roll_quad_in(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            same_row_roll_in->ngrams[i] = i;
            same_row_roll_in->length++;
        }
        else
        {
            same_row_roll_in->ngrams[i] = -1;
        }
    }

    quad_stat *same_row_roll_out = (quad_stat *)malloc(sizeof(quad_stat));
    same_row_roll_in->next = same_row_roll_out;
    strcpy(same_row_roll_out->name, "Quad Same Row Roll Out");
    same_row_roll_out->weight = 0;
    same_row_roll_out->length = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_same_row_roll_quad_out(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            same_row_roll_out->ngrams[i] = i;
            same_row_roll_out->length++;
        }
        else
        {
            same_row_roll_out->ngrams[i] = -1;
        }
    }

    quad_stat *adjacent_finger_roll = (quad_stat *)malloc(sizeof(quad_stat));
    same_row_roll_out->next = adjacent_finger_roll;
    strcpy(adjacent_finger_roll->name, "Quad Adjacent Finger Roll");
    adjacent_finger_roll->weight = 0;
    adjacent_finger_roll->length = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_adjacent_finger_roll_quad(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            adjacent_finger_roll->ngrams[i] = i;
            adjacent_finger_roll->length++;
        }
        else
        {
            adjacent_finger_roll->ngrams[i] = -1;
        }
    }

    quad_stat *adjacent_finger_roll_in = (quad_stat *)malloc(sizeof(quad_stat));
    adjacent_finger_roll->next = adjacent_finger_roll_in;
    strcpy(adjacent_finger_roll_in->name, "Quad Adjacent Finger Roll In");
    adjacent_finger_roll_in->weight = 0;
    adjacent_finger_roll_in->length = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_adjacent_finger_roll_quad_in(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            adjacent_finger_roll_in->ngrams[i] = i;
            adjacent_finger_roll_in->length++;
        }
        else
        {
            adjacent_finger_roll_in->ngrams[i] = -1;
        }
    }

    quad_stat *adjacent_finger_roll_out = (quad_stat *)malloc(sizeof(quad_stat));
    adjacent_finger_roll_in->next = adjacent_finger_roll_out;
    strcpy(adjacent_finger_roll_out->name, "Quad Adjacent Finger Roll Out");
    adjacent_finger_roll_out->weight = 0;
    adjacent_finger_roll_out->length = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_adjacent_finger_roll_quad_out(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            adjacent_finger_roll_out->ngrams[i] = i;
            adjacent_finger_roll_out->length++;
        }
        else
        {
            adjacent_finger_roll_out->ngrams[i] = -1;
        }
    }

    quad_stat *same_row_adjacent_finger_roll = (quad_stat *)malloc(sizeof(quad_stat));
    adjacent_finger_roll_out->next = same_row_adjacent_finger_roll;
    strcpy(same_row_adjacent_finger_roll->name, "Quad Same Row Adjacent Finger Roll");
    same_row_adjacent_finger_roll->weight = 0;
    same_row_adjacent_finger_roll->length = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_same_row_adjacent_finger_roll_quad(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            same_row_adjacent_finger_roll->ngrams[i] = i;
            same_row_adjacent_finger_roll->length++;
        }
        else
        {
            same_row_adjacent_finger_roll->ngrams[i] = -1;
        }
    }

    quad_stat *same_row_adjacent_finger_roll_in = (quad_stat *)malloc(sizeof(quad_stat));
    same_row_adjacent_finger_roll->next = same_row_adjacent_finger_roll_in;
    strcpy(same_row_adjacent_finger_roll_in->name, "Quad Same Row Adjacent Finger Roll In");
    same_row_adjacent_finger_roll_in->weight = 0;
    same_row_adjacent_finger_roll_in->length = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_same_row_adjacent_finger_roll_quad_in(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            same_row_adjacent_finger_roll_in->ngrams[i] = i;
            same_row_adjacent_finger_roll_in->length++;
        }
        else
        {
            same_row_adjacent_finger_roll_in->ngrams[i] = -1;
        }
    }

    quad_stat *same_row_adjacent_finger_roll_out = (quad_stat *)malloc(sizeof(quad_stat));
    same_row_adjacent_finger_roll_in->next = same_row_adjacent_finger_roll_out;
    strcpy(same_row_adjacent_finger_roll_out->name, "Quad Same Row Adjacent Finger Roll Out");
    same_row_adjacent_finger_roll_out->weight = 0;
    same_row_adjacent_finger_roll_out->length = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_same_row_adjacent_finger_roll_quad_out(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            same_row_adjacent_finger_roll_out->ngrams[i] = i;
            same_row_adjacent_finger_roll_out->length++;
        }
        else
        {
            same_row_adjacent_finger_roll_out->ngrams[i] = -1;
        }
    }

    quad_stat *true_roll = (quad_stat *)malloc(sizeof(quad_stat));
    same_row_adjacent_finger_roll_out->next = true_roll;
    strcpy(true_roll->name, "True Roll");
    true_roll->weight = 0;
    true_roll->length = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_true_roll(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            true_roll->ngrams[i] = i;
            true_roll->length++;
        }
        else
        {
            true_roll->ngrams[i] = -1;
        }
    }

    quad_stat *true_roll_in = (quad_stat *)malloc(sizeof(quad_stat));
    true_roll->next = true_roll_in;
    strcpy(true_roll_in->name, "True Roll In");
    true_roll_in->weight = 0;
    true_roll_in->length = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_true_roll_in(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            true_roll_in->ngrams[i] = i;
            true_roll_in->length++;
        }
        else
        {
            true_roll_in->ngrams[i] = -1;
        }
    }

    quad_stat *true_roll_out = (quad_stat *)malloc(sizeof(quad_stat));
    true_roll_in->next = true_roll_out;
    strcpy(true_roll_out->name, "True Roll Out");
    true_roll_out->weight = 0;
    true_roll_out->length = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_true_roll_out(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            true_roll_out->ngrams[i] = i;
            true_roll_out->length++;
        }
        else
        {
            true_roll_out->ngrams[i] = -1;
        }
    }

    quad_stat *same_row_true_roll = (quad_stat *)malloc(sizeof(quad_stat));
    true_roll_out->next = same_row_true_roll;
    strcpy(same_row_true_roll->name, "Same Row True Roll");
    same_row_true_roll->weight = 0;
    same_row_true_roll->length = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_same_row_true_roll(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            same_row_true_roll->ngrams[i] = i;
            same_row_true_roll->length++;
        }
        else
        {
            same_row_true_roll->ngrams[i] = -1;
        }
    }

    quad_stat *same_row_true_roll_in = (quad_stat *)malloc(sizeof(quad_stat));
    same_row_true_roll->next = same_row_true_roll_in;
    strcpy(same_row_true_roll_in->name, "Same Row True Roll In");
    same_row_true_roll_in->weight = 0;
    same_row_true_roll_in->length = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_same_row_true_roll_in(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            same_row_true_roll_in->ngrams[i] = i;
            same_row_true_roll_in->length++;
        }
        else
        {
            same_row_true_roll_in->ngrams[i] = -1;
        }
    }

    quad_stat *same_row_true_roll_out = (quad_stat *)malloc(sizeof(quad_stat));
    same_row_true_roll_in->next = same_row_true_roll_out;
    strcpy(same_row_true_roll_out->name, "Same Row True Roll Out");
    same_row_true_roll_out->weight = 0;
    same_row_true_roll_out->length = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_same_row_true_roll_out(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            same_row_true_roll_out->ngrams[i] = i;
            same_row_true_roll_out->length++;
        }
        else
        {
            same_row_true_roll_out->ngrams[i] = -1;
        }
    }

    quad_stat *adjacent_finger_true_roll = (quad_stat *)malloc(sizeof(quad_stat));
    same_row_true_roll_out->next = adjacent_finger_true_roll;
    strcpy(adjacent_finger_true_roll->name, "Adjacent Finger True Roll");
    adjacent_finger_true_roll->weight = 0;
    adjacent_finger_true_roll->length = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_adjacent_finger_true_roll(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            adjacent_finger_true_roll->ngrams[i] = i;
            adjacent_finger_true_roll->length++;
        }
        else
        {
            adjacent_finger_true_roll->ngrams[i] = -1;
        }
    }

    quad_stat *adjacent_finger_true_roll_in = (quad_stat *)malloc(sizeof(quad_stat));
    adjacent_finger_true_roll->next = adjacent_finger_true_roll_in;
    strcpy(adjacent_finger_true_roll_in->name, "Adjacent Finger True Roll In");
    adjacent_finger_true_roll_in->weight = 0;
    adjacent_finger_true_roll_in->length = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_adjacent_finger_true_roll_in(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            adjacent_finger_true_roll_in->ngrams[i] = i;
            adjacent_finger_true_roll_in->length++;
        }
        else
        {
            adjacent_finger_true_roll_in->ngrams[i] = -1;
        }
    }

    quad_stat *adjacent_finger_true_roll_out = (quad_stat *)malloc(sizeof(quad_stat));
    adjacent_finger_true_roll_in->next = adjacent_finger_true_roll_out;
    strcpy(adjacent_finger_true_roll_out->name, "Adjacent Finger True Roll Out");
    adjacent_finger_true_roll_out->weight = 0;
    adjacent_finger_true_roll_out->length = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_adjacent_finger_true_roll_out(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            adjacent_finger_true_roll_out->ngrams[i] = i;
            adjacent_finger_true_roll_out->length++;
        }
        else
        {
            adjacent_finger_true_roll_out->ngrams[i] = -1;
        }
    }

    quad_stat *same_row_adjacent_finger_true_roll = (quad_stat *)malloc(sizeof(quad_stat));
    adjacent_finger_true_roll_out->next = same_row_adjacent_finger_true_roll;
    strcpy(same_row_adjacent_finger_true_roll->name, "Same Row Adjacent Finger True Roll");
    same_row_adjacent_finger_true_roll->weight = 0;
    same_row_adjacent_finger_true_roll->length = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_same_row_adjacent_finger_true_roll(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            same_row_adjacent_finger_true_roll->ngrams[i] = i;
            same_row_adjacent_finger_true_roll->length++;
        }
        else
        {
            same_row_adjacent_finger_true_roll->ngrams[i] = -1;
        }
    }

    quad_stat *same_row_adjacent_finger_true_roll_in = (quad_stat *)malloc(sizeof(quad_stat));
    same_row_adjacent_finger_true_roll->next = same_row_adjacent_finger_true_roll_in;
    strcpy(same_row_adjacent_finger_true_roll_in->name, "Same Row Adjacent Finger True Roll In");
    same_row_adjacent_finger_true_roll_in->weight = 0;
    same_row_adjacent_finger_true_roll_in->length = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_same_row_adjacent_finger_true_roll_in(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            same_row_adjacent_finger_true_roll_in->ngrams[i] = i;
            same_row_adjacent_finger_true_roll_in->length++;
        }
        else
        {
            same_row_adjacent_finger_true_roll_in->ngrams[i] = -1;
        }
    }

    quad_stat *same_row_adjacent_finger_true_roll_out = (quad_stat *)malloc(sizeof(quad_stat));
    same_row_adjacent_finger_true_roll_in->next = same_row_adjacent_finger_true_roll_out;
    strcpy(same_row_adjacent_finger_true_roll_out->name, "Same Row Adjacent Finger True Roll Out");
    same_row_adjacent_finger_true_roll_out->weight = 0;
    same_row_adjacent_finger_true_roll_out->length = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_same_row_adjacent_finger_true_roll_out(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            same_row_adjacent_finger_true_roll_out->ngrams[i] = i;
            same_row_adjacent_finger_true_roll_out->length++;
        }
        else
        {
            same_row_adjacent_finger_true_roll_out->ngrams[i] = -1;
        }
    }

    quad_stat *chained_roll = (quad_stat *)malloc(sizeof(quad_stat));
    same_row_adjacent_finger_true_roll_out->next = chained_roll;
    strcpy(chained_roll->name, "Chained Roll");
    chained_roll->weight = 0;
    chained_roll->length = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_chained_roll(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            chained_roll->ngrams[i] = i;
            chained_roll->length++;
        }
        else
        {
            chained_roll->ngrams[i] = -1;
        }
    }

    quad_stat *chained_roll_in = (quad_stat *)malloc(sizeof(quad_stat));
    chained_roll->next = chained_roll_in;
    strcpy(chained_roll_in->name, "Chained Roll In");
    chained_roll_in->weight = 0;
    chained_roll_in->length = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_chained_roll_in(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            chained_roll_in->ngrams[i] = i;
            chained_roll_in->length++;
        }
        else
        {
            chained_roll_in->ngrams[i] = -1;
        }
    }

    quad_stat *chained_roll_out = (quad_stat *)malloc(sizeof(quad_stat));
    chained_roll_in->next = chained_roll_out;
    strcpy(chained_roll_out->name, "Chained Roll Out");
    chained_roll_out->weight = 0;
    chained_roll_out->length = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_chained_roll_out(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            chained_roll_out->ngrams[i] = i;
            chained_roll_out->length++;
        }
        else
        {
            chained_roll_out->ngrams[i] = -1;
        }
    }

    quad_stat *chained_roll_mix = (quad_stat *)malloc(sizeof(quad_stat));
    chained_roll_out->next = chained_roll_mix;
    strcpy(chained_roll_mix->name, "Chained Roll Mix");
    chained_roll_mix->weight = 0;
    chained_roll_mix->length = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_chained_roll_mix(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            chained_roll_mix->ngrams[i] = i;
            chained_roll_mix->length++;
        }
        else
        {
            chained_roll_mix->ngrams[i] = -1;
        }
    }

    quad_stat *same_row_chained_roll = (quad_stat *)malloc(sizeof(quad_stat));
    chained_roll_mix->next = same_row_chained_roll;
    strcpy(same_row_chained_roll->name, "Same Row Chained Roll");
    same_row_chained_roll->weight = 0;
    same_row_chained_roll->length = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_same_row_chained_roll(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            same_row_chained_roll->ngrams[i] = i;
            same_row_chained_roll->length++;
        }
        else
        {
            same_row_chained_roll->ngrams[i] = -1;
        }
    }

    quad_stat *same_row_chained_roll_in = (quad_stat *)malloc(sizeof(quad_stat));
    same_row_chained_roll->next = same_row_chained_roll_in;
    strcpy(same_row_chained_roll_in->name, "Same Row Chained Roll In");
    same_row_chained_roll_in->weight = 0;
    same_row_chained_roll_in->length = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_same_row_chained_roll_in(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            same_row_chained_roll_in->ngrams[i] = i;
            same_row_chained_roll_in->length++;
        }
        else
        {
            same_row_chained_roll_in->ngrams[i] = -1;
        }
    }

    quad_stat *same_row_chained_roll_out = (quad_stat *)malloc(sizeof(quad_stat));
    same_row_chained_roll_in->next = same_row_chained_roll_out;
    strcpy(same_row_chained_roll_out->name, "Same Row Chained Roll Out");
    same_row_chained_roll_out->weight = 0;
    same_row_chained_roll_out->length = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_same_row_chained_roll_out(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            same_row_chained_roll_out->ngrams[i] = i;
            same_row_chained_roll_out->length++;
        }
        else
        {
            same_row_chained_roll_out->ngrams[i] = -1;
        }
    }

    quad_stat *same_row_chained_roll_mix = (quad_stat *)malloc(sizeof(quad_stat));
    same_row_chained_roll_out->next = same_row_chained_roll_mix;
    strcpy(same_row_chained_roll_mix->name, "Same Row Chained Roll Mix");
    same_row_chained_roll_mix->weight = 0;
    same_row_chained_roll_mix->length = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_same_row_chained_roll_mix(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            same_row_chained_roll_mix->ngrams[i] = i;
            same_row_chained_roll_mix->length++;
        }
        else
        {
            same_row_chained_roll_mix->ngrams[i] = -1;
        }
    }

    quad_stat *adjacent_finger_chained_roll = (quad_stat *)malloc(sizeof(quad_stat));
    same_row_chained_roll_mix->next = adjacent_finger_chained_roll;
    strcpy(adjacent_finger_chained_roll->name, "Adjacent Finger Chained Roll");
    adjacent_finger_chained_roll->weight = 0;
    adjacent_finger_chained_roll->length = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_adjacent_finger_chained_roll(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            adjacent_finger_chained_roll->ngrams[i] = i;
            adjacent_finger_chained_roll->length++;
        }
        else
        {
            adjacent_finger_chained_roll->ngrams[i] = -1;
        }
    }

    quad_stat *adjacent_finger_chained_roll_in = (quad_stat *)malloc(sizeof(quad_stat));
    adjacent_finger_chained_roll->next = adjacent_finger_chained_roll_in;
    strcpy(adjacent_finger_chained_roll_in->name, "Adjacent Finger Chained Roll In");
    adjacent_finger_chained_roll_in->weight = 0;
    adjacent_finger_chained_roll_in->length = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_adjacent_finger_chained_roll_in(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            adjacent_finger_chained_roll_in->ngrams[i] = i;
            adjacent_finger_chained_roll_in->length++;
        }
        else
        {
            adjacent_finger_chained_roll_in->ngrams[i] = -1;
        }
    }

    quad_stat *adjacent_finger_chained_roll_out = (quad_stat *)malloc(sizeof(quad_stat));
    adjacent_finger_chained_roll_in->next = adjacent_finger_chained_roll_out;
    strcpy(adjacent_finger_chained_roll_out->name, "Adjacent Finger Chained Roll Out");
    adjacent_finger_chained_roll_out->weight = 0;
    adjacent_finger_chained_roll_out->length = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_adjacent_finger_chained_roll_out(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            adjacent_finger_chained_roll_out->ngrams[i] = i;
            adjacent_finger_chained_roll_out->length++;
        }
        else
        {
            adjacent_finger_chained_roll_out->ngrams[i] = -1;
        }
    }

    quad_stat *adjacent_finger_chained_roll_mix = (quad_stat *)malloc(sizeof(quad_stat));
    adjacent_finger_chained_roll_out->next = adjacent_finger_chained_roll_mix;
    strcpy(adjacent_finger_chained_roll_mix->name, "Adjacent Finger Chained Roll Mix");
    adjacent_finger_chained_roll_mix->weight = 0;
    adjacent_finger_chained_roll_mix->length = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_adjacent_finger_chained_roll_mix(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            adjacent_finger_chained_roll_mix->ngrams[i] = i;
            adjacent_finger_chained_roll_mix->length++;
        }
        else
        {
            adjacent_finger_chained_roll_mix->ngrams[i] = -1;
        }
    }

    quad_stat *same_row_adjacent_finger_chained_roll = (quad_stat *)malloc(sizeof(quad_stat));
    adjacent_finger_chained_roll_mix->next = same_row_adjacent_finger_chained_roll;
    strcpy(same_row_adjacent_finger_chained_roll->name, "Same Row Adjacent Finger Chained Roll");
    same_row_adjacent_finger_chained_roll->weight = 0;
    same_row_adjacent_finger_chained_roll->length = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_same_row_adjacent_finger_chained_roll(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            same_row_adjacent_finger_chained_roll->ngrams[i] = i;
            same_row_adjacent_finger_chained_roll->length++;
        }
        else
        {
            same_row_adjacent_finger_chained_roll->ngrams[i] = -1;
        }
    }

    quad_stat *same_row_adjacent_finger_chained_roll_in = (quad_stat *)malloc(sizeof(quad_stat));
    same_row_adjacent_finger_chained_roll->next = same_row_adjacent_finger_chained_roll_in;
    strcpy(same_row_adjacent_finger_chained_roll_in->name, "Same Row Adjacent Finger Chained Roll In");
    same_row_adjacent_finger_chained_roll_in->weight = 0;
    same_row_adjacent_finger_chained_roll_in->length = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_same_row_adjacent_finger_chained_roll_in(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            same_row_adjacent_finger_chained_roll_in->ngrams[i] = i;
            same_row_adjacent_finger_chained_roll_in->length++;
        }
        else
        {
            same_row_adjacent_finger_chained_roll_in->ngrams[i] = -1;
        }
    }

    quad_stat *same_row_adjacent_finger_chained_roll_out = (quad_stat *)malloc(sizeof(quad_stat));
    same_row_adjacent_finger_chained_roll_in->next = same_row_adjacent_finger_chained_roll_out;
    strcpy(same_row_adjacent_finger_chained_roll_out->name, "Same Row Adjacent Finger Chained Roll Out");
    same_row_adjacent_finger_chained_roll_out->weight = 0;
    same_row_adjacent_finger_chained_roll_out->length = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_same_row_adjacent_finger_chained_roll_out(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            same_row_adjacent_finger_chained_roll_out->ngrams[i] = i;
            same_row_adjacent_finger_chained_roll_out->length++;
        }
        else
        {
            same_row_adjacent_finger_chained_roll_out->ngrams[i] = -1;
        }
    }

    quad_stat *same_row_adjacent_finger_chained_roll_mix = (quad_stat *)malloc(sizeof(quad_stat));
    same_row_adjacent_finger_chained_roll_out->next = same_row_adjacent_finger_chained_roll_mix;
    strcpy(same_row_adjacent_finger_chained_roll_mix->name, "Same Row Adjacent Finger Chained Roll Mix");
    same_row_adjacent_finger_chained_roll_mix->weight = 0;
    same_row_adjacent_finger_chained_roll_mix->length = 0;
    for (int i = 0; i < DIM4; i++)
    {
        unflat_quad(i, &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
        if (is_same_row_adjacent_finger_chained_roll_mix(row0, col0, row1, col1, row2, col2, row3, col3))
        {
            same_row_adjacent_finger_chained_roll_mix->ngrams[i] = i;
            same_row_adjacent_finger_chained_roll_mix->length++;
        }
        else
        {
            same_row_adjacent_finger_chained_roll_mix->ngrams[i] = -1;
        }
    }

    same_row_adjacent_finger_chained_roll_mix->next = NULL;
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
    while (quad_head != NULL && (quad_head->length == 0)) {
        quad_stat *temp = quad_head;
        quad_head = quad_head->next;
        free(temp);
    }

    quad_stat *current = quad_head;
    while (current != NULL && current->next != NULL) {
        if (current->next->length == 0) {
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
    stats_quad = (quad_stat *)malloc(sizeof(quad_stat) * QUAD_END);
    quad_stat *current_quad = quad_head;
    for (int i = 0; i < QUAD_END; i++) {
        memcpy(&stats_quad[i], current_quad, sizeof(quad_stat));
        stats_quad[i].next = NULL; // Set next pointer to NULL
        current_quad = current_quad->next;
    }
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
