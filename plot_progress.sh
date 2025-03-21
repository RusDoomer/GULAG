#!/bin/bash
printf "%d/8748 plots\n" "$(find . -type f -name "*.png" | wc -l )"
