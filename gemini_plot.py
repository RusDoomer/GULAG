import pandas as pd
import matplotlib
matplotlib.use('Agg')  # Use the Agg backend (non-GUI)
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
import numpy as np
import sys
import os
from scipy.ndimage import uniform_filter1d

def create_plots(logfile):

    # --- 1. Data Loading and Preprocessing ---

    print(f"Loading data from {logfile}...")

    try:
        df = pd.read_csv(logfile, header=None, names=['Thread', 'Iteration', 'Temperature', 'Candidate Score', 'Max Score', 'Accepted', 'Timestamp', 'Layout'])
    except FileNotFoundError:
        print(f"Error: File not found: {logfile}")
        return
    except Exception as e:
        print(f"Error reading CSV file: {e}")
        return

    num_threads = df['Thread'].nunique()
    total_iterations = df.groupby('Thread')['Iteration'].max().min()  # Iterations per thread. Minimum across threads.
    print(f"Detected {num_threads} threads, with at least {total_iterations} iterations each.")

    # --- 2. Determine Best and Worst Threads ---

    best_thread = df.groupby('Thread')['Max Score'].last().idxmax()
    worst_thread = df.groupby('Thread')['Max Score'].last().idxmin()
    print(f"Best performing thread: {best_thread}")
    print(f"Worst performing thread: {worst_thread}")

    # --- 3. Calculate Rolling Statistics ---

    # Dynamically adjust window size:
    window_size = max(10, int(total_iterations / 100))  # At least 10, up to 1% of total iterations
    print(f"Using window size: {window_size} for rolling statistics.")

    for thread in df['Thread'].unique():
        thread_data = df[df['Thread'] == thread].sort_values('Iteration')
        df.loc[df['Thread'] == thread, 'Rolling Max Score'] = uniform_filter1d(thread_data['Max Score'], size=window_size, mode='nearest') # moving average

        df.loc[df['Thread'] == thread, 'Rolling Max'] = thread_data['Max Score'].rolling(window=window_size,center=True).max() # rolling maximum and minimum
        df.loc[df['Thread'] == thread, 'Rolling Min'] = thread_data['Max Score'].rolling(window=window_size,center=True).min()

    # --- 4. Plotting ---

    def setup_axes(ax): # reduce redundant plot styling
        ax.set_xlabel('Iteration', fontsize=14)
        ax.set_ylabel('Max Score', fontsize=14)
        ax.tick_params(axis='both', which='major', labelsize=12)
        ax.grid(True, linestyle=':', alpha=0.6)
        ax.xaxis.set_major_locator(ticker.MaxNLocator(nbins=20))  # More ticks, less crowding

    print("Creating plots...")

    # Dynamically adjust figure size and DPI based on number of iterations:
    fig_width = 16 + total_iterations / 5000  # Scale width with iterations
    fig_height = 9
    dpi = 300 + total_iterations / 1000  # High DPI, scale with iterations
    # Limit maximum DPI and figure dimensions to prevent memory errors
    max_dpi = 600
    max_width = 40
    dpi = min(dpi, max_dpi)
    fig_width = min(fig_width, max_width)

    # --- 4.1. All Threads Plot ---

    fig, ax = plt.subplots(figsize=(fig_width, fig_height), dpi=dpi)
    setup_axes(ax)

    for thread in df['Thread'].unique():
        thread_data = df[df['Thread'] == thread]

        if thread == best_thread:
            ax.plot(thread_data['Iteration'], thread_data['Rolling Max Score'], color='firebrick', linewidth=2, label=f'Best Thread ({best_thread})',zorder=10)
            ax.fill_between(thread_data['Iteration'], thread_data['Rolling Min'], thread_data['Rolling Max'], color='firebrick', alpha=0.1, zorder=9)

        else:
            ax.plot(thread_data['Iteration'], thread_data['Rolling Max Score'], color='gray', linewidth=0.5, alpha=0.3)
            ax.fill_between(thread_data['Iteration'], thread_data['Rolling Min'], thread_data['Rolling Max'], color='gray', alpha=0.05)


    # Legend and title
    ax.set_title(f'Max Score vs. Iteration (All Threads)\nFile: {logfile}', fontsize=16)
    legend = ax.legend(loc='lower right', fontsize=10, frameon=True)
    legend.get_frame().set_facecolor('white')
    legend.get_frame().set_alpha(0.9)  # Semi-transparent legend box.

    # --- 4.2. Best Thread Plot ---

    fig_best, ax_best = plt.subplots(figsize=(fig_width, fig_height), dpi=dpi)
    setup_axes(ax_best)

    best_thread_data = df[df['Thread'] == best_thread].sort_values('Iteration')
    ax_best.plot(best_thread_data['Iteration'], best_thread_data['Rolling Max Score'], color='firebrick', linewidth=2)
    ax_best.fill_between(best_thread_data['Iteration'], best_thread_data['Rolling Min'], best_thread_data['Rolling Max'], color='firebrick', alpha=0.2)

    # Temperature on secondary axis
    ax_temp = ax_best.twinx()
    temp_rolling = uniform_filter1d(best_thread_data['Temperature'], size=window_size, mode='nearest')
    ax_temp.plot(best_thread_data['Iteration'], temp_rolling, color='royalblue', linewidth=1.5, alpha=0.7)
    ax_temp.set_ylabel('Temperature', fontsize=14, color='royalblue')
    ax_temp.tick_params(axis='y', labelcolor='royalblue', labelsize=12)

    # Legend
    lines, labels = ax_best.get_legend_handles_labels()
    lines2, labels2 = ax_temp.get_legend_handles_labels()

    legend = ax_best.legend(lines + lines2, ['Max Score', 'Temperature'], loc='lower right', fontsize=12, frameon=True)
    legend.get_frame().set_facecolor('white')
    legend.get_frame().set_alpha(0.9)


    ax_best.set_title(f'Max Score and Temperature vs. Iteration (Best Thread: {best_thread})\nFile: {logfile}', fontsize=16)


    # --- 4.3. Worst Thread Plot ---

    fig_worst, ax_worst = plt.subplots(figsize=(fig_width, fig_height), dpi=dpi)
    setup_axes(ax_worst)


    worst_thread_data = df[df['Thread'] == worst_thread].sort_values('Iteration')
    ax_worst.plot(worst_thread_data['Iteration'], worst_thread_data['Rolling Max Score'], color='firebrick', linewidth=2)
    ax_worst.fill_between(worst_thread_data['Iteration'], worst_thread_data['Rolling Min'], worst_thread_data['Rolling Max'], color='firebrick', alpha=0.2)

    # Temperature on secondary axis
    ax_temp_worst = ax_worst.twinx()
    temp_worst_rolling = uniform_filter1d(worst_thread_data['Temperature'], size=window_size, mode='nearest')
    ax_temp_worst.plot(worst_thread_data['Iteration'], temp_worst_rolling, color='royalblue', linewidth=1.5, alpha=0.7)

    ax_temp_worst.set_ylabel('Temperature', fontsize=14, color='royalblue')
    ax_temp_worst.tick_params(axis='y', labelcolor='royalblue', labelsize=12)

    #Legend
    lines_w, labels_w = ax_worst.get_legend_handles_labels()
    lines2_w, labels2_w = ax_temp_worst.get_legend_handles_labels()

    legend = ax_worst.legend(lines_w + lines2_w, ['Max Score', 'Temperature'], loc='lower right', fontsize=12, frameon=True)
    legend.get_frame().set_facecolor('white')
    legend.get_frame().set_alpha(0.9)

    ax_worst.set_title(f'Max Score and Temperature vs. Iteration (Worst Thread: {worst_thread})\nFile: {logfile}', fontsize=16)

    # --- 5. Save Plots ---

    output_filename_all = logfile.replace('_log_', '_plot_all_').replace('.log', '.png')
    output_filename_best = logfile.replace('_log_', '_plot_best_').replace('.log', '.png')
    output_filename_worst = logfile.replace('_log_', '_plot_worst_').replace('.log', '.png')

    print(f"Saving plot to {output_filename_all}...")
    fig.savefig(output_filename_all, bbox_inches='tight')

    print(f"Saving plot to {output_filename_best}...")
    fig_best.savefig(output_filename_best, bbox_inches='tight')

    print(f"Saving plot to {output_filename_worst}...")
    fig_worst.savefig(output_filename_worst, bbox_inches='tight')

    plt.close('all')  # Close all figures to free memory
    print("Done.")

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python script.py <logfile>")
        sys.exit(1)

    logfile = sys.argv[1]
    create_plots(logfile)
