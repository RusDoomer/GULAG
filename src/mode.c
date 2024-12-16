#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <dirent.h>
#include <string.h>
#include <pthread.h>
#include <math.h>

#include "mode.h"
#include "util.h"
#include "io_util.h"
#include "io.h"
#include "analyze.h"

void analysis() {
    layout *lt;
    wprintf(L"1/6: Allocating layout... ");
    alloc_layout(&lt);
    wprintf(L"Done\n\n");

    wprintf(L"2/6: Reading layout... ");
    read_layout(lt, 1);
    wprintf(L"Done\n\n");

    wprintf(L"3/6: Analyzing layout... ");
    single_analyze(lt);
    wprintf(L"Done\n\n");

    wprintf(L"4/6: Calculating Score... ");
    get_score(lt);
    wprintf(L"Done\n\n");

    wprintf(L"5/6: Printing Output...\n\n");
    print_layout(lt);
    wprintf(L"Done\n\n");

    wprintf(L"6/6: Freeing layout... ");
    free_layout(lt);
    wprintf(L"Done\n\n");
    return;
}

void compare() {
    layout *lt1, *lt2, *lt_diff;
    wprintf(L"1/7: Allocating layouts... ");
    alloc_layout(&lt1);
    wprintf(L"%s... ", layout_name);
    alloc_layout(&lt2);
    wprintf(L"%s... ", layout2_name);
    alloc_layout(&lt_diff);
    wprintf(L"diff... ");
    wprintf(L"Done\n\n");

    wprintf(L"2/7: Reading layout... ");
    read_layout(lt1, 1);
    wprintf(L"%s... ", layout_name);
    read_layout(lt2, 2);
    wprintf(L"%s... ", layout2_name);
    wprintf(L"Done\n\n");

    wprintf(L"3/7: Analyzing layout... ");
    single_analyze(lt1);
    wprintf(L"%s... ", layout_name);
    single_analyze(lt2);
    wprintf(L"%s... ", layout2_name);
    wprintf(L"Done\n\n");

    wprintf(L"4/7: Calculating Score... ");
    get_score(lt1);
    wprintf(L"%s... ", layout_name);
    get_score(lt2);
    wprintf(L"%s... ", layout2_name);
    wprintf(L"Done\n\n");

    wprintf(L"5/7: Calculating Difference... ");
    get_layout_diff(lt1, lt2, lt_diff);
    wprintf(L"Done\n\n");

    wprintf(L"6/7: Printing Output...\n\n");
    print_layout(lt_diff);
    wprintf(L"Done\n\n");

    wprintf(L"7/7: Freeing layout... ");
    free_layout(lt1);
    wprintf(L"%s... ", layout_name);
    free_layout(lt2);
    wprintf(L"%s... ", layout2_name);
    free_layout(lt_diff);
    wprintf(L"diff... ");
    wprintf(L"Done\n\n");
    return;
}

void rank() {
    //error("rank not implemented");
    char *path = (char*)malloc(strlen("./data//layouts") + strlen(lang_name) + 1);
    strcpy(path, "./data/");
    strcat(path, lang_name);
    strcat(path, "/layouts");

    DIR *dir = opendir(path);
    if (dir == NULL) {error("Error opening layouts directory");}

    free(layout_name);

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strstr(entry->d_name, ".glg") != NULL) {
            int len = strlen(entry->d_name);
            char temp_name[len - 3];
            strncpy(temp_name, entry->d_name, len - 4);
            temp_name[len - 4] = '\0';
            layout_name = temp_name;
            wprintf(L"%s: ", layout_name);

            layout *lt;
            wprintf(L"Allocating... ");
            alloc_layout(&lt);

            wprintf(L"Reading... ");
            read_layout(lt, 1);

            wprintf(L"Analyzing... ");
            single_analyze(lt);

            wprintf(L"Get Score... ");
            get_score(lt);

            wprintf(L"Ranking...");
            create_node(lt);

            wprintf(L"Freeing... ");
            free_layout(lt);
            wprintf(L"Done\n\n");
        }
    }

    print_ranking();

    layout_name = (char *)malloc(1);
    closedir(dir);
    free(path);
    free_list();
}

typedef struct thread_data {
    layout *lt;
    layout **best_lt;
    int iterations;
} thread_data;

void *thread_function(void *arg) {
    thread_data *data = (thread_data *)arg;
    layout *lt = data->lt;
    int iterations = data->iterations;

    // Allocate max and working layouts
    layout *max_lt, *working_lt;
    alloc_layout(&max_lt);
    alloc_layout(&working_lt);

    // Copy initial layout to working and max
    copy(working_lt, lt);

    // Set name so we can see if we improved
    strcat(working_lt->name, " improved");

    // Analyze and score the initial layout
    single_analyze(working_lt);
    get_score(working_lt);
    copy(max_lt, working_lt);

    // Simulated annealing
    for (int i = 0; i < iterations; i++) {
        // Swap two characters in working_lt
        int row1, col1, row2, col2;
        do {
            row1 = rand() % ROW;
            col1 = rand() % COL;
            row2 = rand() % ROW;
            col2 = rand() % COL;
        } while (pins[row1][col1] || pins[row2][col2] || (row1 == row2 && col1 == col2));

        // Perform the swap
        int temp = working_lt->matrix[row1][col1];
        working_lt->matrix[row1][col1] = working_lt->matrix[row2][col2];
        working_lt->matrix[row2][col2] = temp;

        // Analyze and score the new layout
        single_analyze(working_lt);
        get_score(working_lt);

        // Simulated annealing acceptance criterion
        float delta_score = working_lt->score - max_lt->score;
        if (delta_score > 0 || exp(delta_score / (1000.0 * (1 - (float)i / iterations))) > random_float()) {
            copy(max_lt, working_lt);
        } else {
            // Revert the swap
            temp = working_lt->matrix[row1][col1];
            working_lt->matrix[row1][col1] = working_lt->matrix[row2][col2];
            working_lt->matrix[row2][col2] = temp;
        }
    }

    // Allocate memory for best layout and copy max_lt to it
    layout *best_layout;
    alloc_layout(&best_layout);
    copy(best_layout, max_lt);
    *(data->best_lt) = best_layout;

    // Free layouts
    free_layout(max_lt);
    free_layout(working_lt);

    pthread_exit(NULL);
}

void generate() {
    for (int i = 0; i < ROW; i++)
    {
        for (int j = 0; j < COL; j++)
        {
            pins[i][j] = 0;
        }
    }
    improve(1);
    return;
}

void improve(int shuffle) {
    layout *lt;

    wprintf(L"Pins: \n");
    print_pins();
    wprintf(L"\n");

    wprintf(L"1/8: Allocating layout... ");
    alloc_layout(&lt);
    wprintf(L"Done\n\n");

    wprintf(L"2/8: Reading layout... ");
    read_layout(lt, 1);
    wprintf(L"Done\n\n");

    // If shuffle is true, shuffle the layout matrix
    if (shuffle) {
        wprintf(L"3/8: Shuffling layout... ");
        shuffle_layout(lt);
        strcpy(lt->name, "random shuffle");
        wprintf(L"Done\n\n");
    } else {
        wprintf(L"3/8: Skipping shuffle... ");
        wprintf(L"Done\n\n");
    }

    wprintf(L"4/8: Analyzing starting point... ");
    single_analyze(lt);
    get_score(lt);
    wprintf(L"Done\n\n");

    print_layout(lt);
    wprintf(L"\n");

    // Determine the number of iterations per thread
    int iterations = repetitions / threads;

    // Allocate thread data and pthread_t array
    thread_data *thread_data_array = (thread_data *)malloc(threads * sizeof(thread_data));
    pthread_t *thread_ids = (pthread_t *)malloc(threads * sizeof(pthread_t));
    layout **best_layouts = (layout **)malloc(threads * sizeof(layout *));

    // Initialize thread data and create threads
    wprintf(L"5/8: Initializing threads... ");
    for (int i = 0; i < threads; i++) {
        best_layouts[i] = NULL;
        thread_data_array[i].lt = lt;
        thread_data_array[i].best_lt = &best_layouts[i];
        thread_data_array[i].iterations = iterations;
        pthread_create(&thread_ids[i], NULL, thread_function, (void *)&thread_data_array[i]);
    }
    wprintf(L"Done\n\n");

    // Wait for all threads to complete
    wprintf(L"6/8: Waiting for threads to complete... ");
    for (int i = 0; i < threads; i++) {
        pthread_join(thread_ids[i], NULL);
    }
    wprintf(L"Done\n\n");

    // Find the best layout among all threads
    wprintf(L"7/8: Selecting best layout... ");
    layout *best_layout = best_layouts[0];
    for (int i = 1; i < threads; i++) {
        if (best_layouts[i]->score > best_layout->score) {
            best_layout = best_layouts[i];
        }
    }
    wprintf(L"Done\n\n");

    // Compare with the original layout and print the better one
    wprintf(L"8/8: Printing layout...\n\n");
    if (best_layout->score > lt->score) {
        print_layout(best_layout);
    } else {
        print_layout(lt);
    }
    wprintf(L"Done\n\n");

    // Free all allocated layouts and thread data
    for (int i = 0; i < threads; i++) {
        if (best_layouts[i] != NULL) {
            free_layout(best_layouts[i]);
        }
    }
    free_layout(lt);
    free(thread_data_array);
    free(thread_ids);
    free(best_layouts);
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
