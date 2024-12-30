#ifndef GLOBAL_H
#define GLOBAL_H

#include <wchar.h>
#include "structs.h"

/* Defining dimensions for the layout grid. */
#define row 3
#define col 12
#define dim1 row * col
#define dim2 dim1 * dim1
#define dim3 dim2 * dim1
#define dim4 dim3 * dim1

/* Character count in the chosen language. */
extern int LANG_LENGTH;

/* Maximum length of a language definition file. */
extern int LANG_FILE_LENGTH;

/* Dimensions of the layout grid. */
extern int ROW;
extern int COL;
extern int DIM1;
extern int DIM2;
extern int DIM3;
extern int DIM4;

extern int MAX_SWAPS;
extern int WORKERS;

/*
 * Number of tracked statistics for each ngram type.
 * the name NGRAM_END is a relic of when these were enums
 * I'm not changing it though.
 */
extern int MONO_END;
extern int BI_END;
extern int TRI_END;
extern int QUAD_END;
extern int SKIP_END;
extern int META_END;

/* Paths to data files. */
extern char *lang_name;
extern char *corpus_name;
extern char *layout_name;
extern char *layout2_name;
extern char *weight_name;

/* Control flags for program execution. */
extern char run_mode;
extern int repetitions;
extern int threads;
extern char output_mode;
extern char backend_mode;

extern double layouts_analyzed;
extern double elapsed_compute_time;

/* The selected language's character set. */
extern wchar_t *lang_arr;

/* Hash table for character code lookup. */
extern int *char_table;

/* Pinned key positions on the layout for improvement. */
extern int pins[row][col];

/* Head of the linked list for layout ranking. */
extern layout_node *head_node;

/* Arrays to store raw frequency counts from the corpus. */
extern int *corpus_mono;
extern int **corpus_bi;
extern int ***corpus_tri;
extern int ****corpus_quad;
extern int ***corpus_skip;

/* Arrays to store normalized frequency data (percentages). */
extern float *linear_mono;
extern float *linear_bi;
extern float *linear_tri;
extern float *linear_quad;
extern float *linear_skip;

/* Heads of linked lists for each statistic type. */
extern mono_stat *mono_head;
extern bi_stat *bi_head;
extern tri_stat *tri_head;
extern quad_stat *quad_head;
extern skip_stat *skip_head;
extern meta_stat *meta_head;

/* Arrays to hold all statistics after processing. */
extern mono_stat *stats_mono;
extern bi_stat *stats_bi;
extern tri_stat *stats_tri;
extern quad_stat *stats_quad;
extern skip_stat *stats_skip;
extern meta_stat *stats_meta;

#endif
