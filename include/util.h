#ifndef UTIL_H
#define UTIL_H

#include "global.h"

void error(const char *msg);

void flat_quad(int row0, int col0, int row1, int col1, int row2, int col2,
    int row3, int col3, int *i);
void unflat_quad(int i, int *row0, int *col0, int *row1, int *col1,
    int *row2, int *col2, int *row3, int *col3);
void flat_tri(int row0, int col0, int row1, int col1, int row2, int col2,
    int *i);
void unflat_tri(int i, int *row0, int *col0, int *row1, int *col1,
    int *row2, int *col2);
void flat_bi(int row0, int col0, int row1, int col1, int *i);
void unflat_bi(int i, int *row0, int *col0, int *row1, int *col1);
void flat_mono(int row0, int col0, int *i);
void unflat_mono(int i, int *row0, int *col0);

size_t index_mono(int i);
size_t index_bi(int i, int j);
size_t index_tri(int i, int j, int k);
size_t index_quad(int i, int j, int k, int l);
size_t index_skip(int skip_index, int j, int k);

void normalize_corpus();

void alloc_layout(layout **lt);
void free_layout(layout *lt);

void normalize_score();

void get_score(layout *lt);

void get_layout_diff(layout *lt, layout *lt2, layout *lt_diff);

void create_node(layout *lt);
void free_list();

void shuffle_layout(layout *lt);

void copy(layout *lt_dest, layout *lt_src);

float random_float();

#endif
