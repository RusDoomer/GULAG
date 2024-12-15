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

void start_up()
{
    wprintf(L"1/4: Setting Locale, ");
    // set locale
    const char* locale = setlocale(LC_ALL, "en_US.UTF-8");
    if (locale == NULL) {
        error("Failed to set locale.");
        return;
    }

    wprintf(L"Setting error stream, ");
    // Set stdout to wide-oriented
    if (fwide(stdout, 1) <= 0) {
        error("Failed to set wide-oriented stream.");
        return;
    }

    wprintf(L"Hiding cursor, ");
    // hide cursor
    wprintf(L"\e[?25l");

    wprintf(L"Seeding RNG... ");
    //seed random number
    srand(time(NULL));
    wprintf(L"Done\n\n");

    wprintf(L"2/4: Allocating language array, ");
    // allocate lang array
    lang_arr = (wchar_t *)calloc(101, sizeof(wchar_t));

    wprintf(L"Allocating character hashmap... ");
    // allocate char table array
    char_table = (int *)calloc(UNICODE_MAX+1, sizeof(int));
    wprintf(L"Done\n\n");

    wprintf(L"3/4: Allocating corpus arrays...\n");
    wprintf(L"     Monograms... Integer... ");
    // allocate arrays for ngrams directly from corpus
    corpus_mono = (int *)calloc(LANG_LENGTH, sizeof(int));
    wprintf(L"Floating Point... ");
    linear_mono = (float *)calloc(LANG_LENGTH, sizeof(float));
    wprintf(L"Done\n");

    wprintf(L"     Bigrams... Integer... ");
    corpus_bi = (int **)malloc(LANG_LENGTH * sizeof(int *));
    for (int i = 0; i < LANG_LENGTH; i++) {
        corpus_bi[i] = (int *)calloc(LANG_LENGTH, sizeof(int));
    }
    wprintf(L"Floating Point... ");
    linear_bi = (float *)calloc(LANG_LENGTH * LANG_LENGTH, sizeof(float));
    wprintf(L"Done\n");

    wprintf(L"     Trigrams... Integer... ");
    corpus_tri = (int ***)malloc(LANG_LENGTH * sizeof(int **));
    for (int i = 0; i < LANG_LENGTH; i++) {
        corpus_tri[i] = (int **)malloc(LANG_LENGTH * sizeof(int *));
        for (int j = 0; j < LANG_LENGTH; j++) {
            corpus_tri[i][j] = (int *)calloc(LANG_LENGTH, sizeof(int));
        }
    }
    wprintf(L"Floating Point... ");
    linear_tri = (float *)calloc(LANG_LENGTH * LANG_LENGTH * LANG_LENGTH, sizeof(float));
    wprintf(L"Done\n");

    wprintf(L"     Quadgrams... Integer... ");
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
    wprintf(L"Floating Point... ");
    linear_quad = (float *)calloc(LANG_LENGTH * LANG_LENGTH * LANG_LENGTH * LANG_LENGTH, sizeof(float));
    wprintf(L"Done\n");

    wprintf(L"     Skipgrams...\n");
    corpus_skip = (int ***)malloc(10 * sizeof(int **));
    for (int i = 1; i <= 9; i++) {
        wprintf(L"       Skip-%d... Integer... ", i);
        corpus_skip[i] = (int **)malloc(LANG_LENGTH * sizeof(int *));
        for (int j = 0; j < LANG_LENGTH; j++) {
            corpus_skip[i][j] = (int *)calloc(LANG_LENGTH, sizeof(int));
        }
        wprintf(L"Done\n");
    }
    wprintf(L"       Floating Point... ");
    linear_skip = (float *)calloc(10 * LANG_LENGTH * LANG_LENGTH, sizeof(float));
    wprintf(L"Done\n");
    wprintf(L"     Done\n\n");

    wprintf(L"4/4: Initializing stats...\n");
    initialize_stats();
    wprintf(L"     Done\n\n");
}

void shut_down()
{
    wprintf(L"1/3: Showing cursor... ");
    //show cursor
    wprintf(L"\e[?25h");

    wprintf(L"Freeing character hashmap... ");
    free(char_table);
    wprintf(L"Done\n\n");

    wprintf(L"2/3: Freeing corpus arrays... \n");
    wprintf(L"     Monograms... ");
    free(corpus_mono);
    free(linear_mono);
    wprintf(L"Done\n");

    wprintf(L"     Bigrams... ");
    for (int i = 0; i < LANG_LENGTH; i++) {
        free(corpus_bi[i]);
    }
    free(corpus_bi);
    free(linear_bi);
    wprintf(L"Done\n");

    wprintf(L"     Trigrams... ");
    for (int i = 0; i < LANG_LENGTH; i++) {
        for (int j = 0; j < LANG_LENGTH; j++) {
            free(corpus_tri[i][j]);
        }
        free(corpus_tri[i]);
    }
    free(corpus_tri);
    free(linear_tri);
    wprintf(L"Done\n");

    wprintf(L"     Quadgrams... ");
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
    wprintf(L"Done\n");

    wprintf(L"     Skipgrams...\n");
    for (int i = 1; i <= 9; i++) {
        wprintf(L"       Skip-%d... ", i);
        for (int j = 0; j < LANG_LENGTH; j++) {
            free(corpus_skip[i][j]);
        }
        free(corpus_skip[i]);
        wprintf(L"Done\n");
    }
    free(corpus_skip);
    free(linear_skip);
    wprintf(L"       Done\n");
    wprintf(L"     Done\n\n");

    wprintf(L"3/3: Freeing stats... \n");
    free_stats();
}

int main(int argc, char **argv) {
    wprintf(L"\nWelcome to the GULAG\n\n");
    wprintf(L"----- Starting Up -----\n\n");
    start_up();
    wprintf(L"----- Start Up Complete -----\n\n");

    wprintf(L"----- Setting Up -----\n\n");
    wprintf(L"1/3: Reading config... ");
    /* io.c - holds defaults to be overwritten by args */
    read_config();

    wprintf(L"2/3: Reading command line arguments... ");
    /* io.c - overwrites config */
    read_args(argc, argv);

    wprintf(L"3/3: Checking arguments... ");
    /* io.c - final check that all options are correct */
    check_setup();
    wprintf(L"----- Set Up Complete -----\n\n");

    wprintf(L"| lang: %s | ", lang_name);
    wprintf(L"corpus: %s | ", corpus_name);
    wprintf(L"primary: %s | ", layout_name);
    wprintf(L"secondary: %s | ", layout2_name);
    wprintf(L"weights: %s | ", weight_name);
    wprintf(L"run mode: %c | ", run_mode);
    wprintf(L"reps: %d | ", repetitions);
    wprintf(L"threads: %d | ", threads);
    wprintf(L"out mode: %c |\n\n", output_mode);

    wprintf(L"----- Reading Data -----\n\n");

    wprintf(L"1/4: Reading language... ");
    read_lang(lang_name); /* io.c */

    wprintf(L"2/4: Reading corpus... ");
    wprintf(L"Looking for cache... ");
    int corpus_cache = 0;
    corpus_cache = read_corpus_cache();
    if (!corpus_cache) {
        wprintf(L"Reading raw corpus... ");
        // the next operation is slow so we want to let the user see
        // what step they are stuck on
        fflush(stdout);
        read_corpus();    /* io.c */
        wprintf(L"2.5\4: Creating corpus cache... ");
        cache_corpus();   /* io.c */
    }

    wprintf(L"3/4: Normalize corpus... ");
    normalize_corpus(); /* util.c */

    wprintf(L"4/4: Reading stat weights... ");
    read_weights();       /* io.c */

    wprintf(L"----- Reading Complete -----\n\n");
    wprintf(L"----- Cleaning Up -----\n\n");

    wprintf(L"1/2: Removing irrelevant stats... \n");
    clean_stats();        /* stats.c */
    wprintf(L"2/2: Converting stats linked list to contiguous array... \n");
    stats_to_array();     /* stats.c */

    wprintf(L"----- Clean Up Complete -----\n\n");

    wprintf(L"----- Weights -----\n\n");

    for (int i = 0; i < MONO_END; i++)
    {
        wprintf(L"%s : %f\n", stats_mono[i].name, stats_mono[i].weight);
    }
    for (int i = 0; i < BI_END; i++)
    {
        wprintf(L"%s : %f\n", stats_bi[i].name, stats_bi[i].weight);
    }
    for (int i = 0; i < TRI_END; i++)
    {
        wprintf(L"%s : %f\n", stats_tri[i].name, stats_tri[i].weight);
    }
    for (int i = 0; i < QUAD_END; i++)
    {
        wprintf(L"%s : %f\n", stats_quad[i].name, stats_quad[i].weight);
    }
    for (int i = 0; i < SKIP_END; i++)
    {
        wprintf(L"%s : ", stats_skip[i].name);
        for (int j = 1; j <= 9; j++)
        {
            wprintf(L"%f ", stats_skip[i].weight[j]);
        }
        wprintf(L"\n");
    }
    for (int i = 0; i < META_END; i++)
    {
        wprintf(L"%s : %f\n", stats_meta[i].name, stats_meta[i].weight);
    }
    wprintf(L"\n");

    wprintf(L"----- Weights Complete -----\n\n");

    wprintf(L"----- Running -----\n\n");

    switch(run_mode) {       /* all in mode.c */
        case 'a':
            wprintf(L"Running single analysis\n");
            analysis();      /* analyze one layout */
            break;
        case 'c':
            wprintf(L"Running comparison\n");
            compare();       /* compare 2 layouts */
            break;
        case 'r':
            wprintf(L"Running ranking\n");
            rank();          /* rank all layouts in directory */
            break;
        case 'g':
            wprintf(L"Running generation\n");
            generate();      /* generate a new layout */
            break;
        case 'i':
            wprintf(L"Running optimization\n");
            improve();       /* improve a layout */
            break;
        case 'b':
            wprintf(L"Running benchmark\n");
            gen_benchmark(); /* benchmark to find ideal number of threads */
            break;
        case 'h':
            wprintf(L"Printing help message\n");
            print_help();    /* print help info */
            break;
        case 'f':
            wprintf(L"Printing info message\n");
            print_info();    /* print info screen */
            break;
    }

    wprintf(L"----- Run Complete -----\n\n");
    wprintf(L"----- Shutting Down -----\n\n");

    free(lang_name);
    free(corpus_name);
    free(layout_name);
    free(layout2_name);
    free(weight_name);
    shut_down(); /* reverse start_up */

    wprintf(L"----- Shut Down Complete -----\n\n");

    wprintf(L"You are free to go.\n");
    return 0;
}
