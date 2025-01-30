#ifndef MODE_H
#define MODE_H

/*
 * Performs analysis on a single layout. This involves allocating memory for the
 * layout, reading layout data from a file, analyzing the layout, calculating
 * its score, printing the output, and freeing the allocated memory.
 */
void analysis();

/*
 * Compares two layouts and outputs the difference. This function allocates
 * memory for three layouts, reads data for two layouts from files, performs
 * analysis on both, calculates their scores, computes the difference, prints
 * the difference layout, and frees the allocated memory.
 */
void compare();

/*
 * Ranks all layouts in the layouts directory based on their scores. It reads
 * each layout file, analyzes it, calculates its score, and maintains a ranked
 * list of layouts. Finally, it prints the ranking.
 */
void rank();

/*
 * Initiates the layout generation process without a specific starting layout.
 * Calls improve with shuffle set to 1, effectively starting from a random
 * layout, with no pins. Will still use set of keys from selected layout.
 */
void generate();

/*
 * Improves an existing layout using multiple threads.
 * Each thread runs a simulated annealing process to find a better layout.
 *
 * Parameters:
 *   shuffle: A flag indicating whether to shuffle the layout before starting.
 */
void improve(int shuffle);

/* Generates a new layout using OpenCL. */
void cl_generate();

/*
 * Improves an existing layout using OpenCL.
 *
 * Parameters:
 *   shuffle: A flag indicating whether to shuffle the layout before starting (1) or not (0).
 */
void cl_improve(int shuffle);

/*
 * Performs a benchmark to determine the optimal number of threads for layout
 * generation on pure cpu. It runs the generation process with different numbers
 * of threads and measures performance.
 */
void gen_benchmark();

/*
 * Performs a benchmark to determine the optimal number of threads for layout
 * generation on opencl. It runs the generation process with different numbers
 * of threads and measures performance.
 */
void cl_gen_benchmark();

/*
 * Prints a help message providing usage instructions for the program's command
 * line arguments.
 */
void print_help();

/* Prints an introductory message and information about the GULAG. */
void print_info();

#endif
