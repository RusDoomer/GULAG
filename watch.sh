#!/bin/bash
watch -t -n 1 "./progress.sh && df -h /mnt/small && ls -hlt *.log | head -n 27 && echo ... && ls -hlt *.log | tail -n 1"
