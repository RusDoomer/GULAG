// kernel.cl

#define ROW 3
#define COL 12
#define DIM1 ROW * COL
#define DIM2 DIM1 * DIM1
#define DIM3 DIM2 * DIM1
#define DIM4 DIM3 * DIM1

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
 */

// New layout struct specific for opencl
typedef struct cl_layout {
    char name[100];
    int matrix[ROW][COL];
    float mono_score[MONO_END];
    float bi_score[BI_END];
    float tri_score[TRI_END];
    float quad_score[QUAD_END];
    float skip_score[10][SKIP_END];
    float meta_score[META_END];
    float score;
} cl_layout;

// Redefine necessary structs
typedef struct layout {
    char name[100];
    int matrix[ROW][COL];
    float *mono_score;
    float *bi_score;
    float *tri_score;
    float *quad_score;
    float **skip_score;
    float *meta_score;
    float score;
} layout;

typedef struct mono_stat {
    char name[100];
    int ngrams[DIM1];
    int length;
    float weight;
} mono_stat;

typedef struct bi_stat {
    char name[100];
    int ngrams[DIM2];
    int length;
    float weight;
} bi_stat;

typedef struct tri_stat {
    char name[100];
    int ngrams[DIM3];
    int length;
    float weight;
} tri_stat;

typedef struct quad_stat {
    char name[100];
    int ngrams[DIM4];
    int length;
    float weight;
} quad_stat;

typedef struct skip_stat {
    char name[100];
    int ngrams[DIM2];
    int length;
    float weight[10];
} skip_stat;

typedef struct meta_stat {
    char name[100];
    float weight;
} meta_stat;

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
                             uint seed) {

    int global_id = get_global_id(0);

    // Test: Set the layout name to the name of the second mono_stat
    if (global_id == 0) {

        // Ensure we don't go out of bounds if MONO_END < 2
        if (MONO_END >= 2) {
            for (int i = 0; i < DIM1; i++) {
                //lt->name[i] = (char)stats_mono[1].ngrams[i];
            }
        }

        // Shuffle the layout using the thread-specific seed
        shuffle_layout(lt, seed, global_id);
    }

    // Your other logic can be added here later...
}
