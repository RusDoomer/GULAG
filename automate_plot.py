import os
import subprocess
import glob
import sys
import concurrent.futures
import argparse

def process_log_file(log_file):
    """Process a single log file using plot.py"""
    print(f"Processing {log_file}...")
    try:
        subprocess.run(['python3', 'plot.py', log_file], check=True)
        print(f"Successfully processed {log_file}")
        return True
    except subprocess.CalledProcessError as e:
        print(f"Error processing {log_file}: {e}")
        return False

def plot_all_logs(directory='.', num_threads=4):
    # Find all .log files in the specified directory
    log_files = glob.glob(os.path.join(directory, '*.log'))
    
    if not log_files:
        print(f"No .log files found in {directory}")
        return
    
    print(f"Found {len(log_files)} log files. Processing with {num_threads} threads...")
    
    # Process log files in parallel using ThreadPoolExecutor
    with concurrent.futures.ThreadPoolExecutor(max_workers=num_threads) as executor:
        results = list(executor.map(process_log_file, log_files))
    
    # Summarize results
    successful = results.count(True)
    failed = results.count(False)
    print(f"Processing complete: {successful} successful, {failed} failed")

if __name__ == '__main__':
    # Set up argument parser for better command line handling
    parser = argparse.ArgumentParser(description='Process multiple log files in parallel')
    parser.add_argument('-d', '--directory', default='.', help='Directory containing log files (default: current directory)')
    parser.add_argument('-t', '--threads', type=int, default=4, help='Number of threads to use (default: 4)')
    
    args = parser.parse_args()
    
    plot_all_logs(args.directory, args.threads)
