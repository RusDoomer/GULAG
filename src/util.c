#include <wchar.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "util.h"
#include "global.h"
#include "io.h"
#include "io_util.h"

void error(const char *msg)
{
    fflush(stdout);
    //show cursor
    wprintf(L"\e[?25h");
    fprintf(stderr, "\nERROR: %s\n", msg);
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

size_t index_mono(int i) {
    return i;
}

size_t index_bi(int i, int j) {
    return i * LANG_LENGTH + j;
}

size_t index_tri(int i, int j, int k) {
    return i * LANG_LENGTH * LANG_LENGTH + j * LANG_LENGTH + k;
}

size_t index_quad(int i, int j, int k, int l) {
    return i * LANG_LENGTH * LANG_LENGTH * LANG_LENGTH + j * LANG_LENGTH * LANG_LENGTH + k * LANG_LENGTH + l;
}

size_t index_skip(int skip_index, int j, int k) {
    return skip_index * LANG_LENGTH * LANG_LENGTH + j * LANG_LENGTH + k;
}

void normalize_corpus()
{
    long long total_mono = 0;
    long long total_bi = 0;
    long long total_tri = 0;
    long long total_quad = 0;
    long long total_skip[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    wprintf(L"Calculating totals... ");

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

    wprintf(L"Normalizing... ");
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
    wprintf(L"Done\n\n");
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

void get_layout_diff(layout *lt, layout *lt2, layout *lt_diff)
{
    char truncated_layout_name[49];
    char truncated_layout2_name[49];

    strncpy(truncated_layout_name, layout_name, sizeof(truncated_layout_name) - 1);
    truncated_layout_name[sizeof(truncated_layout_name) - 1] = '\0';

    strncpy(truncated_layout2_name, layout2_name, sizeof(truncated_layout2_name) - 1);
    truncated_layout2_name[sizeof(truncated_layout2_name) - 1] = '\0';

    snprintf(lt_diff->name, sizeof(lt_diff->name), "%s - %s", truncated_layout_name, truncated_layout2_name);

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

void create_node(layout *lt)
{
    layout_node *new_node = (layout_node *)malloc(sizeof(layout_node));

    // Deep copy the name and score from the layout to the new node
    strncpy(new_node->name, lt->name, sizeof(lt->name));
    new_node->score = lt->score;

    // Initialize the next pointer to NULL
    new_node->next = NULL;

    // If the list is empty or the new node's score is higher than the head node's score
    if (head_node == NULL || new_node->score > head_node->score) {
        new_node->next = head_node;
        head_node = new_node;
    } else {
        // Traverse the list to find the correct position
        layout_node *current = head_node;
        while (current->next != NULL && current->next->score >= new_node->score) {
            current = current->next;
        }
        // Insert the new node at the correct position
        new_node->next = current->next;
        current->next = new_node;
    }
}

void free_list() {
    layout_node* current = head_node;
    layout_node* next_node;
    while (current != NULL) {
        next_node = current->next;
        free(current);
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
