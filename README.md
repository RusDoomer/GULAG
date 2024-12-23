# GULAG: GPU Utilizing Layout Analyzer and Generator

## Overview

GULAG is a GPU-accelerated keyboard layout optimizer designed to analyze and generate keyboard layouts based on n-gram frequency analysis. It leverages the power of OpenCL to perform computations in parallel, significantly speeding up the process of finding optimal layouts.

## Table of Contents

-   [Features](#features)
-   [Getting Started](#getting-started)
    -   [Prerequisites](#prerequisites)
    -   [Installation](#installation)
-   [Usage](#usage)
    -   [Configuration](#configuration)
    -   [Running Modes](#running-modes)
    -   [Analyzing Layouts](#analyzing-layouts)
    -   [Generating Layouts](#generating-layouts)
    -   [Comparing Layouts](#comparing-layouts)
    -   [Ranking Layouts](#ranking-layouts)
    -   [Improving Layouts](#improving-layouts)
    -   [Benchmarking](#benchmarking)
-   [Data](#data)
    -   [Languages](#languages)
    -   [Corpora](#corpora)
    -   [Layouts](#layouts)
    -   [Weights](#weights)
-   [Contributing](#contributing)
-   [License](#license)
-   [FAQ](#faq)
-   [Contact](#contact)

## Features

-   **GPU Acceleration:** (FUTURE) Utilizes OpenCL for parallel computation, enabling faster analysis and generation of keyboard layouts.
-   **N-gram Analysis:** Supports analysis based on monograms, bigrams, trigrams, quadgrams, and skipgrams.
-   **Multiple Modes:** Offers various modes of operation, including analysis, comparison, ranking, generation, and optimization of layouts.
-   **Language Support:** Designed to work with different languages, each with its own set of characters and corpus data.
-   **Extensible:** Modular design allows for easy addition of new statistics, languages, and analysis methods.

## Getting Started

### Prerequisites

-   A system with OpenCL-compatible GPU and drivers.
-   OpenCL development package (e.g., `opencl-headers`, `ocl-icd-opencl-dev`).
-   A C compiler (e.g., GCC).
-   `make` utility for building the project.
-   `getopt` for command line arguments parsing.

### Installation

1. Clone the repository:

    ```bash
    git clone https://github.com/your-username/GULAG.git
    cd GULAG
    ```

2. Compile the project:

    ```bash
    make
    ```

## Usage

### Configuration

The `config.conf` file allows you to set default parameters for the program. You can specify:

-   `pins`: Specify pinned keys for the improve mode.
-   `lang`: Default language.
-   `corpus`: Default corpus.
-   `layout`: Default primary layout.
-   `layout2`: Default secondary layout.
-   `weights`: Default weights file.
-   `run_mode`: Default running mode ('a', 'c', 'r', 'g', 'i', 'b', 'h', 'f').
-   `repetitions`: Number of iterations for generation/improvement.
-   `threads`: Number of threads for parallel execution.
-   `output_mode`: Verbosity level ('q', 'n', 'v').

Command line arguments can override these settings.

### Running Modes
The program supports the following running modes, selectable via the `-m` option:
| Mode | Description |
|---|---|
| `a`, `analyze`, `analysis` | Analyze a single layout. |
| `c`, `compare`, `comparison` | Compare two layouts. |
| `r`, `rank`, `ranking` | Rank all layouts in the language directory. |
| `g`, `gen`, `generate` | Generate a new layout. |
| `i`, `improve`, `optimize` | Improve an existing layout. |
| `b`, `bench`, `benchmark` | Benchmark to find the optimal number of threads. |
| `h`, `help` | Print the help message. |
| `f`, `info`, `information` | Print an introductory message about the project. |

### Analyzing Layouts

To analyze a layout, use the `a` mode argument:

```bash
./gulag -m a -l <language> -1 <layout> -c <corpus> -w <weights>
```

### Comparing Layouts

To compare two layouts, use the `c` mode argument:

```bash
./gulag -m c -l <language> -1 <layout1> -2 <layout2> -c <corpus> -w <weights>
```

### Ranking Layouts

To rank all layouts in a language directory, use the `r` mode argument:

```bash
./gulag -m r -l <language> -c <corpus> -w <weights>
```

### Generating Layouts

To generate a new layout, use the `g` mode argumen:

```bash
./gulag -m g -l <language> -c <corpus> -w <weights> -r <repetitions> -t <threads>
```

### Improving Layouts

To improve an existing layout, use the `i` mode argument:

```bash
./gulag -m i -l <language> -1 <layout> -c <corpus> -w <weights> -r <repetitions> -t <threads>
```
You can use the config.conf file to specify pinned keys that should not be changed during optimization.

### Benchmarking

To benchmark and find the optimal number of threads, use the `b` mode argument:

```bash
./gulag -m b -l <language> -c <corpus> -w <weights>
```

## Data

The `data` directory is organized into subdirectories for languages, corpora, layouts, and weights. This structure helps manage the various data files used by GULAG for analysis and optimization.

### Languages

Each language has its own subdirectory under `data/`, named after the language (e.g., `data/english`). Within each language directory, you will find:

-   **`.lang`:** This file defines the character set for the language, including all the keys that can be used in a layout. For example, `english.lang` defines the characters used in the English language layouts.
-   **`corpora/`:** This directory contains one or more corpus files used to gather n-gram frequency data for the specified language.
-   **`layouts/`:** This directory contains keyboard layout files (`.glg`) for the specified language.

### Corpora

Corpora are text files located within the `data/<language>/corpora` directory. They are essential for providing the raw data from which n-gram frequencies are calculated. Each corpus represents a collection of text in a specific language. The first time a corpora is used GULAG will create a cache to increase processing times on future uses of the same corpus.

### Layouts

Layout files (`.glg`) define the arrangement of keys on the keyboard for a particular language. These files are located in the `data/<language>/layouts` directory.

### Weights

Weight files (`.wght`) are located in the `data/weights` directory and specify the importance of each statistic in the overall analysis. These weights linearly influence how the layout optimization process prioritizes different n-gram statistics.

For further details on data formats, how to create or modify them, and their usage, please refer to the `data/README.md` file.

## FAQ

For answers to frequently asked questions, including troubleshooting tips and common issues, please refer to the `docs/faq.md`.

