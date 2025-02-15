import os
import re
import numpy as np
import matplotlib.pyplot as plt

log_path1 = # Path to logs for signature distance ordered matrices (both raw and Moran's I)
log_path2 = # Path to logs for cosine distance ordered matrices (both raw and Moran's I)
log_files1 = os.listdir(log_path1)
log_files2 = os.listdir(log_path2)

# scores[0] = raw signature
# scores[1] = moran's signature
# scores[2] = raw cosine
# scores[3] = moran's cosine
scores = [[],[],[],[]]
for log_file in log_files1:
    # Skip if we don't have logs for both signature and cosine for the same word
    if log_file not in log_files2:
        continue

    word = log_file.split("_")[1]

    order_type = log_file.split("_")[2]

    with open(log_path1 + "/" + log_file, "r") as f1:
        with open(log_path2 + "/" + log_file, "r") as f2:
            lines1 = f1.readlines()
            started_layer = [False for _ in range(13)]
            for line in lines1:
                if re.search(r"\" is: \d+\.\d+", line):
                    layer = int(re.search(r"layer_\d+", line).group().split("_")[1])

                    if not started_layer[layer]:
                        started_layer[layer] = True
                    else:
                        if order_type == "raw.txt":
                            scores[0].append((word, layer, float(re.search(r"\d+\.\d+", line).group())))
                        else:
                            scores[1].append((word, layer, float(re.search(r"\d+\.\d+", line).group())))

            lines2 = f2.readlines()
            started_layer = [False for _ in range(13)]
            for line in lines2:
                if re.search(r"\" is: \d+\.\d+", line):
                    layer = int(re.search(r"layer_\d+", line).group().split("_")[1])

                    if not started_layer[layer]:
                        started_layer[layer] = True
                    else:
                        if order_type == "raw.txt":
                            scores[2].append((word, layer, float(re.search(r"\d+\.\d+", line).group())))
                        else:
                            scores[3].append((word, layer, float(re.search(r"\d+\.\d+", line).group())))

def has_match(score, other_scores):
    for other_score in other_scores:
        if score[0] == other_score[0] and score[1] == other_score[1]:
            return True
    return False

# cleaning up incomplete logs
# signature scores
scores[0] = [score for score in scores[0] if has_match(score, scores[1]) and has_match(score, scores[2]) and has_match(score, scores[3])]
scores[1] = [score for score in scores[1] if has_match(score, scores[0]) and has_match(score, scores[2]) and has_match(score, scores[3])]
# cos scores
scores[2] = [score for score in scores[2] if has_match(score, scores[0]) and has_match(score, scores[1]) and has_match(score, scores[3])]
scores[3] = [score for score in scores[3] if has_match(score, scores[0]) and has_match(score, scores[1]) and has_match(score, scores[2])]

scores[0].sort()
scores[1].sort()
scores[2].sort()
scores[3].sort()

# SIGNATURE --------------------------------------------------------------------------------------------

raw_better = 0
morans_better = 0

avg_diff_raw_better = 0
avg_diff_morans_better = 0

for i in range(len(scores[0])):
    if scores[0][i][2] < scores[1][i][2]:
        raw_better += 1
        avg_diff_raw_better += scores[1][i][2] - scores[0][i][2]
    else:
        morans_better += 1
        avg_diff_morans_better += scores[0][i][2] - scores[1][i][2]

avg_diff_raw_better /= raw_better
avg_diff_morans_better /= morans_better

print(f"Raw ordering (signature) is better in {raw_better} cases (with an average difference of {avg_diff_raw_better:.2f})")
print(f"Moran's I ordering (signature) is better in {morans_better} cases (with an average difference of {avg_diff_morans_better:.2f})")

plt.hist([scores[0][i][2] for i in range(len(scores[0]))], bins=20, alpha=0.5, label="Raw")
plt.xlabel("Score")
plt.ylabel("Frequency")
plt.title("Histogram of scores for Raw orderings (signature)")
plt.savefig(# Path to save the histogram)
plt.clf()

plt.hist([scores[1][i][2] for i in range(len(scores[1]))], bins=20, alpha=0.5, label="Moran's I")
plt.xlabel("Score")
plt.ylabel("Frequency")
plt.title("Histogram of scores for Moran's I orderings (signature)")
plt.savefig(# Path to save the histogram)
plt.clf()

plt.scatter([scores[0][i][2] for i in range(len(scores[0]))], [scores[1][i][2] for i in range(len(scores[1]))], alpha=0.5, s=5)
plt.plot([0, 10], [0, 10], color="red")
plt.xlabel("Raw Score")
plt.ylabel("Moran's I Score")
plt.title("Scores comparison (signature)")
plt.savefig(# Path to save the scatter plot)
plt.clf()


# COSINE --------------------------------------------------------------------------------------------

raw_better = 0
morans_better = 0

avg_diff_raw_better = 0
avg_diff_morans_better = 0

for i in range(len(scores[2])):
    if scores[2][i][2] < scores[3][i][2]:
        raw_better += 1
        avg_diff_raw_better += scores[3][i][2] - scores[2][i][2]
    else:
        morans_better += 1
        avg_diff_morans_better += scores[2][i][2] - scores[3][i][2]

avg_diff_raw_better /= raw_better
avg_diff_morans_better /= morans_better

print(f"Raw ordering (cosine) is better in {raw_better} cases (with an average difference of {avg_diff_raw_better:.2f})")
print(f"Moran's I ordering (cosine) is better in {morans_better} cases (with an average difference of {avg_diff_morans_better:.2f})")

plt.hist([scores[2][i][2] for i in range(len(scores[2]))], bins=20, alpha=0.5, label="Raw")
plt.xlabel("Score")
plt.ylabel("Frequency")
plt.title("Histogram of scores for Raw orderings (cosine)")
plt.savefig(# Path to save the histogram)
plt.clf()

plt.hist([scores[3][i][2] for i in range(len(scores[3]))], bins=20, alpha=0.5, label="Moran's I")
plt.xlabel("Score")
plt.ylabel("Frequency")
plt.title("Histogram of scores for Moran's I orderings (cosine)")
plt.savefig(# Path to save the histogram)
plt.clf()

plt.scatter([scores[2][i][2] for i in range(len(scores[2]))], [scores[3][i][2] for i in range(len(scores[3]))], alpha=0.5, s=5)
plt.plot([0, 10], [0, 10], color="red")
plt.xlabel("Raw Score")
plt.ylabel("Moran's I Score")
plt.title("Scores comparison (cosine)")
plt.savefig(# Path to save the scatter plot)
plt.clf()


# average raw score signature
avg_raw = sum([scores[0][i][2] for i in range(len(scores[0]))]) / len(scores[0])

# average morans score signature
avg_morans = sum([scores[1][i][2] for i in range(len(scores[1]))]) / len(scores[1])

print(f"Average raw score signature: {avg_raw:.2f}")
print(f"Average Moran's I score signature: {avg_morans:.2f}")

# average raw score cos
avg_raw = sum([scores[2][i][2] for i in range(len(scores[2]))]) / len(scores[2])

# average morans score cos
avg_morans = sum([scores[3][i][2] for i in range(len(scores[3]))]) / len(scores[3])

print(f"Average raw score cosine: {avg_raw:.2f}")
print(f"Average Moran's I score cosine: {avg_morans:.2f}")

# per layer averages signature
layer_avgs_raw = [0 for _ in range(13)]
layer_counts_raw = [0 for _ in range(13)]
layer_avgs_morans = [0 for _ in range(13)]
layer_counts_morans = [0 for _ in range(13)]

for score in scores[0]:
    layer_avgs_raw[score[1]] += score[2]
    layer_counts_raw[score[1]] += 1

for score in scores[1]:
    layer_avgs_morans[score[1]] += score[2]
    layer_counts_morans[score[1]] += 1

for i in range(13):
    layer_avgs_raw[i] /= layer_counts_raw[i]
    layer_avgs_morans[i] /= layer_counts_morans[i]

plt.plot(range(13), layer_avgs_raw, label="Layer averages raw (signature)")
plt.plot(range(13), layer_avgs_morans, label="Layer averages Moran's I (signature)")

layer_avgs_diff_raw = [layer_avgs_raw[i] for i in range(13)]

# per layer averages cosine
layer_avgs_raw = [0 for _ in range(13)]
layer_counts_raw = [0 for _ in range(13)]
layer_avgs_morans = [0 for _ in range(13)]
layer_counts_morans = [0 for _ in range(13)]

for score in scores[2]:
    layer_avgs_raw[score[1]] += score[2]
    layer_counts_raw[score[1]] += 1

for score in scores[3]:
    layer_avgs_morans[score[1]] += score[2]
    layer_counts_morans[score[1]] += 1

for i in range(13):
    layer_avgs_raw[i] /= layer_counts_raw[i]
    layer_avgs_morans[i] /= layer_counts_morans[i]

plt.plot(range(13), layer_avgs_raw, label="Layer averages raw (cosine)")
plt.plot(range(13), layer_avgs_morans, label="Layer averages Moran's I (cosine)")
plt.xlabel("Layer")
plt.ylabel("Average score")
plt.title("Layer averages")
plt.legend()
plt.savefig(# Path to save the plot)
plt.clf()


# layer averages differences between signature and cosine
layer_avgs_diff_raw = [layer_avgs_diff_raw[i] - layer_avgs_raw[i] for i in range(13)]

for i in range(13):
    print(f"Layer {i}: {layer_avgs_diff_raw[i]:.2f} (signature - cosine)")

# count how many times cosine is better than signature
cos_better = 0
for i in range(len(scores[2])):
    if scores[2][i][2] < scores[0][i][2]:
        cos_better += 1

print(f"Cosine is better than signature in {cos_better}/{len(scores[2])} cases")

# t-test to see if the cosine average is significantly smaller than the signature average
from scipy.stats import ttest_ind

print("Testing if cosine metric is significantly better than signature metric...")
t_stat, p_val = ttest_ind([scores[2][i][2] for i in range(len(scores[2]))], [scores[0][i][2] for i in range(len(scores[0]))], equal_var=False, alternative="less")

print(f"P-value: {p_val:.2f}")

print("Testing if moran's I on cosine is significantly better than raw ordering...")
t_stat, p_val = ttest_ind([scores[3][i][2] for i in range(len(scores[3]))], [scores[2][i][2] for i in range(len(scores[2]))], equal_var=False, alternative="less")

print(f"P-value: {p_val:.2f}")