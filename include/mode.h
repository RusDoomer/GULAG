#ifndef MODE_H
#define MODE_H

/*
 * Performs analysis on a single layout.
 * This involves allocating memory for the layout, reading layout data from a file,
 * analyzing the layout, calculating its score, printing the output, and freeing the allocated memory.
 *
 * Returns: void.
 */
void analysis();

/*
 * Compares two layouts and outputs the difference.
 * This function allocates memory for three layouts, reads data for two layouts from files,
 * performs analysis on both, calculates their scores, computes the difference,
 * prints the difference layout, and frees the allocated memory.
 *
 * Returns: void.
 */
void compare();

/*
 * Ranks all layouts in the layouts directory based on their scores.
 * It reads each layout file, analyzes it, calculates its score,
 * and maintains a ranked list of layouts. Finally, it prints the ranking.
 *
 * Returns: void.
 */
void rank();

/*
 * Initiates the layout generation process without a specific starting layout.
 * Calls improve with shuffle set to 1, effectively starting from a random layout,
 * with no pins. Will still use set of keys from selected layout.
 *
 * Returns: void.
 */
void generate();

/*
 * Improves an existing layout using multiple threads.
 * Each thread runs a simulated annealing process to find a better layout.
 *
 * Parameters:
 *   shuffle: A flag indicating whether to shuffle the layout before starting (1) or not (0).
 *
 * Returns: void.
 */
void improve(int shuffle);

/*
 * Performs a benchmark to determine the optimal number of threads for layout generation.
 * It runs the generation process with different numbers of threads and measures performance.
 *
 * Returns: void.
 */
void gen_benchmark();

/*
 * Prints a help message providing usage instructions for the program's command line arguments.
 *
 * Returns: void.
 */
void print_help();

/*
 * Prints an introductory message and information about the GULAG project.
 *
 * Returns: void.
 */
void print_info();

#endif
