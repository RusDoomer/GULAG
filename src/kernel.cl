/*
 * kernel.cl - CL implementation of improve and single_analyze functions for the GULAG.
 *
 * Author: Rus Doomer
 *
 * Description: Implements the simulated annealing loop and analysis from
 *              improve() in mode.c with opencl acceleration.
 */


#include "include/structs.h"

#define ROW 3
#define COL 12
#define DIM1 ROW * COL
#define DIM2 DIM1 * DIM1
#define DIM3 DIM2 * DIM1
#define DIM4 DIM3 * DIM1
#define LANG_LENGTH 51

/*
 * Global variables accessible to the kernel, defined in mode.c: [0 - infinite]
 * MONO_END, BI_END, TRI_END, QUAD_END, SKIP_END, META_END:
 *     Define the number of statistics for each ngram type.
 * THREADS: [1 - infinite]
 *     Number of threads to use for parallel processing.
 * REPETITIONS: [THREADS - infinite]
 *     Total number of layouts to analyze during generation.
 * MAX_SWAPS: [1 - DIM1/2]
 *     Maximum number of key swaps to perform in each iteration.
 * WORKERS: [Max of X_ENDs]
 *     Number of work items per work group.
 * LEFT_HAND, RIGHT_HAND:
 *     Indices for hand usage statistics.
 */

/*
 * New layout struct specific for OpenCL.
 * It's a simplified version of the host-side layout struct,
 * containing only necessary information for computation.
 */
typedef struct {
    int matrix[ROW][COL];
    float mono_score[MONO_END];
    float bi_score[BI_END];
    float tri_score[TRI_END];
    float quad_score[QUAD_END];
    float skip_score[10][SKIP_END];
    float meta_score[META_END];
    float score;
} cl_layout;

/*
 * Copy matrix and score from normal layout to cl_layout.
 * Parameters:
 *   lt_dest: Pointer to the destination cl_layout.
 *   lt_src:  Pointer to the source layout.
 * Returns: void.
 */
inline void copy_host_to_cl(__local cl_layout *lt_dest, __global layout *lt_src)
{
    for (int i = 0; i < ROW; i++)
    {
        for (int j = 0; j < COL; j++)
        {
            lt_dest->matrix[i][j] = lt_src->matrix[i][j];
        }
    }

    lt_dest->score = lt_src->score;
}

/*
 * Copy matrix and score from cl_layout to normal layout.
 * Parameters:
 *   lt_dest: Pointer to the destination layout.
 *   lt_src:  Pointer to the source cl_layout.
 * Returns: void.
 */
inline void copy_cl_to_host(__global layout *lt_dest, __local cl_layout *lt_src)
{
    for (int i = 0; i < ROW; i++)
    {
        for (int j = 0; j < COL; j++)
        {
            lt_dest->matrix[i][j] = lt_src->matrix[i][j];
        }
    }

    lt_dest->score = lt_src->score;
}

/*
 * Copy matrix and score from cl_layout to cl_layout.
 * Parameters:
 *   lt_dest: Pointer to the destination cl_layout.
 *   lt_src:  Pointer to the source cl_layout.
 * Returns: void.
 */
inline void copy_cl_to_cl(__local cl_layout *lt_dest, __local cl_layout *lt_src)
{
    for (int i = 0; i < ROW; i++)
    {
        for (int j = 0; j < COL; j++)
        {
            lt_dest->matrix[i][j] = lt_src->matrix[i][j];
        }
    }

    lt_dest->score = lt_src->score;
}

/*
 * Unflattens a 1D index into an 8D matrix coordinate.
 * Parameters:
 *   i: The flattened index.
 *   row0, col0, row1, col1, row2, col2, row3, col3: Pointers to store the
 *                                                 row and column indices.
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
 * Unflattens a 1D index into a 6D matrix coordinate.
 * Parameters:
 *   i: The flattened index.
 *   row0, col0, row1, col1, row2, col2: Pointers to store the row and column indices.
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
 * Unflattens a 1D index into a 4D matrix coordinate.
 * Parameters:
 *   i: The flattened index.
 *   row0, col0, row1, col1: Pointers to store the row and column indices.
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
 * Unflattens a 1D index into a 2D matrix coordinate.
 * Parameters:
 *   i: The flattened index.
 *   row0, col0: Pointers to store the row and column indices.
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
 *   i: The index of the character in the language array.
 * Returns: The index in the linearized monogram array.
 */
size_t index_mono(int i) {
    return i;
}

/*
 * Calculates the index for a bigram in a linearized array.
 * Parameters:
 *   i, j: The indices of the characters in the language array.
 * Returns: The index in the linearized bigram array.
 */
size_t index_bi(int i, int j) {
    return i * LANG_LENGTH + j;
}

/*
 * Calculates the index for a trigram in a linearized array.
 * Parameters:
 *   i, j, k: The indices of the characters in the language array.
 * Returns: The index in the linearized trigram array.
 */
size_t index_tri(int i, int j, int k) {
    return i * LANG_LENGTH * LANG_LENGTH + j * LANG_LENGTH + k;
}

/*
 * Calculates the index for a quadgram in a linearized array.
 * Parameters:
 *   i, j, k, l: The indices of the characters in the language array.
 * Returns: The index in the linearized quadgram array.
 */
size_t index_quad(int i, int j, int k, int l) {
    return i * LANG_LENGTH * LANG_LENGTH * LANG_LENGTH + j * LANG_LENGTH * LANG_LENGTH + k * LANG_LENGTH + l;
}

/*
 * Calculates the index for a skipgram in a linearized array.
 * Parameters:
 *   skip_index: The skip distance (1-9).
 *   j, k: The indices of the characters in the language array.
 * Returns: The index in the linearized skipgram array.
 */
size_t index_skip(int skip_index, int j, int k) {
    return skip_index * LANG_LENGTH * LANG_LENGTH + j * LANG_LENGTH + k;
}

/*
 * Performs meta-analysis on a layout.
 * Currently, only calculates hand balance.
 * Parameters:
 *   lt: Pointer to the cl_layout to analyze.
 * Returns: void.
 */
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
 *   lt: Pointer to the cl_layout.
 *   stats_mono, stats_bi, stats_tri, stats_quad, stats_skip, stats_meta:
 *       Constant pointers to the respective statistic arrays.
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

/* Define a struct for PCG32 state. */
typedef struct {
    ulong state;
    ulong inc;
} pcg32_state_t;

/*
 * PCG32 random number generator function.
 * Parameters:
 *   rng: Pointer to the state of the PRNG.
 * Returns: A pseudo-random 32-bit unsigned integer.
 */
uint pcg32_random_r(pcg32_state_t *rng) {
    ulong oldstate = rng->state;
    rng->state = oldstate * 6364136223846793005UL + rng->inc;
    uint xorshifted = ((oldstate >> 18u) ^ oldstate) >> 27u;
    uint rot = oldstate >> 59u;
    return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
}

/*
 * Initializes the internal state of the random number generator.
 * Parameters:
 *   global_id: The global ID of the work item.
 *   seed:      The initial seed value.
 *   offset:    An offset to ensure different states for different work items.
 * Returns: A pcg32_state_t structure representing the initialized state.
 */
pcg32_state_t initialize_rng(int global_id, int seed, int offset) {
    pcg32_state_t rng;
    rng.state = (ulong)seed + (ulong)offset;
    rng.inc = (ulong)global_id * 2 + 1;
    /* Discard first 10 values */
    for (int i = 0; i < 10; i++) {
        pcg32_random_r(&rng);
    }
    return rng;
}

/*
 * Simplified swap function for OpenCL kernel.
 * Parameters:
 *   lt: Pointer to the cl_layout.
 *   row1, col1: Row and column of the first key.
 *   row2, col2: Row and column of the second key.
 * Returns: void.
 */
inline void swap_keys(__local cl_layout *lt, int row1, int col1, int row2, int col2) {
    int temp = lt->matrix[row1][col1];
    lt->matrix[row1][col1] = lt->matrix[row2][col2];
    lt->matrix[row2][col2] = temp;
}

/*
 * Calculate monogram statistics for a given layout.
 * Parameters:
 *   working: Pointer to the cl_layout being analyzed.
 *   local_id: The local ID of the work item.
 *   stats_mono: Constant pointer to the array of mono_stat structures.
 *   linear_mono: Constant pointer to the linearized monogram frequency data.
 * Returns: void.
 */
inline void calculate_mono_stats(__local cl_layout *working,
                                 size_t local_id,
                                 __constant mono_stat *stats_mono,
                                 __constant float *linear_mono) {
    int row0, col0;
    for (int i = local_id; i < MONO_END; i += WORKERS) {
        working->mono_score[i] = 0;
        int length = stats_mono[i].length;
        for (int j = 0; j < length; j++) {
            unflat_mono(stats_mono[i].ngrams[j], &row0, &col0);
            if (working->matrix[row0][col0] != -1) {
                size_t index = index_mono(working->matrix[row0][col0]);
                working->mono_score[i] += linear_mono[index];
            }
        }
    }
}

/*
 * Calculate bigram statistics for a given layout.
 * Parameters:
 *   working: Pointer to the cl_layout being analyzed.
 *   local_id: The local ID of the work item.
 *   stats_bi: Constant pointer to the array of bi_stat structures.
 *   linear_bi: Constant pointer to the linearized bigram frequency data.
 * Returns: void.
 */
inline void calculate_bi_stats(__local cl_layout *working,
                               size_t local_id,
                               __constant bi_stat *stats_bi,
                               __constant float *linear_bi) {
    int row0, col0, row1, col1;
    for (int i = local_id; i < BI_END; i += WORKERS) {
        working->bi_score[i] = 0;
        int length = stats_bi[i].length;
        for (int j = 0; j < length; j++) {
            unflat_bi(stats_bi[i].ngrams[j], &row0, &col0, &row1, &col1);
            if (working->matrix[row0][col0] != -1 && working->matrix[row1][col1] != -1) {
                size_t index = index_bi(working->matrix[row0][col0], working->matrix[row1][col1]);
                working->bi_score[i] += linear_bi[index];
            }
        }
    }
}

/*
 * Calculate trigram statistics for a given layout.
 * Parameters:
 *   working: Pointer to the cl_layout being analyzed.
 *   local_id: The local ID of the work item.
 *   stats_tri: Constant pointer to the array of tri_stat structures.
 *   linear_tri: Constant pointer to the linearized trigram frequency data.
 * Returns: void.
 */
inline void calculate_tri_stats(__local cl_layout *working,
                                size_t local_id,
                                __constant tri_stat *stats_tri,
                                __constant float *linear_tri) {
    int row0, col0, row1, col1, row2, col2;
    for (int i = local_id; i < TRI_END; i += WORKERS) {
        working->tri_score[i] = 0;
        int length = stats_tri[i].length;
        for (int j = 0; j < length; j++) {
            unflat_tri(stats_tri[i].ngrams[j], &row0, &col0, &row1, &col1, &row2, &col2);
            if (working->matrix[row0][col0] != -1 && working->matrix[row1][col1] != -1 && working->matrix[row2][col2] != -1) {
                size_t index = index_tri(working->matrix[row0][col0], working->matrix[row1][col1], working->matrix[row2][col2]);
                working->tri_score[i] += linear_tri[index];
            }
        }
    }
}

/*
 * Calculate quadgram statistics for a given layout.
 * Parameters:
 *   working: Pointer to the cl_layout being analyzed.
 *   local_id: The local ID of the work item.
 *   stats_quad: Constant pointer to the array of quad_stat structures.
 *   linear_quad: Constant pointer to the linearized quadgram frequency data.
 * Returns: void.
 */
inline void calculate_quad_stats(__local cl_layout *working,
                                 size_t local_id,
                                 __constant quad_stat *stats_quad,
                                 __constant float *linear_quad) {
    int row0, col0, row1, col1, row2, col2, row3, col3;
    for (int i = local_id; i < QUAD_END; i += WORKERS) {
        working->quad_score[i] = 0;
        int length = stats_quad[i].length;
        for (int j = 0; j < length; j++) {
            unflat_quad(stats_quad[i].ngrams[j], &row0, &col0, &row1, &col1, &row2, &col2, &row3, &col3);
            if (working->matrix[row0][col0] != -1 && working->matrix[row1][col1] != -1 && working->matrix[row2][col2] != -1 && working->matrix[row3][col3] != -1) {
                size_t index = index_quad(working->matrix[row0][col0], working->matrix[row1][col1], working->matrix[row2][col2], working->matrix[row3][col3]);
                working->quad_score[i] += linear_quad[index];
            }
        }
    }
}

/*
 * Calculate skipgram statistics for a given layout.
 * Parameters:
 *   working: Pointer to the cl_layout being analyzed.
 *   local_id: The local ID of the work item.
 *   stats_skip: Constant pointer to the array of skip_stat structures.
 *   linear_skip: Constant pointer to the linearized skipgram frequency data.
 * Returns: void.
 */
inline void calculate_skip_stats(__local cl_layout *working,
                                 size_t local_id,
                                 __constant skip_stat *stats_skip,
                                 __constant float *linear_skip) {
    int row0, col0, row1, col1;
    for (int i = local_id; i < SKIP_END; i += WORKERS) {
        int length = stats_skip[i].length;
        for (int k = 1; k <= 9; k++) {
            working->skip_score[k][i] = 0;
            for (int j = 0; j < length; j++) {
                unflat_bi(stats_skip[i].ngrams[j], &row0, &col0, &row1, &col1);
                if (working->matrix[row0][col0] != -1 && working->matrix[row1][col1] != -1) {
                    size_t index = index_skip(k, working->matrix[row0][col0], working->matrix[row1][col1]);
                    working->skip_score[k][i] += linear_skip[index];
                }
            }
        }
    }
}

/*
 * Main kernel for layout improvement using simulated annealing.
 */
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
                             __global layout *layouts,
                             __constant int *pins,
                             int seed,
                             __global int *reps) {
    /* Identify the work item */
    size_t global_id = get_global_id(0);
    size_t group_id = get_group_id(0);
    size_t local_id = get_local_id(0);

    /* Initialize the random number generator */
    pcg32_state_t rng = initialize_rng(global_id, seed, 0);

    /* Each workgroup gets a copy of the initial layout in local memory */
    __local cl_layout working;
    __local cl_layout best_layout;

    if (local_id == 0) {
        copy_host_to_cl(&working, &layouts[group_id]);
        copy_cl_to_cl(&best_layout, &working);
    }
    barrier(CLK_LOCAL_MEM_FENCE);

    /* Simulated Annealing Logic */
    float T = 1000;
    int initial_swap_count = MAX_SWAPS;
    int swap_count;
    float max_T = T;
    int improvement_counter = 0;
    int iterations = REPETITIONS / THREADS;

    for (int i = 0; i < iterations; i++)
    {
        /* Temperature-dependent swap count */
        swap_count = (int)(initial_swap_count * (T / max_T));
        swap_count = swap_count < 1 ? 1 : swap_count;
        swap_count = swap_count > initial_swap_count ? initial_swap_count : swap_count;

        /* Store swaps for potential reversal */
        int swap_rows1[MAX_SWAPS];
        int swap_cols1[MAX_SWAPS];
        int swap_rows2[MAX_SWAPS];
        int swap_cols2[MAX_SWAPS];

        /* Make MAX_SWAPS number of swaps */
        if (local_id == 0) {
            reps[get_group_id(0)] = i+1;
            for (int j = 0; j < MAX_SWAPS; j++) {
                int row1, col1, row2, col2;
                do {
                    /* Generate random indices within the valid range */
                    row1 = pcg32_random_r(&rng) % ROW;
                    col1 = pcg32_random_r(&rng) % COL;
                    row2 = pcg32_random_r(&rng) % ROW;
                    col2 = pcg32_random_r(&rng) % COL;
                /* Check for pins and ensure we don't swap the same key with itself */
                } while (pins[row1 * COL + col1] || pins[row2 * COL + col2] || (row1 == row2 && col1 == col2));

                /* Store swap locations */
                swap_rows1[j] = row1;
                swap_cols1[j] = col1;
                swap_rows2[j] = row2;
                swap_cols2[j] = col2;

                /* Perform the swap */
                swap_keys(&working, row1, col1, row2, col2);
            }
        }
        barrier(CLK_LOCAL_MEM_FENCE);

        /* Calculate statistics */
        calculate_mono_stats(&working, local_id, stats_mono, linear_mono);
        calculate_bi_stats(&working, local_id, stats_bi, linear_bi);
        calculate_tri_stats(&working, local_id, stats_tri, linear_tri);
        calculate_quad_stats(&working, local_id, stats_quad, linear_quad);
        calculate_skip_stats(&working, local_id, stats_skip, linear_skip);

        barrier(CLK_LOCAL_MEM_FENCE);

        /* Only the first worker runs meta_analysis and get_score */
        if (local_id == 0) {
            cl_meta_analysis(&working);
            cl_get_score(&working, stats_mono, stats_bi, stats_tri, stats_quad, stats_skip, stats_meta);

            /* Exponentiate the score difference for acceptance probability */
            float delta_score = working.score - best_layout.score;
            if (delta_score > 0 || (1.0 / (1.0 + native_exp(-10 * delta_score / T))) > (float)pcg32_random_r(&rng) / 4294967295.0f) {
                copy_cl_to_cl(&best_layout, &working);
                improvement_counter++;
            } else {
                /* Revert the swaps */
                for (int j = swap_count - 1; j >= 0; j--) {
                    swap_keys(&working, swap_rows1[j], swap_cols1[j], swap_rows2[j], swap_cols2[j]);
                }
            }

            /* Adaptive cooling */
            if (i > 0 && i % (REPETITIONS / (20 * THREADS)) == 0) {
                float improvement_rate = (float)improvement_counter / (REPETITIONS / (20 * THREADS));
                if (improvement_rate > 0.2) {
                    max_T *= 0.95;
                } else {
                    max_T *= 1.05;
                }
                max_T = max_T > 1500.0 ? 1500.0 : max_T;
                max_T = max_T < T ? T : max_T;
                improvement_counter = 0;
            }

            /* Reheating */
            if (i > 0 && i % (REPETITIONS / (10 * THREADS)) == 0) {
                T = max_T;
            }

            /* Non-monotonic "jolt" */
            if (i > 0 && i % (REPETITIONS / (50 * THREADS)) == 0) {
                T *= (1.0 + (float)pcg32_random_r(&rng) / 4294967295.0f * 0.3);
                if (T > max_T) {
                    T = max_T;
                }
            }

            /* Temperature cooling */
            float progress = (float)i / (REPETITIONS / THREADS);
            T = max_T * (1.0 - progress);
            T = T < 1.0 ? 1.0 : T;
        }
        barrier(CLK_LOCAL_MEM_FENCE);
    }

    /* Copy layout back to global memory */
    if (local_id ==0) {
        copy_cl_to_host(&layouts[group_id], &best_layout);
    }
}
