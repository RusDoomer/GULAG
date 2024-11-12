#ifndef GLOBAL_H
#define GLOBAL_H

extern int ROW;
extern int COL;
extern int DIM1;
extern int DIM2;
extern int DIM3;
extern int DIM4;

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
extern int pins[ROW][COL];

typedef struct layout {
    char name[100];
    int **matrix;
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

typedef struct stat {
    char name[100];
    char type;
    int *ngrams;
    float weight;
} stat;

extern int *corpus_mono;
extern int **corpus_bi;
extern int ***corpus_tri;
extern int ****corpus_quad;
extern int ***corpus_skip;

extern stat *stats_mono;
extern stat *stats_bi;
extern stat *stats_tri;
extern stat *stats_quad;
extern stat *stats_skip;

#endif
