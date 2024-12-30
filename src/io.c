/*
 * io.c - Input/output operations for the GULAG.
 *
 * Author: Rus Doomer
 *
 * Description: This file handles file operations, argument parsing,
 *              and user interaction for the GULAG. It includes functions
 *              for reading configuration files, processing command-line arguments,
 *              handling corpus data, and managing layout input/output.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <getopt.h>
#include <unistd.h>
#include <stdarg.h>

#include "io.h"
#include "io_util.h"
#include "util.h"
#include "global.h"
#include "structs.h"

#define UNICODE_MAX 65535
#define BUFFER_SIZE 10000

#include <wctype.h>

/*
 * Prints a message to the standard output stream, with verbosity control.
 * The message will only be printed if the current output mode meets or
 * exceeds the required verbosity level specified by 'required_level'.
 *
 * Parameters:
 *   required_level: The minimum verbosity level required to print the message.
 *                   'q' for quiet, 'n' for normal, 'v' for verbose.
 *   format:         The format string for the message, compatible with vwprintf.
 *   ...:            Variable arguments for the format string.
 *
 * Returns:
 *   void
 */
void log_print(char required_level, const wchar_t *format, ...) {
    /* Check if the current output mode meets or exceeds the required level */
    if ((required_level == 'q' && (output_mode == 'q' || output_mode == 'n' || output_mode == 'v')) ||
        (required_level == 'n' && (output_mode == 'n' || output_mode == 'v')) ||
        (required_level == 'v' && output_mode == 'v')) {

        va_list args;
        va_start(args, format);
        vwprintf(format, args);
        va_end(args);

        fflush(stdout);
    }
}

/*
 * Reads the configuration file to set up initial program parameters.
 * This function parses 'config.conf' to initialize various settings
 * such as pinned key positions, language, corpus, layout names,
 * weights file, run mode, number of repetitions, number of threads,
 * output mode, and backend mode.
 *
 * Returns: void.
 */
void read_config()
{
    FILE *config;

    char discard[999];
    char buff[999];

    /* config.conf assumed to be in the working directory. */
    config = fopen("config.conf", "r");
    if (config == NULL) {
        /* util.c - error handling */
        error("Required file config.conf not found.");
    }
    log_print('q',L"config.conf found... ");

    char c;
    int i = 0;

    /* Read and set pinned key positions. */
    if (fscanf(config, " %s", discard) != 1) {
        error("Failed to read from config file.");
    }
    if (strcmp(discard, "pins:") != 0) {
        error("Expected 'pins:' at the start of the config file.");
    }

    while (i < ROW * COL && fscanf(config, " %c", &c) == 1) {
        if (c != '.') {
            pins[i / COL][i % COL] = 1;
        } else {
            pins[i / COL][i % COL] = 0;
        }
        i++;
    }

    /* Read and set various parameters from the configuration file. */
    if (fscanf(config, "%s %s", discard, buff) != 2) {
        error("Failed to read language name from config file.");
    }
    lang_name = (char *)malloc((sizeof(char) * strlen(buff)) + 1);
    strcpy(lang_name, buff);

    if (fscanf(config, "%s %s", discard, buff) != 2) {
        error("Failed to read corpus name from config file.");
    }
    corpus_name = (char *)malloc((sizeof(char) * strlen(buff)) + 1);
    strcpy(corpus_name, buff);

    if (fscanf(config, "%s %s", discard, buff) != 2) {
        error("Failed to read layout name from config file.");
    }
    layout_name = (char *)malloc((sizeof(char) * strlen(buff)) + 1);
    strcpy(layout_name, buff);

    if (fscanf(config, "%s %s", discard, buff) != 2) {
        error("Failed to read layout2 name from config file.");
    }
    layout2_name = (char *)malloc((sizeof(char) * strlen(buff)) + 1);
    strcpy(layout2_name, buff);

    if (fscanf(config, "%s %s", discard, buff) != 2) {
        error("Failed to read weight name from config file.");
    }
    weight_name = (char *)malloc((sizeof(char) * strlen(buff)) + 1);
    strcpy(weight_name, buff);

    /* io_util.c - validates and converts run mode */
    if (fscanf(config, "%s %s", discard, buff) != 2) {
        error("Failed to read run mode from config file.");
    }
    run_mode = check_run_mode(buff);

    if (fscanf(config, "%s %s", discard, buff) != 2) {
        error("Failed to read repetitions from config file.");
    }
    repetitions = atoi(buff);

    if (fscanf(config, "%s %s", discard, buff) != 2) {
        error("Failed to read threads from config file.");
    }
    threads = atoi(buff);

    /* io_util.c - validates and converts output mode */
    if (fscanf(config, "%s %s", discard, buff) != 2) {
        error("Failed to read output mode from config file.");
    }
    output_mode = check_output_mode(buff);

    /* io_util.c - validates and converts backend mode */
    if (fscanf(config, "%s %s", discard, buff) != 2) {
        error("Failed to read backend mode from config file.");
    }
    backend_mode = check_backend_mode(buff);

    fclose(config);
}

/*
 * Processes command line arguments to override configuration settings.
 * It parses arguments passed to the main function and updates
 * corresponding global variables such as language name, corpus name,
 * layout names, weight file, repetitions, threads, run mode, output mode,
 * and backend mode.
 *
 * Parameters:
 *   argc: The number of command line arguments.
 *   argv: An array of C-style strings containing the command line arguments.
 * Returns: void.
 */
void read_args(int argc, char **argv)
{
    int opt;
    /* Parse command line arguments. */
    while ((opt = getopt(argc, argv, "l:c:1:2:w:r:t:m:o:b:")) != -1) {
    switch (opt) {
        case 'l':
            free(lang_name);
            lang_name = strdup(optarg);
            break;
        case 'c':
            free(corpus_name);
            corpus_name = strdup(optarg);
            break;
        case '1':
            free(layout_name);
            layout_name = strdup(optarg);
            break;
        case '2':
            free(layout2_name);
            layout2_name = strdup(optarg);
            break;
        case 'w':
            free(weight_name);
            weight_name = strdup(optarg);
            break;
        case 'r':
            repetitions = atoi(optarg);
            break;
        case 't':
            threads = atoi(optarg);
            break;
        case 'm':
            /* io_util.c - validates and converts run mode */
            run_mode = check_run_mode(optarg);
            break;
        case 'o':
            /* io_util.c - validates and converts output mode */
            output_mode = check_output_mode(optarg);
            break;
        case 'b':
            /* io_util.c - validates and converts backend mode */
            backend_mode = check_backend_mode(optarg);
            break;
        case '?':
            /* util.c - error handling */
            error("Improper Usage: %s -l lang_name -c corpus_name "
                "-1 layout_name -2 layout2_name -w weight_name -r repetitions "
                "-t threads -m run_mode -o output_mode -b backen_mode");
        default:
            abort();
        }
    }
}

/*
 * Validates the current program settings to ensure they are legal and consistent.
 * Checks if the language, corpus, layouts, weights, run mode, output mode,
 * number of threads, and repetitions are valid. Terminates the program with
 * an error message if an invalid setting is found.
 *
 * Returns: void.
 */
void check_setup()
{
    /* Ensure necessary parameters are set and have valid values. */
    if (lang_name == NULL) {error("no lang selected");}
    if (corpus_name == NULL) {error("no corpus selected");}
    if (layout_name == NULL) {error("no layout selected");}
    if (layout2_name == NULL) {error("no layout2 selected");}
    if (weight_name == NULL) {error("no weight selected");}
    if (run_mode != 'a' && run_mode != 'c' && run_mode != 'r' && run_mode != 'g'
        && run_mode != 'i' && run_mode != 'b' && run_mode != 'h' && run_mode != 'f')
    {
        error("invalid run mode selected");
    }
    if (output_mode != 'q' && output_mode != 'n' && output_mode != 'v')
    {
        error("invalid output mode selected");
    }
    if (backend_mode != 'c' && backend_mode != 'o')
    {
        error("invalid backend mode selected");
    }
    if (threads < 1) {error("invalid threads selected");}
    if (repetitions < threads) {error("invalid repetitions selected");}
}

/*
 * Reads and sets the current language's character set from a language file.
 * The function constructs the path to the language file based on 'lang_name',
 * reads the character set into 'lang_arr', and sets up the 'char_table' for
 * character code lookups. It performs checks to ensure the language file
 * is correctly formatted and only contains legal characters.
 *
 * Returns: void.
 */
void read_lang()
{
    FILE *lang;
    /* Construct the path to the language file. */
    char *path = (char*)malloc(strlen("./data//.lang") + strlen(lang_name) * 2 + 1);
    strcpy(path, "./data/");
    strcat(path, lang_name);
    strcat(path, "/");
    strcat(path, lang_name);
    strcat(path, ".lang");
    lang = fopen(path, "r");
    free(path);
    if (lang == NULL) {
        error("Lang file not found.");
    }
    log_print('v',L"Lang file found... ");

    log_print('v',L"Reading... ");
    wchar_t a;
    /* Read the language file character set into lang_arr. */
    for (int i = 0; i < 101; i++) {
        if ((a = fgetwc(lang)) == EOF || a == L'\n') {lang_arr[i] = L'@';}
        else if (a == L'@') {
            error("'@' found in lang, illegal character.");
        } else {
            lang_arr[i] = a;
        }
    }

    log_print('v',L"Checking correctness... ");

     /* Validate the format of the language file. */
    if (lang_arr[0] != L' ' || lang_arr[1] != L' ') {
        error("Lang file must begin with 2 spaces");
    }

    if (lang_arr[100] != L'@') {
        error("Lang file too long (>100 characters)");
    }

    /*
     * io_util.c - check for duplicate characters
     * this allows duplicate characters that are side by side for
     * of shifted pair (and to allow the double space at the start)
     */
    if (check_duplicates(lang_arr) != -1) {
        error("Lang file contains duplicate characters.");
    }

    /* Populate the character table for code lookups. */
    for (int i = 0; i < 101; i++) {
        if (lang_arr[i] == L'@') {
            char_table[L'@'] = -1;
        } else if (lang_arr[i] < UNICODE_MAX) {
            char_table[lang_arr[i]] = i/2;
        } else {
            error("Lang file contains illegal character not caught before.");
        }
    }
}

/*
 * Attempts to read corpus data from a cache file.
 * This function constructs the path to the cache file based on 'lang_name'
 * and 'corpus_name'. If the cache file exists, it reads the pre-computed
 * ngram frequencies into the global corpus arrays.
 *
 * Returns:
 *   1 if the cache file was successfully read, 0 otherwise.
 */
int read_corpus_cache()
{
    FILE *corpus;
    /* Construct the path to the corpus cache file. */
    char *path = (char*)malloc(strlen("./data//corpora/.cache") +
        strlen(lang_name) + strlen(corpus_name) + 1);
    strcpy(path, "./data/");
    strcat(path, lang_name);
    strcat(path, "/corpora/");
    strcat(path, corpus_name);
    strcat(path, ".cache");
    corpus = fopen(path, "r");
    if (corpus == NULL) {
        free(path);
        log_print('v',L"Cache not found... ");
        return 0;
    }
    log_print('v',L"Cache found... ");
    log_print('v',L"Reading from cache... ");
    wchar_t curr;
    int i,j,k,l, value;
    /* Read cached frequencies for ngrams */
    while ((curr = fgetwc(corpus)) != WEOF) {
        switch(curr)
        {
            case 'q':
                fwscanf(corpus, L" %d %d %d %d %d ", &i, &j, &k, &l, &value);
                corpus_quad[i][j][k][l] = value;
                break;
            case 't':
                fwscanf(corpus, L" %d %d %d %d ", &i, &j, &k, &value);
                corpus_tri[i][j][k] = value;
                break;
            case 'b':
                fwscanf(corpus, L" %d %d %d ", &i, &j, &value);
                corpus_bi[i][j] = value;
                break;
            case 'm':
                fwscanf(corpus, L" %d %d ", &i, &value);
                corpus_mono[i] = value;
                break;
            case '1':
                fwscanf(corpus, L" %d %d %d ", &i, &j, &value);
                corpus_skip[1][i][j] = value;
                break;
            case '2':
                fwscanf(corpus, L" %d %d %d ", &i, &j, &value);
                corpus_skip[2][i][j] = value;
                break;
            case '3':
                fwscanf(corpus, L" %d %d %d ", &i, &j, &value);
                corpus_skip[3][i][j] = value;
                break;
            case '4':
                fwscanf(corpus, L" %d %d %d ", &i, &j, &value);
                corpus_skip[4][i][j] = value;
                break;
            case '5':
                fwscanf(corpus, L" %d %d %d ", &i, &j, &value);
                corpus_skip[5][i][j] = value;
                break;
            case '6':
                fwscanf(corpus, L" %d %d %d ", &i, &j, &value);
                corpus_skip[6][i][j] = value;
                break;
            case '7':
                fwscanf(corpus, L" %d %d %d ", &i, &j, &value);
                corpus_skip[7][i][j] = value;
                break;
            case '8':
                fwscanf(corpus, L" %d %d %d ", &i, &j, &value);
                corpus_skip[8][i][j] = value;
                break;
            case '9':
                fwscanf(corpus, L" %d %d %d ", &i, &j, &value);
                corpus_skip[9][i][j] = value;
                break;
            default:
                break;
        }
    }
    fclose(corpus);
    free(path);
    return 1;
}

/*
 * Reads and processes a corpus text file to collect ngram frequency data.
 * It constructs the path to the corpus file and reads it character by character,
 * updating the frequency counts in the global corpus arrays for monograms,
 * bigrams, trigrams, quadgrams, and skipgrams.
 *
 * Returns: void.
 */
void read_corpus()
{
    FILE *corpus;
    /* Construct the path to the corpus text file. */
    char *path = (char*)malloc(strlen("./data//corpora/.txt") +
        strlen(lang_name) + strlen(corpus_name) + 1);
    strcpy(path, "./data/");
    strcat(path, lang_name);
    strcat(path, "/corpora/");
    strcat(path, corpus_name);
    strcat(path, ".txt");
    corpus = fopen(path, "r");
    if (corpus == NULL) {
        error("Corpus file not found, make sure the file ends in .txt, but the name in config/parameters does not");
    }
    log_print('v',L"Corpus file found... ");

    /* Memory for the last 11 seen characters */
    int mem[] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};

    wchar_t curr;
    /* io_util.c - convert characters based on the lang file */
    while ((curr = fgetwc(corpus)) != WEOF) {
        mem[0] = convert_char(curr);
        /* If character is valid in the language */
        if (mem[0] > 0 && mem[0] < 51) {
            corpus_mono[mem[0]]++;

            /* If there is a previous character, record the bigram */
            if (mem[1] > 0 && mem[1] < 51) {
                corpus_bi[mem[1]][mem[0]]++;
                /* If there are two previous characters, record the trigram */
                if (mem[2] > 0 && mem[2] < 51) {
                    corpus_tri[mem[2]][mem[1]][mem[0]]++;
                    /* If there are three previous characters, record the quadgram */
                    if (mem[3] > 0 && mem[3] < 51) {
                        corpus_quad[mem[3]][mem[2]][mem[1]][mem[0]]++;
                    }
                }
            }

            /* Record skipgrams from skip-1 to skip-9 */
            for (int i = 2; i < 11; i++)
            {
                if (mem[i] > 0 && mem[i] < 51)
                {
                    corpus_skip[i-1][mem[i]][mem[0]]++;
                }
            }
        }
        /* io_util.c - shift over an array one index, dropping the last value */
        iterate(mem, 11);
    }

    fclose(corpus);
    free(path);
}

/*
 * Creates or updates a cache file with the current corpus frequency data.
 * This function writes the current state of the global corpus arrays to a
 * cache file, allowing for quicker initialization in future runs.
 *
 * Returns: void.
 */
void cache_corpus()
{
    FILE *corpus;
    /* Construct the path to the corpus cache file. */
    char *path = (char*)malloc(strlen("./data//corpora/.cache") +
        strlen(lang_name) + strlen(corpus_name) + 1);
    strcpy(path, "./data/");
    strcat(path, lang_name);
    strcat(path, "/corpora/");
    strcat(path, corpus_name);
    strcat(path, ".cache");
    corpus = fopen(path, "w");
    if (corpus == NULL) {
        error("Corpus cache file failed to be created.");
    }
    log_print('n',L"Created cache file... ");

    for (int i = 0; i < 51; i++) {
        for (int j = 0; j < 51; j++) {
            for (int k = 0; k < 51; k++) {
                for (int l = 0; l < 51; l++) {
                    /* Write all quadgrams to the cache file. */
                    if (corpus_quad[i][j][k][l] > 0) {
                        fprintf(corpus, "q %d %d %d %d %d\n", i, j, k, l,
                            corpus_quad[i][j][k][l]);
                    }
                }
                /* Write all trigrams to the cache file. */
                if (corpus_tri[i][j][k] > 0) {
                    fprintf(corpus, "t %d %d %d %d\n", i, j, k,
                        corpus_tri[i][j][k]);
                }
            }
            /* Write all bigrams to the cache file. */
            if (corpus_bi[i][j] > 0) {
                fprintf(corpus, "b %d %d %d\n", i, j, corpus_bi[i][j]);
            }
            /* Write all skipgrams to the cache file. */
            for (int skip = 1; skip < 10; skip++) {
                if (corpus_skip[skip][i][j] > 0) {
                    fprintf(corpus, "%d %d %d %d\n", skip, i, j,
                        corpus_skip[skip][i][j]);
                }
            }
        }
        /* Write all monograms to the cache file. */
        if (corpus_mono[i] > 0) {
            fprintf(corpus, "m %d %d\n", i, corpus_mono[i]);
        }
    }
    fclose(corpus);
    free(path);
}

/*
 * Reads and applies weights from a weight file to the corresponding statistics.
 * It opens the weight file specified by 'weight_name' and parses it line by
 * line, updating the 'weight' field of each statistic based on its name.
 *
 * Returns: void.
 */
void read_weights()
{
    FILE *weight_file;
    /* Construct the path to the weights file. */
    char *path = (char*)malloc(strlen("./data/weights/.wght") + strlen(weight_name) + 1);
    strcpy(path, "./data/weights/");
    strcat(path, weight_name);
    strcat(path, ".wght");
    weight_file = fopen(path, "r");
    if (weight_file == NULL) {
        error("Weights file not found.");
    }
    log_print('n',L"Weights file found... ");

    wchar_t line[256];
    char name_buffer[256];

    /* Read each line from the weights file. */
    while (fgetws(line, sizeof(line) / sizeof(line[0]), weight_file) != NULL)
    {
         /* Parse the line: split on ':' to get name and weight(s) */
        wchar_t *delimiter = wcschr(line, L':');
        /* Skip lines that don't have the expected format */
        if (delimiter == NULL) {continue;}

        *delimiter = L'\0';
        wchar_t *name_part = line;
        wchar_t *weight_part = delimiter + 1;

         /* Trim whitespace around name and weight */
        while (*name_part && iswspace(*name_part))
            name_part++;
        while (name_part[wcslen(name_part) - 1] && iswspace(name_part[wcslen(name_part) - 1]))
            name_part[wcslen(name_part) - 1] = L'\0';

        while (*weight_part && iswspace(*weight_part))
            weight_part++;
        while (weight_part[wcslen(weight_part) - 1] && iswspace(weight_part[wcslen(weight_part) - 1]))
            weight_part[wcslen(weight_part) - 1] = L'\0';

        /* Convert wide name to a normal string */
        wcstombs(name_buffer, name_part, sizeof(name_buffer));

        /* Parse and process weights */
        float weights[10] = {0};
        int weight_count = 0;
        wchar_t *state;
        wchar_t *token = wcstok(weight_part, L" ", &state);
        while (token != NULL && weight_count < 10)
        {
            float value;
            if (swscanf(token, L"%f", &value) == 1)
            {
                weights[weight_count++] = value;
            }
            token = wcstok(NULL, L" ", &state);
        }

        /* Find the matching stat in the linked lists and update its weight(s) */
        mono_stat *current_mono = mono_head;
        while (current_mono != NULL)
        {
            if (strcmp(current_mono->name, name_buffer) == 0)
            {
                current_mono->weight = weights[0];
                break;
            }
            current_mono = current_mono->next;
        }

        bi_stat *current_bi = bi_head;
        while (current_bi != NULL)
        {
            if (strcmp(current_bi->name, name_buffer) == 0)
            {
                current_bi->weight = weights[0];
                break;
            }
            current_bi = current_bi->next;
        }

        tri_stat *current_tri = tri_head;
        while (current_tri != NULL)
        {
            if (strcmp(current_tri->name, name_buffer) == 0)
            {
                current_tri->weight = weights[0];
                break;
            }
            current_tri = current_tri->next;
        }

        quad_stat *current_quad = quad_head;
        while (current_quad != NULL)
        {
            if (strcmp(current_quad->name, name_buffer) == 0)
            {
                current_quad->weight = weights[0];
                break;
            }
            current_quad = current_quad->next;
        }

        skip_stat *current_skip = skip_head;
        while (current_skip != NULL)
        {
            if (strcmp(current_skip->name, name_buffer) == 0)
            {
                /* Update weights array for skip_stat */
                for (int i = 1; i <= 9; i++)
                {
                    current_skip->weight[i] = weights[i - 1];
                }
                break;
            }
            current_skip = current_skip->next;
        }

        meta_stat *current_meta = meta_head;
        while (current_meta != NULL)
        {
            if (strcmp(current_meta->name, name_buffer) == 0)
            {
                current_meta->weight = weights[0];
                break;
            }
            current_meta = current_meta->next;
        }
    }

    fclose(weight_file);
    free(path);
}

/*
 * Reads a keyboard layout from a file and initializes a layout structure.
 * The layout file is specified by either 'layout_name' or 'layout2_name'
 * based on the 'which_layout' parameter. The function reads the layout matrix
 * and sets the layout name in the provided layout structure.
 *
 * Parameters:
 *   lt:           A pointer to the layout structure to be initialized.
 *   which_layout: An integer indicating which layout to read (1 or 2).
 * Returns: void.
 */
void read_layout(layout *lt, int which_layout)
{
    FILE *layout_file;
    /* Construct the path to the layout file based on 'which_layout' parameter. */
    char *path = (char*)malloc(strlen("./data//layouts/.glg")
            + strlen(lang_name) + strlen(layout_name) + 1);
    if (which_layout == 2) {
        path = (char*)realloc(path, strlen("./data//layouts/.glg")
            + strlen(lang_name) + strlen(layout2_name) + 1);
    }
    strcpy(path, "./data/");
    strcat(path, lang_name);
    strcat(path, "/layouts/");
    if (which_layout == 1) {strcat(path, layout_name);}
    else if (which_layout == 2) {strcat(path, layout2_name);}
    else {error("invalid layout selected to read");}
    strcat(path, ".glg");
    layout_file = fopen(path, "r");
    if (layout_file == NULL) {
        error("Layout file not found.");
    }

    /* Set the layout name in the layout structure. */
    if (which_layout == 1) {strncpy(lt->name, layout_name, 99);}
    else if (which_layout == 2) {strncpy(lt->name, layout2_name, 99);}

    wchar_t curr;
    /* Read the layout matrix from the file. */
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            if (fwscanf(layout_file, L" %lc ", &curr) != 1) {
                error("layout not 3x12 (fill dead-keys with @'s)");
            }
            lt->matrix[i][j] = convert_char(curr);
        }
    }

    fclose(layout_file);
    free(path);
    return;
}

/*
 * Prints the layout name and score.
 * Parameters:
 *   lt: A pointer to the layout structure to be printed.
 * Returns: void.
 */
void quiet_print(layout *lt)
{
    log_print('q',L"%s\n", lt->name);
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            log_print('q',L"%lc ", convert_back(lt->matrix[i][j]));
        }
        log_print('q',L"\n");
    }

    log_print('q',L"score : %f\n\n", lt->score);
    return;
}

/*
 * Prints the layout along with ngram stats.
 * Parameters:
 *   lt: A pointer to the layout structure to be printed.
 * Returns: void.
 */
void normal_print(layout *lt)
{
    quiet_print(lt);
    log_print('n',L"\nMONOGRAM STATS\n");
    for (int i = 0; i < MONO_END; i++) {log_print('n',L"%s : %f\%\n", stats_mono[i].name, lt->mono_score[i]);}
    log_print('n',L"\nBIGRAM STATS\n");
    for (int i = 0; i < BI_END; i++) {log_print('n',L"%s : %f\%\n", stats_bi[i].name, lt->bi_score[i]);}
    log_print('n',L"\nTRIGRAM STATS\n");
    for (int i = 0; i < TRI_END; i++) {log_print('n',L"%s : %f\%\n", stats_tri[i].name, lt->tri_score[i]);}
    log_print('n',L"\nQUADGRAM STATS\n");
    for (int i = 0; i < QUAD_END; i++) {log_print('n',L"%s : %f\%\n", stats_quad[i].name, lt->quad_score[i]);}
    log_print('n',L"\nSKIPGRAM STATS\n");
    for (int i = 0; i < SKIP_END; i++)
    {
        log_print('n',L"%s : ", stats_skip[i].name);
        for (int j = 1; j <= 9; j++)
        {
            log_print('n',L"%f", lt->skip_score[j][i]);
            log_print('n',L"%|");
        }
        log_print('n',L"\n");
    }
    log_print('n',L"\nMETA STATS\n");
    for (int i = 0; i < META_END; i++) {log_print('n',L"%s : %f\%\n", stats_meta[i].name, lt->meta_score[i]);}
}

/*
 * Prints detailed information, currently the same as normal_print.
 * Parameters:
 *   lt: A pointer to the layout structure to be printed.
 * Returns: void.
 */
void verbose_print(layout *lt)
{
    normal_print(lt);
}

/*
 * Prints the contents of a layout structure to the standard output.
 * The function uses the current 'output_mode' to determine the level of detail
 * to print, ranging from just the layout matrix and score to detailed
 * statistics for each ngram type.
 *
 * Parameters:
 *   lt: A pointer to the layout structure to be printed.
 * Returns: void.
 */
void print_layout(layout *lt)
{
    switch(output_mode)
    {
        case 'q': quiet_print(lt);
                  break;
        case 'n': normal_print(lt);
                  break;
        default:
        case 'v': verbose_print(lt);
                  break;
    }
    return;
}

/*
 * Prints the ranked list of layouts to the standard output.
 * Layouts are printed in descending order of their score, along with their names.
 * The function traverses the 'head_node' linked list to access each layout node.
 *
 * Returns: void.
 */
void print_ranking()
{
    log_print('q',L"Raking:\n\n");
    layout_node *current = head_node;
    if (current == NULL) {log_print('q',L"no layouts in ranking?\n");}
    while (current != NULL) {
        log_print('q',L"%s -> %f\n", current->name, current->score);
        current = current->next;
    }
}

/*
 * Prints the current pin configuration for layout improvement to the standard output.
 * Each element in the 'pins' array is printed, showing which positions are pinned.
 *
 * Returns: void.
 */
void print_pins()
{
    for (int i = 0; i < ROW; i++)
    {
        for (int j = 0; j < COL; j++)
        {
            log_print('v',L"%d ", pins[i][j]);
        }
        log_print('v',L"\n");
    }
}
