/*
 * main.c - Main source file for the GULAG.
 *
 * Author: Rus Doomer
 *
 * Description: This file contains the main function and logic for
 *              initializing, running, and shutting down the GULAG.
 */

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>

#include "io.h"
#include "global.h"
#include "util.h"
#include "mode.h"
#include "stats.h"

#define UNICODE_MAX 65535

/*
 * Performs program initialization, including setting the locale,
 * allocating memory, hiding the cursor and seeding the random number generator.
 * Returns: void.
 */
void start_up()
{
    /* Set locale for wide character support. */
    log_print('n',L"1/4: Setting Locale... ");
    const char* locale = setlocale(LC_ALL, "en_US.UTF-8");
    if (locale == NULL) {
        error("Failed to set locale.");
        return;
    }

    /* Set stdout to wide-oriented. */
    log_print('n',L"Setting error stream... ");
    if (fwide(stdout, 1) <= 0) {
        error("Failed to set wide-oriented stream.");
        return;
    }

    /* Hide cursor. */
    log_print('n',L"Hiding cursor... ");
    wprintf(L"\e[?25l");

    /* Seed random number generator. */
    log_print('n',L"Seeding RNG... ");
    srand(time(NULL));
    log_print('n',L"Done\n\n");

    /* Allocate language array. */
    log_print('n',L"2/4: Allocating language array... ");
    lang_arr = (wchar_t *)calloc(101, sizeof(wchar_t));

    /* Allocate character hash table array. */
    log_print('n',L"Allocating character hashmap... ");
    char_table = (int *)calloc(UNICODE_MAX+1, sizeof(int));
    log_print('n',L"Done\n\n");

    /* Allocate arrays for ngrams directly from corpus. */
    log_print('n',L"3/4: Allocating corpus arrays...\n");
    log_print('v',L"     Monograms... Integer... ");
    corpus_mono = (int *)calloc(LANG_LENGTH, sizeof(int));
    log_print('v',L"Floating Point... ");
    linear_mono = (float *)calloc(LANG_LENGTH, sizeof(float));
    log_print('v',L"Done\n");

    log_print('v',L"     Bigrams... Integer... ");
    corpus_bi = (int **)malloc(LANG_LENGTH * sizeof(int *));
    for (int i = 0; i < LANG_LENGTH; i++) {
        corpus_bi[i] = (int *)calloc(LANG_LENGTH, sizeof(int));
    }
    log_print('v',L"Floating Point... ");
    linear_bi = (float *)calloc(LANG_LENGTH * LANG_LENGTH, sizeof(float));
    log_print('v',L"Done\n");

    log_print('v',L"     Trigrams... Integer... ");
    corpus_tri = (int ***)malloc(LANG_LENGTH * sizeof(int **));
    for (int i = 0; i < LANG_LENGTH; i++) {
        corpus_tri[i] = (int **)malloc(LANG_LENGTH * sizeof(int *));
        for (int j = 0; j < LANG_LENGTH; j++) {
            corpus_tri[i][j] = (int *)calloc(LANG_LENGTH, sizeof(int));
        }
    }
    log_print('v',L"Floating Point... ");
    linear_tri = (float *)calloc(LANG_LENGTH * LANG_LENGTH * LANG_LENGTH, sizeof(float));
    log_print('v',L"Done\n");

    log_print('v',L"     Quadgrams... Integer... ");
    corpus_quad = (int ****)malloc(LANG_LENGTH * sizeof(int ***));
    for (int i = 0; i < LANG_LENGTH; i++) {
        corpus_quad[i] = (int ***)malloc(LANG_LENGTH * sizeof(int **));
        for (int j = 0; j < LANG_LENGTH; j++) {
            corpus_quad[i][j] = (int **)malloc(LANG_LENGTH * sizeof(int *));
            for (int k = 0; k < LANG_LENGTH; k++) {
                corpus_quad[i][j][k] = (int *)calloc(LANG_LENGTH, sizeof(int));
            }
        }
    }
    log_print('v',L"Floating Point... ");
    linear_quad = (float *)calloc(LANG_LENGTH * LANG_LENGTH * LANG_LENGTH * LANG_LENGTH, sizeof(float));
    log_print('v',L"Done\n");

    log_print('v',L"     Skipgrams...\n");
    corpus_skip = (int ***)malloc(10 * sizeof(int **));
    for (int i = 1; i <= 9; i++) {
        log_print('v',L"       Skip-%d... Integer... ", i);
        corpus_skip[i] = (int **)malloc(LANG_LENGTH * sizeof(int *));
        for (int j = 0; j < LANG_LENGTH; j++) {
            corpus_skip[i][j] = (int *)calloc(LANG_LENGTH, sizeof(int));
        }
        log_print('v',L"Done\n");
    }
    log_print('v',L"       Floating Point... ");
    linear_skip = (float *)calloc(10 * LANG_LENGTH * LANG_LENGTH, sizeof(float));
    log_print('v',L"Done\n");
    log_print('n',L"     Done\n\n");

    /* stats.c - initializes and trims statistics */
    log_print('n',L"4/4: Initializing stats...\n");
    initialize_stats();
    log_print('n',L"     Done\n\n");
}

/*
 * Performs cleanup, including showing the cursor and freeing allocated memory.
 * Returns: void.
 */
void shut_down()
{
    /* Show cursor. */
    log_print('n',L"1/3: Showing cursor... ");
    wprintf(L"\e[?25h");

    /* Free language array. */
    log_print('n',L"Freeing language array... ");
    free(lang_arr);
    log_print('n',L"Done\n\n");

    /* Free character hash table array. */
    log_print('n',L"Freeing character hashmap... ");
    free(char_table);
    log_print('n',L"Done\n\n");

    /* Free arrays for ngrams directly from corpus. */
    log_print('n',L"2/3: Freeing corpus arrays... ");
    log_print('v',L"\n     Monograms... ");
    free(corpus_mono);
    free(linear_mono);
    log_print('v',L"Done\n");

    log_print('v',L"     Bigrams... ");
    for (int i = 0; i < LANG_LENGTH; i++) {
        free(corpus_bi[i]);
    }
    free(corpus_bi);
    free(linear_bi);
    log_print('v',L"Done\n");

    log_print('v',L"     Trigrams... ");
    for (int i = 0; i < LANG_LENGTH; i++) {
        for (int j = 0; j < LANG_LENGTH; j++) {
            free(corpus_tri[i][j]);
        }
        free(corpus_tri[i]);
    }
    free(corpus_tri);
    free(linear_tri);
    log_print('v',L"Done\n");

    log_print('v',L"     Quadgrams... ");
    for (int i = 0; i < LANG_LENGTH; i++) {
        for (int j = 0; j < LANG_LENGTH; j++) {
            for (int k = 0; k < LANG_LENGTH; k++) {
                free(corpus_quad[i][j][k]);
            }
            free(corpus_quad[i][j]);
        }
        free(corpus_quad[i]);
    }
    free(corpus_quad);
    free(linear_quad);
    log_print('v',L"Done\n");

    log_print('v',L"     Skipgrams...\n");
    for (int i = 1; i <= 9; i++) {
        log_print('v',L"       Skip-%d... ", i);
        for (int j = 0; j < LANG_LENGTH; j++) {
            free(corpus_skip[i][j]);
        }
        free(corpus_skip[i]);
        log_print('v',L"Done\n");
    }
    free(corpus_skip);
    free(linear_skip);
    log_print('v',L"       Done\n");
    log_print('n',L"     Done\n\n");

    /* stats.c - frees all stats */
    log_print('n',L"3/3: Freeing stats... ");
    free_stats();
    log_print('n',L"     Done\n\n");
}

/*
 * Program entry point.
 * Parameters:
 *   argc: Argument count.
 *   argv: Argument list.
 * Returns: 0 if successful.
 */
int main(int argc, char **argv) {
    struct timespec start, end;
    double elapsed;

    clock_gettime(CLOCK_MONOTONIC, &start);
    log_print('q',L"\nWelcome to the GULAG\n\n");
    log_print('q',L"----- Starting Up -----\n\n");
    start_up();

    clock_gettime(CLOCK_MONOTONIC, &end);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    log_print('q',L"----- Start Up Complete : %.9lf seconds -----\n\n", elapsed);

    clock_gettime(CLOCK_MONOTONIC, &start);
    log_print('q',L"----- Setting Up -----\n\n");
    /* io.c - holds defaults to be overwritten by args */
    log_print('q',L"1/3: Reading config... ");
    read_config();
    log_print('q',L"Done\n\n");

    /* io.c - overwrites config */
    log_print('q',L"2/3: Reading command line arguments... ");
    read_args(argc, argv);
    log_print('q',L"Done\n\n");

    /* io.c - final check that all options are correct */
    log_print('q',L"3/3: Checking arguments... ");
    check_setup();
    log_print('q',L"Done\n\n");

    clock_gettime(CLOCK_MONOTONIC, &end);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    log_print('q',L"----- Set Up Complete : %.9lf seconds -----\n\n", elapsed);

    log_print('n',L"| lang: %s | ", lang_name);
    log_print('n',L"corpus: %s | ", corpus_name);
    log_print('n',L"primary: %s | ", layout_name);
    log_print('n',L"secondary: %s | ", layout2_name);
    log_print('n',L"weights: %s | ", weight_name);
    log_print('n',L"run mode: %c | ", run_mode);
    log_print('n',L"reps: %d | ", repetitions);
    log_print('n',L"threads: %d | ", threads);
    log_print('n',L"out mode: %c |\n\n", output_mode);

    clock_gettime(CLOCK_MONOTONIC, &start);
    log_print('q',L"----- Reading Data -----\n\n");

    /* io.c - read language file and fill array */
    log_print('n',L"1/4: Reading language... ");
    read_lang(lang_name);
    log_print('n',L"Done\n\n");

    /* io.c - read from cache if it exists */
    log_print('n',L"2/4: Reading corpus... ");
    log_print('v',L"Looking for cache... ");
    int corpus_cache = 0;
    corpus_cache = read_corpus_cache();
    log_print('n',L"Done\n\n");
    if (!corpus_cache) {
        /* The next operation is slow so we want to let the user see
           what step they are stuck on. */
        /* io.c - read entire corpus file and fill arrays */
        log_print('v',L"Reading raw corpus... ");
        read_corpus();
        log_print('n',L"Done\n\n");

        /* io.c - create new corpus cache */
        log_print('n',L"2.5\4: Creating corpus cache... ");
        cache_corpus();
        log_print('n',L"Done\n\n");
    }

    /* util.c - take corpus arrays from raw frequencies to percentages */
    log_print('n',L"3/4: Normalize corpus... ");
    normalize_corpus();
    log_print('n',L"Done\n\n");

    /* io.c - read weights and fill in stats*/
    log_print('n',L"4/4: Reading stat weights... ");
    read_weights();
    log_print('n',L"Done\n\n");

    clock_gettime(CLOCK_MONOTONIC, &end);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    log_print('q',L"----- Reading Complete : %.9lf seconds -----\n\n", elapsed);

    clock_gettime(CLOCK_MONOTONIC, &start);
    log_print('q',L"----- Cleaning Up -----\n\n");

    /* stats.c - remove stats with 0 length or weight */
    log_print('n',L"1/3: Removing irrelevant stats... ");
    clean_stats();
    log_print('n',L"     Done\n\n");

    /* stats.c - convert stats linked list to array */
    log_print('n',L"2/3: Converting stats linked list to contiguous array... ");
    stats_to_array();
    log_print('n',L"     Done\n\n");

    clock_gettime(CLOCK_MONOTONIC, &end);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    log_print('q',L"----- Clean Up Complete : %.9lf seconds -----\n\n", elapsed);

    log_print('v',L"----- Weights -----\n\n");

    for (int i = 0; i < MONO_END; i++)
    {
        log_print('v',L"%s : %f\n", stats_mono[i].name, stats_mono[i].weight);
    }
    for (int i = 0; i < BI_END; i++)
    {
        log_print('v',L"%s : %f\n", stats_bi[i].name, stats_bi[i].weight);
    }
    for (int i = 0; i < TRI_END; i++)
    {
        log_print('v',L"%s : %f\n", stats_tri[i].name, stats_tri[i].weight);
    }
    for (int i = 0; i < QUAD_END; i++)
    {
        log_print('v',L"%s : %f\n", stats_quad[i].name, stats_quad[i].weight);
    }
    for (int i = 0; i < SKIP_END; i++)
    {
        log_print('v',L"%s : ", stats_skip[i].name);
        for (int j = 1; j <= 9; j++)
        {
            log_print('v',L"%f ", stats_skip[i].weight[j]);
        }
        log_print('v',L"\n");
    }
    for (int i = 0; i < META_END; i++)
    {
        log_print('v',L"%s : %f\n", stats_meta[i].name, stats_meta[i].weight);
    }
    log_print('v',L"\n");

    log_print('v',L"----- Weights Complete -----\n\n");

    clock_gettime(CLOCK_MONOTONIC, &start);
    log_print('q',L"----- Running -----\n\n");

    /* all in mode.c */
    switch(run_mode) {
        case 'a':
            /* analyze one layout */
            log_print('n',L"Running single analysis\n");
            analysis();
            log_print('n',L"Done\n\n");
            break;
        case 'c':
            /* compare 2 layouts */
            log_print('n',L"Running comparison\n");
            compare();
            log_print('n',L"Done\n\n");
            break;
        case 'r':
            /* rank all layouts in directory */
            log_print('n',L"Running ranking\n");
            rank();
            log_print('n',L"\nDone\n\n");
            break;
        case 'g':
            /* generate a new layout */
            log_print('n',L"Running generation\n");
            generate();
            log_print('n',L"Done\n\n");
            break;
        case 'i':
            /* improve a layout */
            log_print('n',L"Running optimization\n");
            //improve(0);
            cl_improve(0);
            log_print('n',L"Done\n\n");
            break;
        case 'b':
            /* benchmark to find ideal number of threads */
            log_print('n',L"Running benchmark\n");
            gen_benchmark();
            log_print('n',L"Done\n\n");
            break;
        case 'h':
            /* print help info */
            log_print('n',L"Printing help message\n");
            print_help();
            log_print('n',L"Done\n\n");
            break;
        case 'f':
            /* print info screen */
            log_print('n',L"Printing info message\n");
            print_info();
            log_print('n',L"Done\n\n");
            break;
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    log_print('q',L"----- Run Complete : %.9lf seconds -----\n\n", elapsed);

    clock_gettime(CLOCK_MONOTONIC, &start);
    log_print('q',L"----- Shutting Down -----\n\n");

    free(lang_name);
    free(corpus_name);
    free(layout_name);
    free(layout2_name);
    free(weight_name);

    /* reverse start_up */
    shut_down();

    clock_gettime(CLOCK_MONOTONIC, &end);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    log_print('q',L"----- Shut Down Complete : %.9lf seconds -----\n\n", elapsed);

    log_print('q',L"You are free to go.\n\n");
    return 0;
}
