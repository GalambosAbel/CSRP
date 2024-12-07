import os
import re
import numpy as np
import matplotlib.pyplot as plt

log_path = # path to the logs
log_files = os.listdir(log_path)

scores = [[],[]]
for log_file in log_files:
    word = log_file.split("_")[1]
    if word in ["security", "expense", "value", "question", "force", "front", "lot", "model", "name", "side", "state", "title"]: # incomplete logs????
        continue

    order_type = log_file.split("_")[2]

    with open(log_path + "/" + log_file, "r") as f:
        lines = f.readlines()
        for line in lines:
            if re.search(r"\" is: \d+\.\d+", line):
                layer = int(re.search(r"layer_\d+", line).group().split("_")[1])

                if order_type == "raw.txt":
                    scores[0].append((word, layer, float(re.search(r"\d+\.\d+", line).group())))
                else:
                    scores[1].append((word, layer, float(re.search(r"\d+\.\d+", line).group())))

scores[0] = [(scores[0][i][0], scores[0][i][1], scores[0][i][2]) for i in range(1, len(scores[0]), 2)]
scores[1] = [(scores[1][i][0], scores[1][i][1], scores[1][i][2]) for i in range(1, len(scores[1]), 2)]

scores[0].sort()
scores[1].sort()

# print(scores[0])
# print(scores[1])

# for i in range(len(scores[0])):
#     if scores[0][i][0] != scores[1][i][0] or scores[0][i][1] != scores[1][i][1]:
#         print(scores[0][i][0], scores[0][i][1], scores[1][i][0], scores[1][i][1])
#         input()

# print(len(scores[0]))
# print(len(scores[1]))

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

raw_better_below_5 = 0
morans_better_below_5 = 0

avg_diff_raw_better_below_5 = 0
avg_diff_morans_better_below_5 = 0
for i in range(len(scores[0])):
    if scores[0][i][2] < scores[1][i][2] and scores[1][i][2] < 5:
        raw_better_below_5 += 1
        avg_diff_raw_better_below_5 += scores[1][i][2] - scores[0][i][2]
    elif scores[0][i][2] >= scores[1][i][2] and scores[0][i][2] < 5:
        morans_better_below_5 += 1
        avg_diff_morans_better_below_5 += scores[0][i][2] - scores[1][i][2]

avg_diff_raw_better_below_5 /= raw_better_below_5
avg_diff_morans_better_below_5 /= morans_better_below_5

print(f"Raw ordering is better in {raw_better} cases (with an average difference of {avg_diff_raw_better:.2f})")
print(f"Moran's I ordering is better in {morans_better} cases (with an average difference of {avg_diff_morans_better:.2f})")

print(f"Raw ordering is better in {raw_better_below_5} cases (with an average difference of {avg_diff_raw_better_below_5:.2f}) when the score is below 5")
print(f"Moran's I ordering is better in {morans_better_below_5} cases (with an average difference of {avg_diff_morans_better_below_5:.2f}) when the score is below 5")

plt.hist([scores[0][i][2] for i in range(len(scores[0]))], bins=20, alpha=0.5, label="Raw")
plt.xlabel("Score")
plt.ylabel("Frequency")
plt.title("Histogram of scores for Raw orderings")
plt.savefig(# path to save)
plt.clf()
plt.hist([scores[1][i][2] for i in range(len(scores[1]))], bins=20, alpha=0.5, label="Moran's I")
plt.xlabel("Score")
plt.ylabel("Frequency")
plt.title("Histogram of scores for Moran's I orderings")
plt.savefig(# path to save)
plt.clf()

plt.scatter([scores[0][i][2] for i in range(len(scores[0]))], [scores[1][i][2] for i in range(len(scores[1]))], alpha=0.5)
plt.plot([0, 10], [0, 10], color="red")
plt.xlabel("Raw Score")
plt.ylabel("Moran's I Score")
plt.title("Scores comparison")
plt.savefig(# path to save)
plt.clf()

# t-test to see if the morans i average is significantly smaller than the raw average
from scipy.stats import ttest_ind

print("Testing if Moran's I is significantly smaller than raw...")
t_stat, p_val = ttest_ind([scores[0][i][2] for i in range(len(scores[0]))], [scores[1][i][2] for i in range(len(scores[1]))], equal_var=False, alternative="greater")

print(f"P-value: {p_val:.2f}")