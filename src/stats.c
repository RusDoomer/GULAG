#include "stats.h"
#include "mono.h"
#include "bi.h"
#include "tri.h"
#include "quad.h"
#include "skip.h"

#include "global.h"
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>

void initialize_stats()
{
    wprintf(L"     Initializing monogram stats... ");
    initialize_mono_stats();
    wprintf(L"trimming monogram stats... ");
    trim_mono_stats();
    wprintf(L"Done\n");

    wprintf(L"     Initializing bigram stats...   ");
    initialize_bi_stats();
    wprintf(L"trimming bigram stats...   ");
    trim_bi_stats();
    wprintf(L"Done\n");

    wprintf(L"     Initializing trigram stats...  ");
    initialize_tri_stats();
    wprintf(L"trimming trigram stats...  ");
    trim_tri_stats();
    wprintf(L"Done\n");

    wprintf(L"     Initializing quadgram stats... ");
    initialize_quad_stats();
    wprintf(L"trimming quadgram stats... ");
    trim_quad_stats();
    wprintf(L"Done\n");

    wprintf(L"     Initializing skipgram stats... ");
    initialize_skip_stats();
    wprintf(L"trimming skipgram stats... ");
    trim_skip_stats();
    wprintf(L"Done\n");
}

void clean_stats()
{
    wprintf(L"     Cleaning monogram stats... ");
    clean_mono_stats();
    wprintf(L"Done\n");
    wprintf(L"     Cleaning bigram stats... ");
    clean_bi_stats();
    wprintf(L"Done\n");
    wprintf(L"     Cleaning trigram stats... ");
    clean_tri_stats();
    wprintf(L"Done\n");
    wprintf(L"     Cleaning quadgram stats... ");
    clean_quad_stats();
    wprintf(L"Done\n");
    wprintf(L"     Cleaning skipgram stats... ");
    clean_skip_stats();
    wprintf(L"Done\n");

    wprintf(L"     Done\n\n");
}

void stats_to_array()
{
    wprintf(L"     Converting monogram stats... ");
    mono_to_array();
    wprintf(L"Done\n");
    wprintf(L"     Converting bigram stats... ");
    bi_to_array();
    wprintf(L"Done\n");
    wprintf(L"     Converting trigram stats... ");
    tri_to_array();
    wprintf(L"Done\n");
    wprintf(L"     Converting quadgram stats... ");
    quad_to_array();
    wprintf(L"Done\n");
    wprintf(L"     Converting skipgram stats... ");
    skip_to_array();
    wprintf(L"Done\n");

    wprintf(L"     Done\n\n");
}

void free_stats()
{
    wprintf(L"     Freeing monogram stats... ");
    free_mono_stats();
    wprintf(L"Done\n");
    wprintf(L"     Freeing bigram stats... ");
    free_bi_stats();
    wprintf(L"Done\n");
    wprintf(L"     Freeing trigram stats... ");
    free_tri_stats();
    wprintf(L"Done\n");
    wprintf(L"     Freeing quadgram stats... ");
    free_quad_stats();
    wprintf(L"Done\n");
    wprintf(L"     Freeing skipgram stats... ");
    free_skip_stats();
    wprintf(L"Done\n");

    wprintf(L"     Done\n\n");
}
