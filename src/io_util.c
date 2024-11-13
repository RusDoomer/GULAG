#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "io_util.h"
#include "util.h"
#include "global.h"

int convert_char(wchar_t c)
{
    int ret = char_table[(int) c];
    if (ret != 0) {return ret;}
    else {return -1;}
}

wchar_t convert_back(int i)
{
    if (i < 50 && i >= 0) {
        return lang_arr[i*2];
    }
    return L'@';
}

void iterate(int *mem, int size)
{
    for (int i = size - 1; i > 0; i--)
    {
        mem[i] = mem[i - 1];
    }
}

int check_duplicates(wchar_t *arr)
{
    int dups = -1;
    for (int i = 0; i < 101; i++) {
        for (int j = i + 2; j < 101; j++) {
            if (arr[i] == arr[j] && arr[i] != L'@') {
                dups++;
            }
        }
    }
    return dups;
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
