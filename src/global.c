#include <wchar.h>

#include "global.h"

int ROW = 3;
int COL = 12;
int DIM1 = ROW * COL;
int DIM2 = DIM1 * DIM1;
int DIM3 = DIM2 * DIM1;
int DIM4 = DIM3 * DIM1;

char *lang_name = NULL;
char *corpus_name = NULL;
char *layout_name = NULL;
char *layout2_name = NULL;
char *weight_name = NULL;
char run_mode = 'a';
int repetitions = 10000;
int threads = 8;
char output_mode = 'n';

wchar_t *lang_arr;
int *char_table;
int pins[ROW][COL];

layout_node *head_node;

int *corpus_mono;
int **corpus_bi;
int ***corpus_tri;
int ****corpus_quad;
int ***corpus_skip;

stat *stats_mono;
stat *stats_bi;
stat *stats_tri;
stat *stats_quad;
stat *stats_skip;
