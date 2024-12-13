#include "stats.h"
#include "mono.h"
#include "bi.h"
#include "tri.h"
#include "quad.h"
#include "skip.h"

void initialize_stats()
{
    initialize_mono_stats();
    trim_mono_stats();
    initialize_bi_stats();
    trim_bi_stats();
    initialize_tri_stats();
    trim_tri_stats();
    initialize_quad_stats();
    trim_quad_stats();
    initialize_skip_stats();
    trim_skip_stats();
}

void clean_stats()
{
    clean_mono_stats();
    clean_bi_stats();
    clean_tri_stats();
    clean_quad_stats();
    clean_skip_stats();
}

void free_stats()
{
    free_mono_stats();
    free_bi_stats();
    free_tri_stats();
    free_quad_stats();
    free_skip_stats();
}
