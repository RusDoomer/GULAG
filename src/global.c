#include <wchar.h>

#include "global.h"

#define row 3
#define col 12
#define dim1 row * col
#define dim2 dim1 * dim1
#define dim3 dim2 * dim1
#define dim4 dim3 * dim1

int ROW = row;
int COL = col;
int DIM1 = dim1;
int DIM2 = dim2;
int DIM3 = dim3;
int DIM4 = dim4;

int MONO_END = 0;
int BI_END = 0;
int TRI_END = 0;
int QUAD_END = 0;
int SKIP_END = 0;
int META_END = 0;

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
int pins[row][col];

layout_node *head_node;

int *corpus_mono;
int **corpus_bi;
int ***corpus_tri;
int ****corpus_quad;
int ***corpus_skip;

// linked lists
stat *mono_head;
stat *bi_head;
stat *tri_head;
stat *quad_head;
stat *skip_head;

// arrays to back linked list
stat **stats_mono;
stat **stats_bi;
stat **stats_tri;
stat **stats_quad;
stat **stats_skip;
