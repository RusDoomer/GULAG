#ifndef IO_UTIL_H
#define IO_UTIL_H

#include "global.h"
#include "structs.h"

/*
 * Converts a character to its corresponding index in the language array.
 * Parameters:
 *   c: The character to convert.
 * Returns: The index of the character in the language array, or -1 if not found.
 */
int convert_char(wchar_t c);

/*
 * Converts an index in the language array back to its corresponding character.
 * Parameters:
 *   i: The index to convert.
 * Returns: The character corresponding to the index, or L'@' if out of bounds.
 */
wchar_t convert_back(int i);

/*
 * Shifts all elements in an integer array one position to the right, discarding the last element.
 * Parameters:
 *   mem: Pointer to the integer array.
 *   size: The size of the array.
 * Returns: void.
 */
void iterate(int *mem, int size);

/*
 * Checks for duplicate characters in a wide character array, excluding adjacent duplicates.
 * Parameters:
 *   arr: The wide character array to check.
 * Returns: The number of duplicate pairs found (excluding adjacent pairs), or -1 if no duplicates.
 */
int check_duplicates(wchar_t *arr);

/*
 * Validates and converts a run mode string to its corresponding character representation.
 * Parameters:
 *   optarg: The string representing the run mode.
 * Returns: The character representing the validated run mode, or 'a' if invalid.
 */
char check_run_mode(char *optarg);

/*
 * Validates and converts an output mode string to its corresponding character representation.
 * Parameters:
 *   optarg: The string representing the output mode.
 * Returns: The character representing the validated output mode, or 'n' if invalid.
 */
char check_output_mode(char *optarg);

#endif
