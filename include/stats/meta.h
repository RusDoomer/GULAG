#ifndef META_H
#define META_H

#include "global.h"

void initialize_meta_stats();
void trim_meta_stats();
void clean_meta_stats();
void meta_to_array();
void free_meta_stats();
void meta_analysis(layout *lt);

#endif
