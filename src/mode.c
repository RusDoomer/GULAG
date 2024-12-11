#include <wchar.h>

#include "mode.h"
#include "util.h"
#include "io.h"

void analysis() {
    layout *lt;
    alloc_layout(&lt);
    read_layout(lt, 1);
    error("analysis not implemented");
    get_score(lt);
    print_layout(lt);
    free_layout(lt);
    return;
}

void compare() {
    error("compare not implemented");
    return;
}

void rank() {
    error("rank not implemented");
    return;
}

void generate() {
    error("generate not implemented");
    return;
}

void improve() {
    error("improve not implemented");
    return;
}

void gen_benchmark() {
    error("gen benchmark not implemented");
    return;
}

void print_help() {
    wprintf(L"Arguments:\n");
    // 80     @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    wprintf(L"  -l <language> : Chooses the language, the basis of all data in this program.\n");
    wprintf(L"                  The language chooses which corpora and layouts you can access.\n");
    wprintf(L"  -c <corpus>   : Chooses the corpus file within the language directory.\n");
    wprintf(L"  -1 <layout>   : Chooses the primary layout within the language directory.\n");
    wprintf(L"  -2 <layout>   : Chooses the secondary layout within the language directory.\n");
    wprintf(L"  -w <weights>  : Chooses the weights file within the weights directory.\n");
    wprintf(L"  -r <val>      : Chooses the total number of layouts to analyze during\n");
    wprintf(L"                  generation modes, it is recommended to set this number\n");
    wprintf(L"                  between 5,000 and 100,000.\n");
    wprintf(L"  -t <val>      : Chooses the number of layouts to analyze concurrently in the\n");
    wprintf(L"                  generation modes. It is recommended to set this number based\n");
    wprintf(L"                  on the benchmark output.\n");


    wprintf(L"Modes:\n");
    // 80     @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    wprintf(L"  -m <mode>     : Decides what the program does.\n");
    wprintf(L"    a;analyze;analysis   : Analyzes the primary layout.\n");
    wprintf(L"    c;compare;comparison : Analyzes the primary and secondary layouts and prints\n");
    wprintf(L"                           the difference (primary minus secondary).\n");
    wprintf(L"    r;rank;ranking       : Analyzes every layout in the language and prints them\n");
    wprintf(L"                           in descending order by score.\n");
    wprintf(L"    g;gen;generate       : Creates a new layout attempting to optimize score;\n");
    wprintf(L"                           uses first 36 characters in language.\n");
    wprintf(L"    i;improve;optimize   : Optimizes an existing layout, won't swap keys pinned\n");
    wprintf(L"                           in the config.\n");
    wprintf(L"    b;bench;benchmark    : Prints the optimal number of threads for generation\n");
    wprintf(L"                           performance on this system.\n");
    wprintf(L"    h;help               : Prints this message.\n");
    wprintf(L"    if;info;information  : Prints more in-depth information about this program.\n");
    // 80     @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    wprintf(L"  -o <mode>     : decides how verbose the output is.\n");
    wprintf(L"    q;quiet;SHUTUP       : The most concise; prints only the essential stats.\n");
    wprintf(L"    n;norm;normal        : Prints most stats; ignores the most pedantic.\n");
    wprintf(L"    v;loud;verbose       : The most verbose; prints all stats.\n");
    // 80     @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    wprintf(L"Config:\n");
    wprintf(L"  All of these options can be set in config.conf but command line arguments will\n");
    wprintf(L"  be prioritized. config.conf also sets the pins for the improve mode; all\n");
    wprintf(L"  positions that are not \'.\' will be pinned.\n");
}

void print_info() {
    error("not implemented");
    return;
}
