#ifndef IO_UTIL_H
#define IO_UTIL_H

#include "global.h"

int convert_char(wchar_t c);
wchar_t convert_back(int i);
void iterate(int *mem, int size);
int check_duplicates(wchar_t *arr);
char check_run_mode(char *optarg);
char check_output_mode(char *optarg);

#endif
