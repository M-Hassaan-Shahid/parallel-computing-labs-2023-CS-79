"""
Sequential implementation for counting occurrences of a target value in a large array.
Serves as a baseline for comparing parallel implementations.
"""

import random
import time

# Constants
N = 100_000_000
TARGET = 42

# Generate array with random values
arr = [random.randint(0, 99) for _ in range(N)]

# Sequential counting
start = time.time()
count = sum(1 for x in arr if x == TARGET)
end = time.time()

print(f"Count = {count}")
print(f"Time = {end - start:.2f} seconds")
