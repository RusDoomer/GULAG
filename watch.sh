#!/bin/bash
watch -t -n 1 "./progress.sh && df -h /mnt/small && ls -hlt *.*g | head -n 25 && echo ... && ls -hlt  *.log | tail -n 1"
