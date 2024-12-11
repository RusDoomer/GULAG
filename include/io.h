#ifndef IO_H
#define IO_H

#include "global.h"

void read_config();

void read_args(int argc, char **argv);

void check_setup();

void read_lang();

int read_corpus_cache();
void read_corpus();
void cache_corpus();

void read_weights();

void read_layout(layout *lt, int which_layout);

void print_layout(layout *lt);

void print_ranking();

#endif
