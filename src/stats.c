/*
 * stats.c - Statistic management for the GULAG.
 *
 * This file contains functions for initializing, cleaning, and freeing
 * statistics used in the GULAG. Statistics are categorized into
 * monograms, bigrams, trigrams, quadgrams, skipgrams, and meta-statistics.
 */


#include "stats.h"
#include "mono.h"
#include "bi.h"
#include "tri.h"
#include "quad.h"
#include "skip.h"
#include "meta.h"

#include "global.h"
#include "structs.h"
#include "io.h"

/*
 * Initializes all statistic data structures for the GULAG. This involves
 * initializing arrays for each type of n-gram statistic as well as
 * meta-statistics. The function delegates the initialization of each statistic
 * type to its respective module.
 */
void initialize_stats()
{
    /* initializes array for monogram stats */
    log_print('v',L"\n");
    log_print('v',L"     Initializing monogram stats... ");
    initialize_mono_stats(); /* stats/mono.c */
    log_print('v',L"trimming monogram stats... ");
    trim_mono_stats(); /* stats/mono.c */
    log_print('v',L"Done\n");

    /* initializes array for bigram stats */
    log_print('v',L"     Initializing bigram stats...   ");
    initialize_bi_stats(); /* stats/bi.c */
    log_print('v',L"trimming bigram stats...   ");
    trim_bi_stats(); /* stats/bi.c */
    log_print('v',L"Done\n");

    /* initializes array for trigram stats */
    log_print('v',L"     Initializing trigram stats...  ");
    initialize_tri_stats(); /* stats/tri.c */
    log_print('v',L"trimming trigram stats...  ");
    trim_tri_stats(); /* stats/tri.c */
    log_print('v',L"Done\n");

    /* initializes array for quadgram stats */
    log_print('v',L"     Initializing quadgram stats... ");
    initialize_quad_stats(); /* stats/quad.c */
    log_print('v',L"trimming quadgram stats... ");
    trim_quad_stats(); /* stats/quad.c */
    log_print('v',L"Done\n");

    /* initializes array for skipgram stats */
    log_print('v',L"     Initializing skipgram stats... ");
    initialize_skip_stats(); /* stats/skip.c */
    log_print('v',L"trimming skipgram stats... ");
    trim_skip_stats(); /* stats/skip.c */
    log_print('v',L"Done\n");

    /* initializes array for meta stats */
    log_print('v',L"     Initializing meta stats...     ");
    initialize_meta_stats(); /* stats/meta.c */
    log_print('v',L"trimming meta stats...     ");
    trim_meta_stats(); /* stats/meta.c */
    log_print('v',L"Done\n");
}

/*
 * Cleans the statistics data by skipping entries with zero length or weight.
 * This function iterates through each category of statistics (monograms,
 * bigrams, trigrams, quadgrams, skipgrams, and meta-statistics) and marks skip
 * on any statistic that has a length of 0 or a weight of 0. This helps purge
 * irrelevant statistics for layout analysis.
 */
void clean_stats()
{
    /* remove stats with 0 length or weight */
    log_print('v',L"\n     Cleaning monogram stats... ");
    clean_mono_stats(); /* stats/mono.c */
    log_print('v',L"Done\n");

    log_print('v',L"     Cleaning bigram stats... ");
    clean_bi_stats(); /* stats/bi.c */
    log_print('v',L"Done\n");

    log_print('v',L"     Cleaning trigram stats... ");
    clean_tri_stats(); /* stats/tri.c */
    log_print('v',L"Done\n");

    log_print('v',L"     Cleaning quadgram stats... ");
    clean_quad_stats(); /* stats/quad.c */
    log_print('v',L"Done\n");

    log_print('v',L"     Cleaning skipgram stats... ");
    clean_skip_stats(); /* stats/skip.c */
    log_print('v',L"Done\n");

    log_print('v',L"     Cleaning meta stats... ");
    clean_meta_stats(); /* stats/meta.c */
    log_print('v',L"defining meta stats...     ");
    define_meta_stats(); /* stats/meta.c */
    log_print('v',L"Done\n");
}

/*
 * Frees the memory allocated for all statistics data structures. This function
 * deallocates the memory used by the statistics arrays for each n-gram
 * category. It ensures that all dynamically allocated memory for statistics is
 * properly released.
 */
void free_stats()
{
    /* frees all stats in the linked list */
    log_print('v',L"\n     Freeing monogram stats... ");
    free_mono_stats(); /* stats/mono.c */
    log_print('v',L"Done\n");

    log_print('v',L"     Freeing bigram stats... ");
    free_bi_stats(); /* stats/bi.c */
    log_print('v',L"Done\n");

    log_print('v',L"     Freeing trigram stats... ");
    free_tri_stats(); /* stats/tri.c */
    log_print('v',L"Done\n");

    log_print('v',L"     Freeing quadgram stats... ");
    free_quad_stats(); /* stats/quad.c */
    log_print('v',L"Done\n");

    log_print('v',L"     Freeing skipgram stats... ");
    free_skip_stats(); /* stats/skip.c */
    log_print('v',L"Done\n");

    log_print('v',L"     Freeing meta stats... ");
    free_meta_stats(); /* stats/meta.c */
    log_print('v',L"Done\n");
}
