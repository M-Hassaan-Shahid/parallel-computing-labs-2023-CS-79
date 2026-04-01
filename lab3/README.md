# Lab 3: Collective Communication

## Overview

This lab covers MPI collective communication operations: broadcast (`MPI_Bcast`), reduction (`MPI_Reduce`), scatter (`MPI_Scatter`), and gather (`MPI_Gather`). A matrix-vector multiplication combining Scatter and Gather is also implemented.

## Files

- `bcast.c` – Demonstrates `MPI_Bcast`; process 0 broadcasts an integer to all other processes
- `reduce_sum.c` – Parallel sum of 1…1000 using `MPI_Reduce` with `MPI_SUM`; replaces the manual send/receive approach from Lab 2
- `scatter_gather.c` – Distributes a 1000-element array with `MPI_Scatter`, each process computes a partial sum, then results are aggregated with `MPI_Reduce` and `MPI_Gather`
- `matvec.c` – Parallel matrix-vector multiplication: matrix rows are scattered to processes (each computes its row-dot-products with the broadcast vector), results gathered back to rank 0
- `scatterv_gatherv.c` – Extended version using `MPI_Scatterv`/`MPI_Gatherv` for uneven distributions

## Compilation and Execution

```bash
# Broadcast example
mpicc -o bcast bcast.c
mpirun -np 4 ./bcast

# Reduce sum
mpicc -O2 -o reduce_sum reduce_sum.c
mpirun -np 4 ./reduce_sum

# Scatter/Gather (dataset size must be divisible by number of processes)
mpicc -O2 -o scatter_gather scatter_gather.c
mpirun -np 4 ./scatter_gather

# Matrix-vector multiplication (8x8 matrix, rows must be divisible by np)
mpicc -O2 -o matvec matvec.c
mpirun -np 4 ./matvec

# ScatterV/GatherV (handles uneven distributions)
mpicc -O2 -o scatterv_gatherv scatterv_gatherv.c
mpirun -np 3 ./scatterv_gatherv
```

## Example Output

```
# bcast (4 processes)
Master Process 0 is broadcasting value 1000 to all workers
Worker Process 0 has received shared value = 1000
Worker Process 1 has received shared value = 1000
Worker Process 2 has received shared value = 1000
Worker Process 3 has received shared value = 1000

# reduce_sum (4 processes)
Aggregated sum from 1 to 1000: 500500
Verification (expected sum): 500500
Execution time with 4 processes: 0.00001200 seconds

# matvec (4 processes, 8x8 matrix)
Result vector y (expected: 8, 16, 24, ...):
y[0] =    8.0
y[1] =   16.0
y[2] =   24.0
...
y[7] =   64.0
```

## Challenges Faced

- **Divisibility constraint:** `MPI_Scatter` requires the data size to divide evenly among processes. The programs check for this upfront and exit cleanly if violated; `scatterv_gatherv.c` removes this constraint.
- **Broadcast before Scatter in matvec:** The input vector must be broadcast to all processes before the Scatter so each process can compute dot products locally; forgetting this step produces incorrect results.
- **Collective synchronisation:** All processes must call collective functions in the same order; a branch that causes one process to skip a collective (e.g., early return on error) deadlocks the others.
