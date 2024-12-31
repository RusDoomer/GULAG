#ifndef STRUCTS_H
#define STRUCTS_H

// Dimensions of the layout grid.
#define row 3
#define col 12
#define dim1 row * col
#define dim2 dim1 * dim1
#define dim3 dim2 * dim1
#define dim4 dim3 * dim1

/* Structure for a keyboard layout and its stats. */
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

/* Node for a linked list of layouts, used for ranking. */
typedef struct layout_node {
    char name[100];
    float score;
    struct layout_node *next;
} layout_node;

/* Structures to represent statistics based on ngrams. */
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

typedef struct skip_stat {
    char name[100];
    int ngrams[dim2];
    int length;
    /* multiple weights for skip-X-grams */
    float weight[10];
    struct skip_stat *next;
} skip_stat;

/*
 * Structure to represent a meta statistic which is based on
 * more than one kind of ngram, calculated through other stats
 */
typedef struct meta_stat {
    char name[100];
    float weight;
    struct meta_stat *next;
} meta_stat;

#endif
