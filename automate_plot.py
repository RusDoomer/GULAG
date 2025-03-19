import os
import subprocess
import glob
import sys

def plot_all_logs(directory='.'):
    # Find all .log files in the specified directory
    log_files = glob.glob(os.path.join(directory, '*.log'))
    
    if not log_files:
        print(f"No .log files found in {directory}")
        return
    
    print(f"Found {len(log_files)} log files. Processing...")
    
    # Run plot.py on each log file
    for log_file in log_files:
        print(f"Processing {log_file}...")
        try:
            subprocess.run(['python3', 'plot.py', log_file], check=True)
            print(f"Successfully processed {log_file}")
        except subprocess.CalledProcessError as e:
            print(f"Error processing {log_file}: {e}")

if __name__ == '__main__':
    # Use command line argument as directory if provided, otherwise use current directory
    directory = sys.argv[1] if len(sys.argv) > 1 else '.'
    plot_all_logs(directory)
