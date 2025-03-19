import sys
import csv
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from collections import defaultdict

def main():
    if len(sys.argv) != 2:
        print("Usage: python plotter.py <logfile>")
        sys.exit(1)

    filename = sys.argv[1]
    output_base = filename.replace('_log_', '_plot_').replace('.log', '.png')

    # Read and process the CSV data
    data = defaultdict(list)
    with open(filename, newline='') as f:
        reader = csv.reader(f)
        for row in reader:
            try:
                thread_id = int(row[0])
                iteration = int(row[1])
                T = float(row[2])
                max_score = float(row[4])
                data[thread_id].append((iteration, T, max_score))
            except (IndexError, ValueError):
                continue

    # Determine best and worst threads based on final score
    best_score = -np.inf
    best_thread = None
    worst_score = np.inf
    worst_thread = None
    all_threads = list(data.keys())

    for tid in all_threads:
        entries = sorted(data[tid], key=lambda x: x[0])
        final_score = entries[-1][2]
        if final_score > best_score:
            best_score = final_score
            best_thread = tid
        if final_score < worst_score:
            worst_score = final_score
            worst_thread = tid

    # Collect all scores and temperatures for global scaling
    all_scores = [e[2] for t in data.values() for e in t]
    all_Ts = [e[1] for t in data.values() for e in t]
    global_min_score, global_max_score = min(all_scores), max(all_scores)
    global_min_T, global_max_T = min(all_Ts), max(all_Ts)

    # Process each thread's data for plotting
    smoothed = {}
    for tid in all_threads:
        entries = sorted(data[tid], key=lambda x: x[0])
        iterations = np.array([e[0] for e in entries])
        Ts = np.array([e[1] for e in entries])
        scores = np.array([e[2] for e in entries])

        N = len(iterations)
        window_length = max(10, int(0.01 * N))
        window_length = min(window_length, N)

        # Compute rolling mean, min, max for scores
        df = pd.DataFrame({'score': scores})
        rolling = df.rolling(window=window_length, min_periods=1)
        mean_score = rolling.mean().values.flatten()
        min_score = rolling.min().values.flatten()
        max_score = rolling.max().values.flatten()

        # Compute rolling mean for temperature
        df_T = pd.DataFrame({'T': Ts})
        rolling_T = df_T.rolling(window=window_length, min_periods=1)
        mean_T = rolling_T.mean().values.flatten()

        smoothed[tid] = {
            'iterations': iterations,
            'mean_score': mean_score,
            'min_score': min_score,
            'max_score': max_score,
            'mean_T': mean_T
        }

    # Create main plot
    fig_main, ax_main = plt.subplots(figsize=(12, 8))
    plt.subplots_adjust(right=0.75)  # Adjust for legend

    for tid in all_threads:
        color = 'red' if tid == best_thread else 'gray'
        alpha_line = 1.0 if tid == best_thread else 0.5
        alpha_shade = 0.3 if tid == best_thread else 0.2
        zorder = 2 if tid == best_thread else 1

        ax_main.plot(smoothed[tid]['iterations'], smoothed[tid]['mean_score'],
                     color=color, alpha=alpha_line, zorder=zorder, lw=1.5)
        ax_main.fill_between(smoothed[tid]['iterations'],
                            smoothed[tid]['min_score'],
                            smoothed[tid]['max_score'],
                            color=color, alpha=alpha_shade, zorder=zorder - 1)

    ax_main.set_xlabel('Iteration', fontsize=12)
    ax_main.set_ylabel('Max Score', fontsize=12)
    ax_main.set_title(f"Max Score Progression for {filename}", fontsize=14)
    ax_main.set_ylim(global_min_score - 0.1*(global_max_score - global_min_score),
                    global_max_score + 0.1*(global_max_score - global_min_score))

    # Legend placement outside the plot
    legend_elements = [plt.Line2D([0], [0], color='red', lw=2, label='Best Thread'),
                       plt.Line2D([0], [0], color='gray', alpha=0.5, lw=2, label='Other Threads')]
    ax_main.legend(handles=legend_elements, loc='upper left', bbox_to_anchor=(1, 1), frameon=False)

    # Save main plot as PNG
    fig_main.savefig(f"{output_base}", dpi=300, bbox_inches='tight')
    plt.close(fig_main)

    # Create best thread plot (score vs. temperature)
    fig_best, ax_score = plt.subplots(figsize=(12, 6))
    ax_temp = ax_score.twinx()

    tid = best_thread
    iterations = smoothed[tid]['iterations']
    ax_score.plot(iterations, smoothed[tid]['mean_score'], 'r', label='Score', lw=2)
    ax_score.fill_between(iterations, smoothed[tid]['min_score'],
                         smoothed[tid]['max_score'], color='r', alpha=0.3, lw=0)
    ax_temp.plot(iterations, smoothed[tid]['mean_T'], 'b', label='Temperature', lw=2)

    ax_score.set_xlabel('Iteration', fontsize=12)
    ax_score.set_ylabel('Max Score', color='r', fontsize=12)
    ax_score.tick_params(axis='y', labelcolor='r')
    ax_temp.set_ylabel('Temperature', color='b', fontsize=12)
    ax_temp.tick_params(axis='y', labelcolor='b')

    # Set consistent limits
    ax_score.set_ylim(global_min_score - 0.1*(global_max_score - global_min_score),
                     global_max_score + 0.1*(global_max_score - global_min_score))
    ax_temp.set_ylim(global_min_T - 0.1*(global_max_T - global_min_T),
                    global_max_T + 0.1*(global_max_T - global_min_T))

    # Dual-axis legends
    ax_score.legend(loc='upper left', frameon=False)
    ax_temp.legend(loc='upper right', frameon=False)

    # Save best thread plot as PNG
    fig_best.savefig(f"{output_base}_best.png", dpi=300, bbox_inches='tight')
    plt.close(fig_best)

    # Create worst thread plot (score vs. temperature)
    fig_worst, ax_score_w = plt.subplots(figsize=(12, 6))
    ax_temp_w = ax_score_w.twinx()

    tid = worst_thread
    iterations = smoothed[tid]['iterations']
    ax_score_w.plot(iterations, smoothed[tid]['mean_score'], 'r', label='Score', lw=2)
    ax_score_w.fill_between(iterations, smoothed[tid]['min_score'],
                           smoothed[tid]['max_score'], color='r', alpha=0.3, lw=0)
    ax_temp_w.plot(iterations, smoothed[tid]['mean_T'], 'b', label='Temperature', lw=2)

    ax_score_w.set_xlabel('Iteration', fontsize=12)
    ax_score_w.set_ylabel('Max Score', color='r', fontsize=12)
    ax_score_w.tick_params(axis='y', labelcolor='r')
    ax_temp_w.set_ylabel('Temperature', color='b', fontsize=12)
    ax_temp_w.tick_params(axis='y', labelcolor='b')

    # Set consistent limits
    ax_score_w.set_ylim(global_min_score - 0.1*(global_max_score - global_min_score),
                       global_max_score + 0.1*(global_max_score - global_min_score))
    ax_temp_w.set_ylim(global_min_T - 0.1*(global_max_T - global_min_T),
                      global_max_T + 0.1*(global_max_T - global_min_T))

    # Dual-axis legends
    ax_score_w.legend(loc='upper left', frameon=False)
    ax_temp_w.legend(loc='upper right', frameon=False)

    # Save worst thread plot as PNG
    fig_worst.savefig(f"{output_base}_worst.png", dpi=300, bbox_inches='tight')
    plt.close(fig_worst)

    print(f"Plots saved to {output_base}, {output_base}_best.png, and {output_base}_worst.png")

if __name__ == "__main__":
    main()
