#include <wchar.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "util.h"
#include "global.h"
#include "io.h"

void error(const char *msg)
{
    //show cursor
    wprintf(L"\e[?25h");
    fprintf(stderr, "ERROR: %s\n", msg);
    exit(EXIT_FAILURE);
}

void flat_quad(int row0, int col0, int row1, int col1, int row2, int col2,
    int row3, int col3, int *i)
{
    *i = ((row0 * COL + col0) * DIM3) +
         ((row1 * COL + col1) * DIM2) +
         ((row2 * COL + col2) * DIM1) +
         (row3 * COL + col3);
}

void unflat_quad(int i, int *row0, int *col0, int *row1, int *col1,
    int *row2, int *col2, int *row3, int *col3)
{
    *row3 = (i % (DIM1)) / COL;
    *col3 = i % COL;
    i /= (DIM1);

    *row2 = (i % (DIM1)) / COL;
    *col2 = i % COL;
    i /= (DIM1);

    *row1 = (i % (DIM1)) / COL;
    *col1 = i % COL;
    i /= (DIM1);

    *row0 = i / COL;
    *col0 = i % COL;
}

void flat_tri(int row0, int col0, int row1, int col1, int row2, int col2,
    int *i)
{
    *i = ((row0 * COL + col0) * DIM2) +
         ((row1 * COL + col1) * DIM1) +
         (row2 * COL + col2);
}

void unflat_tri(int i, int *row0, int *col0, int *row1, int *col1,
    int *row2, int *col2)
{
    *row2 = (i % (DIM1)) / COL;
    *col2 = i % COL;
    i /= (DIM1);

    *row1 = (i % (DIM1)) / COL;
    *col1 = i % COL;
    i /= (DIM1);

    *row0 = i / COL;
    *col0 = i % COL;
}

void flat_bi(int row0, int col0, int row1, int col1, int *i)
{
    *i = ((row0 * COL + col0) * DIM1) +
         (row1 * COL + col1);
}

void unflat_bi(int i, int *row0, int *col0, int *row1, int *col1)
{
    *row1 = (i % (DIM1)) / COL;
    *col1 = i % COL;
    i /= (DIM1);

    *row0 = i / COL;
    *col0 = i % COL;
}

void flat_mono(int row0, int col0, int *i)
{
    *i = row0 * COL + col0;
}

void unflat_mono(int i, int *row0, int *col0)
{
    *row0 = i / COL;
    *col0 = i % COL;
}

// Function to check if layout is fully allocated
int is_layout_allocated(layout *lt) {

    return lt != NULL &&
           lt->mono_score != NULL &&
           lt->bi_score != NULL &&
           lt->tri_score != NULL &&
           lt->quad_score != NULL &&
           lt->skip_score != NULL &&
           lt->meta_score != NULL;
}

void alloc_layout(layout **lt)
{
    *lt = (layout *)malloc(sizeof(layout));
    if (*lt == NULL) {error("failed to malloc layout");}

    (*lt)->score = 0;

    (*lt)->mono_score = (float *)calloc(MONO_END, sizeof(float));
    (*lt)->bi_score = (float *)calloc(BI_END, sizeof(float));
    (*lt)->tri_score = (float *)calloc(TRI_END, sizeof(float));
    (*lt)->quad_score = (float *)calloc(QUAD_END, sizeof(float));
    (*lt)->skip_score = (float **)malloc(10 * sizeof(float *));
    for (int i = 1; i < 10; i++) {
        (*lt)->skip_score[i] = (float *)calloc(SKIP_END, sizeof(float));
    }
    (*lt)->meta_score = (float *)calloc(META_END, sizeof(float));
}

void free_layout(layout *lt)
{
    for (int i = 1; i < 10; i++) {
        free(lt->skip_score[i]);
    }
    free(lt->meta_score);
    free(lt->skip_score);
    free(lt->quad_score);
    free(lt->tri_score);
    free(lt->bi_score);
    free(lt->mono_score);

    free(lt);
}

void get_score(layout *lt)
{
    lt->score = 0;
    for (int i = 0; i < MONO_END; i++)
    {
        lt->score += lt->mono_score[i];
    }
    for (int i = 0; i < BI_END; i++)
    {
        lt->score += lt->bi_score[i];
    }
    for (int i = 0; i < TRI_END; i++)
    {
        lt->score += lt->tri_score[i];
    }
    for (int i = 0; i < QUAD_END; i++)
    {
        lt->score += lt->quad_score[i];
    }
    for (int i = 0; i > 10; i++)
    {
        for (int j = 0; j < SKIP_END; j++)
        {
            lt->score += lt->skip_score[i][j];
        }
    }
    for (int i = 0; i < META_END; i++)
    {
        lt->score += lt->meta_score[i];
    }
}

void get_layout_diff(layout *lt, layout *lt2, layout *lt_diff)
{
    error("get layout diff not implemented");
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            if (lt->matrix[i][j] == lt2->matrix[i][j]) {
                lt_diff->matrix[i][j] = lt->matrix[i][j];
            } else {
                lt_diff->matrix[i][j] = -1;
            }
        }
    }

    lt_diff->score = lt->score - lt2->score;

    lt_diff->mono_score[0] = 100;
    for (int i = 1; i < MONO_END; i++) {
        lt_diff->mono_score[i] =
            ((lt->mono_score[i] * 100) / lt->mono_score[0]) -
            ((lt2->mono_score[i] * 100) / lt2->mono_score[0]);
    }

    lt_diff->bi_score[0] = 100;
    for (int i = 1; i < BI_END; i++) {
        lt_diff->bi_score[i] =
            ((lt->bi_score[i] * 100) / lt->bi_score[0]) -
            ((lt2->bi_score[i] * 100) / lt2->bi_score[0]);
    }

    lt_diff->tri_score[0] = 100;
    for (int i = 1; i < TRI_END; i++) {
        lt_diff->tri_score[i] =
            ((lt->tri_score[i] * 100) / lt->tri_score[0]) -
            ((lt2->tri_score[i] * 100) / lt2->tri_score[0]);
    }

    lt_diff->quad_score[0] = 100;
    for (int i = 1; i < QUAD_END; i++) {
        lt_diff->quad_score[i] =
            ((lt->quad_score[i] * 100) / lt->quad_score[0]) -
            ((lt2->quad_score[i] * 100) / lt2->quad_score[0]);
    }

    for (int h = 1; h < 10; h++) {
        lt_diff->skip_score[h][0] = 100;
        for (int i = 1; i < SKIP_END; i++) {
            lt_diff->skip_score[h][i] =
                ((lt->skip_score[h][i] * 100) / lt->skip_score[h][0]) -
                ((lt2->skip_score[h][i] * 100) / lt2->skip_score[h][0]);
        }
    }

    for (int i = 0; i < META_END; i++) {
        lt_diff->meta_score[i] =
            ((lt->meta_score[i] * 100) / lt->mono_score[0]) -
            ((lt2->meta_score[i] * 100) / lt2->mono_score[0]);
    }
}

layout_node* create_node(const char* name, float score)
{
    layout_node *new_node = NULL;
    error("create node not implemented");
    return new_node;
}

void free_list() {
    layout_node* current = head_node;
    layout_node* next_node;
    while (current != NULL) {
        next_node = current->next;
        error("free list not implemented");
        current = next_node;
    }
}

void shuffle_layout(layout *lt)
{
    for (int i = DIM1 - 1; i > 0; i--) {
        int j = rand() % (i + 1);

        int i_row = i / COL;
        int i_col = i % COL;
        int j_row = j / COL;
        int j_col = j % COL;

        int temp = lt->matrix[i_row][i_col];
        lt->matrix[i_row][i_col] = lt->matrix[j_row][j_col];
        lt->matrix[j_row][j_col] = temp;
    }
}

void skeleton_copy(layout **lt_dest, layout *lt_src)
{
    error("skeleton copy not implemented");
}

void gen_swap(int **swaps, int reps, layout **lt)
{
    error("gen swap not implemented");
}

float random_float() {
    return (float)rand() / RAND_MAX;
}

void decide_swapbacks(int *swap_back, float *prev_scores, layout **lt,
    float temp, int threads)
{
    error("decide swapbacks not implemented");
}

void gen_swap_back(int **swaps, int *swap_back, layout **lt)
{
    error("gen swap back not implemented");
}

void imp_swap(int **swaps, int reps, layout **lt)
{
    error("imp swap not implemented");
}
