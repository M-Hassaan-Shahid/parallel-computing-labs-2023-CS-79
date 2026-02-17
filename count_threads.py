"""
Threading implementation for counting occurrences of a target value in a large array.
Uses Python's threading module with lock synchronization for thread-safe counting.
"""

import random
import time
import threading

# Constants
N = 100_000_000
TARGET = 42
NUM_THREADS = 4

# Generate array with random values
arr = [random.randint(0, 99) for _ in range(N)]

# Shared state
global_count = 0
lock = threading.Lock()


def count_chunk(start, end):
    """
    Count occurrences of TARGET in a specific chunk of the array.
    
    Args:
        start: Starting index of the chunk
        end: Ending index of the chunk
    """
    local_count = 0
    for i in range(start, end):
        if arr[i] == TARGET:
            local_count += 1
    
    # Update global count in thread-safe manner
    with lock:
        global global_count
        global_count += local_count


# Create and start threads
threads = []
chunk_size = N // NUM_THREADS

start_time = time.time()

for t in range(NUM_THREADS):
    s = t * chunk_size
    e = N if t == NUM_THREADS - 1 else (t + 1) * chunk_size
    thread = threading.Thread(target=count_chunk, args=(s, e))
    threads.append(thread)
    thread.start()

# Wait for all threads to complete
for t in threads:
    t.join()

end_time = time.time()

print(f"Count = {global_count}")
print(f"Time with {NUM_THREADS} threads = {end_time - start_time:.2f} seconds")
