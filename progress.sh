#!/bin/bash
printf "%d/2916 runs | " "$(find . -type f -name "*.log" | wc -l )"
printf "%d/8748 plots\n" "$(find . -type f -name "*.png" | wc -l )"
