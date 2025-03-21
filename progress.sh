#!/bin/bash
printf "%d/2916 runs\n" "$(find . -type f -name "*.log" | wc -l )"
