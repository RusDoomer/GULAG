#include "stats.h"
#include "mono.h"
#include "bi.h"
#include "tri.h"
#include "quad.h"
#include "skip.h"
#include "meta.h"

#include "global.h"
#include "io.h"
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>

void initialize_stats()
{
    log_print('v',L"     Initializing monogram stats... ");
    initialize_mono_stats();
    log_print('v',L"trimming monogram stats... ");
    trim_mono_stats();
    log_print('v',L"Done\n");

    log_print('v',L"     Initializing bigram stats...   ");
    initialize_bi_stats();
    log_print('v',L"trimming bigram stats...   ");
    trim_bi_stats();
    log_print('v',L"Done\n");

    log_print('v',L"     Initializing trigram stats...  ");
    initialize_tri_stats();
    log_print('v',L"trimming trigram stats...  ");
    trim_tri_stats();
    log_print('v',L"Done\n");

    log_print('v',L"     Initializing quadgram stats... ");
    initialize_quad_stats();
    log_print('v',L"trimming quadgram stats... ");
    trim_quad_stats();
    log_print('v',L"Done\n");

    log_print('v',L"     Initializing skipgram stats... ");
    initialize_skip_stats();
    log_print('v',L"trimming skipgram stats... ");
    trim_skip_stats();
    log_print('v',L"Done\n");

    log_print('v',L"     Initializing meta stats...     ");
    initialize_meta_stats();
    log_print('v',L"trimming meta stats...     ");
    trim_meta_stats();
    log_print('v',L"Done\n");
}

void clean_stats()
{
    log_print('v',L"\n     Cleaning monogram stats... ");
    clean_mono_stats();
    log_print('v',L"Done\n");
    log_print('v',L"     Cleaning bigram stats... ");
    clean_bi_stats();
    log_print('v',L"Done\n");
    log_print('v',L"     Cleaning trigram stats... ");
    clean_tri_stats();
    log_print('v',L"Done\n");
    log_print('v',L"     Cleaning quadgram stats... ");
    clean_quad_stats();
    log_print('v',L"Done\n");
    log_print('v',L"     Cleaning skipgram stats... ");
    clean_skip_stats();
    log_print('v',L"Done\n");
    log_print('v',L"     Cleaning meta stats... ");
    clean_meta_stats();
    log_print('v',L"Done\n");
}

void stats_to_array()
{
    log_print('v',L"\n     Converting monogram stats... ");
    mono_to_array();
    log_print('v',L"Done\n");
    log_print('v',L"     Converting bigram stats... ");
    bi_to_array();
    log_print('v',L"Done\n");
    log_print('v',L"     Converting trigram stats... ");
    tri_to_array();
    log_print('v',L"Done\n");
    log_print('v',L"     Converting quadgram stats... ");
    quad_to_array();
    log_print('v',L"Done\n");
    log_print('v',L"     Converting skipgram stats... ");
    skip_to_array();
    log_print('v',L"Done\n");
    log_print('v',L"     Converting meta stats... ");
    meta_to_array();
    log_print('v',L"Done\n");
}

void free_stats()
{
    log_print('v',L"\n     Freeing monogram stats... ");
    free_mono_stats();
    log_print('v',L"Done\n");
    log_print('v',L"     Freeing bigram stats... ");
    free_bi_stats();
    log_print('v',L"Done\n");
    log_print('v',L"     Freeing trigram stats... ");
    free_tri_stats();
    log_print('v',L"Done\n");
    log_print('v',L"     Freeing quadgram stats... ");
    free_quad_stats();
    log_print('v',L"Done\n");
    log_print('v',L"     Freeing skipgram stats... ");
    free_skip_stats();
    log_print('v',L"Done\n");
    log_print('v',L"     Freeing meta stats... ");
    free_meta_stats();
    log_print('v',L"Done\n");
}
