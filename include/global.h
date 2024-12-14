#ifndef GLOBAL_H
#define GLOBAL_H

#include <wchar.h>

#define row 3
#define col 12
#define dim1 row * col
#define dim2 dim1 * dim1
#define dim3 dim2 * dim1
#define dim4 dim3 * dim1

extern int ROW;
extern int COL;
extern int DIM1;
extern int DIM2;
extern int DIM3;
extern int DIM4;

// the name NGRAM_END is a relic of when these were enums
// I'm not changing it though, this is the number of those stats
extern int MONO_END;
extern int BI_END;
extern int TRI_END;
extern int QUAD_END;
extern int SKIP_END;
extern int META_END;

extern char *lang_name;
extern char *corpus_name;
extern char *layout_name;
extern char *layout2_name;
extern char *weight_name;
extern char run_mode;
extern int repetitions;
extern int threads;
extern char output_mode;

extern wchar_t *lang_arr;
extern int *char_table;
extern int pins[row][col];

typedef struct layout {
    char name[100];
    int matrix[row][col];
    float *mono_score;
    float *bi_score;
    float *tri_score;
    float *quad_score;
    float **skip_score;
    float *meta_score;
    float score;
} layout;

typedef struct layout_node {
    struct layout *lt;
    struct layout_node *next;
} layout_node;

extern layout_node *head_node;

typedef struct mono_stat {
    char name[100];
    int ngrams[dim1];
    int length;
    float weight;
    struct mono_stat *next;
} mono_stat;

typedef struct bi_stat {
    char name[100];
    int ngrams[dim2];
    int length;
    float weight;
    struct bi_stat *next;
} bi_stat;

typedef struct tri_stat {
    char name[100];
    int ngrams[dim3];
    int length;
    float weight;
    struct tri_stat *next;
} tri_stat;

typedef struct quad_stat {
    char name[100];
    int ngrams[dim4];
    int length;
    float weight;
    struct quad_stat *next;
} quad_stat;

extern int *corpus_mono;
extern int **corpus_bi;
extern int ***corpus_tri;
extern int ****corpus_quad;
extern int ***corpus_skip;

extern mono_stat *mono_head;
extern bi_stat *bi_head;
extern tri_stat *tri_head;
extern quad_stat *quad_head;
extern bi_stat *skip_head;

extern mono_stat *stats_mono;
extern bi_stat *stats_bi;
extern tri_stat *stats_tri;
extern quad_stat *stats_quad;
extern bi_stat *stats_skip;

#endif
