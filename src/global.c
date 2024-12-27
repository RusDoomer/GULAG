/*
 * global.c - Global variable definitions and constants for the GULAG.
 *
 * Author: Rus Doomer
 *
 * Description: This file contains the definitions of global variables and constants used
 *              throughout the GULAG. These variables manage configuration settings,
 *              language data, corpus statistics, and layout information. Management
 *              of these resources is crucial for the efficient operation of the GULAG.
 */

#include <wchar.h>

#include "global.h"
#include "structs.h"

/* Defining dimensions for the layout grid. */
#define row 3
#define col 12
#define dim1 row * col
#define dim2 dim1 * dim1
#define dim3 dim2 * dim1
#define dim4 dim3 * dim1

/* Character count in the chosen language. */
int LANG_LENGTH = 51;

/* Maximum length of a language definition file. */
int LANG_FILE_LENGTH = 100;

/* Re-iterate the dimensions for external use. */
int ROW = row;
int COL = col;
int DIM1 = dim1;
int DIM2 = dim2;
int DIM3 = dim3;
int DIM4 = dim4;

int MAX_SWAPS = dim1 / 2;
int WORKERS = 1;

/*
 * Number of tracked statistics for each ngram type.
 * the name NGRAM_END is a relic of when these were enums
 * I'm not changing it though.
 */
int MONO_END = 0;
int BI_END = 0;
int TRI_END = 0;
int QUAD_END = 0;
int SKIP_END = 0;
int META_END = 0;

/* Paths to data files. */
char *lang_name = NULL;
char *corpus_name = NULL;
char *layout_name = NULL;
char *layout2_name = NULL;
char *weight_name = NULL;

/* Control flags for program execution. */
char run_mode = 'a';
int repetitions = 10000;
int threads = 8;
char output_mode = 'v';

double layouts_per_second_real = 0;
double layouts_per_second_total = 0;

/* The selected language's character set. */
wchar_t *lang_arr;

/* Hash table for character code lookup. */
int *char_table;

/* Pinned key positions on the layout for improvement. */
int pins[row][col];

/* Head of the linked list for layout ranking. */
layout_node *head_node;

/* Arrays to store raw frequency counts from the corpus. */
int *corpus_mono;
int **corpus_bi;
int ***corpus_tri;
int ****corpus_quad;
int ***corpus_skip;

/* Arrays to store normalized frequency data (percentages). */
float *linear_mono;
float *linear_bi;
float *linear_tri;
float *linear_quad;
float *linear_skip;

/* Heads of linked lists for each statistic type. */
mono_stat *mono_head;
bi_stat *bi_head;
tri_stat *tri_head;
quad_stat *quad_head;
skip_stat *skip_head;
meta_stat *meta_head;

/* Arrays to hold all statistics after processing. */
mono_stat *stats_mono;
bi_stat *stats_bi;
tri_stat *stats_tri;
quad_stat *stats_quad;
skip_stat *stats_skip;
meta_stat *stats_meta;
