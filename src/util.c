/*
 * util.c - Utility functions for the GULAG project.
 *
 * Author: Rus Doomer
 *
 * Description: This file contains various utility functions used throughout
 *              the GULAG project, including error handling, data structure
 *              manipulation, and other generic helper functions.
 */

#include <wchar.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "util.h"
#include "global.h"
#include "io.h"
#include "io_util.h"

/*
 * Error handling function.
 * Shows the cursor, prints an error message to standard error, and terminates the program.
 * Parameters:
 *   msg: The error message to be displayed.
 * Returns: Does not return (terminates program).
 */
void error(const char *msg)
{
    fflush(stdout);
    /* show cursor */
    wprintf(L"\e[?25h");
    fprintf(stderr, "\nERROR: %s\n", msg);
    exit(EXIT_FAILURE);
}

/*
 * Flattens an 8D matrix coordinate into a 1D index.
 * Parameters:
 *   row0, col0, row1, col1, row2, col2, row3, col3: Row and column indices.
 *   i: Pointer to store the flattened index.
 * Returns: void.
 */
void flat_quad(int row0, int col0, int row1, int col1, int row2, int col2,
    int row3, int col3, int *i)
{
    *i = ((row0 * COL + col0) * DIM3) +
         ((row1 * COL + col1) * DIM2) +
         ((row2 * COL + col2) * DIM1) +
         (row3 * COL + col3);
}

/*
 * Unflattens a 1D index into a 8D matrix coordinate.
 * Parameters:
 *   i: The flattened index.
 *   row0, col0, row1, col1, row2, col2, row3, col3: Pointers to store the
 *                                                   row and column indices.
 * Returns: void.
 */
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

/*
 * Flattens a 6D matrix coordinate into a 1D index.
 * Parameters:
 *   row0, col0, row1, col1, row2, col2: Row and column indices.
 *   i: Pointer to store the flattened index.
 * Returns: void.
 */
void flat_tri(int row0, int col0, int row1, int col1, int row2, int col2,
    int *i)
{
    *i = ((row0 * COL + col0) * DIM2) +
         ((row1 * COL + col1) * DIM1) +
         (row2 * COL + col2);
}

/*
 * Unflattens a 1D index into a 6D matrix coordinate.
 * Parameters:
 *   i: The flattened index.
 *   row0, col0, row1, col1, row2, col2: Pointers to store the row and
 *                                       column indices.
 * Returns: void.
 */
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

/*
 * Flattens a 4D matrix coordinate into a 1D index.
 * Parameters:
 *   row0, col0, row1, col1: Row and column indices.
 *   i: Pointer to store the flattened index.
 * Returns: void.
 */
void flat_bi(int row0, int col0, int row1, int col1, int *i)
{
    *i = ((row0 * COL + col0) * DIM1) +
         (row1 * COL + col1);
}

/*
 * Unflattens a 1D index into a 4D matrix coordinate.
 * Parameters:
 *   i: The flattened index.
 *   row0, col0, row1, col1: Pointers to store the row and column indices.
 * Returns: void.
 */
void unflat_bi(int i, int *row0, int *col0, int *row1, int *col1)
{
    *row1 = (i % (DIM1)) / COL;
    *col1 = i % COL;
    i /= (DIM1);

    *row0 = i / COL;
    *col0 = i % COL;
}

/*
 * Flattens a 2D matrix coordinate into a 1D index.
 * Parameters:
 *   row0, col0: Row and column indices.
 *   i: Pointer to store the flattened index.
 * Returns: void.
 */
void flat_mono(int row0, int col0, int *i)
{
    *i = row0 * COL + col0;
}

/*
 * Unflattens a 1D index into a 2D matrix coordinate.
 * Parameters:
 *   i: The flattened index.
 *   row0, col0: Pointers to store the row and column indices.
 * Returns: void.
 */
void unflat_mono(int i, int *row0, int *col0)
{
    *row0 = i / COL;
    *col0 = i % COL;
}

/*
 * Calculates the index for a monogram in a linearized array.
 * Parameters:
 *   i: The index of the character in the language array.
 * Returns: The index in the linearized monogram array.
 */
size_t index_mono(int i) {
    return i;
}

/*
 * Calculates the index for a bigram in a linearized array.
 * Parameters:
 *   i, j: The indices of the characters in the language array.
 * Returns: The index in the linearized bigram array.
 */
size_t index_bi(int i, int j) {
    return i * LANG_LENGTH + j;
}

/*
 * Calculates the index for a trigram in a linearized array.
 * Parameters:
 *   i, j, k: The indices of the characters in the language array.
 * Returns: The index in the linearized trigram array.
 */
size_t index_tri(int i, int j, int k) {
    return i * LANG_LENGTH * LANG_LENGTH + j * LANG_LENGTH + k;
}

/*
 * Calculates the index for a quadgram in a linearized array.
 * Parameters:
 *   i, j, k, l: The indices of the characters in the language array.
 * Returns: The index in the linearized quadgram array.
 */
size_t index_quad(int i, int j, int k, int l) {
    return i * LANG_LENGTH * LANG_LENGTH * LANG_LENGTH + j * LANG_LENGTH * LANG_LENGTH + k * LANG_LENGTH + l;
}

/*
 * Calculates the index for a skipgram in a linearized array.
 * Parameters:
 *   skip_index: The skip distance (1-9).
 *   j, k: The indices of the characters in the language array.
 * Returns: The index in the linearized skipgram array.
 */
size_t index_skip(int skip_index, int j, int k) {
    return skip_index * LANG_LENGTH * LANG_LENGTH + j * LANG_LENGTH + k;
}

/*
 * Normalizes the corpus data from raw frequencies to percentages.
 * Returns: void.
 */
void normalize_corpus()
{
    long long total_mono = 0;
    long long total_bi = 0;
    long long total_tri = 0;
    long long total_quad = 0;
    long long total_skip[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    log_print('n',L"Calculating totals... ");

    for (int i = 0; i < LANG_LENGTH; i++) {
        total_mono += corpus_mono[i];
        for (int j = 0; j < LANG_LENGTH; j++) {
            total_bi += corpus_bi[i][j];
            for (int k = 0; k < LANG_LENGTH; k++) {
                total_tri += corpus_tri[i][j][k];
                for (int l = 0; l < LANG_LENGTH; l++) {
                    total_quad += corpus_quad[i][j][k][l];
                }
            }
        }
    }

    for (int i = 1; i <= 9; i++) {
        for (int j = 0; j < LANG_LENGTH; j++) {
            for (int k = 0; k < LANG_LENGTH; k++) {
                total_skip[i] += corpus_skip[i][j][k];
            }
        }
    }

    log_print('n',L"Normalizing... ");

    if (total_mono > 0) {
        for (int i = 0; i < LANG_LENGTH; i++) {
            linear_mono[index_mono(i)] = (float)corpus_mono[i] * 100 / total_mono;
        }
    }

    if (total_bi > 0) {
        for (int i = 0; i < LANG_LENGTH; i++) {
            for (int j = 0; j < LANG_LENGTH; j++) {
                linear_bi[index_bi(i, j)] = (float)corpus_bi[i][j] * 100 / total_bi;
            }
        }
    }

    if (total_tri > 0) {
        for (int i = 0; i < LANG_LENGTH; i++) {
            for (int j = 0; j < LANG_LENGTH; j++) {
                for (int k = 0; k < LANG_LENGTH; k++) {
                    linear_tri[index_tri(i, j, k)] = (float)corpus_tri[i][j][k] * 100 / total_tri;
                }
            }
        }
    }

    if (total_quad > 0) {
        for (int i = 0; i < LANG_LENGTH; i++) {
            for (int j = 0; j < LANG_LENGTH; j++) {
                for (int k = 0; k < LANG_LENGTH; k++) {
                    for (int l = 0; l < LANG_LENGTH; l++) {
                        linear_quad[index_quad(i, j, k, l)] = (float)corpus_quad[i][j][k][l] * 100 / total_quad;
                    }
                }
            }
        }
    }

    if (total_skip > 0) {
        for (int i = 1; i <= 9; i++) {
            for (int j = 0; j < LANG_LENGTH; j++) {
                for (int k = 0; k < LANG_LENGTH; k++) {
                    linear_skip[index_skip(i, j, k)] = (float)corpus_skip[i][j][k] * 100 / total_skip[i];
                }
            }
        }
    }
}

/*
 * Allocates memory for a new layout data structure.
 * Parameters:
 *   lt: Pointer to a layout pointer where the newly allocated layout will be stored.
 * Returns: void.
 */
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

/*
 * Frees the memory occupied by a layout data structure.
 * Parameters:
 *   lt: Pointer to the layout to be freed.
 * Returns: void.
 */
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

/*
 * Calculates and assigns the overall score to a layout based on its statistics.
 * Parameters:
 *   lt: Pointer to the layout.
 * Returns: void.
 */
void get_score(layout *lt)
{
    lt->score = 0;
    for (int i = 0; i < MONO_END; i++)
    {
        lt->score += lt->mono_score[i] * stats_mono[i].weight;
    }
    for (int i = 0; i < BI_END; i++)
    {
        lt->score += lt->bi_score[i] * stats_bi[i].weight;
    }
    for (int i = 0; i < TRI_END; i++)
    {
        lt->score += lt->tri_score[i] * stats_tri[i].weight;
    }
    for (int i = 0; i < QUAD_END; i++)
    {
        lt->score += lt->quad_score[i] * stats_quad[i].weight;
    }
    for (int i = 1; i <= 9; i++)
    {
        for (int j = 0; j < SKIP_END; j++)
        {
            lt->score += lt->skip_score[i][j] * stats_skip[j].weight[i];
        }
    }
    for (int i = 0; i < META_END; i++)
    {
        lt->score += lt->meta_score[i] * stats_meta[i].weight;
    }
}

/*
 * Calculates the difference between two layouts and stores the result in a third layout.
 * Parameters:
 *   lt: Pointer to the first layout.
 *   lt2: Pointer to the second layout.
 *   lt_diff: Pointer to the layout where the difference will be stored.
 * Returns: void.
 */
void get_layout_diff(layout *lt, layout *lt2, layout *lt_diff)
{
    char truncated_layout_name[49];
    char truncated_layout2_name[49];

    /* truncate both layout names to ensure they fit when combined */
    strncpy(truncated_layout_name, layout_name, sizeof(truncated_layout_name) - 1);
    truncated_layout_name[sizeof(truncated_layout_name) - 1] = '\0';

    strncpy(truncated_layout2_name, layout2_name, sizeof(truncated_layout2_name) - 1);
    truncated_layout2_name[sizeof(truncated_layout2_name) - 1] = '\0';

    snprintf(lt_diff->name, sizeof(lt_diff->name), "%s - %s", truncated_layout_name, truncated_layout2_name);

    /* copy matrix filling non-matching keys with @ */
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            if (lt->matrix[i][j] == lt2->matrix[i][j]) {
                lt_diff->matrix[i][j] = lt->matrix[i][j];
            } else {
                lt_diff->matrix[i][j] = -1;
            }
        }
    }

    /* calculate score differences */
    lt_diff->score = lt->score - lt2->score;

    for (int i = 0; i < MONO_END; i++) {
        lt_diff->mono_score[i] = lt->mono_score[i] - lt2->mono_score[i];
    }

    for (int i = 0; i < BI_END; i++) {
        lt_diff->bi_score[i] = lt->bi_score[i] - lt2->bi_score[i];
    }

    for (int i = 0; i < TRI_END; i++) {
        lt_diff->tri_score[i] = lt->tri_score[i] - lt2->tri_score[i];
    }

    for (int i = 0; i < QUAD_END; i++) {
        lt_diff->quad_score[i] = lt->quad_score[i] - lt2->quad_score[i];
    }

    for (int h = 1; h < 10; h++) {
        for (int i = 0; i < SKIP_END; i++) {
            lt_diff->skip_score[h][i] = lt->skip_score[h][i] - lt2->skip_score[h][i];
        }
    }

    for (int i = 0; i < META_END; i++) {
        lt_diff->meta_score[i] = lt->meta_score[i] - lt2->meta_score[i];
    }
}

/*
 * Creates a new node for the layout ranking list and inserts it in the correct position.
 * Parameters:
 *   lt: Pointer to the layout to be added to the ranking.
 * Returns: void.
 */
void create_node(layout *lt)
{
    layout_node *new_node = (layout_node *)malloc(sizeof(layout_node));

    /* Deep copy the name and score from the layout to the new node */
    strncpy(new_node->name, lt->name, sizeof(lt->name));
    new_node->score = lt->score;

    /* Initialize the next pointer to NULL */
    new_node->next = NULL;

     /* If the list is empty or the new node's score is higher than the head node's score */
    if (head_node == NULL || new_node->score > head_node->score) {
        new_node->next = head_node;
        head_node = new_node;
    } else {
        /* Traverse the list to find the correct position */
        layout_node *current = head_node;
        while (current->next != NULL && current->next->score >= new_node->score) {
            current = current->next;
        }
        /* Insert the new node at the correct position */
        new_node->next = current->next;
        current->next = new_node;
    }
}

/*
 * Frees all nodes in the layout ranking list.
 * Returns: void.
 */
void free_list() {
    layout_node* current = head_node;
    layout_node* next_node;
    while (current != NULL) {
        next_node = current->next;
        free(current);
        current = next_node;
    }
}

/*
 * Randomly shuffles the keys in a layout.
 * Parameters:
 *   lt: Pointer to the layout to be shuffled.
 * Returns: void.
 */
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

/*
 * Copies the contents of one layout to another.
 * Parameters:
 *   lt_dest: Pointer to the destination layout.
 *   lt_src: Pointer to the source layout.
 * Returns: void.
 */
void copy(layout *lt_dest, layout *lt_src)
{
    strcpy(lt_dest->name, lt_src->name);
    for (int i = 0; i < ROW; i++)
    {
        for (int j = 0; j < COL; j++)
        {
            lt_dest->matrix[i][j] = lt_src->matrix[i][j];
        }
    }
    lt_dest->score = lt_src->score;
    for (int i = 0; i < MONO_END; i++)
    {
        lt_dest->mono_score[i] = lt_src->mono_score[i];
    }
    for (int i = 0; i < BI_END; i++)
    {
        lt_dest->bi_score[i] = lt_src->bi_score[i];
    }
    for (int i = 0; i < TRI_END; i++)
    {
        lt_dest->tri_score[i] = lt_src->tri_score[i];
    }
    for (int i = 0; i < QUAD_END; i++)
    {
        lt_dest->quad_score[i] = lt_src->quad_score[i];
    }
    for (int i = 0; i < META_END; i++)
    {
        lt_dest->meta_score[i] = lt_src->meta_score[i];
    }
    for (int j = 1; j <= 9; j++)
    {
        for (int i = 0; i < SKIP_END; i++)
        {
            lt_dest->skip_score[j][i] = lt_src->skip_score[j][i];
        }
    }
}

/*
 * Generates a random float between 0 and 1.
 * Returns: The generated random float.
 */
float random_float() {
    return (float)rand() / RAND_MAX;
}

