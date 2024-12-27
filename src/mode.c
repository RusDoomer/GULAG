/*
 * mode.c - Modes for the GULAG.
 *
 * Author: Rus Doomer
 *
 * Description: This file implements the various modes of operation for the GULAG:
 *              analysis, comparison, ranking, generation, and improvement.
 *              It utilizes multithreading to enhance performance, specifically
 *              in the layout improvement process.
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
 * Performs analysis on a single layout.
 * This involves allocating memory for the layout, reading layout data from a file,
 * analyzing the layout, calculating its score, printing the output, and freeing the allocated memory.
 *
 * Returns: void.
 */
void analysis() {
    layout *lt;

    /* util.c - allocates memory for a layout structure */
    log_print('n',L"1/6: Allocating layout... ");
    alloc_layout(&lt);
    log_print('n',L"Done\n\n");

    /* io.c - reads a keyboard layout from a file and initializes a layout structure */
    log_print('n',L"2/6: Reading layout... ");
    read_layout(lt, 1);
    log_print('n',L"Done\n\n");

    /* analyze.c - performs a single layout analysis */
    log_print('n',L"3/6: Analyzing layout... ");
    single_analyze(lt);
    log_print('n',L"Done\n\n");

    /* util.c - calculates and assigns the overall score to a layout based on its statistics */
    log_print('n',L"4/6: Calculating Score... ");
    get_score(lt);
    log_print('n',L"Done\n\n");

    /* io.c - prints the contents of a layout structure to the standard output */
    log_print('n',L"5/6: Printing Output...\n\n");
    print_layout(lt);
    log_print('n',L"Done\n\n");

    /* util.c - frees the memory occupied by a layout data structure */
    log_print('n',L"6/6: Freeing layout... ");
    free_layout(lt);
    log_print('n',L"Done\n\n");
    return;
}

/*
 * Compares two layouts and outputs the difference.
 * This function allocates memory for three layouts, reads data for two layouts from files,
 * performs analysis on both, calculates their scores, computes the difference,
 * prints the difference layout, and frees the allocated memory.
 *
 * Returns: void.
 */
void compare() {
    layout *lt1, *lt2, *lt_diff;

    /* util.c - allocates memory for the layouts */
    log_print('n',L"1/7: Allocating layouts... ");
    alloc_layout(&lt1);
    log_print('n',L"%s... ", layout_name);
    alloc_layout(&lt2);
    log_print('n',L"%s... ", layout2_name);
    alloc_layout(&lt_diff);
    log_print('n',L"diff... ");
    log_print('n',L"Done\n\n");

    /* io.c - reading keyboard layouts and initializing */
    log_print('n',L"2/7: Reading layout... ");
    read_layout(lt1, 1);
    log_print('n',L"%s... ", layout_name);
    read_layout(lt2, 2);
    log_print('n',L"%s... ", layout2_name);
    log_print('n',L"Done\n\n");

    /* analyze.c - perform layout analyses */
    log_print('n',L"3/7: Analyzing layout... ");
    single_analyze(lt1);
    log_print('n',L"%s... ", layout_name);
    single_analyze(lt2);
    log_print('n',L"%s... ", layout2_name);
    log_print('n',L"Done\n\n");


    /* util.c - calculate the overall scores */
    log_print('n',L"4/7: Calculating Score... ");
    get_score(lt1);
    log_print('n',L"%s... ", layout_name);
    get_score(lt2);
    log_print('n',L"%s... ", layout2_name);
    log_print('n',L"Done\n\n");

    /* util.c - calculate the difference between the two layouts */
    log_print('n',L"5/7: Calculating Difference... ");
    get_layout_diff(lt1, lt2, lt_diff);
    log_print('n',L"Done\n\n");

    /* io.c - print the diff layout */
    log_print('n',L"6/7: Printing Output...\n\n");
    print_layout(lt_diff);
    log_print('n',L"Done\n\n");

    /* util.c - free the memory */
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

/*
 * Ranks all layouts in the layouts directory based on their scores.
 * It reads each layout file, analyzes it, calculates its score,
 * and maintains a ranked list of layouts. Finally, it prints the ranking.
 *
 * Returns: void.
 */
void rank() {

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

            /* util.c - allocate memory for a layout */
            log_print('n',L"Allocating... ");
            alloc_layout(&lt);

            /* io.c - read the keyboard layout */
            log_print('n',L"Reading... ");
            read_layout(lt, 1);

            /* analyze.c - perform a single layout analysis */
            log_print('n',L"Analyzing... ");
            single_analyze(lt);

            /* util.c - calculate the overall score */
            log_print('n',L"Get Score... ");
            get_score(lt);

            /*
             * util.c - create a new node for the layout ranking list
             *          and insert it in the correct position
             */
            log_print('n',L"Ranking...");
            create_node(lt);

            /* util.c - frees the memory occupied by a layout data structure */
            log_print('n',L"Freeing... ");
            free_layout(lt);
            log_print('n',L"Done\n\n");
        }
    }

    /* io.c - print the ranked list of layouts */
    print_ranking();

    /* Reset layout_name to a safe state */
    layout_name = (char *)malloc(1);
    closedir(dir);
    free(path);

    /* util.c - free all nodes in the layout ranking list */
    free_list();
}

/*
 * Structure to hold data for each thread in the layout improvement process.
 */
typedef struct thread_data {
    layout *lt;
    layout **best_lt;
    int iterations;
    int thread_id;
} thread_data;

/*
 * Function executed by each thread to improve a layout.
 * It performs simulated annealing with adaptive cooling and reheating
 * to find a layout with a better score.
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
    /* util.c - allocate memory for layouts */
    alloc_layout(&max_lt);
    alloc_layout(&working_lt);

    /* util.c - copy initial layout to working and max */
    copy(working_lt, lt);

    /* Set name so we can see if we improved */
    strcat(working_lt->name, " improved");

    /* analyze.c - analyze and score the initial layout */
    single_analyze(working_lt);
    /* util.c - calculate the overall score */
    get_score(working_lt);
    /* util.c - copies the layout */
    copy(max_lt, working_lt);

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

        /* analyze.c - analyze the new layout */
        single_analyze(working_lt);
        /* util.c - calculates the new score */
        get_score(working_lt);

        /* Exponentiate the score difference for acceptance probability (using sigmoid) */
        float delta_score = working_lt->score - max_lt->score;
        if (delta_score > 0 || (1.0 / (1.0 + exp(-10 * delta_score / T))) > random_float()) {
            /* util.c - copy the new layout if it passes */
            copy(max_lt, working_lt);
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
    /* util.c - allocates memory for best layout */
    alloc_layout(&best_layout);
    /* util.c - copy max_lt to best layout */
    copy(best_layout, max_lt);
    *(data->best_lt) = best_layout;


    /* util.c - free layouts */
    free_layout(max_lt);
    free_layout(working_lt);

    pthread_exit(NULL);
}

/*
 * Initiates the layout generation process without a specific starting layout.
 * Calls improve with shuffle set to 1, effectively starting from a random layout,
 * with no pins. Will still use set of keys from selected layout.
 *
 * Returns: void.
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
 *   shuffle: A flag indicating whether to shuffle the layout before starting (1) or not (0).
 *
 * Returns: void.
 */
void improve(int shuffle) {
    layout *lt;

    /* io.c - prints the current pins */
    log_print('v',L"Pins: \n");
    print_pins();
    log_print('v',L"\n");

    /* util.c - allocate memory for layout */
    log_print('n',L"1/9: Allocating layout... ");
    alloc_layout(&lt);
    log_print('n',L"Done\n\n");

    /* io.c - read the starting keyboard layout */
    log_print('n',L"2/9: Reading layout... ");
    read_layout(lt, 1);
    log_print('n',L"Done\n\n");

    if (shuffle) {
        /* util.c - shuffles the matrix */
        log_print('n',L"3/9: Shuffling layout... ");
        shuffle_layout(lt);
        strcpy(lt->name, "random shuffle");
        log_print('n',L"Done\n\n");
    } else {
        log_print('n',L"3/8: Skipping shuffle... ");
        log_print('n',L"Done\n\n");
    }

    /* analyze.c - perform a single layout analysis */
    log_print('n',L"4/9: Analyzing starting point... ");
    single_analyze(lt);
    /* util.c - calculate the overall score */
    get_score(lt);
    log_print('n',L"Done\n\n");

    /* io.c - prints the starting layout */
    print_layout(lt);
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

    /* analyze.c - perform a single layout analysis */
    log_print('n',L"8/9: Analyzing best layout... ");
    single_analyze(best_layout);
    /* util.c - calculates the overall score */
    get_score(best_layout);
    log_print('n',L"Done\n\n");

    /* Compare with the original layout and print the better one */
    log_print('n',L"9/9: Printing layout...\n\n");
    if (best_layout->score > lt->score) {
        /* io.c - prints the best layout */
        print_layout(best_layout);
    } else {
        /* io.c - prints the starting layout */
        print_layout(lt);
    }
    log_print('n',L"Done\n\n");

    /* util.c - free all allocated layouts and thread data */
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

    fread(source, 1, *length, file);
    source[*length] = '\0';

    fclose(file);
    return source;
}

void cl_improve(int shuffle) {

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
    lt->score = -115684;
    print_layout(lt);
    log_print('n', L"\n");

    // OpenCL setup
    cl_platform_id *platforms;
    cl_uint num_platforms;
    cl_device_id device;
    cl_context context;
    cl_command_queue queue;
    cl_program program;
    cl_kernel kernel;
    cl_int err;

    // Get all platforms
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

    // Attempt to find a GPU device across all platforms
    log_print('v', L"     Finding a suitable device... ");
    int device_found = 0;
    for (int i = 0; i < num_platforms; i++) {
        err = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_GPU, 1, &device, NULL);
        if (err == CL_SUCCESS) {
            device_found = 1;
            break;
        }
    }

    // If no GPU is found, try to get any other available device
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
    free(platforms); // Free platforms array after use

    // Create context
    log_print('v', L"     Creating context... ");
    context = clCreateContext(NULL, 1, &device, NULL, NULL, &err);
    if (err != CL_SUCCESS) {error("OpenCL Error: Failed to create context.");}
    log_print('v', L"Done\n");

    // Create command queue (using the non-deprecated function)
    log_print('v', L"     Creating command queue... ");
    cl_queue_properties props[] = {0}; // No special properties needed
    queue = clCreateCommandQueueWithProperties(context, device, props, &err);
    if (err != CL_SUCCESS) {error("OpenCL Error: Failed to create command queue.");}
    log_print('v', L"Done\n");

    // Read kernel source
    log_print('v', L"     Reading kernel source... ");
    size_t source_length;
    char* kernel_source = read_source_file("src/kernel.cl", &source_length);
    if (kernel_source == NULL) {error("Failed to read kernel source file.");}
    log_print('v', L"Done\n");

    // Create and build program
    log_print('v', L"     Creating and building program... ");
    program = clCreateProgramWithSource(context, 1, (const char**)&kernel_source, &source_length, &err);
    if (err != CL_SUCCESS) {error("OpenCL Error: Failed to create program from source.");}

    // Find mono_stat indexes for Hand Balance Meta Stat
    int index_left_hand_usage = find_stat_index("Left Hand Usage", 'm', lt);
    int index_right_hand_usage = find_stat_index("Right Hand Usage", 'm', lt);
    if (index_left_hand_usage == -1 || index_right_hand_usage == -1) {error("hand usage stats must have non-zero weight even if you don't care, sorry :)");}
    if (index_left_hand_usage >= MONO_END || index_right_hand_usage >= MONO_END) {error("the index of left hand usage or right hand usage is out of bounds, idk what you did");}

    // Compiler options to pass constants to the kernel using #define
    char options[512]; // Ensure this is large enough for all defines
    sprintf(options, "-D MONO_END=%d -D BI_END=%d -D TRI_END=%d -D QUAD_END=%d -D SKIP_END=%d -D META_END=%d -D THREADS=%d -D REPETITIONS=%d -D MAX_SWAPS=%d -D WORKERS=%d -D LEFT_HAND=%d -D RIGHT_HAND=%d",
            MONO_END, BI_END, TRI_END, QUAD_END, SKIP_END, META_END, threads, repetitions, MAX_SWAPS, WORKERS, index_left_hand_usage, index_right_hand_usage);

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

    // Create kernel
    log_print('v', L"     Creating kernel... ");
    kernel = clCreateKernel(program, "improve_kernel", &err);
    if (err != CL_SUCCESS) {error("OpenCL Error: Failed to create kernel.");}
    log_print('v', L"Done\n");

    // Allocate and copy data to device buffers
    log_print('v', L"     Allocating and copying data to device buffers...\n");
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
    cl_mem buffer_stats_mono = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(mono_stat) * MONO_END, stats_mono, &err);
    if (err != CL_SUCCESS) {error("OpenCL Error: Failed to create buffer for stats_mono.");}
    cl_mem buffer_stats_bi = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(bi_stat) * BI_END, stats_bi, &err);
    if (err != CL_SUCCESS) {error("OpenCL Error: Failed to create buffer for stats_bi.");}
    cl_mem buffer_stats_tri = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(tri_stat) * TRI_END, stats_tri, &err);
    if (err != CL_SUCCESS) {error("OpenCL Error: Failed to create buffer for stats_tri.");}
    cl_mem buffer_stats_quad = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(quad_stat) * QUAD_END, stats_quad, &err);
    if (err != CL_SUCCESS) {error("OpenCL Error: Failed to create buffer for stats_quad.");}
    cl_mem buffer_stats_skip = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(skip_stat) * SKIP_END, stats_skip, &err);
    if (err != CL_SUCCESS) {error("OpenCL Error: Failed to create buffer for stats_skip.");}
    cl_mem buffer_stats_meta = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(meta_stat) * META_END, stats_meta, &err);
    if (err != CL_SUCCESS) {error("OpenCL Error: Failed to create buffer for stats_meta.");}
    cl_mem buffer_layout = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(layout), lt, &err);
    if (err != CL_SUCCESS) {error("OpenCL Error: Failed to create buffer for layout.");}
    cl_mem buffer_pins = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(int) * ROW * COL, pins, &err);
    if (err != CL_SUCCESS) {error("OpenCL Error: Failed to create buffer for pins.");}
    log_print('v', L"     Done\n");

    // Generate a seed on the host
    unsigned int seed = (unsigned int)time(NULL);

    // Set kernel arguments
    log_print('v', L"     Setting kernel arguments... ");
    err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &buffer_linear_mono);
    if (err != CL_SUCCESS) {error("OpenCL Error: Failed to set kernel argument 0.");}
    err = clSetKernelArg(kernel, 1, sizeof(cl_mem), &buffer_linear_bi);
    if (err != CL_SUCCESS) {error("OpenCL Error: Failed to set kernel argument 1.");}
    err = clSetKernelArg(kernel, 2, sizeof(cl_mem), &buffer_linear_tri);
    if (err != CL_SUCCESS) {error("OpenCL Error: Failed to set kernel argument 2.");}
    err = clSetKernelArg(kernel, 3, sizeof(cl_mem), &buffer_linear_quad);
    if (err != CL_SUCCESS) {error("OpenCL Error: Failed to set kernel argument 3.");}
    err = clSetKernelArg(kernel, 4, sizeof(cl_mem), &buffer_linear_skip);
    if (err != CL_SUCCESS) {error("OpenCL Error: Failed to set kernel argument 4.");}
    err = clSetKernelArg(kernel, 5, sizeof(cl_mem), &buffer_stats_mono);
    if (err != CL_SUCCESS) {error("OpenCL Error: Failed to set kernel argument 5.");}
    err = clSetKernelArg(kernel, 6, sizeof(cl_mem), &buffer_stats_bi);
    if (err != CL_SUCCESS) {error("OpenCL Error: Failed to set kernel argument 6.");}
    err = clSetKernelArg(kernel, 7, sizeof(cl_mem), &buffer_stats_tri);
    if (err != CL_SUCCESS) {error("OpenCL Error: Failed to set kernel argument 7.");}
    err = clSetKernelArg(kernel, 8, sizeof(cl_mem), &buffer_stats_quad);
    if (err != CL_SUCCESS) {error("OpenCL Error: Failed to set kernel argument 8.");}
    err = clSetKernelArg(kernel, 9, sizeof(cl_mem), &buffer_stats_skip);
    if (err != CL_SUCCESS) {error("OpenCL Error: Failed to set kernel argument 9.");}
    err = clSetKernelArg(kernel, 10, sizeof(cl_mem), &buffer_stats_meta);
    if (err != CL_SUCCESS) {error("OpenCL Error: Failed to set kernel argument 10.");}
    err = clSetKernelArg(kernel, 11, sizeof(cl_mem), &buffer_layout);
    if (err != CL_SUCCESS) {error("OpenCL Error: Failed to set kernel argument 11.");}
    err = clSetKernelArg(kernel, 12, sizeof(cl_mem), &buffer_pins);
    if (err != CL_SUCCESS) {error("OpenCL Error: Failed to set kernel argument 12.");}
    err = clSetKernelArg(kernel, 13, sizeof(int), &seed);
    if (err != CL_SUCCESS) {error("OpenCL Error: Failed to set kernel argument 13.");}
    log_print('v', L"Done\n");

    struct timespec start, end;
    double elapsed;

    clock_gettime(CLOCK_MONOTONIC, &start);

    // Enqueue kernel
    log_print('v', L"6/9: Enqueueing kernel... ");
    size_t global_size = threads * WORKERS; // Example: same number of threads as your C code
    size_t local_size = WORKERS; // Example: one workgroup
    err = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &global_size, &local_size, 0, NULL, NULL);
    if (err != CL_SUCCESS) {error("OpenCL Error: Failed to enqueue kernel.");}
    log_print('v', L"Done\n");

    // Wait for kernel to finish (for now)
    log_print('v', L"     Waiting for kernel to finish... ");
    clFinish(queue);
    log_print('v', L"Done\n");

    // Read results back
    log_print('v', L"7/9: Reading results back from OpenCL device...\n\n");
    err = clEnqueueReadBuffer(queue, buffer_layout, CL_TRUE, 0, sizeof(layout), lt, 0, NULL, NULL);
    if (err != CL_SUCCESS) {error("OpenCL Error: Failed to read buffer for layout.");}
    log_print('v', L"Done\n");

    clock_gettime(CLOCK_MONOTONIC, &end);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    // Cleanup
    log_print('v', L"8/9: Cleaning up OpenCL...\n");
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
    clReleaseMemObject(buffer_layout);
    clReleaseMemObject(buffer_pins);
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);
    log_print('v', L"Done\n\n");

    log_print('v', L"cl score : %f\n", lt->score);
    log_print('v', L"time per layout : %.9lf seconds\n", elapsed / repetitions);
    log_print('v', L"layouts / sec   : %.9lf\n\n", repetitions / elapsed);

    // ... rest of your improve logic (print best layout, etc.) ...
    log_print('v', L"9/9: Printing layout...\n\n");
    single_analyze(lt);
    get_score(lt);
    print_layout(lt);
    log_print('v', L"Done\n\n");

    free_layout(lt);
}

/*
 * Performs a benchmark to determine the optimal number of threads for layout generation.
 * It runs the generation process with different numbers of threads and measures performance.
 *
 * Returns: void.
 */
void gen_benchmark()
{
    repetitions = 10000;
    int num_cpus = sysconf(_SC_NPROCESSORS_ONLN);
    int power_of_2 = 1;
    int count = 1;

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
    log_print('v',L"\n\n");

    /* temporarily set output mode to quiet */
    char temp = output_mode;
    output_mode = 'q';

    /* run benchmark for each thread count */
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
    log_print('q',L"Choose the lowest number of threads with acceptable Layouts/Second for best results.");
    log_print('q',L"\n\n");

    /* free allocated memory */
    free(thread_array);
    free(results);
}

/*
 * Prints a help message providing usage instructions for the program's command line arguments.
 *
 * Returns: void.
 */
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

/*
 * Prints an introductory message and information about the GULAG.
 *
 * Returns: void.
 */
void print_info() {
    log_print('q',L"Welcome to GULAG v0.1 (AKA CULAG)\n");
    return;
}
