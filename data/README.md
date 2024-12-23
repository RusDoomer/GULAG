# Data Directory

The `data` directory contains all the necessary files for defining languages, corpora, layouts, and weights used by the GULAG. This structure allows for easy management and extension of the data used in the analysis and optimization of keyboard layouts.

## Directory Structure

-   **`languages/`**
    -   Each language has its own subdirectory (e.g., `data/english/`).
    -   Contains a `.lang` file defining the language's character set.
    -   Includes `corpora/` and `layouts/` subdirectories specific to the language.
-   **`weights/`**
    -   Contains weight files (`.wght`) that define the importance of each statistic.

## Languages

Each language directory (e.g., `data/english/`) contains the following:

-   **`<language>.lang`**: Defines the character set for the language.
    -   The first two characters must be spaces
    -   Shifted characters are treated as the same characters and are to be placed adjacent to their unshifted counterparts.
    -   The `@` symbol is reserved and cannot be part of the language.
    -   The file must not exceed 100 characters (49 unshifted characters + space).
    -   Example: `data/english/english.lang`
-   **`corpora/`**: Contains text files used as corpora for the language.
    -   Each file is a plain text file representing a corpus.
    -   The program analyzes these files to gather n-gram frequency data.
    -   The first time a corpus is used, a `.cache` file will be generated to speed up future processing.
    -   Example: `data/english/corpora/shai.txt`
-   **`layouts/`**: Contains layout files (`.glg`) for the language.
    -   Each `.glg` file defines a keyboard layout using the characters specified in the language's `.lang` file.
    -   Layouts must be 3x12 matrices (3 rows, 12 columns).
    -   Uses `@` to fill dead-keys.
    -   Example: `data/english/layouts/xenia.glg`

## Weights

-   **`weights/`**: Contains `.wght` files that assign weights to different statistics.
    -   Each line in a `.wght` file corresponds to a statistic and its associated weight(s).
    -   Format: `statistic_name: weight1 weight2 ...`
    -   Weights are separated by spaces.
    -   The number of weights depends on the statistic type (e.g., skipgrams have multiple weights).
    -   You must include weights for all defined statistics.
    -   Setting the weight to 0 will skip that statistic during analysis.
    -   Example: `data/weights/default.wght`

## Creating and Modifying Data

### Adding a New Language

1. Create a new directory under `data/` named after the language.
2. Create a `<language>.lang` file defining the character set.
3. Add at least one corpus file in the `corpora/` subdirectory.
4. Add at least one layout file in the `layouts/` subdirectory.

### Adding a New Corpus

1. Place a unicode text file in the `data/<language>/corpora/` directory.
2. Ensure that the language of the corpora matches the `.lang` file's character set.

### Adding a New Layout

1. Create a `.glg` file in the `data/<language>/layouts/` directory.
2. Define the layout using the characters from the corresponding `.lang` file.

### Adding a New Weight File

1. Create a `.wght` file in the `data/weights/` directory.
2. Define the weights for each statistic, one per line, using the format `statistic_name: weight1 weight2 ...`.
3. In case you do not know which stats are defined you can run a simple verbose analysis `./gulag -m a -o v -w <new_weights>` and all stat weights will be listed, with `-inf` set for missing weights.

## Notes

-   Ensure that all data files are correctly formatted to avoid errors during processing.
-   The `.cache` files are automatically generated and should not be manually edited.
-   When adding new statistics or modifying existing ones, ensure that the corresponding weight files are updated accordingly.
