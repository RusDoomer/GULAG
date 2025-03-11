# Frequently Asked Questions (FAQ)

This document addresses common questions about the project.

---

## 1. General Questions

### What is this project about?

This project is a keyboard layout analyzer written in C. It is designed to be GPU accelerated for maximum performance using OpenCL, although this feature is not yet implemented.

### How can I get support?

Join the akl Discord server: [https://discord.gg/dvorak](https://discord.gg/dvorak) and ping me `@RusDomer`.

---

## 2. Installation

### What are the prerequisites for this project?

To install and run this project, you will need the following:

*   A C compiler (e.g., GCC, Clang)
*   OpenCL
*   An OpenCL-compatible GPU
*   Make

---

## 3. Troubleshooting

### Why can't GULAG find the shai corpus?

This program is BYOC, so you must download the corpus yourself, the simplest way to do that is to download [this file](https://colemak.com/pub/corpus/iweb-corpus-samples-cleaned.txt.xz) unpack it and move the file to `./data/english/corpora/shai.txt`.
