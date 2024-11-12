#include "io.h"

void start_up()
{
    // set locale
    setlocale(LC_ALL, "en_US.UTF-8");

    // hide cursor
    wprintf(L"\e[?25l");
}

void shut_down()
{
    //show cursor
    wprintf(L"\e[?25h");
}

int main(int argc, char **argv) {
    start_up();

    /* io.c - holds defaults to be overwritten by args */
    read_config();

    /* io.c - overwrites config */
    read_args(argc, argv);

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
        read_corpus(); /* io.c */
        cache_corpus();/* io.c */
    }
    read_weights(); /* io.c */

    switch(run_mode) {
        case 'a':
            analysis(); /* analyze one layout */
            break;
        case 'c':
            compare(); /* compare 2 (or more?) layouts */
            break;
        case 'r':
            rank(); /* rank all layouts in directory */
            break;
        case 'g':
            generate(); /* generate a new layout */
            break;
        case 'i':
            improve(); /* improve a layout */
            break;
        case 'b':
            gen_benchmark(); /* benchmark to find ideal number of threads */
            break;
        case 'h':
            print_help(); /* print help info */
            break;
        case 'f':
            print_info(); /* print info screen */
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
