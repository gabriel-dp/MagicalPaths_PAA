from random import randint as random

# Program constants
N = 100000
M = N * 2
K = 15
OUTPUT_PATH = "./tests/input.random"

# Edges are stored in a set to avoid duplication
edges = set()

# Guarantee a minimum path from 1 to N
for i in range(1, N):
    edges.add((i, i+1, random(1, 10)))

# Generate M random edges
while len(edges) < M:
    u = random(1, N)
    v = random(1, N)
    c = random(1, 10)
    edges.add((u, v, c))

# Write in output file
with open(OUTPUT_PATH, "w") as f:
    f.write(f"{N} {M} {K}\n")
    for u, v, c in edges:
        f.write(f"{u} {v} {c}\n")