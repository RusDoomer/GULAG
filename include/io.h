#ifndef IO_H
#define IO_H

#include "global.h"
#include "structs.h"

/*
 * Prints a message to the standard output stream, with verbosity control.
 * The message will only be printed if the current output mode meets or
 * exceeds the required verbosity level specified by 'required_level'.
 *
 * Parameters:
 *   required_level: The minimum verbosity level required to print the message.
 *                   'q' for quiet, 'n' for normal, 'v' for verbose.
 *   format:         The format string for the message, compatible with vwprintf.
 *   ...:            Variable arguments for the format string.
 *
 * Returns:
 *   void
 */
void log_print(char required_level, const wchar_t *format, ...);

/*
 * Reads the configuration file to set up initial program parameters.
 * This function parses 'config.conf' to initialize various settings
 * such as pinned key positions, language, corpus, layout names,
 * weights file, run mode, number of repetitions, number of threads,
 * and output mode.
 *
 * Returns: void.
 */
void read_config();


/*
 * Processes command line arguments to override configuration settings.
 * It parses arguments passed to the main function and updates
 * corresponding global variables such as language name, corpus name,
 * layout names, weight file, repetitions, threads, run mode, and output mode.
 *
 * Parameters:
 *   argc: The number of command line arguments.
 *   argv: An array of C-style strings containing the command line arguments.
 * Returns: void.
 */
void read_args(int argc, char **argv);

/*
 * Validates the current program settings to ensure they are legal and consistent.
 * Checks if the language, corpus, layouts, weights, run mode, output mode,
 * number of threads, and repetitions are valid. Terminates the program with
 * an error message if an invalid setting is found.
 *
 * Returns: void.
 */
void check_setup();

/*
 * Reads and sets the current language's character set from a language file.
 * The function constructs the path to the language file based on 'lang_name',
 * reads the character set into 'lang_arr', and sets up the 'char_table' for
 * character code lookups. It performs checks to ensure the language file
 * is correctly formatted and only contains legal characters.
 *
 * Returns: void.
 */
void read_lang();

/*
 * Attempts to read corpus data from a cache file.
 * This function constructs the path to the cache file based on 'lang_name'
 * and 'corpus_name'. If the cache file exists, it reads the pre-computed
 * ngram frequencies into the global corpus arrays.
 *
 * Returns:
 *   1 if the cache file was successfully read, 0 otherwise.
 */
int read_corpus_cache();

/*
 * Reads and processes a corpus text file to collect ngram frequency data.
 * It constructs the path to the corpus file and reads it character by character,
 * updating the frequency counts in the global corpus arrays for monograms,
 * bigrams, trigrams, quadgrams, and skipgrams.
 *
 * Returns: void.
 */
void read_corpus();

/*
 * Creates or updates a cache file with the current corpus frequency data.
 * This function writes the current state of the global corpus arrays to a
 * cache file, allowing for quicker initialization in future runs.
 *
 * Returns: void.
 */
void cache_corpus();

/*
 * Reads and applies weights from a weight file to the corresponding statistics.
 * It opens the weight file specified by 'weight_name' and parses it line by
 * line, updating the 'weight' field of each statistic based on its name.
 *
 * Returns: void.
 */
void read_weights();


/*
 * Reads a keyboard layout from a file and initializes a layout structure.
 * The layout file is specified by either 'layout_name' or 'layout2_name'
 * based on the 'which_layout' parameter. The function reads the layout matrix
 * and sets the layout name in the provided layout structure.
 *
 * Parameters:
 *   lt:           A pointer to the layout structure to be initialized.
 *   which_layout: An integer indicating which layout to read (1 or 2).
 * Returns: void.
 */
void read_layout(layout *lt, int which_layout);

/*
 * Prints the contents of a layout structure to the standard output.
 * The function uses the current 'output_mode' to determine the level of detail
 * to print, ranging from just the layout matrix and score to detailed
 * statistics for each ngram type.
 *
 * Parameters:
 *   lt: A pointer to the layout structure to be printed.
 * Returns: void.
 */
void print_layout(layout *lt);

/*
 * Prints the ranked list of layouts to the standard output.
 * Layouts are printed in descending order of their score, along with their names.
 * The function traverses the 'head_node' linked list to access each layout node.
 *
 * Returns: void.
 */
void print_ranking();

/*
 * Prints the current pin configuration for layout improvement to the standard output.
 * Each element in the 'pins' array is printed, showing which positions are pinned.
 *
 * Returns: void.
 */
void print_pins();

#endif
