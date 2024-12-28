// kernel.cl

#include "structs.h"

#define ROW 3
#define COL 12
#define DIM1 ROW * COL
#define DIM2 DIM1 * DIM1
#define DIM3 DIM2 * DIM1
#define DIM4 DIM3 * DIM1
#define LANG_LENGTH 51

//global variables we have access to
/*
 * MONO_END - 0 to infinite
 * BI_END - 0 to infinite
 * TRI_END - 0 to infinite
 * QUAD_END - 0 to infinite
 * SKIP_END - 0 to infinite
 * META_END - 0 to infinite
 * THREADS - 1 to infinite
 * REPETITIONS - THREADS to infinite
 * MAX_SWAPS - 1 to DIM1 / 2
 * WORKERS - 1 to idk?
 * LEFT_HAND - 0 to MONO_END
 * RIGHT_HAND - 0 to MONO_END
 */

// New layout struct specific for opencl
typedef struct cl_layout {
    int matrix[ROW][COL];
    float mono_score[MONO_END];
    float bi_score[BI_END];
    float tri_score[TRI_END];
    float quad_score[QUAD_END];
    float skip_score[10][SKIP_END];
    float meta_score[META_END];
    float score;
} cl_layout;

// Copy matrix from normal layout to cl_layout
inline void copy_host_to_cl(__local cl_layout *lt_dest, __global layout *lt_src)
{
    for (int i = 0; i < ROW; i++)
    {
        for (int j = 0; j < COL; j++)
        {
            lt_dest->matrix[i][j] = lt_src->matrix[i][j];
        }
    }
}

// Copy matrix from cl_layout to normal layout
inline void copy_cl_to_host(__global layout *lt_dest, __local cl_layout *lt_src)
{
    for (int i = 0; i < ROW; i++)
    {
        for (int j = 0; j < COL; j++)
        {
            lt_dest->matrix[i][j] = lt_src->matrix[i][j];
        }
    }
}

/*
 * Flattens an 8D matrix coordinate into a 1D index.
 * Parameters:
 *   row0, col0, row1, col1, row2, col2, row3, col3: Row and column indices.
 *   i: Pointer to store the flattened index.
 * Returns: void.
 */
inline void flat_quad(int row0, int col0, int row1, int col1, int row2, int col2,
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
inline void unflat_quad(int i, int *row0, int *col0, int *row1, int *col1,
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
inline void flat_tri(int row0, int col0, int row1, int col1, int row2, int col2,
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
inline void unflat_tri(int i, int *row0, int *col0, int *row1, int *col1,
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
inline void flat_bi(int row0, int col0, int row1, int col1, int *i)
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
inline void unflat_bi(int i, int *row0, int *col0, int *row1, int *col1)
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
inline void flat_mono(int row0, int col0, int *i)
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
inline void unflat_mono(int i, int *row0, int *col0)
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

// Basic version of meta analysis only for hand balance
// I hope no one ever wants more complicated stats for meta analysis
inline void cl_meta_analysis(__local cl_layout *lt)
{
    lt->meta_score[0] = 0;
    lt->meta_score[0] += lt->mono_score[LEFT_HAND];
    lt->meta_score[0] -= lt->mono_score[RIGHT_HAND];
    /* Take absolute value. */
    if (lt->meta_score[0] < 0) {lt->meta_score[0] *= -1;}
}

/*
 * Calculates and assigns the overall score to a cl_layout based on its statistics.
 * Parameters:
 *   lt: Pointer to the cl_layout.
 * Returns: void.
 */
inline void cl_get_score(__local cl_layout *lt,
                  __constant mono_stat *stats_mono,
                  __constant bi_stat *stats_bi,
                  __constant tri_stat *stats_tri,
                  __constant quad_stat *stats_quad,
                  __constant skip_stat *stats_skip,
                  __constant meta_stat *stats_meta)
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

// Simple pseudo-random number generator (Park-Miller)
uint park_miller_rng(uint seed) {
    uint a = 16807;
    uint m = 2147483647;
    return (seed * a) % m;
}

// Function to shuffle the layout matrix (within the kernel)
void shuffle_layout(__global layout *lt, uint seed, int global_id) {
    for (int i = DIM1 - 1; i > 0; i--) {
        seed = park_miller_rng(seed + global_id); // Use a different seed for each thread
        int j = seed % (i + 1);

        int i_row = i / COL;
        int i_col = i % COL;
        int j_row = j / COL;
        int j_col = j % COL;

        // Swap elements
        int temp = lt->matrix[i_row][i_col];
        lt->matrix[i_row][i_col] = lt->matrix[j_row][j_col];
        lt->matrix[j_row][j_col] = temp;
    }
}

// Function to shuffle the layout matrix (within the kernel)
void shuffle_cl_layout(__local cl_layout *lt, uint seed, int global_id) {
    for (int i = DIM1 - 1; i > 0; i--) {
        seed = park_miller_rng(seed + global_id); // Use a different seed for each thread
        int j = seed % (i + 1);

        int i_row = i / COL;
        int i_col = i % COL;
        int j_row = j / COL;
        int j_col = j % COL;

        // Swap elements
        int temp = lt->matrix[i_row][i_col];
        lt->matrix[i_row][i_col] = lt->matrix[j_row][j_col];
        lt->matrix[j_row][j_col] = temp;
    }
}

// Kernel function
__kernel void improve_kernel(__constant float *linear_mono,
                             __constant float *linear_bi,
                             __constant float *linear_tri,
                             __constant float *linear_quad,
                             __constant float *linear_skip,
                             __constant mono_stat *stats_mono,
                             __constant bi_stat *stats_bi,
                             __constant tri_stat *stats_tri,
                             __constant quad_stat *stats_quad,
                             __constant skip_stat *stats_skip,
                             __constant meta_stat *stats_meta,
                             __global layout *lt,
                             __constant int *pins,
                             int seed) {
    // access pins like this pins[i][j] becomes pins[i * COL + j];
    size_t global_id = get_global_id(0);
    size_t group_id = get_group_id(0);
    size_t local_id = get_local_id(0);
    size_t total = get_global_size(0);

    __local cl_layout working;

    // Each worker copies a portion of the host layout to the local layout
    if (local_id == 0) {
        copy_host_to_cl(&working, lt);
        working.score = 0;
    }
    barrier(CLK_LOCAL_MEM_FENCE);

    for (int iter = 0; iter < REPETITIONS / THREADS; iter++)
    {
        if (local_id == 0) {
            seed = park_miller_rng(seed);
            shuffle_cl_layout(&working, seed, global_id);
            working.score = 0;
        }
        barrier(CLK_LOCAL_MEM_FENCE);

        int row0, col0, row1, col1, row2, col2, row3, col3;

        // Calculate monogram statistics.
        for (int i = local_id; i < MONO_END; i += WORKERS) {
            working.mono_score[i] = 0;
            int length = stats_mono[i].length;
            for (int j = 0; j < length; j++) {
                unflat_mono(stats_mono[i].ngrams[j], &row0, &col0);
                if (working.matrix[row0][col0] != -1) {
                    size_t index = index_mono(working.matrix[row0][col0]);
                    working.mono_score[i] += linear_mono[index];
                }
            }
        }

        // Calculate bigram statistics.
        for (int i = local_id; i < BI_END; i += WORKERS) {
            working.bi_score[i] = 0;
            int length = stats_bi[i].length;
            for (int j = 0; j < length; j++) {
                unflat_bi(stats_bi[i].ngrams[j], &row0, &col0, &row1, &col1);
                if (working.matrix[row0][col0] != -1 && working.matrix[row1][col1] != -1) {
                    size_t index = index_bi(working.matrix[row0][col0], working.matrix[row1][col1]);
                    working.bi_score[i] += linear_bi[index];
                }
            }
        }

        // Calculate trigram statistics.
        for (int i = local_id; i < TRI_END; i += WORKERS) {
            working.tri_score[i] = 0;
            int length = stats_tri[i].length;
            for (int j = 0; j < length; j++) {
                unflat_tri(stats_tri[i].ngrams[j], &row0, &col0, &row1, &col1, &row2, &col2);
                if (working.matrix[row0][col0] != -1 && working.matrix[row1][col1] != -1 && working.matrix[row2][col2] != -1) {
                    size_t index = index_tri(working.matrix[row0][col0], working.matrix[row1][col1], working.matrix[row2][col2]);
                    working.tri_score[i] += linear_tri[index];
                }
            }
        }

        // Calculate quadgram statistics.
        for (int i = local_id; i < QUAD_END; i += WORKERS) {
            working.quad_score[i] = 0;
            int length = stats_quad[i].length;
            for (int j = 0; j < length; j++) {
                unflat_quad(stats_quad[i].ngrams[j], &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
                if (working.matrix[row0][col0] != -1 && working.matrix[row1][col1] != -1 && working.matrix[row2][col2] != -1 && working.matrix[row3][col3] != -1) {
                    size_t index = index_quad(working.matrix[row0][col0], working.matrix[row1][col1], working.matrix[row2][col2], working.matrix[row3][col3]);
                    working.quad_score[i] += linear_quad[index];
                }
            }
        }

        // Calculate skipgram statistics.
        for (int i = local_id; i < SKIP_END; i += WORKERS) {
            int length = stats_skip[i].length;
            for (int k = 1; k <= 9; k++) {
                working.skip_score[k][i] = 0;
                for (int j = 0; j < length; j++) {
                    unflat_bi(stats_skip[i].ngrams[j], &row0, &col0, &row1, &col1);
                    if (working.matrix[row0][col0] != -1 && working.matrix[row1][col1] != -1) {
                        size_t index = index_skip(k, working.matrix[row0][col0], working.matrix[row1][col1]);
                        working.skip_score[k][i] += linear_skip[index];
                    }
                }
            }
        }

        barrier(CLK_LOCAL_MEM_FENCE);

        // Only the first worker runs meta_analysis and get_score
        if (local_id == 0) {
            cl_meta_analysis(&working);
            cl_get_score(&working, stats_mono, stats_bi, stats_tri, stats_quad, stats_skip, stats_meta);
        }
        barrier(CLK_LOCAL_MEM_FENCE);
    }
    barrier(CLK_GLOBAL_MEM_FENCE);
    // Copy the score back to the host layout
    if (global_id == total - 2) {lt->score = working.score;}
}
