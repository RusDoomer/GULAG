import pandas as pd
import matplotlib.pyplot as plt
import sys
import re
import os

def plot_progress(log_file):
    df = pd.read_csv(
        log_file,
        names=['thread_id', 'iteration', 'T', 'new_score', 
               'max_score', 'accepted', 'timestamp'],
        delimiter=',',
        encoding='utf-8'
    )
    
    thread_max_scores = df.groupby('thread_id')['max_score'].max()
    best_thread_id = thread_max_scores.idxmax()

    # Compute global min/max with buffer
    global_T_min = df['T'].min()
    global_T_max = df['T'].max()
    global_score_min = df['max_score'].min()
    global_score_max = df['max_score'].max()

    # Add 5% buffer to all axes
    temp_range = global_T_max - global_T_min
    score_range = global_score_max - global_score_min
    buffer_T = temp_range * 0.05 if temp_range != 0 else 0.1
    buffer_score = score_range * 0.05 if score_range != 0 else 0.1

    threads = df['thread_id'].unique().tolist()

    # Get the log filename without path and extension
    base_filename = os.path.basename(log_file)
    base_without_ext, ext = os.path.splitext(base_filename)

    # Validate and split the filename (unchanged)
    if '_log_' not in base_without_ext:
        raise ValueError(f"Invalid log file format: '{log_file}'. Expected '_log_' in the filename.")
    
    algorithm_part, rest_part = base_without_ext.split('_log_', 1)
    split_rest = rest_part.split('_')
    if len(split_rest) < 3:
        raise ValueError(f"Invalid log file format: '{log_file}'. Expected '_log_' followed by weight name and timestamp (YYYYMMDD_HHMMSS).")
    
    weight_name = '_'.join(split_rest[:-2])

    # Construct output filename (unchanged)
    new_base = base_without_ext.replace('_log_', '_plot_')
    output_filename = f"{new_base}.png"

    # Consistent font size
    font_size = 24  # Choose a base font size

    # LARGER FIGURE WITH HIGHER RESOLUTION
    plt.figure(figsize=(40, 12), dpi=600)
    plt.subplots_adjust(left=0.05, right=0.98, top=0.92, bottom=0.15)

    # Plot non-best threads with thinner lines (unchanged)
    for thread in [t for t in threads if t != best_thread_id]:
        thread_data = df[df['thread_id'] == thread]
        plt.plot(
            thread_data['iteration'],
            thread_data['max_score'],
            color='#6d6e71',
            alpha=0.5,
            linewidth=0.5,  
            label=None
        )
    
    # Plot best thread (unchanged except no legend)
    best_thread_data = df[df['thread_id'] == best_thread_id]
    plt.plot(
        best_thread_data['iteration'],
        best_thread_data['max_score'],
        color='#8B0000',
        alpha=1.0,
        linewidth=1.0,  
        label=None
    )

    # Main plot formatting
    plt.title(
        f"Simulated Annealing Progress\n"
        f"Algorithm: {algorithm_part}\n"
        f"Weights: {weight_name}",
        pad=10,  
        fontsize=font_size * 1.5,  # Larger title
        weight='bold'
    )

    plt.xlabel('Iteration', fontsize=font_size)
    plt.ylabel('Max Score', fontsize=font_size)
    plt.xticks(fontsize=font_size)
    plt.yticks(fontsize=font_size)

    # Grid formatting (unchanged)
    plt.grid(linestyle='--', alpha=0.6)

    props = dict(boxstyle='round', facecolor='#f8f9fa', alpha=0.75)  
    plt.text(
        0.95, 0.05,
        f"Best Thread: {best_thread_id}\n"
        f"Peak Score: {thread_max_scores[best_thread_id]:.2f}",
        transform=plt.gca().transAxes,
        ha='right',
        va='bottom',
        fontsize=font_size * 1.2,  # Larger text box
        bbox=props
    )

    # Save main plot
    plt.savefig(output_filename, dpi=600, bbox_inches='tight')
    print(f"Main plot saved to {output_filename}")

    # Generate per-thread plots
    for thread in threads:
        fig, ax1 = plt.subplots(figsize=(40, 12), dpi=600)
        plt.subplots_adjust(left=0.05, right=0.98, top=0.92, bottom=0.15)

        thread_data = df[df['thread_id'] == thread]
        
        ax2 = ax1.twinx()

        # Temperature plot with buffer (unchanged except no legend)
        ax1.plot(
            thread_data['iteration'],
            thread_data['T'],
            color='#0000FF',
            linewidth=1.0
        )
        ax1.set_ylim(
            global_T_min - buffer_T,
            global_T_max + buffer_T
        )
        ax1.set_ylabel('Temperature (K)', color='#0000FF', fontsize=font_size)
        ax1.tick_params(axis='y', labelcolor='#0000FF', labelsize=font_size)
        ax1.tick_params(axis='x', labelsize=font_size) # x axis ticks too


        # Max score plot with buffer (unchanged except no legend)
        ax2.plot(
            thread_data['iteration'],
            thread_data['max_score'],
            color='#8B0000',
            linewidth=1.0
        )
        ax2.set_ylim(
            global_score_min - buffer_score,
            global_score_max + buffer_score
        )
        ax2.set_ylabel('Max Score', color='#8B0000', fontsize=font_size)
        ax2.tick_params(axis='y', labelcolor='#8B0000', labelsize=font_size)


        # Formatting
        plt.title(
            f"Thread {thread} Details\n"
            f"Algorithm: {algorithm_part}\n"
            f"Weights: {weight_name}",
            pad=10,
            fontsize=font_size * 1.5,  # Larger title
            weight='bold'
        )

        ax1.set_xlabel('Iteration', fontsize=font_size)
        ax1.grid(linestyle='--', alpha=0.5, which='major')

        # Save plot
        per_thread_filename = f"{new_base}_thread_{thread}.png"
        plt.savefig(per_thread_filename, dpi=600, bbox_inches='tight', pad_inches=0.2)
        print(f"Thread {thread} plot saved to {per_thread_filename}")
        plt.close(fig)

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python plotter.py [LOG_FILE]")
        sys.exit(1)
        
    try:
        plot_progress(sys.argv[1])
    except Exception as e:
        print(f"Error processing file: {e}")
