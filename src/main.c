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
#define LANG_FILE_LENGTH 100
#define LANG_LENGTH 51

void start_up()
{
    // set locale
    const char* locale = setlocale(LC_ALL, "en_US.UTF-8");
    if (locale == NULL) {
        error("Failed to set locale.");
        return;
    }

    // Set stdout to wide-oriented
    if (fwide(stdout, 1) <= 0) {
        error("Failed to set wide-oriented stream.");
        return;
    }

    // hide cursor
    wprintf(L"\e[?25l");

    //seed random number
    srand(time(NULL));

    // allocate lang array
    lang_arr = (wchar_t *)calloc(101, sizeof(wchar_t));

    // allocate char table array
    char_table = (int *)calloc(UNICODE_MAX+1, sizeof(int));

    // allocate arrays for ngrams directly from corpus
    corpus_mono = (int *)calloc(LANG_LENGTH, sizeof(int));

    corpus_bi = (int **)malloc(LANG_LENGTH * sizeof(int *));
    for (int i = 0; i < LANG_LENGTH; i++) {
        corpus_bi[i] = (int *)calloc(LANG_LENGTH, sizeof(int));
    }

    corpus_tri = (int ***)malloc(LANG_LENGTH * sizeof(int **));
    for (int i = 0; i < LANG_LENGTH; i++) {
        corpus_tri[i] = (int **)malloc(LANG_LENGTH * sizeof(int *));
        for (int j = 0; j < LANG_LENGTH; j++) {
            corpus_tri[i][j] = (int *)calloc(LANG_LENGTH, sizeof(int));
        }
    }

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

    corpus_skip = (int ***)malloc(10 * sizeof(int **));
    for (int i = 1; i <= 9; i++) {
        corpus_skip[i] = (int **)malloc(LANG_LENGTH * sizeof(int *));
        for (int j = 0; j < LANG_LENGTH; j++) {
            corpus_skip[i][j] = (int *)calloc(LANG_LENGTH, sizeof(int));
        }
    }

    initialize_stats();
}

void shut_down()
{
    //show cursor
    wprintf(L"\e[?25h");

    free(char_table);

    free(corpus_mono);

    for (int i = 0; i < LANG_LENGTH; i++) {
        free(corpus_bi[i]);
    }
    free(corpus_bi);

    for (int i = 0; i < LANG_LENGTH; i++) {
        for (int j = 0; j < LANG_LENGTH; j++) {
            free(corpus_tri[i][j]);
        }
        free(corpus_tri[i]);
    }
    free(corpus_tri);

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

    for (int i = 1; i <= 9; i++) {
        for (int j = 0; j < LANG_LENGTH; j++) {
            free(corpus_skip[i][j]);
        }
        free(corpus_skip[i]);
    }
    free(corpus_skip);

    free_stats();
}

int main(int argc, char **argv) {
    start_up();

    /* io.c - holds defaults to be overwritten by args */
    read_config();

    /* io.c - overwrites config */
    read_args(argc, argv);

    /* io.c - final check that all options are correct */
    check_setup();

    wprintf(L"| lang: %s | ", lang_name);
    wprintf(L"corpus: %s | ", corpus_name);
    wprintf(L"primary: %s | ", layout_name);
    wprintf(L"secondary: %s |\n", layout2_name);
    wprintf(L"| weights: %s | ", weight_name);
    wprintf(L"run mode: %c | ", run_mode);
    wprintf(L"reps: %d | ", repetitions);
    wprintf(L"threads: %d | ", threads);
    wprintf(L"out mode: %c |\n", output_mode);

    read_lang(lang_name); /* io.c */

    int corpus_cache = 0;
    corpus_cache = read_corpus_cache();
    if (!corpus_cache) {
        read_corpus();    /* io.c */
        cache_corpus();   /* io.c */
    }

    read_weights();       /* io.c */
    clean_stats();        /* stats.c */

    switch(run_mode) {       /* all in mode.c */
        case 'a':
            analysis();      /* analyze one layout */
            break;
        case 'c':
            compare();       /* compare 2 layouts */
            break;
        case 'r':
            rank();          /* rank all layouts in directory */
            break;
        case 'g':
            generate();      /* generate a new layout */
            break;
        case 'i':
            improve();       /* improve a layout */
            break;
        case 'b':
            gen_benchmark(); /* benchmark to find ideal number of threads */
            break;
        case 'h':
            print_help();    /* print help info */
            break;
        case 'f':
            print_info();    /* print info screen */
            break;
    }


    free(lang_name);
    free(corpus_name);
    free(layout_name);
    free(layout2_name);
    free(weight_name);
    shut_down(); /* reverse start_up */
    return 0;
}
