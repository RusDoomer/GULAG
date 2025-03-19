#!/usr/bin/env python3
import subprocess
import itertools

# Read input files
with open('starting_layouts.txt', 'r') as f:
    layouts = [line.strip() for line in f if line.strip()]

with open('weight_names.txt', 'r') as f:
    weights = [line.strip() for line in f if line.strip()]

with open('threads.txt', 'r') as f:
    threads = [line.strip() for line in f if line.strip()]

with open('repetitions.txt', 'r') as f:
    repetitions = [line.strip() for line in f if line.strip()]

# Number of times to repeat each combination
num_repeats = 3

# Counter for tracking progress
total_combinations = (len(weights) * len(threads) * len(repetitions) * 
                     (1 + len(layouts))) * num_repeats
completed = 0

print(f"Total runs to execute: {total_combinations}")

# Run mode 'g' (no layouts variations)
for weight, thread, rep in itertools.product(weights, threads, repetitions):
    for run_num in range(num_repeats):
        completed += 1
        
        command = [
            "./gulag",
            "-m", "g",
            "-w", weight,
            "-t", thread,
            "-r", rep
        ]
        
        print(f"[{completed}/{total_combinations}] Running: {' '.join(command)}")
        
        # Execute the command
        subprocess.run(command)

# Run mode 'i' (with layouts variations)
for weight, thread, rep, layout in itertools.product(weights, threads, repetitions, layouts):
    for run_num in range(num_repeats):
        completed += 1
        
        command = [
            "./gulag",
            "-m", "i",
            "-1", layout,
            "-w", weight,
            "-t", thread,
            "-r", rep
        ]
        
        print(f"[{completed}/{total_combinations}] Running: {' '.join(command)}")
        
        # Execute the command
        subprocess.run(command)

print("All runs completed!")
