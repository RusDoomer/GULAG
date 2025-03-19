import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import argparse
import os
from tqdm import tqdm

def main():
    parser = argparse.ArgumentParser(description='Generate optimization progress plots from log files')
    parser.add_argument('logfile', type=str, help='Path to the log file')
    args = parser.parse_args()

    # Configure matplotlib
    plt.style.use('seaborn-v0_8')
    plt.rcParams.update({
        'figure.dpi': 300,
        'savefig.dpi': 300,
        'font.size': 10,
        'axes.labelsize': 12,
        'axes.titlesize': 14,
        'legend.fontsize': 10,
        'xtick.labelsize': 10,
        'ytick.labelsize': 10
    })

    # Read data
    print("Reading log file...")
    columns = ['thread_id', 'iteration', 'temperature', 'new_score', 
               'max_score', 'accepted', 'timestamp', 'layout_state']
    dtype = {
        'thread_id': 'int32',
        'iteration': 'int32',
        'temperature': 'float32',
        'new_score': 'float32',
        'max_score': 'float32',
        'accepted': 'int8',
        'timestamp': 'float64',
        'layout_state': 'string'
    }
    
    df = pd.read_csv(args.logfile, header=None, names=columns, dtype=dtype)
    print(f"Loaded {len(df):,} rows from {args.logfile}")

    # Preprocess data
    print("Processing threads...")
    groups = df.groupby('thread_id', observed=True)
    thread_data = {}
    
    best_thread = None
    worst_thread = None
    max_score = -np.inf
    min_score = np.inf

    for thread_id, group in tqdm(groups, total=groups.ngroups):
        group = group.sort_values('iteration')
        n_points = len(group)
        window_size = max(1, int(n_points * 0.01))
        
        roller = group['max_score'].rolling(window_size, min_periods=1)
        thread_data[thread_id] = {
            'iterations': group['iteration'].values,
            'max_score': group['max_score'].values,
            'mean': roller.mean().values,
            'min': roller.min().values,
            'max': roller.max().values,
            'temperature': group['temperature'].values,
            'temp_mean': group['temperature'].rolling(window_size, min_periods=1).mean().values
        }
        
        current_max = group['max_score'].max()
        if current_max > max_score:
            max_score = current_max
            best_thread = thread_id
        if current_max < min_score:
            min_score = current_max
            worst_thread = thread_id

    print(f"Best thread: {best_thread} (max score: {max_score:.2f})")
    print(f"Worst thread: {worst_thread} (max score: {min_score:.2f})")

    # Calculate global ranges
    global_min = df['max_score'].min()
    global_max = df['max_score'].max()
    temp_max = df['temperature'].max()

    # Create combined range for non-best threads
    non_best_iters = np.concatenate([d['iterations'] for tid, d in thread_data.items() if tid != best_thread])
    non_best_scores = np.concatenate([d['max_score'] for tid, d in thread_data.items() if tid != best_thread])
    sort_idx = np.argsort(non_best_iters)
    non_best_iters = non_best_iters[sort_idx]
    non_best_scores = non_best_scores[sort_idx]

    # Calculate combined rolling stats for non-best threads
    combined_window = max(1, int(len(non_best_iters) * 0.0005))  # 0.05% window
    roller = pd.Series(non_best_scores).rolling(combined_window, min_periods=1)
    combined_min = roller.min().values
    combined_max = roller.max().values

    # Generate plots
    base_name = os.path.basename(args.logfile)
    plot_base = base_name.replace('_log_', '_plot_').rsplit('.', 1)[0]

    # Main plot with combined ranges
    print("Generating main plot...")
    plt.figure(figsize=(20, 10))
    
    # Plot combined non-best range
    plt.fill_between(non_best_iters, combined_min, combined_max,
                    color='gray', alpha=0.15, label='All Threads Range')
    
    # Plot individual non-best threads
    for tid, data in thread_data.items():
        if tid != best_thread:
            plt.plot(data['iterations'], data['mean'], 
                    color='gray', alpha=0.2, lw=0.5)
    
    # Plot best thread
    best_data = thread_data[best_thread]
    plt.fill_between(best_data['iterations'], best_data['min'], best_data['max'],
                    color='darkred', alpha=0.2, label='Best Thread Range')
    plt.plot(best_data['iterations'], best_data['mean'], 
            color='darkred', lw=2, label='Best Thread')
    
    plt.xlabel('Iterations')
    plt.ylabel('Max Score (Smoothed)')
    plt.ylim(global_min, global_max)
    plt.title('Optimization Progress - All Threads')
    plt.legend(loc='upper left', frameon=True)
    plt.savefig(f"{plot_base}_all.png", bbox_inches='tight')
    plt.close()

    # Best thread plot
    print("Generating best thread plot...")
    fig, ax1 = plt.subplots(figsize=(20, 10))
    
    ax1.plot(best_data['iterations'], best_data['mean'], 
            color='darkred', lw=2, label='Max Score')
    ax1.fill_between(best_data['iterations'], best_data['min'], best_data['max'],
                    color='darkred', alpha=0.2, label='Score Range')
    ax1.set_ylabel('Max Score', color='darkred')
    ax1.set_ylim(global_min, global_max)
    ax1.tick_params(axis='y', labelcolor='darkred')
    
    ax2 = ax1.twinx()
    ax2.plot(best_data['iterations'], best_data['temp_mean'],
            color='darkblue', alpha=0.7, lw=1, label='Temperature')
    ax2.set_ylabel('Temperature', color='darkblue')
    ax2.tick_params(axis='y', labelcolor='darkblue')
    
    lines, labels = ax1.get_legend_handles_labels()
    lines2, labels2 = ax2.get_legend_handles_labels()
    ax1.legend(lines + lines2, labels + labels2, loc='upper left')
    
    plt.title(f"Best Thread {best_thread} Progress")
    plt.savefig(f"{plot_base}_best.png", bbox_inches='tight')
    plt.close()

    # Worst thread plot
    print("Generating worst thread plot...")
    worst_data = thread_data[worst_thread]
    fig, ax1 = plt.subplots(figsize=(20, 10))
    
    ax1.plot(worst_data['iterations'], worst_data['mean'], 
            color='darkgreen', lw=2, label='Max Score')
    ax1.fill_between(worst_data['iterations'], worst_data['min'], worst_data['max'],
                    color='darkgreen', alpha=0.2, label='Score Range')
    ax1.set_ylabel('Max Score', color='darkgreen')
    ax1.set_ylim(global_min, global_max)
    ax1.tick_params(axis='y', labelcolor='darkgreen')
    
    ax2 = ax1.twinx()
    ax2.plot(worst_data['iterations'], worst_data['temp_mean'],
            color='darkblue', alpha=0.7, lw=1, label='Temperature')
    ax2.set_ylabel('Temperature', color='darkblue')
    ax2.tick_params(axis='y', labelcolor='darkblue')
    
    lines, labels = ax1.get_legend_handles_labels()
    lines2, labels2 = ax2.get_legend_handles_labels()
    ax1.legend(lines + lines2, labels + labels2, loc='upper left')
    
    plt.title(f"Worst Thread {worst_thread} Progress")
    plt.savefig(f"{plot_base}_worst.png", bbox_inches='tight')
    plt.close()

    print(f"Plots saved as {plot_base}_*.png")
    
if __name__ == '__main__':
    main()
