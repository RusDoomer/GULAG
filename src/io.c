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

#define UNICODE_MAX 65535
#define BUFFER_SIZE 10000

#include <wctype.h>

void log_print(char required_level, const wchar_t *format, ...) {
    // Check if the current output mode meets or exceeds the required level
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

void read_config()
{
    FILE *config;

    char discard[999];
    char buff[999];

    config = fopen("config.conf", "r");
    if (config == NULL) {
        error("Required file config.conf not found.");
    }
    wprintf(L"config.conf found... ");

    char c;
    int i = 0;

    fscanf(config, " %s", discard);
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

    fscanf(config, "%s %s", discard, buff);
    lang_name = (char *)malloc((sizeof(char) * strlen(buff)) + 1);
    strcpy(lang_name, buff);

    fscanf(config, "%s %s", discard, buff);
    corpus_name = (char *)malloc((sizeof(char) * strlen(buff)) + 1);
    strcpy(corpus_name, buff);

    fscanf(config, "%s %s", discard, buff);
    layout_name = (char *)malloc((sizeof(char) * strlen(buff)) + 1);
    strcpy(layout_name, buff);

    fscanf(config, "%s %s", discard, buff);
    layout2_name = (char *)malloc((sizeof(char) * strlen(buff)) + 1);
    strcpy(layout2_name, buff);

    fscanf(config, "%s %s", discard, buff);
    weight_name = (char *)malloc((sizeof(char) * strlen(buff)) + 1);
    strcpy(weight_name, buff);

    fscanf(config, "%s %s", discard, buff);
    run_mode = check_run_mode(buff);

    fscanf(config, "%s %s", discard, buff);
    repetitions = atoi(buff);

    fscanf(config, "%s %s", discard, buff);
    threads = atoi(buff);

    fscanf(config, "%s %s", discard, buff);
    output_mode = check_output_mode(buff);

    fclose(config);
    wprintf(L"Done\n\n");
}

void read_args(int argc, char **argv)
{
    int opt;
    while ((opt = getopt(argc, argv, "l:c:1:2:w:r:t:m:o:")) != -1) {
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
            run_mode = check_run_mode(optarg);
            break;
        case 'o':
            output_mode = check_output_mode(optarg);
            break;
        case '?':
            error("Improper Usage: %s -l lang_name -c corpus_name "
                "-1 layout_name -2 layout2_name -w weight_name -r repetitions "
                "-t threads -m run_mode -o output_mode");
        default:
            abort();
        }
    }
    wprintf(L"Done\n\n");
}

void check_setup()
{
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
    if (threads < 1) {error("invalid threads selected");}
    if (repetitions < 1) {error("invalid repetitions selected");}
    wprintf(L"Done\n\n");
}

void read_lang()
{
    FILE *lang;
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
    wprintf(L"Lang file found... ");

    wprintf(L"Reading... ");
    wchar_t a;
    for (int i = 0; i < 101; i++) {
        if ((a = fgetwc(lang)) == EOF || a == L'\n') {lang_arr[i] = L'@';}
        else if (a == L'@') {
            error("'@' found in lang, illegal character.");
        } else {
            lang_arr[i] = a;
        }
    }

    wprintf(L"Checking correctness... ");

    if (lang_arr[0] != L' ' || lang_arr[1] != L' ') {
        error("Lang file must begin with 2 spaces");
    }

    if (lang_arr[100] != L'@') {
        error("Lang file too long (>100 characters)");
    }

    // this allows duplicate characters that are side by side for
    // of shifted pair (and to allow the double space at the start)
    if (check_duplicates(lang_arr) != -1) {
        error("Lang file contains duplicate characters.");
    }

    for (int i = 0; i < 101; i++) {
        if (lang_arr[i] == L'@') {
            char_table[L'@'] = -1;
        } else if (lang_arr[i] < UNICODE_MAX) {
            char_table[lang_arr[i]] = i/2;
        } else {
            error("Lang file contains illegal character not caught before.");
        }
    }

    wprintf(L"Done\n\n");
}

int read_corpus_cache()
{
    FILE *corpus;
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
        wprintf(L"Cache not found... ");
        return 0;
    }
    wprintf(L"Cache found... ");
    wprintf(L"Reading from cache... ");
    wchar_t curr;
    int i,j,k,l, value;
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
    wprintf(L"Done\n\n");
    return 1;
}

void read_corpus()
{
    FILE *corpus;
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
    wprintf(L"Corpus file found... ");

    int mem[] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};

    wchar_t curr;
    while ((curr = fgetwc(corpus)) != WEOF) {
        mem[0] = convert_char(curr);
        if (mem[0] > 0 && mem[0] < 51) {
            corpus_mono[mem[0]]++;

            // consecutive ngrams
            if (mem[1] > 0 && mem[1] < 51) {
                corpus_bi[mem[1]][mem[0]]++;
                if (mem[2] > 0 && mem[2] < 51) {
                    corpus_tri[mem[2]][mem[1]][mem[0]]++;
                    if (mem[3] > 0 && mem[3] < 51) {
                        corpus_quad[mem[3]][mem[2]][mem[1]][mem[0]]++;
                    }
                }
            }

            // skip ngrams
            for (int i = 2; i < 11; i++)
            {
                if (mem[i] > 0 && mem[i] < 51)
                {
                    corpus_skip[i-1][mem[i]][mem[0]]++;
                }
            }
        }
        iterate(mem, 11);
    }

    fclose(corpus);
    free(path);
    wprintf(L"Done\n\n");
}

void cache_corpus()
{
    FILE *corpus;
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
    wprintf(L"Created cache file... ");

    for (int i = 0; i < 51; i++) {
        for (int j = 0; j < 51; j++) {
            for (int k = 0; k < 51; k++) {
                for (int l = 0; l < 51; l++) {
                    if (corpus_quad[i][j][k][l] > 0) {
                        fprintf(corpus, "q %d %d %d %d %d\n", i, j, k, l,
                            corpus_quad[i][j][k][l]);
                    }
                }
                if (corpus_tri[i][j][k] > 0) {
                    fprintf(corpus, "t %d %d %d %d\n", i, j, k,
                        corpus_tri[i][j][k]);
                }
            }
            if (corpus_bi[i][j] > 0) {
                fprintf(corpus, "b %d %d %d\n", i, j, corpus_bi[i][j]);
            }
            for (int skip = 1; skip < 10; skip++) {
                if (corpus_skip[skip][i][j] > 0) {
                    fprintf(corpus, "%d %d %d %d\n", skip, i, j,
                        corpus_skip[skip][i][j]);
                }
            }
        }
        if (corpus_mono[i] > 0) {
            fprintf(corpus, "m %d %d\n", i, corpus_mono[i]);
        }
    }
    fclose(corpus);
    free(path);
    wprintf(L"Done\n\n");
}

void read_weights()
{
    FILE *weight_file;
    char *path = (char*)malloc(strlen("./data/weights/.wght") + strlen(weight_name) + 1);
    strcpy(path, "./data/weights/");
    strcat(path, weight_name);
    strcat(path, ".wght");
    weight_file = fopen(path, "r");
    if (weight_file == NULL) {
        error("Weights file not found.");
    }
    wprintf(L"Weights file found... ");

    wchar_t line[256];
    char name_buffer[256];

    while (fgetws(line, sizeof(line) / sizeof(line[0]), weight_file) != NULL)
    {
        // Parse the line: split on ':' to get name and weight(s)
        wchar_t *delimiter = wcschr(line, L':');
        if (delimiter == NULL)
        {
            continue; // Skip lines that don't have the expected format
        }

        *delimiter = L'\0';
        wchar_t *name_part = line;
        wchar_t *weight_part = delimiter + 1;

        // Trim whitespace around name and weight
        while (*name_part && iswspace(*name_part))
            name_part++;
        while (name_part[wcslen(name_part) - 1] && iswspace(name_part[wcslen(name_part) - 1]))
            name_part[wcslen(name_part) - 1] = L'\0';

        while (*weight_part && iswspace(*weight_part))
            weight_part++;
        while (weight_part[wcslen(weight_part) - 1] && iswspace(weight_part[wcslen(weight_part) - 1]))
            weight_part[wcslen(weight_part) - 1] = L'\0';

        // Convert wide name to a normal string
        wcstombs(name_buffer, name_part, sizeof(name_buffer));

        // Parse and process weights
        float weights[10] = {0}; // Temporary storage for weights
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

        // Find the matching stat in the linked lists and update its weight(s)
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
                // Update weights array for skip_stat
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
    wprintf(L"Done\n\n");
}


void read_layout(layout *lt, int which_layout)
{
    FILE *layout_file;
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

    if (which_layout == 1) {strncpy(lt->name, layout_name, 99);}
    else if (which_layout == 2) {strncpy(lt->name, layout2_name, 99);}


    wchar_t curr;
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

void quiet_print(layout *lt)
{
    wprintf(L"%s\n", lt->name);
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            wprintf(L"%lc ", convert_back(lt->matrix[i][j]));
        }
        wprintf(L"\n");
    }

    wprintf(L"score : %f\n", lt->score);
    return;
}

void normal_print(layout *lt)
{
    quiet_print(lt);
    wprintf(L"\nMONOGRAM STATS\n");
    for (int i = 0; i < MONO_END; i++) {wprintf(L"%s : %f\%\n", stats_mono[i].name, lt->mono_score[i]);}
    wprintf(L"\nBIGRAM STATS\n");
    for (int i = 0; i < BI_END; i++) {wprintf(L"%s : %f\%\n", stats_bi[i].name, lt->bi_score[i]);}
    wprintf(L"\nTRIGRAM STATS\n");
    for (int i = 0; i < TRI_END; i++) {wprintf(L"%s : %f\%\n", stats_tri[i].name, lt->tri_score[i]);}
    wprintf(L"\nQUADGRAM STATS\n");
    for (int i = 0; i < QUAD_END; i++) {wprintf(L"%s : %f\%\n", stats_quad[i].name, lt->quad_score[i]);}
    wprintf(L"\nSKIPGRAM STATS\n");
    for (int i = 0; i < SKIP_END; i++)
    {
        wprintf(L"%s : ", stats_skip[i].name);
        for (int j = 1; j <= 9; j++)
        {
            wprintf(L"%f", lt->skip_score[j][i]);
            wprintf(L"%|");
        }
        wprintf(L"\n");
    }
    wprintf(L"\nMETA STATS\n");
    for (int i = 0; i < META_END; i++) {wprintf(L"%s : %f\%\n", stats_meta[i].name, lt->meta_score[i]);}
}

void verbose_print(layout *lt)
{
    normal_print(lt);
}

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

void print_ranking()
{
    wprintf(L"Raking:\n\n");
    layout_node *current = head_node;
    if (current == NULL) {wprintf(L"no layouts in ranking?\n");}
    while (current != NULL) {
        wprintf(L"%s -> %f\n", current->name, current->score);
        current = current->next;
    }
    wprintf(L"\nDone\n\n");
}

void print_pins()
{
    for (int i = 0; i < ROW; i++)
    {
        for (int j = 0; j < COL; j++)
        {
            wprintf(L"%d ", pins[i][j]);
        }
        wprintf(L"\n");
    }
}
