/*
 * mode.c - Modes for the GULAG.
 *
 * This file implements the various modes of operation for the GULAG: analysis,
 * comparison, ranking, generation, and improvement. It utilizes multithreading
 * to enhance performance, specifically in the layout improvement process.
 */

#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <dirent.h>
#include <string.h>
#include <pthread.h>
#include <math.h>
#include <unistd.h>
#include <time.h>

#include <CL/cl.h>

#include "mode.h"
#include "stats_util.h"
#include "util.h"
#include "io_util.h"
#include "io.h"
#include "analyze.h"
#include "global.h"
#include "structs.h"

/*
 * Performs analysis on a single layout. This involves allocating memory for the
 * layout, reading layout data from a file, analyzing the layout, calculating
 * its score, printing the output, and freeing the allocated memory.
 */
void analysis() {
    /* Work for timing total/real layouts/second */
    layouts_analyzed = 1;
    struct timespec compute_start, compute_end;
    clock_gettime(CLOCK_MONOTONIC, &compute_start);

    layout *lt;

    /* allocates memory for a layout structure */
    log_print('n',L"1/6: Allocating layout... ");
    alloc_layout(&lt); /* util.c */
    log_print('n',L"Done\n\n");

    /* reads a keyboard layout from a file and initializes a layout structure */
    log_print('n',L"2/6: Reading layout... ");
    read_layout(lt, 1); /* io.c */
    log_print('n',L"Done\n\n");

    /* performs a single layout analysis */
    log_print('n',L"3/6: Analyzing layout... ");
    single_analyze(lt); /* analyze.c */
    log_print('n',L"Done\n\n");

    /* calculates and assigns the overall score to a layout based on its statistics */
    log_print('n',L"4/6: Calculating Score... ");
    get_score(lt); /* util.c */
    log_print('n',L"Done\n\n");

    /* prints the contents of a layout structure to the standard output */
    log_print('n',L"5/6: Printing Output...\n\n");
    print_layout(lt); /* io.c */
    log_print('n',L"Done\n\n");

    /* frees the memory occupied by a layout data structure */
    log_print('n',L"6/6: Freeing layout... ");
    free_layout(lt); /* util.c */
    log_print('n',L"Done\n\n");

    clock_gettime(CLOCK_MONOTONIC, &compute_end);
    elapsed_compute_time += (compute_end.tv_sec - compute_start.tv_sec) + (compute_end.tv_nsec - compute_start.tv_nsec) / 1e9;
    return;
}

/*
 * Compares two layouts and outputs the difference. This function allocates
 * memory for three layouts, reads data for two layouts from files, performs
 * analysis on both, calculates their scores, computes the difference, prints
 * the difference layout, and frees the allocated memory.
 */
void compare() {
    /* Work for timing total/real layouts/second */
    layouts_analyzed = 2;
    struct timespec compute_start, compute_end;

    layout *lt1, *lt2, *lt_diff;

    /* allocates memory for the layouts */
    log_print('n',L"1/7: Allocating layouts... ");
    alloc_layout(&lt1); /* util.c */
    log_print('n',L"%s... ", layout_name);
    alloc_layout(&lt2); /* util.c */
    log_print('n',L"%s... ", layout2_name);
    alloc_layout(&lt_diff); /* util.c */
    log_print('n',L"diff... ");
    log_print('n',L"Done\n\n");

    /* reading keyboard layouts and initializing */
    log_print('n',L"2/7: Reading layout... ");
    read_layout(lt1, 1); /* io.c */
    log_print('n',L"%s... ", layout_name);
    read_layout(lt2, 2); /* io.c */
    log_print('n',L"%s... ", layout2_name);
    log_print('n',L"Done\n\n");

    clock_gettime(CLOCK_MONOTONIC, &compute_start);

    /* perform layout analyses */
    log_print('n',L"3/7: Analyzing layout... ");
    single_analyze(lt1); /* analyze.c */
    log_print('n',L"%s... ", layout_name);
    single_analyze(lt2); /* analyze.c */
    log_print('n',L"%s... ", layout2_name);
    log_print('n',L"Done\n\n");

    /* calculate the overall scores */
    log_print('n',L"4/7: Calculating Score... ");
    get_score(lt1); /* util.c */
    log_print('n',L"%s... ", layout_name);
    get_score(lt2); /* util.c */
    log_print('n',L"%s... ", layout2_name);
    log_print('n',L"Done\n\n");

    clock_gettime(CLOCK_MONOTONIC, &compute_end);
    elapsed_compute_time += (compute_end.tv_sec - compute_start.tv_sec) + (compute_end.tv_nsec - compute_start.tv_nsec) / 1e9;

    /* calculate the difference between the two layouts */
    log_print('n',L"5/7: Calculating Difference... ");
    get_layout_diff(lt1, lt2, lt_diff); /* util.c */
    log_print('n',L"Done\n\n");

    /* print the diff layout */
    log_print('n',L"6/7: Printing Output...\n\n");
    print_layout(lt_diff);
    log_print('n',L"Done\n\n");

    /* free the memory */
    log_print('n',L"7/7: Freeing layout... ");
    free_layout(lt1); /* util.c */
    log_print('n',L"%s... ", layout_name);
    free_layout(lt2); /* util.c */
    log_print('n',L"%s... ", layout2_name);
    free_layout(lt_diff); /* util.c */
    log_print('n',L"diff... ");
    log_print('n',L"Done\n\n");
    return;
}

/*
 * Ranks all layouts in the layouts directory based on their scores. It reads
 * each layout file, analyzes it, calculates its score, and maintains a ranked
 * list of layouts. Finally, it prints the ranking.
 */
void rank() {
    /* Work for timing total/real layouts/second */
    layouts_analyzed = 0;
    struct timespec compute_start, compute_end;
    clock_gettime(CLOCK_MONOTONIC, &compute_start);

    /* Construct the path to the layouts directory */
    char *path = (char*)malloc(strlen("./data//layouts") + strlen(lang_name) + 1);
    strcpy(path, "./data/");
    strcat(path, lang_name);
    strcat(path, "/layouts");

    /* Open the directory */
    DIR *dir = opendir(path);
    if (dir == NULL) {error("Error opening layouts directory");}

    /* Free layout_name since it will be reallocated for each layout */
    free(layout_name);

    /* Iterate over each entry in the directory */
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        /* Check if the entry is a .glg file */
        if (strstr(entry->d_name, ".glg") != NULL) {
            /* Extract the layout name */
            int len = strlen(entry->d_name);
            char temp_name[len - 3];
            strncpy(temp_name, entry->d_name, len - 4);
            temp_name[len - 4] = '\0';
            layout_name = temp_name;
            log_print('n',L"%s: ", layout_name);

            layout *lt;

            /* allocate memory for a layout */
            log_print('n',L"Allocating... ");
            alloc_layout(&lt); /* util.c */

            /* read the keyboard layout */
            log_print('n',L"Reading... ");
            read_layout(lt, 1); /* io.c */

            /* perform a single layout analysis */
            log_print('n',L"Analyzing... ");
            single_analyze(lt); /* analyze.c */

            /* calculate the overall score */
            log_print('n',L"Get Score... ");
            get_score(lt); /* util.c */

            /*
             * create a new node for the layout ranking list and insert it in
             * the correct position
             */
            log_print('n',L"Ranking...");
            create_node(lt); /* util.c */

            /* frees the memory occupied by a layout data structure */
            log_print('n',L"Freeing... ");
            free_layout(lt); /* util.c */
            log_print('n',L"Done\n");
            layouts_analyzed++;
        }
    }
    log_print('n',L"\n");

    /* print the ranked list of layouts */
    print_ranking(); /* io.c */
    log_print('q',L"Done\n\n");

    /* Reset layout_name to a safe state */
    layout_name = (char *)malloc(1);
    closedir(dir);
    free(path);

    /* free all nodes in the layout ranking list */
    free_list(); /* util.c */

    clock_gettime(CLOCK_MONOTONIC, &compute_end);
    elapsed_compute_time += (compute_end.tv_sec - compute_start.tv_sec) + (compute_end.tv_nsec - compute_start.tv_nsec) / 1e9;
}

/* Structure to hold data for each thread in the layout improvement process. */
typedef struct thread_data {
    layout *lt;
    layout **best_lt;
    int iterations;
    int thread_id;
} thread_data;

/*
 * Function executed by each thread to improve a layout. It performs simulated
 * annealing to find a layout with a better score.
 *
 * Parameters:
 *   arg: A pointer to a thread_data structure.
 *
 * Returns: A pointer to the best layout found by the thread.
 */
void *thread_function(void *arg) {
    thread_data *data = (thread_data *)arg;
    layout *lt = data->lt;
    int iterations = data->iterations;
    int thread_id = data->thread_id;

    /* Allocate max and working layouts */
    layout *max_lt, *working_lt;
    /* Allocate memory for layouts */
    alloc_layout(&max_lt);     /* util.c */
    alloc_layout(&working_lt); /* util.c */

    /* copy initial layout to working and max */
    copy(working_lt, lt); /* util.c */

    /* Set name so we can see if we improved */
    strcat(working_lt->name, " improved");

    /* analyze and score the initial layout */
    single_analyze(working_lt); /* analyze.c */
    /* calculate the overall score */
    get_score(working_lt); /* util.c */
    /* copies the layout */
    copy(max_lt, working_lt); /* util.c */

    /* Simulated annealing with enhancements */
    struct timespec start, current;
    clock_gettime(CLOCK_MONOTONIC, &start);

    /* Initial temperature */
    float T = 1000.0;
    int reheating_count = 0;
    /* Starting number of swaps */
    int initial_swap_count = MAX_SWAPS;
    int swap_count;
    /* Store the initial temperature for scaling */
    float max_T = T;

    /* For adaptive cooling */
    int improvement_counter = 0;

    if (thread_id == 0) {log_print('n',L"Done\n\n");}
    if (thread_id == 0) {log_print('n',L"6/9: Waiting for threads to complete... \n");}

    for (int i = 0; i < iterations; i++) {
        /* Temperature-dependent swap count */
        swap_count = (int)(initial_swap_count * (T / max_T));
        swap_count = swap_count < 1 ? 1 : swap_count;
        swap_count = swap_count > initial_swap_count ? initial_swap_count : swap_count;

        /* Store the swaps for potential reversal */
        int swap_rows1[swap_count];
        int swap_cols1[swap_count];
        int swap_rows2[swap_count];
        int swap_cols2[swap_count];

        /* Perform the swaps */
        for (int j = 0; j < swap_count; j++) {
            int row1, col1, row2, col2;
            do {
                row1 = rand() % ROW;
                col1 = rand() % COL;
                row2 = rand() % ROW;
                col2 = rand() % COL;
            } while (pins[row1][col1] || pins[row2][col2] || (row1 == row2 && col1 == col2));

            /* Store swap locations for BOTH positions */
            swap_rows1[j] = row1;
            swap_cols1[j] = col1;
            swap_rows2[j] = row2;
            swap_cols2[j] = col2;

            /* Perform the swap */
            int temp = working_lt->matrix[row1][col1];
            working_lt->matrix[row1][col1] = working_lt->matrix[row2][col2];
            working_lt->matrix[row2][col2] = temp;
        }

        /* analyze the new layout */
        single_analyze(working_lt); /* analyze.c */
        /* calculates the new score */
        get_score(working_lt); /* util.c */

        /* Exponentiate the score difference for acceptance probability (using sigmoid) */
        float delta_score = working_lt->score - max_lt->score;
        if (delta_score > 0 || (1.0 / (1.0 + exp(-10 * delta_score / T))) > random_float()) {
            /* copy the new layout if it passes */
            copy(max_lt, working_lt); /* util.c */
            /* Increment improvement counter */
            improvement_counter++;
        } else {
            /* Revert the swaps in reverse order if it fails */
            for (int j = swap_count - 1; j >= 0; j--) {
                int row1 = swap_rows1[j];
                int col1 = swap_cols1[j];
                int row2 = swap_rows2[j];
                int col2 = swap_cols2[j];

                /* Perform the reverse swap */
                int temp = working_lt->matrix[row1][col1];
                working_lt->matrix[row1][col1] = working_lt->matrix[row2][col2];
                working_lt->matrix[row2][col2] = temp;
            }
        }

        /* Adaptive cooling - Modified to adjust reheating temperature */
        if (i > 0 && i % (iterations / 20) == 0) {
            double improvement_rate = (double)improvement_counter / (iterations / 20);
            if (improvement_rate > 0.2) {
                /* Cool faster if improving rapidly */
                max_T *= 0.95;
            } else {
                /* Cool slower if not improving much */
                max_T *= 1.05;
            }
            /* Limit max_T to a reasonable upper bound */
            max_T = max_T > 1500.0 ? 1500.0 : max_T;
            /* Don't let max_T be less than the current T */
            max_T = max_T < T ? T : max_T;
            /* Reset counter */
            improvement_counter = 0;
        }

        /* Reheating with temperature clamp */
        if (i > 0 && i % (iterations / 10) == 0) {
            float old_T = T;
            /* Reheat to the potentially adjusted max_T */
            T = max_T;
            reheating_count++;
            if (thread_id == 0) {log_print('v', L"\nReheating (%d) | Old Temp: %f - New Temp: %f\n", reheating_count, old_T, T);}
        }

        /* Non-monotonic "jolt" */
        if (i > 0 && i % (iterations / 50) == 0) {
            T *= (1.0 + random_float() * 0.3);
            if (T > max_T) {
                T = max_T;
            }
        }

        /* Temperature cooling tied to iteration count */
        float progress = (float)i / iterations;
        /* Linear decrease */
        T = max_T * (1.0 - progress);
        /* Exponential decrease - You can try this too (seems worse) */
        /* T = max_T * exp(-5.0 * progress); */
        /* Prevent T from going below 1.0 */
        T = T < 1.0 ? 1.0 : T;

        /* Percentage completion and estimated time for the first thread */
        if (thread_id == 0 && i % 100 == 0) {
            clock_gettime(CLOCK_MONOTONIC, &current);
            double elapsed = (current.tv_sec - start.tv_sec) + (current.tv_nsec - start.tv_nsec) / 1e9;
            double progress_percent = (double)i / iterations;
            double iterationsPerSecond = i / elapsed;
            double totalIterationsPerSecond = iterationsPerSecond * threads;
            int estimatedRemaining = (int)((iterations - i) / iterationsPerSecond);

            /* Calculate hours, minutes, and seconds */
            int hours = estimatedRemaining / 3600;
            int minutes = (estimatedRemaining % 3600) / 60;
            int seconds = estimatedRemaining % 60;

            /* Print the result (with correct pluralization) */
            log_print('n', L"\r%3d%%  ETA: %02dh %02dm %02ds, %8.0lf layout%s/sec                 ",
                (int)(progress_percent * 100), hours, minutes, seconds, totalIterationsPerSecond,
                totalIterationsPerSecond == 1 ? "" : "s");
            fflush(stdout);
        }
    }
    if (thread_id == 0) {
        /* Newline after percentage reaches 100% */
        log_print('q', L"\n");
    }

    layout *best_layout;
    /* allocates memory for best layout */
    alloc_layout(&best_layout); /* util.c */
    /* copy max_lt to best layout */
    copy(best_layout, max_lt); /* util.c */
    *(data->best_lt) = best_layout;


    /* free layouts */
    free_layout(max_lt);     /* util.c */
    free_layout(working_lt); /* util.c */

    pthread_exit(NULL);
}

/*
 * Initiates the layout generation process without a specific starting layout.
 * Calls improve with shuffle set to 1, effectively starting from a random
 * layout, with no pins. Will still use set of keys from selected layout.
 */
void generate() {
    /* No specific layout used, so unpin all positions for a fresh start */
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            pins[i][j] = 0;
        }
    }
    improve(1);
}

/*
 * Improves an existing layout using multiple threads.
 * Each thread runs a simulated annealing process to find a better layout.
 *
 * Parameters:
 *   shuffle: A flag indicating whether to shuffle the layout before starting.
 */
void improve(int shuffle) {
    /* Work for timing total/real layouts/second */
    layouts_analyzed += ((int)repetitions/threads + 1) * threads;
    layouts_analyzed += 2;
    struct timespec compute_start, compute_end;
    clock_gettime(CLOCK_MONOTONIC, &compute_start);

    layout *lt;

    /* prints the current pins */
    log_print('v',L"Pins: \n");
    print_pins(); /* io.c */
    log_print('v',L"\n");

    /* allocate memory for layout */
    log_print('n',L"1/9: Allocating layout... ");
    alloc_layout(&lt); /* util.c */
    log_print('n',L"Done\n\n");

    /* read the starting keyboard layout */
    log_print('n',L"2/9: Reading layout... ");
    read_layout(lt, 1); /* io.c */
    log_print('n',L"Done\n\n");

    if (shuffle) {
        /* shuffles the matrix */
        log_print('n',L"3/9: Shuffling layout... ");
        shuffle_layout(lt); /* util.c */
        strcpy(lt->name, "random shuffle");
        log_print('n',L"Done\n\n");
    } else {
        log_print('n',L"3/8: Skipping shuffle... ");
        log_print('n',L"Done\n\n");
    }

    /* perform a single layout analysis */
    log_print('n',L"4/9: Analyzing starting point... ");
    single_analyze(lt); /* analyze.c */
    /* calculate the overall score */
    get_score(lt); /* util.c */
    log_print('n',L"Done\n\n");

    /* prints the starting layout */
    print_layout(lt); /* io.c */
    log_print('n',L"\n");

    int iterations = repetitions / threads;

    /* Allocate memory for thread data and thread IDs */
    thread_data *thread_data_array = (thread_data *)malloc(threads * sizeof(thread_data));
    pthread_t *thread_ids = (pthread_t *)malloc(threads * sizeof(pthread_t));
    layout **best_layouts = (layout **)malloc(threads * sizeof(layout *));

    /* Create and start the threads */
    log_print('n',L"5/9: Initializing threads... ");
    for (int i = 0; i < threads; i++) {
        best_layouts[i] = NULL;
        thread_data_array[i].lt = lt;
        thread_data_array[i].best_lt = &best_layouts[i];
        thread_data_array[i].iterations = iterations;
        thread_data_array[i].thread_id = i;
        pthread_create(&thread_ids[i], NULL, thread_function, (void *)&thread_data_array[i]);
    }

    /* Wait for all threads to complete */
    for (int i = 0; i < threads; i++) {
        pthread_join(thread_ids[i], NULL);
    }
    log_print('n',L"Done\n\n");

    /* Find the best layout among all threads */
    log_print('n',L"7/9: Selecting best layout... ");
    layout *best_layout = best_layouts[0];
    for (int i = 1; i < threads; i++) {
        if (best_layouts[i]->score > best_layout->score) {
            best_layout = best_layouts[i];
        }
    }
    log_print('n',L"Done\n\n");

    /* perform a single layout analysis */
    log_print('n',L"8/9: Analyzing best layout... ");
    single_analyze(best_layout); /* analyze.c */
    /* calculates the overall score */
    get_score(best_layout); /* util.c */
    log_print('n',L"Done\n\n");

    /* Compare with the original layout and print the better one */
    log_print('n',L"9/9: Printing layout...\n\n");
    if (best_layout->score > lt->score) {
        /* prints the best layout */
        print_layout(best_layout); /* io.c */
    } else {
        /* prints the starting layout */
        print_layout(lt); /* io.c */
    }
    log_print('n',L"Done\n\n");

    /* free all allocated layouts and thread data */
    for (int i = 0; i < threads; i++) {
        if (best_layouts[i] != NULL) {
            free_layout(best_layouts[i]); /* util.c */
        }
    }

    free_layout(lt);
    free(thread_data_array);
    free(thread_ids);
    free(best_layouts);
    clock_gettime(CLOCK_MONOTONIC, &compute_end);
    elapsed_compute_time += (compute_end.tv_sec - compute_start.tv_sec) + (compute_end.tv_nsec - compute_start.tv_nsec) / 1e9;
}

/* Generates a new layout using OpenCL. */
void cl_generate() {
    /* No specific layout used, so unpin all positions for a fresh start */
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            pins[i][j] = 0;
        }
    }
    cl_improve(1);
}

/*
 * Reads the content of a file into a dynamically allocated string.
 *
 * Parameters:
 *   filename: The path to the file.
 *   length: Pointer to size_t to store the length of the file content.
 *
 * Returns:
 *   A pointer to the string containing the file content, or NULL on failure.
 */
char* read_source_file(const char* filename, size_t* length) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    *length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *source = (char *)malloc(*length + 1);
    if (source == NULL) {
        fclose(file);
        return NULL;
    }

    size_t bytes_read = fread(source, 1, *length, file);
    if (bytes_read != *length) {
        free(source);
        fclose(file);
        return NULL;
    }
    source[*length] = '\0';

    fclose(file);
    return source;
}

/*
 * Improves an existing layout using OpenCL.
 *
 * Parameters:
 *   shuffle: A flag indicating whether to shuffle the layout before starting (1) or not (0).
 */
void cl_improve(int shuffle) {
    /* Work for timing total/real layouts/second */
    layouts_analyzed += ((int) repetitions / threads) * threads;
    layouts_analyzed += 2;
    struct timespec compute_start, compute_end;

    log_print('v', L"Pins: \n");
    print_pins();
    log_print('v', L"\n");

    layout *lt;
    log_print('n', L"1/9: Allocating layout... ");
    alloc_layout(&lt);
    log_print('n', L"Done\n\n");

    log_print('n', L"2/9: Reading layout... ");
    read_layout(lt, 1);
    log_print('n', L"Done\n\n");

    if (shuffle) {
        log_print('n', L"3/9: Shuffling layout... ");
        shuffle_layout(lt);
        strcpy(lt->name, "random shuffle");
        log_print('n', L"Done\n\n");
    } else {
        log_print('n', L"3/9: Skipping shuffle... ");
        log_print('n', L"Done\n\n");
    }

    log_print('n', L"4/9: Analyzing starting point... ");
    single_analyze(lt);
    get_score(lt);
    log_print('n', L"Done\n\n");

    print_layout(lt);
    log_print('n', L"\n");

    /* Set the "ideal" number of work items */
    WORKERS = -1;
    int temp_total = 0;
    for (int i = 0; i < MONO_LENGTH; i++) {
        if (!stats_mono[i].skip) {temp_total++;}
    }
    if (temp_total > WORKERS) {WORKERS = temp_total;}

    temp_total = 0;
    for (int i = 0; i < BI_LENGTH; i++) {
        if (!stats_bi[i].skip) {temp_total++;}
    }
    if (temp_total > WORKERS) {WORKERS = temp_total;}

    temp_total = 0;
    for (int i = 0; i < TRI_LENGTH; i++) {
        if (!stats_tri[i].skip) {temp_total++;}
    }
    if (temp_total > WORKERS) {WORKERS = temp_total;}

    temp_total = 0;
    for (int i = 0; i < QUAD_LENGTH; i++) {
        if (!stats_quad[i].skip) {temp_total++;}
    }
    if (temp_total > WORKERS) {WORKERS = temp_total;}

    temp_total = 0;
    for (int i = 0; i < SKIP_LENGTH; i++) {
        if (!stats_skip[i].skip) {temp_total++;}
    }
    if (temp_total > WORKERS) {WORKERS = temp_total;}

    temp_total = 0;
    for (int i = 0; i < META_LENGTH; i++) {
        if (!stats_meta[i].skip) {temp_total++;}
    }
    if (temp_total > WORKERS) {WORKERS = temp_total;}

    /* OpenCL setup */
    cl_platform_id *platforms;
    cl_uint num_platforms;
    cl_device_id device;
    cl_context context;
    cl_command_queue queue;
    cl_program program;
    cl_kernel kernel;
    cl_int err;

    /* Get all platforms */
    log_print('v', L"5/9: Initializing OpenCL...\n");
    log_print('v', L"     Getting platforms... ");
    err = clGetPlatformIDs(0, NULL, &num_platforms);
    if (err != CL_SUCCESS) {error("OpenCL Error: Failed to get number of platforms.");}

    platforms = (cl_platform_id *)malloc(sizeof(cl_platform_id) * num_platforms);
    err = clGetPlatformIDs(num_platforms, platforms, NULL);
    if (err != CL_SUCCESS) {
        free(platforms);
        error("OpenCL Error: Failed to get platform IDs.");
    }
    log_print('v', L"Done\n");

    /* Attempt to find a GPU device across all platforms */
    log_print('v', L"     Finding a suitable device... ");
    int device_found = 0;
    for (int i = 0; i < num_platforms; i++) {
        err = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_GPU, 1, &device, NULL);
        if (err == CL_SUCCESS) {
            device_found = 1;
            break;
        }
    }

    /* If no GPU is found, try to get any other available device */
    if (!device_found) {
        for (int i = 0; i < num_platforms; i++) {
            err = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_ALL, 1, &device, NULL);
            if (err == CL_SUCCESS) {
                device_found = 1;
                break;
            }
        }
    }

    if (!device_found) {
        free(platforms);
        error("OpenCL Error: No suitable device found.");
    }
    log_print('v', L"Done\n");
    free(platforms);

    /* Create context */
    log_print('v', L"     Creating context... ");
    context = clCreateContext(NULL, 1, &device, NULL, NULL, &err);
    if (err != CL_SUCCESS) {error("OpenCL Error: Failed to create context.");}
    log_print('v', L"Done\n");

    /* Create command queue (using the non-deprecated function) */
    log_print('v', L"     Creating command queue... ");
    /* no special properties */
    cl_queue_properties props[] = {0};
    queue = clCreateCommandQueueWithProperties(context, device, props, &err);
    if (err != CL_SUCCESS) {error("OpenCL Error: Failed to create command queue.");}
    log_print('v', L"Done\n");

    /* Read kernel source */
    log_print('v', L"     Reading kernel source... ");
    size_t source_length;
    char* kernel_source = read_source_file("src/kernel.cl", &source_length);
    if (kernel_source == NULL) {error("Failed to read kernel source file.");}
    log_print('v', L"Done\n");

    /* Create and build program */
    log_print('v', L"     Creating and building program... ");
    program = clCreateProgramWithSource(context, 1, (const char**)&kernel_source, &source_length, &err);
    if (err != CL_SUCCESS) {error("OpenCL Error: Failed to create program from source.");}

    /* Compiler options to pass constants to the kernel using compiler flags */
    /* Ensure this is large enough for all defines */
    char options[512];
    sprintf(options, "-Iinclude -cl-fast-relaxed-math -D MONO_LENGTH=%d -D BI_LENGTH=%d -D TRI_LENGTH=%d -D QUAD_LENGTH=%d -D SKIP_LENGTH=%d -D META_LENGTH=%d -D THREADS=%d -D REPETITIONS=%d -D MAX_SWAPS=%d -D WORKERS=%d",
            MONO_LENGTH, BI_LENGTH, TRI_LENGTH, QUAD_LENGTH, SKIP_LENGTH, META_LENGTH, threads, repetitions, MAX_SWAPS, WORKERS);

    err = clBuildProgram(program, 1, &device, options, NULL, NULL);
    if (err != CL_SUCCESS) {
        size_t log_size;
        clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
        char *log = (char *)malloc(log_size);
        clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, log_size, log, NULL);
        log_print('q', L"OpenCL build log:\n%s\n", log);
        free(log);
        error("OpenCL Error: Failed to build program.");
    }
    log_print('v', L"Done\n");

    /* Create kernel */
    log_print('v', L"     Creating kernel... ");
    kernel = clCreateKernel(program, "improve_kernel", &err);
    if (err != CL_SUCCESS) {error("OpenCL Error: Failed to create kernel.");}
    log_print('v', L"Done\n");

    /* Create an array of layouts on the host */
    /* NOT FULL LAYOUTS ONLY MATRIX, NAME, AND OVERALL SCORE */
    log_print('v', L"     Creating array of layouts on host... ");
    layout *layouts = (layout *)malloc(sizeof(layout) * threads);
    if (layouts == NULL) {error("Failed to allocate memory for layouts.");}
    for (int i = 0; i < threads; i++)
    {
        skeleton_copy(&layouts[i], lt);
    }
    log_print('v', L"Done\n");

    int *reps_data = (int *)malloc(sizeof(int) * threads);
    for (int i = 0; i < threads; i++) {reps_data[i] = 0;}

    /* Allocate and copy data to device buffers */
    log_print('v', L"     Allocating and copying data to device buffers...");

    cl_mem buffer_mono_index_array = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(int) * DIM1 * 2, mono_index_array, &err);
    if (err != CL_SUCCESS) {error("OpenCL Error: Failed to create buffer for mono_index_array.");}
    cl_mem buffer_bi_index_array = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(int) * DIM2 * 4, bi_index_array, &err);
    if (err != CL_SUCCESS) {error("OpenCL Error: Failed to create buffer for bi_index_array.");}
    cl_mem buffer_tri_index_array = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(int) * DIM3 * 6, tri_index_array, &err);
    if (err != CL_SUCCESS) {error("OpenCL Error: Failed to create buffer for tri_index_array.");}
    cl_mem buffer_quad_index_array = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(int) * DIM4 * 8, quad_index_array, &err);
    if (err != CL_SUCCESS) {error("OpenCL Error: Failed to create buffer for quad_index_array.");}
    cl_mem buffer_linear_mono = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(float) * LANG_LENGTH, linear_mono, &err);
    if (err != CL_SUCCESS) {error("OpenCL Error: Failed to create buffer for linear_mono.");}
    cl_mem buffer_linear_bi = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(float) * LANG_LENGTH * LANG_LENGTH, linear_bi, &err);
    if (err != CL_SUCCESS) {error("OpenCL Error: Failed to create buffer for linear_bi.");}
    cl_mem buffer_linear_tri = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(float) * LANG_LENGTH * LANG_LENGTH * LANG_LENGTH, linear_tri, &err);
    if (err != CL_SUCCESS) {error("OpenCL Error: Failed to create buffer for linear_tri.");}
    cl_mem buffer_linear_quad = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(float) * LANG_LENGTH * LANG_LENGTH * LANG_LENGTH * LANG_LENGTH, linear_quad, &err);
    if (err != CL_SUCCESS) {error("OpenCL Error: Failed to create buffer for linear_quad.");}
    cl_mem buffer_linear_skip = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(float) * 10 * LANG_LENGTH * LANG_LENGTH, linear_skip, &err);
    if (err != CL_SUCCESS) {error("OpenCL Error: Failed to create buffer for linear_skip.");}
    cl_mem buffer_stats_mono = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(mono_stat) * MONO_LENGTH, stats_mono, &err);
    if (err != CL_SUCCESS) {error("OpenCL Error: Failed to create buffer for stats_mono.");}
    cl_mem buffer_stats_bi = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(bi_stat) * BI_LENGTH, stats_bi, &err);
    if (err != CL_SUCCESS) {error("OpenCL Error: Failed to create buffer for stats_bi.");}
    cl_mem buffer_stats_tri = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(tri_stat) * TRI_LENGTH, stats_tri, &err);
    if (err != CL_SUCCESS) {error("OpenCL Error: Failed to create buffer for stats_tri.");}
    cl_mem buffer_stats_quad = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(quad_stat) * QUAD_LENGTH, stats_quad, &err);
    if (err != CL_SUCCESS) {error("OpenCL Error: Failed to create buffer for stats_quad.");}
    cl_mem buffer_stats_skip = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(skip_stat) * SKIP_LENGTH, stats_skip, &err);
    if (err != CL_SUCCESS) {error("OpenCL Error: Failed to create buffer for stats_skip.");}
    cl_mem buffer_stats_meta = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(meta_stat) * META_LENGTH, stats_meta, &err);
    if (err != CL_SUCCESS) {error("OpenCL Error: Failed to create buffer for stats_meta.");}
    cl_mem buffer_layouts = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(layout) * threads, NULL, &err);
    if (err != CL_SUCCESS) {error("OpenCL Error: Failed to create buffer for layouts.");}
    err = clEnqueueWriteBuffer(queue, buffer_layouts, CL_TRUE, 0, sizeof(layout) * threads, layouts, 0, NULL, NULL);
    if (err != CL_SUCCESS) {error("OpenCL Error: Failed to write layouts to buffer.");}
    cl_mem buffer_pins = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(int) * ROW * COL, pins, &err);
    if (err != CL_SUCCESS) {error("OpenCL Error: Failed to create buffer for pins.");}
    cl_mem buffer_reps = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(int) * threads, NULL, &err);
    if (err != CL_SUCCESS) { error("OpenCL Error: Failed to create buffer for reps."); }
    log_print('v', L"     Done\n");

    /* Generate a seed on the host */
    unsigned int seed = (unsigned int)time(NULL);

    /* Set kernel arguments */
    log_print('v', L"     Setting kernel arguments... ");

    err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &buffer_mono_index_array);
    if (err != CL_SUCCESS) {error("OpenCL Error: Failed to set kernel argument 0.");}
    err = clSetKernelArg(kernel, 1, sizeof(cl_mem), &buffer_bi_index_array);
    if (err != CL_SUCCESS) {error("OpenCL Error: Failed to set kernel argument 1.");}
    err = clSetKernelArg(kernel, 2, sizeof(cl_mem), &buffer_tri_index_array);
    if (err != CL_SUCCESS) {error("OpenCL Error: Failed to set kernel argument 2.");}
    err = clSetKernelArg(kernel, 3, sizeof(cl_mem), &buffer_quad_index_array);
    if (err != CL_SUCCESS) {error("OpenCL Error: Failed to set kernel argument 3.");}
    err = clSetKernelArg(kernel, 4, sizeof(cl_mem), &buffer_linear_mono);
    if (err != CL_SUCCESS) {error("OpenCL Error: Failed to set kernel argument 4.");}
    err = clSetKernelArg(kernel, 5, sizeof(cl_mem), &buffer_linear_bi);
    if (err != CL_SUCCESS) {error("OpenCL Error: Failed to set kernel argument 5.");}
    err = clSetKernelArg(kernel, 6, sizeof(cl_mem), &buffer_linear_tri);
    if (err != CL_SUCCESS) {error("OpenCL Error: Failed to set kernel argument 6.");}
    err = clSetKernelArg(kernel, 7, sizeof(cl_mem), &buffer_linear_quad);
    if (err != CL_SUCCESS) {error("OpenCL Error: Failed to set kernel argument 7.");}
    err = clSetKernelArg(kernel, 8, sizeof(cl_mem), &buffer_linear_skip);
    if (err != CL_SUCCESS) {error("OpenCL Error: Failed to set kernel argument 8.");}
    err = clSetKernelArg(kernel, 9, sizeof(cl_mem), &buffer_stats_mono);
    if (err != CL_SUCCESS) {error("OpenCL Error: Failed to set kernel argument 9.");}
    err = clSetKernelArg(kernel, 10, sizeof(cl_mem), &buffer_stats_bi);
    if (err != CL_SUCCESS) {error("OpenCL Error: Failed to set kernel argument 10.");}
    err = clSetKernelArg(kernel, 11, sizeof(cl_mem), &buffer_stats_tri);
    if (err != CL_SUCCESS) {error("OpenCL Error: Failed to set kernel argument 11.");}
    err = clSetKernelArg(kernel, 12, sizeof(cl_mem), &buffer_stats_quad);
    if (err != CL_SUCCESS) {error("OpenCL Error: Failed to set kernel argument 12.");}
    err = clSetKernelArg(kernel, 13, sizeof(cl_mem), &buffer_stats_skip);
    if (err != CL_SUCCESS) {error("OpenCL Error: Failed to set kernel argument 13.");}
    err = clSetKernelArg(kernel, 14, sizeof(cl_mem), &buffer_stats_meta);
    if (err != CL_SUCCESS) {error("OpenCL Error: Failed to set kernel argument 14.");}
    err = clSetKernelArg(kernel, 15, sizeof(cl_mem), &buffer_layouts);
    if (err != CL_SUCCESS) {error("OpenCL Error: Failed to set kernel argument 15.");}
    err = clSetKernelArg(kernel, 16, sizeof(cl_mem), &buffer_pins);
    if (err != CL_SUCCESS) {error("OpenCL Error: Failed to set kernel argument 16.");}
    err = clSetKernelArg(kernel, 17, sizeof(int), &seed);
    if (err != CL_SUCCESS) {error("OpenCL Error: Failed to set kernel argument 17.");}
    err = clSetKernelArg(kernel, 18, sizeof(cl_mem), &buffer_reps);
    if (err != CL_SUCCESS) { error("OpenCL Error: Failed to set kernel argument 18."); }
    log_print('v', L"Done\n");

    log_print('v', L"     Done\n\n");

    /* timing opencl execution time */
    struct timespec start, end;
    double elapsed;

    clock_gettime(CLOCK_MONOTONIC, &start);
    clock_gettime(CLOCK_MONOTONIC, &compute_start);

    /* Enqueue kernel */
    log_print('v', L"6/9: Enqueueing kernel... ");
    /* threads layouts in parallel */
    size_t global_size = threads * WORKERS;
    /* WORKERS threads per layout */
    size_t local_size = WORKERS;
    err = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &global_size, &local_size, 0, NULL, NULL);
    if (err != CL_SUCCESS) {error("OpenCL Error: Failed to enqueue kernel.");}
    log_print('v', L"Done\n");

    /* Wait for kernel to finish */
    log_print('v', L"     Waiting for kernel to finish... ");
    clFinish(queue);
    log_print('v', L"Done\n");
    log_print('v', L"     Done\n\n");

    /* Read back the array of layouts from the buffer */
    err = clEnqueueReadBuffer(queue, buffer_layouts, CL_TRUE, 0, sizeof(layout) * threads, layouts, 0, NULL, NULL);
    if (err != CL_SUCCESS) {error("OpenCL Error: Failed to read buffer for layouts.");}

    /* Read back the iteration counts from the device */
    err = clEnqueueReadBuffer(queue, buffer_reps, CL_TRUE, 0, sizeof(int) * threads, reps_data, 0, NULL, NULL);
    if (err != CL_SUCCESS) { error("OpenCL Error: Failed to read buffer for reps."); }

    /* calculate opencl execution time */
    clock_gettime(CLOCK_MONOTONIC, &end);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    /* Find the best layout among all threads */
    log_print('n', L"7/9: Selecting best layout... ");
    layout *best_layout;
    alloc_layout(&best_layout);
    skeleton_copy(best_layout, lt);
    if (output_mode == 'v') {
        log_print('v', L"\n\nPRINTING LAYOUTS\n\n");
        /* make output mode quiet so we don't spam */
        char temp = output_mode;
        output_mode = 'q';
        for (int i = 0; i < threads; i++) {
            log_print('q', L"Layout %d: CL Results - REPS COMPLETED: %d\n", i, reps_data[i]);
            strcat(layouts[i].name, " \"improved\"");
            print_layout(&layouts[i]);
            if (layouts[i].score > best_layout->score) {
                skeleton_copy(best_layout, &layouts[i]);
            }
            log_print('q', L"\n");
        }
        /* reset output mode */
        output_mode = temp;
    } else {
        for (int i = 0; i < threads; i++) {
            strcat(layouts[i].name, " \"improved\"");
            if (layouts[i].score > best_layout->score) {
                skeleton_copy(best_layout, &layouts[i]);
            }
        }
    }
    log_print('n',L"Done\n\n");

    /* Cleanup */
    log_print('v', L"8/9: Cleaning up OpenCL...");
    clReleaseMemObject(buffer_mono_index_array);
    clReleaseMemObject(buffer_bi_index_array);
    clReleaseMemObject(buffer_tri_index_array);
    clReleaseMemObject(buffer_quad_index_array);
    clReleaseMemObject(buffer_linear_mono);
    clReleaseMemObject(buffer_linear_bi);
    clReleaseMemObject(buffer_linear_tri);
    clReleaseMemObject(buffer_linear_quad);
    clReleaseMemObject(buffer_linear_skip);
    clReleaseMemObject(buffer_stats_mono);
    clReleaseMemObject(buffer_stats_bi);
    clReleaseMemObject(buffer_stats_tri);
    clReleaseMemObject(buffer_stats_quad);
    clReleaseMemObject(buffer_stats_skip);
    clReleaseMemObject(buffer_stats_meta);
    clReleaseMemObject(buffer_layouts);
    clReleaseMemObject(buffer_pins);
    clReleaseMemObject(buffer_reps);
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);
    log_print('v', L"Done\n\n");

    log_print('v', L"cl score : %f\n", best_layout->score);
    log_print('v', L"time per layout : %.9lf seconds\n", elapsed / repetitions);
    log_print('v', L"layouts / sec   : %.9lf\n\n", repetitions / elapsed);

    /* print final layout */
    log_print('v', L"9/9: Printing layout...\n\n");
    /* perform a single layout analysis */
    log_print('n',L"8/9: Analyzing best layout... ");
    single_analyze(best_layout); /* analyze.c */
    /* calculates the overall score */
    get_score(best_layout); /* util.c */
    log_print('n',L"Done\n\n");
    print_layout(best_layout);
    log_print('v', L"Done\n\n");

    free(layouts);
    free_layout(lt);
    free(reps_data);
    clock_gettime(CLOCK_MONOTONIC, &compute_end);
    elapsed_compute_time += (compute_end.tv_sec - compute_start.tv_sec) + (compute_end.tv_nsec - compute_start.tv_nsec) / 1e9;
}

/*
 * Performs a benchmark to determine the optimal number of threads for layout
 * generation on pure cpu. It runs the generation process with different numbers
 * of threads and measures performance.
 */
void gen_benchmark()
{
    repetitions = 10000;
    int num_cpus = sysconf(_SC_NPROCESSORS_ONLN);
    int power_of_2 = 1;
    int count = 1;

    log_print('n',L"1/2: Planning runs... ");
    /* find the highest power of 2 that does not exceed the number of CPU threads */
    while (power_of_2 <= num_cpus) {
        power_of_2 *= 2;
        count++;
    }

    /* for cpu threads */
    int total = count + 3;

    /* allocate arrays for test cases */
    int *thread_array = (int *)calloc(total, sizeof(int));
    double *results = (double *)calloc(total, sizeof(double));

    /* fill in thread counts based on powers of 2 and cores */
    thread_array[0] = 1;
    for (int i = 1; i < count; i++) {thread_array[i] = thread_array[i-1] * 2;}
    thread_array[count] = num_cpus / 2;
    thread_array[count + 1] = num_cpus;
    thread_array[count + 2] = num_cpus * 2;

    /* print the tests to be done */
    for (int i = 0; i < total; i++) {log_print('v',L"%d ", thread_array[i]);}
    log_print('v',L"\n");
    log_print('n',L"Done\n\n");

    log_print('n',L"2/2: Benchmarking... \n");
    /* temporarily set output mode to quiet */
    char temp = output_mode;
    output_mode = 'q';

    /* run benchmark for each thread count */
    for (int i = 0; i < total; i++)
    {

        log_print('q',L"BENCHMARK RUN %d/%d\n", i+1, total);
        threads = thread_array[i];

        struct timespec start, end;
        clock_gettime(CLOCK_MONOTONIC, &start);

        generate();

        clock_gettime(CLOCK_MONOTONIC, &end);
        double elapsed = (end.tv_sec - start.tv_sec) +
                         (end.tv_nsec - start.tv_nsec) / 1e9;
        double time_per_repetition = elapsed / repetitions;
        results[i] = 1.0 / time_per_repetition;
        log_print('q',L"Done\n\n");
    }

    /* reset output mode */
    output_mode = temp;

    /* print benchmark results */
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
    log_print('q',L"Choose the lowest number of threads with acceptable Layouts/Second for best results.\n\n");

    /* free allocated memory */
    free(thread_array);
    free(results);
}

/*
 * Performs a benchmark to determine the optimal number of threads for layout
 * generation on opencl. It runs the generation process with different numbers
 * of threads and measures performance.
 */
void cl_gen_benchmark()
{
    repetitions = 10000;

    /* OpenCL setup (same as in cl_improve) */
    cl_platform_id *platforms;
    cl_uint num_platforms;
    cl_device_id device;
    cl_int err;

    log_print('n',L"1/3: Setting up OpenCL... \n");
    /* Get all platforms */
    log_print('v', L"     Getting platforms... ");
    err = clGetPlatformIDs(0, NULL, &num_platforms);
    if (err != CL_SUCCESS) {error("OpenCL Error: Failed to get number of platforms.");}

    platforms = (cl_platform_id *)malloc(sizeof(cl_platform_id) * num_platforms);
    err = clGetPlatformIDs(num_platforms, platforms, NULL);
    if (err != CL_SUCCESS) {
        free(platforms);
        error("OpenCL Error: Failed to get platform IDs.");
    }
    log_print('v', L"Done\n");

    /* Attempt to find a GPU device across all platforms */
    log_print('v', L"     Finding a suitable device... ");
    int device_found = 0;
    for (int i = 0; i < num_platforms; i++) {
        err = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_GPU, 1, &device, NULL);
        if (err == CL_SUCCESS) {
            device_found = 1;
            break;
        }
    }

    /* If no GPU is found, try to get any other available device */
    if (!device_found) {
        for (int i = 0; i < num_platforms; i++) {
            err = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_ALL, 1, &device, NULL);
            if (err == CL_SUCCESS) {
                device_found = 1;
                break;
            }
        }
    }

    if (!device_found) {
        free(platforms);
        error("OpenCL Error: No suitable device found.");
    }
    log_print('v', L"Done\n");
    free(platforms); // Free platforms after we found a device

    /* Get device information */
    cl_uint num_compute_units;
    char device_name[512];

    err = clGetDeviceInfo(device, CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(num_compute_units), &num_compute_units, NULL);
    if (err != CL_SUCCESS) {error("OpenCL Error: Failed to get number of compute units.");}

    err = clGetDeviceInfo(device, CL_DEVICE_NAME, sizeof(device_name), &device_name, NULL);
    if (err != CL_SUCCESS) {error("OpenCL Error: Failed to get device name.");}

    log_print('v', L"     OpenCL Device Info:\n");
    log_print('v', L"       Device Name: %s\n", device_name);
    log_print('v', L"       Compute Units: %u\n", num_compute_units);

    log_print('n',L"Done\n\n");

    log_print('n',L"2/3: Planning runs... ");
    int starting_power = 16;
    int power_of_2 = starting_power;
    int count = 1;

    /* find the highest power of 2 that does not exceed the number of CPU threads */
    while (power_of_2 <= num_compute_units * starting_power) {
        power_of_2 *= 2;
        count++;
    }

    /* for cpu threads */
    int total = count + 6;

    /* allocate arrays for test cases */
    int *thread_array = (int *)calloc(total, sizeof(int));
    double *results = (double *)calloc(total, sizeof(double));

    /* fill in thread counts based on powers of 2 and cores */
    thread_array[0] = starting_power;
    for (int i = 1; i < count; i++) {thread_array[i] = thread_array[i-1] * 2;}
    thread_array[count] = num_compute_units;
    thread_array[count + 1] = num_compute_units * 2;
    thread_array[count + 2] = num_compute_units * 4;
    thread_array[count + 3] = num_compute_units * 8;
    thread_array[count + 4] = num_compute_units * 16;
    thread_array[count + 5] = num_compute_units * 32;

    /* print the tests to be done */
    for (int i = 0; i < total; i++) {log_print('v',L"%d ", thread_array[i]);}
    log_print('v',L"\n");
    log_print('n',L"Done\n\n");


    log_print('n',L"3/3: Benchmarking... \n");
    /* temporarily set output mode to quiet */
    char temp = output_mode;
    output_mode = 'q';

    /* run benchmark for each thread count */
    for (int i = 0; i < total; i++)
    {

        log_print('q',L"BENCHMARK RUN %d/%d\n", i+1, total);
        threads = thread_array[i];

        struct timespec start, end;
        clock_gettime(CLOCK_MONOTONIC, &start);

        cl_generate();

        clock_gettime(CLOCK_MONOTONIC, &end);
        double elapsed = (end.tv_sec - start.tv_sec) +
                         (end.tv_nsec - start.tv_nsec) / 1e9;
        double time_per_repetition = elapsed / repetitions;
        results[i] = 1.0 / time_per_repetition;
        log_print('q',L"Done\n\n");
    }

    /* reset output mode */
    output_mode = temp;

    /* print benchmark results */
    log_print('q',L"\nBENCHMARK RESULTS:\n\n");
    log_print('q',L"Threads - Layouts/Second:\n\n");
    log_print('q',L"Powers of 2:\n");
    for (int i = 0; i < count; i++)
    {
        log_print('q',L"%7d - %lf\n", thread_array[i], results[i]);
    }
    log_print('q',L"\n");
    log_print('q',L"Based on CUs/SMs/Cores:\n");
    for (int i = count; i < total; i++)
    {
        log_print('q',L"%7d - %lf\n", thread_array[i], results[i]);
    }
    log_print('q',L"\n");
    log_print('q',L"Choose the lowest number of threads with acceptable Layouts/Second for best results.\n\n");

    /* free allocated memory */
    free(thread_array);
    free(results);
}

/*
 * Prints a help message providing usage instructions for the program's command
 * line arguments.
 */
void print_help() {
    /* Work for timing total/real layouts/second */
    layouts_analyzed = 0;
    struct timespec compute_start, compute_end;
    clock_gettime(CLOCK_MONOTONIC, &compute_start);

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
    log_print('q',L"    f;info;information   : Prints more in-depth information about this program.\n");
    // 80           @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    log_print('q',L"  -o <mode>     : decides how verbose the output is.\n");
    log_print('q',L"    q;quiet;SHUTUP       : The most concise; prints only the essential stats.\n");
    log_print('q',L"    n;norm;normal        : Prints most stats; ignores the most pedantic.\n");
    log_print('q',L"    v;loud;verbose       : The most verbose; prints all stats.\n");
    // 80           @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    log_print('q',L"  -b <mode>     : decides which backend to use.\n");
    log_print('q',L"    c;cpu                : Uses a pure C cpu backend, best for CPU.\n");
    log_print('q',L"    o;ocl;opencl         : Uses an opencl backend, best for GPU, worse for CPU.\n");
    // 80           @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    log_print('q',L"Config:\n");
    log_print('q',L"  All of these options can be set in config.conf but command line arguments will\n");
    log_print('q',L"  be prioritized. config.conf also sets the pins for the improve mode; all\n");
    log_print('q',L"  positions that are not \'.\' will be pinned.\n");

    clock_gettime(CLOCK_MONOTONIC, &compute_end);
    elapsed_compute_time += (compute_end.tv_sec - compute_start.tv_sec) + (compute_end.tv_nsec - compute_start.tv_nsec) / 1e9;
}

/* Prints an introductory message and information about the GULAG. */
void print_info() {
    /* Work for timing total/real layouts/second */
    layouts_analyzed = 0;
    struct timespec compute_start, compute_end;
    clock_gettime(CLOCK_MONOTONIC, &compute_start);

    log_print('q', L"Welcome to GULAG v1.0\n\n");
    log_print('q', L"GULAG is a tool designed to analyze and optimize keyboard layouts based on statistical analysis of language corpora.\n\n");
    log_print('q', L"It evaluates layouts using various ngram metrics:\n");
    log_print('q', L"  - Monogram, Bigram, Trigram, Quadgram, and Skipgram statistics\n");
    log_print('q', L"  - Same Row, Adjacent Finger, and Directional metrics\n\n");
    log_print('q', L"GULAG can be used to:\n");
    log_print('q', L"  - Analyze existing layouts\n");
    log_print('q', L"  - Compare two layouts\n");
    log_print('q', L"  - Rank multiple layouts\n");
    log_print('q', L"  - Generate new layouts using a simulated annealing algorithm\n");
    log_print('q', L"  - Improve existing layouts through optimization\n");
    log_print('q', L"  - Benchmark different settings for optimal performance\n\n");
    log_print('q', L"Key features:\n");
    log_print('q', L"  - Multithreading support for faster processing\n");
    log_print('q', L"  - OpenCL support for GPU acceleration\n");
    log_print('q', L"  - Customizable weights for different statistics\n");
    log_print('q', L"  - Configurable options via command line and config.conf\n\n");
    log_print('q', L"To get started, use the \"-m h\" flag for help on command line arguments or run the default benchmark with \"-m b\".\n\n");

    clock_gettime(CLOCK_MONOTONIC, &compute_end);
    elapsed_compute_time += (compute_end.tv_sec - compute_start.tv_sec) + (compute_end.tv_nsec - compute_start.tv_nsec) / 1e9;
}
