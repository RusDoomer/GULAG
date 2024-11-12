#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <getopt.h>
#include <unistd.h>

#include "io.h"
#include "io_util.h"
#include "global.h"

#define UNICODE_MAX 65535
#define BUFFER_SIZE 10000


void read_config()
{
    FILE *config;

    char discard[999];
    char buff[999];

    config = fopen("config.txt", "r");
    if (config == NULL) {
        error("Required file config.txt not found.");
    }

    char c;
    int i = 0;

    fscanf(config, " %s", discard);
    if (strcmp(discard, "pins:") != 0) {
        error("Expected 'pins:' at the start of the config file.");
    }

    while (i < ROW * COL && fscanf(config, " %c", &c) == 1) {
        if (c != '.') {
            pins[i / COL][i % COL] = 0;
        } else {
            pins[i / COL][i % COL] = 1;
        }
        i++;
    }

    fscanf(config, "%s %s", discard, buff);
    *lang_name = (char *)malloc((sizeof(char) * strlen(buff)) + 1);
    strcpy(*lang_name, buff);

    fscanf(config, "%s %s", discard, buff);
    *corpus_name = (char *)malloc((sizeof(char) * strlen(buff)) + 1);
    strcpy(*corpus_name, buff);

    fscanf(config, "%s %s", discard, buff);
    *layout_name = (char *)malloc((sizeof(char) * strlen(buff)) + 1);
    strcpy(*layout_name, buff);

    fscanf(config, "%s %s", discard, buff);
    *layout2_name = (char *)malloc((sizeof(char) * strlen(buff)) + 1);
    strcpy(*layout2_name, buff);

    fscanf(config, "%s %s", discard, buff);
    *weight_name = (char *)malloc((sizeof(char) * strlen(buff)) + 1);
    strcpy(*weight_name, buff);

    fscanf(config, "%s %s", discard, buff);
    if (strcmp(buff, "a") == 0 || strcmp(buff, "analyze") == 0
        || strcmp(buff, "analysis") == 0) {
        *run_mode = 'a';
    } else if (strcmp(buff, "c") == 0 || strcmp(buff, "compare") == 0
        || strcmp(buff, "comparison") == 0) {
        *run_mode = 'c';
    } else if (strcmp(buff, "r") == 0 || strcmp(buff, "rank") == 0
        || strcmp(buff, "ranking") == 0) {
        *run_mode = 'r';
    } else if (strcmp(buff, "g") == 0 || strcmp(buff, "generate") == 0
        || strcmp(buff, "gen") == 0) {
        *run_mode = 'g';
    } else if (strcmp(buff, "i") == 0 || strcmp(buff, "improve") == 0
        || strcmp(buff, "optimize") == 0) {
        *run_mode = 'i';
    } else if (strcmp(buff, "b") == 0 || strcmp(buff, "benchmark") == 0
        || strcmp(buff, "bench") == 0) {
        *run_mode = 'b';
    } else if (strcmp(buff, "h") == 0 || strcmp(buff, "help") == 0) {
        *run_mode = 'h';
    } else if (strcmp(buff, "if") == 0 || strcmp(buff, "info") == 0
        || strcmp(buff, "information") == 0) {
        *run_mode = 'f';
    } else {
        error("Invalid run mode in config.txt.");
    }
    fscanf(config, "%s %s", discard, buff);
    *repetitions = atoi(buff);
    fscanf(config, "%s %s", discard, buff);
    *threads = atoi(buff);
    fscanf(config, "%s %s", discard, buff);
    if (strcmp(buff, "q") == 0 || strcmp(buff, "quiet") == 0
        || strcmp(buff, "SHUTUP") == 0) {
        *output_mode = 'q';
    } else if (strcmp(buff, "n") == 0 || strcmp(buff, "normal") == 0
        || strcmp(buff, "norm") == 0) {
        *output_mode = 'n';
    } else if (strcmp(buff, "v") == 0 || strcmp(buff, "verbose") == 0
        || strcmp(buff, "loud") == 0) {
        *output_mode = 'v';
    } else {
        error("Invalid output mode in config.txt.");
    }

    fclose(config);
}

char check_run_mode(char *optarg)
{
    if (strcmp(optarg, "a") == 0
        || strcmp(optarg, "analyze") == 0
        || strcmp(optarg, "analysis") == 0) {
        return 'a';
    } else if (strcmp(optarg, "c") == 0
        || strcmp(optarg, "compare") == 0
        || strcmp(optarg, "comparison") == 0) {
        return 'c';
    } else if (strcmp(optarg, "r") == 0
        || strcmp(optarg, "rank") == 0
        || strcmp(optarg, "ranking") == 0) {
        return 'r';
    } else if (strcmp(optarg, "g") == 0
        || strcmp(optarg, "gen") == 0
        || strcmp(optarg, "generate") == 0) {
        return 'g';
    } else if (strcmp(optarg, "i") == 0
        || strcmp(optarg, "improve") == 0
        || strcmp(optarg, "optimize") == 0) {
        return 'i';
    } else if (strcmp(optarg, "b") == 0
        || strcmp(optarg, "bench") == 0
        || strcmp(optarg, "benchmark") == 0) {
        return 'b';
    } else if (strcmp(optarg, "h") == 0
        || strcmp(optarg, "help") == 0) {
        return 'h';
    } else if (strcmp(optarg, "if") == 0
        || strcmp(optarg, "info") == 0
        || strcmp(optarg, "information") == 0) {
        return 'f';
    } else {
        error("Invalid run mode in arguments.");
        return 'a';
    }
}

char check_output_mode(char *optarg)
{
    if (strcmp(optarg, "q") == 0 || strcmp(optarg, "quiet") == 0
        || strcmp(optarg, "SHUTUP") == 0) {
        return 'q';
    } else if (strcmp(optarg, "n") == 0
        || strcmp(optarg, "norm") == 0
        || strcmp(optarg, "normal") == 0) {
        return 'n';
    } else if (strcmp(optarg, "v") == 0
        || strcmp(optarg, "loud") == 0
        || strcmp(optarg, "verbose") == 0) {
        return 'v';
    } else {
        error("Invalid output mode in arguments.");
        return 'n';
    }
}

void read_args(int argc, char **argv)
{
    int opt;
    while ((opt = getopt(argc, argv, "l:c:1:2:w:r:t:m:o:")) != -1) {
    switch (opt) {
        case 'l':
            free(*lang_name);
            *lang_name = strdup(optarg);
            break;
        case 'c':
            free(*corpus_name);
            *corpus_name = strdup(optarg);
            break;
        case '1':
            free(*layout_name);
            *layout_name = strdup(optarg);
            break;
        case '2':
            free(*layout2_name);
            *layout2_name = strdup(optarg);
            break;
        case 'w':
            free(*weight_name);
            *weight_name = strdup(optarg);
            break;
        case 'r':
            *repetitions = atoi(optarg);
            break;
        case 't':
            *threads = atoi(optarg);
            break;
        case 'm':
            *run_mode = check_run_mode(optarg);
            break;
        case 'o':
            *output_mode = check_output_mode(optarg);
            break;
        case '?':
            error("Improper Usage: %s -l lang_name -c corpus_name "
                "-1 layout_name -2 layout2_name -w weight_name -r repetitions "
                "-t threads -m run_mode -o output_mode");
        default:
            abort();
        }
    }
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

    wchar_t a;
    for (int i = 0; i < 101; i++) {
        if ((a = fgetwc(lang)) == EOF || a == L'\n') {lang_arr[i] = L'@';}
        else if (a == L'@') {
            error("'@' found in lang, illegal character.");
        } else {
            lang_arr[i] = a;
        }
    }

    if (lang_arr[0] != L' ' || lang_arr[1] != L' ') {
        error("Lang file must begin with 2 spaces");
    }

    if (lang_arr[100] != L'@') {
        error("Lang file too long (>100 characters)");
    }

    // this allows duplicate characters that are side by side in case
    // of no shifted pair (and to allow the double space at the start)
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

    return;
}

int read_corpus_cache()
{
    FILE *corpus;
    char *path = (char*)malloc(strlen("./data//corpora/cache_") +
        strlen(lang_name) + strlen(corpus_name) + 1);
    strcpy(path, "./data/");
    strcat(path, lang_name);
    strcat(path, "/corpora/cache_");
    strcat(path, corpus_name);
    corpus = fopen(path, "r");
    if (corpus == NULL) {
        free(path);
        return 0;
    }
    // wprintf(L"reading from cache\n");
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
    return 1;
}

void read_corpus()
{
    FILE *corpus;
    char *path = (char*)malloc(strlen("./data//corpora/") +
        strlen(lang_name) + strlen(corpus_name) + 1);
    strcpy(path, "./data/");
    strcat(path, lang_name);
    strcat(path, "/corpora/");
    strcat(path, corpus_name);
    corpus = fopen(path, "r");
    if (corpus == NULL) {
        error("Corpus file not found.");
    }

    wchar_t buff[BUFFER_SIZE];
    int mem[] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    size_t bytes_read;

    wchar_t curr;
    //wprintf(L"start reading\n");
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
    return;
}

void cache_corpus()
{
    FILE *corpus;
    char *path = (char*)malloc(strlen("./data//corpora/cache_") +
        strlen(lang_name) + strlen(corpus_name) + 1);
    strcpy(path, "./data/");
    strcat(path, lang_name);
    strcat(path, "/corpora/cache_");
    strcat(path, corpus_name);
    corpus = fopen(path, "w");
    if (corpus == NULL) {
        error("Corpus cache file failed to be created.");
    }

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
    return;
}

void read_weights()
{
    FILE *weight;
    char *path = (char*)malloc(strlen("./data/weights/") + strlen(weight_name) + 1);
    strcpy(path, "./data/weights/");
    strcat(path, weight_name);
    weight = fopen(path, "r");
    if (weight == NULL) {
        error("Weights file not found.");
    }

    int count = 1;
    weights_mono[0] = 1;
    while (count < MONO_END) {
        if (fwscanf(weight, L"%f", &weights_mono[count]) != 1) {
            if (feof(weight)) {error("Weights file incomplete");}
            else {fwscanf(weight, L"%*s");}
        } else {
            count++;
        }
    }

    count = 1;
    weights_bi[0] = 1;
    while (count < BI_END) {
        if (fwscanf(weight, L"%f", &weights_bi[count]) != 1) {
            if (feof(weight)) {error("Weights file incomplete");}
            else {fwscanf(weight, L"%*s");}
        } else {
            count++;
        }
    }

    count = 1;
    weights_tri[0] = 1;
    while (count < TRI_END) {
        if (fwscanf(weight, L"%f", &weights_tri[count]) != 1) {
            if (feof(weight)) {error("Weights file incomplete");}
            else {fwscanf(weight, L"%*s");}
        } else {
            count++;
        }
    }

    count = 1;
    weights_quad[0] = 1;
    while (count < QUAD_END) {
        if (fwscanf(weight, L"%f", &weights_quad[count]) != 1) {
            if (feof(weight)) {error("Weights file incomplete");}
            else {fwscanf(weight, L"%*s");}
        } else {
            count++;
        }
    }

    for (int i = 1; i < 10; i++) {
        count = 1;
        weights_skip[i][0] = 1;
        while (count < SKIP_END) {
            if (fwscanf(weight, L"%f", &weights_skip[i][count]) != 1) {
                if (feof(weight)) {error("Weights file incomplete");}
                else {fwscanf(weight, L"%*s");}
            } else {
                count++;
            }
        }
    }

    count = 0;
    while (count < META_END) {
        if (fwscanf(weight, L"%f", &weights_meta[count]) != 1) {
            if (feof(weight)) {error("Weights file incomplete");}
            else {fwscanf(weight, L"%*s");}
        } else {
            count++;
        }
    }

    fclose(weight);
    free(path);
    return;
}

void read_layout(layout *lt)
{
    FILE *layout_file;
    char *path = (char*)malloc(strlen("./data//layouts/") + strlen(lang_name) +
        strlen(layout_name) + 1);
    strcpy(path, "./data/");
    strcat(path, lang_name);
    strcat(path, "/layouts/");
    strcat(path, layout_name);
    layout_file = fopen(path, "r");
    if (layout_file == NULL) {
        error("Layout file not found.");
    }

    wchar_t curr;
    int temp;
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            if (fwscanf(layout_file, L" %lc ", &curr) != 1) {
                error("layout not 3x12 (fill deadspace with @s)");
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
    layout_node *current = head_node;
    while (current != NULL) {
        wprintf(L"%s -> %f\n", current->lt->name, current->lt->score);
        current = current->next;
    }
}
