# Lab 0: Foundations – Sequential vs Parallel Counting

## Overview

This lab explores the fundamentals of parallel computing by implementing a counting task (find occurrences of a target value in a 100-million-element array) using multiple parallelization approaches in C and Python. The goal is to measure and compare execution times, understand Amdahl's Law, and observe the effect of Python's GIL.

## Files

- `count_seq.c` – Sequential C implementation; establishes the baseline execution time
- `count_pthread.c` – Multi-threaded C implementation using POSIX threads (pthreads) with mutex-protected global counter
- `count_seq.py` – Sequential Python implementation; baseline for Python comparisons
- `count_threads.py` – Python threading implementation; demonstrates GIL impact on CPU-bound tasks
- `count_mp.py` – Python multiprocessing implementation; bypasses the GIL using separate processes
- `search_pthread.c` – C pthread search with early-termination optimisation

## Compilation and Execution

### C Programs

```bash
# Sequential
gcc -O2 -o count_seq count_seq.c
./count_seq

# Pthreads (default 4 threads, set NUM_THREADS in source to change)
gcc -O2 -pthread -o count_pthread count_pthread.c
./count_pthread

# Pthread search with early termination
gcc -O2 -pthread -o search_pthread search_pthread.c
./search_pthread
```

### Python Programs

```bash
python3 count_seq.py
python3 count_threads.py
python3 count_mp.py
```

## Example Output

```
# count_seq
Count = 999803
Time = 3.12 seconds

# count_pthread (4 threads)
Count = 999803
Time with 4 threads = 0.16 seconds

# count_mp.py (4 processes)
Count = 999803
Time = 3.41 seconds
```

## Key Results

| Implementation         | Threads/Procs | Time (s) | Speedup |
|------------------------|---------------|----------|---------|
| C Sequential           | 1             | 3.12     | 1.00×   |
| C Pthreads             | 4             | 0.16     | 19.50×  |
| Python Sequential      | 1             | 71.49    | 1.00×   |
| Python Threading       | 4             | 7.12     | 10.04×  |
| Python Multiprocessing | 4             | 3.41     | 20.96×  |

## Challenges Faced

- **Super-linear speedup in C:** The 19.50× speedup with 4 threads was unexpected. Investigation revealed that the data working-set fit in L3 cache when split across threads, producing cache-related super-linear behaviour.
- **GIL in Python threading:** Despite using 4 threads, Python threading cannot truly parallelise CPU-bound work due to the Global Interpreter Lock; only `multiprocessing` achieves true parallelism.
- **Mutex overhead:** Early versions with fine-grained locking were slower; switching to a local counter per thread with a single lock at the end significantly reduced contention.
