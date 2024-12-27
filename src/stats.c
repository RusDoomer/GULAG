/*
 * stats.c - Statistic management for the GULAG.
 *
 * Author: Rus Doomer
 *
 * Description: This file contains functions for initializing, cleaning,
 *              converting, and freeing statistics used in the GULAG.
 *              Statistics are categorized into monograms, bigrams, trigrams,
 *              quadgrams, skipgrams, and meta-statistics. These statistics
 *              are used to analyze and evaluate keyboard layouts.
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
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>

/*
 * Initializes all statistical data structures for the GULAG.
 * This involves initializing linked lists for each type of n-gram statistic
 * (monograms, bigrams, trigrams, quadgrams, and skipgrams) as well as meta-statistics.
 * The function delegates the initialization of each statistic type to its respective module.
 * After initial setup, it trims each statistic list to optimize data storage.
 */
void initialize_stats()
{
    /*
     * stats/mono.c - initializes linked list for monogram stats
     *                trims the length of the linked list
     */
    log_print('v',L"     Initializing monogram stats... ");
    initialize_mono_stats();
    log_print('v',L"trimming monogram stats... ");
    trim_mono_stats();
    log_print('v',L"Done\n");

    /*
     * stats/bi.c - initializes linked list for bigram stats
     *              trims the length of the linked list
     */
    log_print('v',L"     Initializing bigram stats...   ");
    initialize_bi_stats();
    log_print('v',L"trimming bigram stats...   ");
    trim_bi_stats();
    log_print('v',L"Done\n");

    /*
     * stats/tri.c - initializes linked list for trigram stats
     *               trims the length of the linked list
     */
    log_print('v',L"     Initializing trigram stats...  ");
    initialize_tri_stats();
    log_print('v',L"trimming trigram stats...  ");
    trim_tri_stats();
    log_print('v',L"Done\n");

    /*
     * stats/quad.c - initializes linked list for quadgram stats
     *                trims the length of the linked list
     */
    log_print('v',L"     Initializing quadgram stats... ");
    initialize_quad_stats();
    log_print('v',L"trimming quadgram stats... ");
    trim_quad_stats();
    log_print('v',L"Done\n");

    /*
     * stats/skip.c - initializes linked list for skipgram stats
     *                trims the length of the linked list
     */
    log_print('v',L"     Initializing skipgram stats... ");
    initialize_skip_stats();
    log_print('v',L"trimming skipgram stats... ");
    trim_skip_stats();
    log_print('v',L"Done\n");

    /*
     * stats/meta.c - initializes linked list for meta stats
     *                trims the length of the linked list
     */
    log_print('v',L"     Initializing meta stats...     ");
    initialize_meta_stats();
    log_print('v',L"trimming meta stats...     ");
    trim_meta_stats();
    log_print('v',L"Done\n");
}

/*
 * Cleans the statistics data by removing entries with zero length or weight.
 * This function iterates through each category of statistics (monograms, bigrams,
 * trigrams, quadgrams, skipgrams, and meta-statistics) and removes any statistic
 * that has a length of 0 or a weight of 0. This helps in maintaining a clean
 * and relevant set of statistics for layout analysis.
 */
void clean_stats()
{
    /* stats/mono.c - remove stats with 0 length or weight */
    log_print('v',L"\n     Cleaning monogram stats... ");
    clean_mono_stats();
    log_print('v',L"Done\n");

    /* stats/bi.c */
    log_print('v',L"     Cleaning bigram stats... ");
    clean_bi_stats();
    log_print('v',L"Done\n");

    /* stats/tri.c */
    log_print('v',L"     Cleaning trigram stats... ");
    clean_tri_stats();
    log_print('v',L"Done\n");

    /* stats/quad.c */
    log_print('v',L"     Cleaning quadgram stats... ");
    clean_quad_stats();
    log_print('v',L"Done\n");

    /* stats/skip.c */
    log_print('v',L"     Cleaning skipgram stats... ");
    clean_skip_stats();
    log_print('v',L"Done\n");

    /* stats/meta.c */
    log_print('v',L"     Cleaning meta stats... ");
    clean_meta_stats();
    log_print('v',L"Done\n");
}

/*
 * Converts the linked list statistics data structures into arrays.
 * This function transforms the data from linked lists to contiguous arrays for
 * each category of n-gram statistics (monograms, bigrams, trigrams, quadgrams,
 * skipgrams, and meta-statistics). This conversion facilitates quicker access
 * and streamlined processing of the statistics during layout analysis.
 */
void stats_to_array()
{
    /* stats/mono.c - convert linked list stats to contiguous array */
    log_print('v',L"\n     Converting monogram stats... ");
    mono_to_array();
    log_print('v',L"Done\n");

    /* stats/bi.c */
    log_print('v',L"     Converting bigram stats... ");
    bi_to_array();
    log_print('v',L"Done\n");

    /* stats/tri.c */
    log_print('v',L"     Converting trigram stats... ");
    tri_to_array();
    log_print('v',L"Done\n");

    /* stats/quad.c */
    log_print('v',L"     Converting quadgram stats... ");
    quad_to_array();
    log_print('v',L"Done\n");

    /* stats/skip.c */
    log_print('v',L"     Converting skipgram stats... ");
    skip_to_array();
    log_print('v',L"Done\n");

    /* stats/meta.c */
    log_print('v',L"     Converting meta stats... ");
    meta_to_array();
    log_print('v',L"Done\n");
}

/*
 * Frees the memory allocated for all statistics data structures.
 * This function deallocates the memory used by the statistics arrays and their
 * corresponding linked lists for each n-gram category. It ensures that all
 * dynamically allocated memory for statistics is properly released.
 */
void free_stats()
{
    /* stats/mono.c - frees all stats in the linked list */
    log_print('v',L"\n     Freeing monogram stats... ");
    free_mono_stats();
    log_print('v',L"Done\n");

    /* stats/bi.c */
    log_print('v',L"     Freeing bigram stats... ");
    free_bi_stats();
    log_print('v',L"Done\n");

    /* stats/tri.c */
    log_print('v',L"     Freeing trigram stats... ");
    free_tri_stats();
    log_print('v',L"Done\n");

    /* stats/quad.c */
    log_print('v',L"     Freeing quadgram stats... ");
    free_quad_stats();
    log_print('v',L"Done\n");

    /* stats/skip.c */
    log_print('v',L"     Freeing skipgram stats... ");
    free_skip_stats();
    log_print('v',L"Done\n");

    /* stats/meta.c */
    log_print('v',L"     Freeing meta stats... ");
    free_meta_stats();
    log_print('v',L"Done\n");
}
