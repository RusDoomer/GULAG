// kernel.cl

// Redefine necessary constants
#define ROW 3
#define COL 12
#define DIM1 (ROW * COL)
#define DIM2 (DIM1 * DIM1)
#define DIM3 (DIM2 * DIM1)
#define DIM4 (DIM3 * DIM1)

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

// Kernel function
__kernel void improve_kernel(__global float *linear_mono,
                             __global float *linear_bi,
                             __global float *linear_tri,
                             __global float *linear_quad,
                             __global float *linear_skip,
                             __global mono_stat *stats_mono,
                             __global bi_stat *stats_bi,
                             __global tri_stat *stats_tri,
                             __global quad_stat *stats_quad,
                             __global skip_stat *stats_skip,
                             __global meta_stat *stats_meta,
                             __global layout *lt,
                             int MONO_END,
                             int BI_END,
                             int TRI_END,
                             int QUAD_END,
                             int SKIP_END,
                             int META_END,
                             int threads,
                             int repetitions,
                             __global int *pins) {

    int global_id = get_global_id(0);

    if (global_id == 0) {
        // Check if sizes are greater than 0
        if (MONO_END > 0 && BI_END > 0 && TRI_END > 0 && QUAD_END > 0 &&
            SKIP_END > 0 && META_END > 0 && threads > 0 && repetitions > 0 &&
            get_global_size(0) > 0 && // Check the global work size
            sizeof(stats_mono[0].ngrams) > 0 && sizeof(stats_bi[0].ngrams) > 0 &&
            sizeof(stats_tri[0].ngrams) > 0 && sizeof(stats_quad[0].ngrams) > 0 &&
            sizeof(stats_skip[0].ngrams) > 0 && sizeof(stats_meta[0].name) > 0) {

            // Indicate success in the layout name
            for (int i = 0; i < 20; i++) {
                lt[0].name[i] = "OpenCL check passed"[i];
            }
            lt[0].name[20] = '\0';
        } else {
            // Indicate failure in the layout name
            for (int i = 0; i < 21; i++) {
                lt[0].name[i] = "OpenCL check failed"[i];
            }
            lt[0].name[21] = '\0';
        }
    }
}
