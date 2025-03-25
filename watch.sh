#!/bin/bash
watch -n 1 "./progress.sh && df -h /dev/sdb1 && ls -hlt *.log | head -n 27 && echo ... && ls -hlt *.log | tail -n 1"
