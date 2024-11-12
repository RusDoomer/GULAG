#include "io_util.h"

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
