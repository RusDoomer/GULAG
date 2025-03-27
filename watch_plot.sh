#!/bin/bash
watch -t -n 1 "./plot_progress.sh && df -h /dev/sdb1 && ls -hlt *.png | head -n 27 && echo ... && ls -hlt *.png | tail -n 1"
