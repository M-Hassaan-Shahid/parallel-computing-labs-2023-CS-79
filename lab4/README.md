# Lab 4: The Art of Communication – Deep Dive into MPI

## Overview

This lab reinforces point-to-point and collective MPI communication through code analysis, performance measurement, custom collective design, and parallel prefix sums.

## Files

### Part 1: Code Analysis
- `mysterious_exchange.c` – 2-process value swap using tagged sends/receives
- `collective_confusion.c` – Two sequential broadcasts with different roots
- `mixed_mode.c` – Broadcast followed by point-to-point exchange

### Part 2: Performance Measurement
- `broadcast_scaling.c` – Compares `MPI_Bcast` vs manual linear broadcast for message sizes 1, 1000, 10000 ints
- `reduction_tree.c` – Binary tree reduction vs `MPI_Reduce` (timed over 1000 iterations)
- `scatter_overhead.c` – `MPI_Scatter` vs manual loop of sends for N = 1000, 10000, 100000

### Part 3: Custom Collective
- `my_allgather.c` – `MPI_Allgather` implemented via ring algorithm; verified against built-in

### Part 4: Prefix Sum (Scan)
- `prefix_scan_linear.c` – Linear pipeline scan using point-to-point; initial values = rank+1
- `mpi_scan.c` – Same prefix sum using `MPI_Scan`

### Part 5: Debugging
- `broken_broadcast.c` – Bug: root sends sequentially (O(P)); correct but inefficient
- `faulty_reduce.c` – Bug: always sums regardless of `op` parameter; fixed to respect `MPI_SUM`/`MPI_MAX`/`MPI_MIN`
- `confused_gather.c` – Bug: root tries `MPI_Recv` from itself causing deadlock; fixed with `memcpy` for root's own data

### Part 6: Open-Ended Design
- `global_prefix_sum.c` – Distributed global prefix sum using local prefix + `MPI_Scan` offset

## Compilation and Execution

```bash
# Part 1
mpicc -o mysterious_exchange mysterious_exchange.c && mpirun -np 2 ./mysterious_exchange
mpicc -o collective_confusion collective_confusion.c && mpirun -np 2 ./collective_confusion
mpicc -o mixed_mode mixed_mode.c && mpirun -np 2 ./mixed_mode

# Part 2
mpicc -O2 -o broadcast_scaling broadcast_scaling.c && mpirun -np 4 ./broadcast_scaling
mpicc -O2 -o reduction_tree reduction_tree.c && mpirun -np 4 ./reduction_tree
mpicc -O2 -o scatter_overhead scatter_overhead.c && mpirun -np 4 ./scatter_overhead

# Part 3
mpicc -O2 -o my_allgather my_allgather.c && mpirun -np 4 ./my_allgather

# Part 4
mpicc -o prefix_scan_linear prefix_scan_linear.c && mpirun -np 4 ./prefix_scan_linear
mpicc -o mpi_scan mpi_scan.c && mpirun -np 4 ./mpi_scan

# Part 5
mpicc -o broken_broadcast broken_broadcast.c && mpirun -np 4 ./broken_broadcast
mpicc -o faulty_reduce faulty_reduce.c && mpirun -np 4 ./faulty_reduce
mpicc -o confused_gather confused_gather.c && mpirun -np 4 ./confused_gather

# Part 6
mpicc -O2 -o global_prefix_sum global_prefix_sum.c && mpirun -np 2 ./global_prefix_sum
mpirun -np 4 ./global_prefix_sum
```
