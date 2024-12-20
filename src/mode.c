#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <dirent.h>
#include <string.h>
#include <pthread.h>
#include <math.h>
#include <unistd.h>
#include <time.h>

#include "mode.h"
#include "util.h"
#include "io_util.h"
#include "io.h"
#include "analyze.h"

void analysis() {
    layout *lt;
    log_print('n',L"1/6: Allocating layout... ");
    alloc_layout(&lt);
    log_print('n',L"Done\n\n");

    log_print('n',L"2/6: Reading layout... ");
    read_layout(lt, 1);
    log_print('n',L"Done\n\n");

    log_print('n',L"3/6: Analyzing layout... ");
    single_analyze(lt);
    log_print('n',L"Done\n\n");

    log_print('n',L"4/6: Calculating Score... ");
    get_score(lt);
    log_print('n',L"Done\n\n");

    log_print('n',L"5/6: Printing Output...\n\n");
    print_layout(lt);
    log_print('n',L"Done\n\n");

    log_print('n',L"6/6: Freeing layout... ");
    free_layout(lt);
    log_print('n',L"Done\n\n");
    return;
}

void compare() {
    layout *lt1, *lt2, *lt_diff;
    log_print('n',L"1/7: Allocating layouts... ");
    alloc_layout(&lt1);
    log_print('n',L"%s... ", layout_name);
    alloc_layout(&lt2);
    log_print('n',L"%s... ", layout2_name);
    alloc_layout(&lt_diff);
    log_print('n',L"diff... ");
    log_print('n',L"Done\n\n");

    log_print('n',L"2/7: Reading layout... ");
    read_layout(lt1, 1);
    log_print('n',L"%s... ", layout_name);
    read_layout(lt2, 2);
    log_print('n',L"%s... ", layout2_name);
    log_print('n',L"Done\n\n");

    log_print('n',L"3/7: Analyzing layout... ");
    single_analyze(lt1);
    log_print('n',L"%s... ", layout_name);
    single_analyze(lt2);
    log_print('n',L"%s... ", layout2_name);
    log_print('n',L"Done\n\n");

    log_print('n',L"4/7: Calculating Score... ");
    get_score(lt1);
    log_print('n',L"%s... ", layout_name);
    get_score(lt2);
    log_print('n',L"%s... ", layout2_name);
    log_print('n',L"Done\n\n");

    log_print('n',L"5/7: Calculating Difference... ");
    get_layout_diff(lt1, lt2, lt_diff);
    log_print('n',L"Done\n\n");

    log_print('n',L"6/7: Printing Output...\n\n");
    print_layout(lt_diff);
    log_print('n',L"Done\n\n");

    log_print('n',L"7/7: Freeing layout... ");
    free_layout(lt1);
    log_print('n',L"%s... ", layout_name);
    free_layout(lt2);
    log_print('n',L"%s... ", layout2_name);
    free_layout(lt_diff);
    log_print('n',L"diff... ");
    log_print('n',L"Done\n\n");
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
            log_print('n',L"%s: ", layout_name);

            layout *lt;
            log_print('n',L"Allocating... ");
            alloc_layout(&lt);

            log_print('n',L"Reading... ");
            read_layout(lt, 1);

            log_print('n',L"Analyzing... ");
            single_analyze(lt);

            log_print('n',L"Get Score... ");
            get_score(lt);

            log_print('n',L"Ranking...");
            create_node(lt);

            log_print('n',L"Freeing... ");
            free_layout(lt);
            log_print('n',L"Done\n\n");
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
    int thread_id;
} thread_data;

void *thread_function(void *arg) {
    thread_data *data = (thread_data *)arg;
    layout *lt = data->lt;
    int iterations = data->iterations;
    int thread_id = data->thread_id;

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

    // Simulated annealing with percentage completion and estimated time for the first thread
    struct timespec start, current;
    clock_gettime(CLOCK_MONOTONIC, &start);
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

        // Percentage completion and estimated time for the first thread
        if (thread_id == 0 && i % 100 == 0) { // Only update every 100 iterations
            clock_gettime(CLOCK_MONOTONIC, &current);
            double elapsed = (current.tv_sec - start.tv_sec) + (current.tv_nsec - start.tv_nsec) / 1e9;
            double progress = (double)i / iterations;
            double iterationsPerSecond = i / elapsed;
            double totalIterationsPerSecond = iterationsPerSecond * threads;
            int estimatedRemaining = (iterations - i) / iterationsPerSecond; // Calculate remaining time for this thread

            log_print('n', L"\r%3d%%  ETA: %5ds, %8.0lf layouts/sec             ",
                     (int)(progress * 100), estimatedRemaining, totalIterationsPerSecond);
            fflush(stdout);
        }
    }
    if (thread_id == 0) {
        log_print('q', L"\n"); // Newline after percentage reaches 100%
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
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            pins[i][j] = 0;
        }
    }
    improve(1);
}

void improve(int shuffle) {
    layout *lt;

    log_print('v',L"Pins: \n");
    print_pins();
    log_print('v',L"\n");

    log_print('n',L"1/9: Allocating layout... ");
    alloc_layout(&lt);
    log_print('n',L"Done\n\n");

    log_print('n',L"2/9: Reading layout... ");
    read_layout(lt, 1);
    log_print('n',L"Done\n\n");

    if (shuffle) {
        log_print('n',L"3/9: Shuffling layout... ");
        shuffle_layout(lt);
        strcpy(lt->name, "random shuffle");
        log_print('n',L"Done\n\n");
    } else {
        log_print('n',L"3/8: Skipping shuffle... ");
        log_print('n',L"Done\n\n");
    }

    log_print('n',L"4/9: Analyzing starting point... ");
    single_analyze(lt);
    get_score(lt);
    log_print('n',L"Done\n\n");

    print_layout(lt);
    log_print('n',L"\n");

    normalize = 1;
    int iterations = repetitions / threads;

    thread_data *thread_data_array = (thread_data *)malloc(threads * sizeof(thread_data));
    pthread_t *thread_ids = (pthread_t *)malloc(threads * sizeof(pthread_t));
    layout **best_layouts = (layout **)malloc(threads * sizeof(layout *));

    log_print('n',L"5/9: Initializing threads... ");
    for (int i = 0; i < threads; i++) {
        best_layouts[i] = NULL;
        thread_data_array[i].lt = lt;
        thread_data_array[i].best_lt = &best_layouts[i];
        thread_data_array[i].iterations = iterations;
        thread_data_array[i].thread_id = i;
        pthread_create(&thread_ids[i], NULL, thread_function, (void *)&thread_data_array[i]);
    }
    log_print('n',L"Done\n\n");

    // Wait for all threads to complete
    log_print('n',L"6/9: Waiting for threads to complete... ");
    for (int i = 0; i < threads; i++) {
        pthread_join(thread_ids[i], NULL);
    }
    log_print('n',L"Done\n\n");

    // Find the best layout among all threads
    log_print('n',L"7/9: Selecting best layout... ");
    layout *best_layout = best_layouts[0];
    for (int i = 1; i < threads; i++) {
        if (best_layouts[i]->score > best_layout->score) {
            best_layout = best_layouts[i];
        }
    }
    log_print('n',L"Done\n\n");

    normalize = 0;
    log_print('n',L"8/9: Analyzing best layout... ");
    single_analyze(best_layout);
    get_score(best_layout);
    log_print('n',L"Done\n\n");

    // Compare with the original layout and print the better one
    log_print('n',L"9/9: Printing layout...\n\n");
    if (best_layout->score > lt->score) {
        print_layout(best_layout);
    } else {
        print_layout(lt);
    }
    log_print('n',L"Done\n\n");

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

void gen_benchmark()
{
    repetitions = 10000;
    int num_cpus = sysconf(_SC_NPROCESSORS_ONLN);
    int power_of_2 = 1;
    int count = 1;

    while (power_of_2 <= num_cpus) {
        power_of_2 *= 2;
        count++;
    }

    int total = count + 3;

    int *thread_array = (int *)calloc(total, sizeof(int));
    double *results = (double *)calloc(total, sizeof(double));

    thread_array[0] = 1;
    for (int i = 1; i < count; i++) {thread_array[i] = thread_array[i-1] * 2;}
    thread_array[count] = num_cpus / 2;
    thread_array[count + 1] = num_cpus;
    thread_array[count + 2] = num_cpus * 2;

    for (int i = 0; i < total; i++) {log_print('v',L"%d ", thread_array[i]);}
    log_print('v',L"\n\n");

    char temp = output_mode;
    output_mode = 'q';
    for (int i = 0; i < total; i++)
    {

        log_print('q',L"BENCHMARK RUN %d/%d\n\n", i+1, total);
        threads = thread_array[i];

        struct timespec start, end;
        clock_gettime(CLOCK_MONOTONIC, &start);

        generate();

        clock_gettime(CLOCK_MONOTONIC, &end);
        double elapsed = (end.tv_sec - start.tv_sec) +
                         (end.tv_nsec - start.tv_nsec) / 1e9;
        double time_per_repetition = elapsed / repetitions;
        results[i] = 1.0 / time_per_repetition;
    }
    output_mode = temp;

    log_print('q',L"\nBENCHMARK RESULTS:\n\n");
    log_print('q',L"Threads - Layouts/Second:\n\n");
    log_print('q',L"Powers of 2:\n");
    for (int i = 0; i < count; i++)
    {
        log_print('q',L"%7d - %lf\n", thread_array[i], results[i]);
    }
    log_print('q',L"\n");
    log_print('q',L"Based on CPU cores:\n");
    for (int i = count; i < total; i++)
    {
        log_print('q',L"%7d - %lf\n", thread_array[i], results[i]);
    }
    log_print('q',L"\n");
    log_print('q',L"Choose the lowest number of threads with acceptable Layouts/Second for best results.");
    log_print('q',L"\n\n");

    free(thread_array);
    free(results);
}

void print_help() {
    log_print('q',L"Arguments:\n");
    // 80           @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    log_print('q',L"  -l <language> : Chooses the language, the basis of all data in this program.\n");
    log_print('q',L"                  The language chooses which corpora and layouts you can access.\n");
    log_print('q',L"  -c <corpus>   : Chooses the corpus file within the language directory.\n");
    log_print('q',L"  -1 <layout>   : Chooses the primary layout within the language directory.\n");
    log_print('q',L"  -2 <layout>   : Chooses the secondary layout within the language directory.\n");
    log_print('q',L"  -w <weights>  : Chooses the weights file within the weights directory.\n");
    log_print('q',L"  -r <val>      : Chooses the total number of layouts to analyze during\n");
    log_print('q',L"                  generation modes, it is recommended to set this number\n");
    log_print('q',L"                  between 5,000 and 100,000.\n");
    log_print('q',L"  -t <val>      : Chooses the number of layouts to analyze concurrently in the\n");
    log_print('q',L"                  generation modes. It is recommended to set this number based\n");
    log_print('q',L"                  on the benchmark output.\n");


    log_print('q',L"Modes:\n");
    // 80           @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    log_print('q',L"  -m <mode>     : Decides what the program does.\n");
    log_print('q',L"    a;analyze;analysis   : Analyzes the primary layout.\n");
    log_print('q',L"    c;compare;comparison : Analyzes the primary and secondary layouts and prints\n");
    log_print('q',L"                           the difference (primary minus secondary).\n");
    log_print('q',L"    r;rank;ranking       : Analyzes every layout in the language and prints them\n");
    log_print('q',L"                           in descending order by score.\n");
    log_print('q',L"    g;gen;generate       : Creates a new layout attempting to optimize score;\n");
    log_print('q',L"                           uses first 36 characters in language.\n");
    log_print('q',L"    i;improve;optimize   : Optimizes an existing layout, won't swap keys pinned\n");
    log_print('q',L"                           in the config.\n");
    log_print('q',L"    b;bench;benchmark    : Prints the optimal number of threads for generation\n");
    log_print('q',L"                           performance on this system.\n");
    log_print('q',L"    h;help               : Prints this message.\n");
    log_print('q',L"    if;info;information  : Prints more in-depth information about this program.\n");
    // 80           @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    log_print('q',L"  -o <mode>     : decides how verbose the output is.\n");
    log_print('q',L"    q;quiet;SHUTUP       : The most concise; prints only the essential stats.\n");
    log_print('q',L"    n;norm;normal        : Prints most stats; ignores the most pedantic.\n");
    log_print('q',L"    v;loud;verbose       : The most verbose; prints all stats.\n");
    // 80           @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    log_print('q',L"Config:\n");
    log_print('q',L"  All of these options can be set in config.conf but command line arguments will\n");
    log_print('q',L"  be prioritized. config.conf also sets the pins for the improve mode; all\n");
    log_print('q',L"  positions that are not \'.\' will be pinned.\n");
}

void print_info() {
    log_print('q',L"Welcome to GULAG v0.1 (AKA CULAG)\n");
    return;
}
