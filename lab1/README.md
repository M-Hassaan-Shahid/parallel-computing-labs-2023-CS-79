# Lab 1: MPI Introduction – SPMD and Parallel Sum

## Overview

This lab introduces the Message Passing Interface (MPI) programming model. It covers the Single Program Multiple Data (SPMD) paradigm, point-to-point communication, and a parallel sum that scales with the number of processes.

## Files

- `hello_mpi.c` – Minimal MPI "Hello World"; verifies the MPI environment and prints each process's rank and communicator size
- `pingpong.c` – Classic MPI ping-pong benchmark between process 0 and process 1; measures one-way latency and bandwidth
- `count_mpi.c` – MPI parallel counting of a target value in a 100-million-element distributed array using `MPI_Reduce`
- `count_seq.c` – Sequential baseline used for speedup comparison

## Compilation and Execution

```bash
# Hello MPI
mpicc -o hello_mpi hello_mpi.c
mpirun -np 4 ./hello_mpi

# Ping-Pong (requires exactly 2 processes)
mpicc -o pingpong pingpong.c
mpirun -np 2 ./pingpong

# Parallel counting (adjust -np to change number of processes)
mpicc -O2 -o count_mpi count_mpi.c
mpirun -np 4 ./count_mpi

# Sequential baseline
gcc -O2 -o count_seq count_seq.c
./count_seq
```

## Example Output

```
# hello_mpi (4 processes)
Hello from process 0 of 4
Hello from process 2 of 4
Hello from process 1 of 4
Hello from process 3 of 4

# pingpong
Ping-Pong results (100 iterations, 1000 doubles per message):
  Total time : 0.004231 s
  Latency    : 21.155 us (one-way)
  Bandwidth  : 1895.42 MB/s

# count_mpi (4 processes)
Total count = 999803
Time with 4 processes = 0.312000 seconds
```

## Challenges Faced

- **Process ordering:** MPI processes execute independently; `printf` output can appear in any order. Using `MPI_Barrier` before timing sections ensures fair comparisons.
- **Load imbalance:** When `N` is not evenly divisible by the number of processes, some processes receive one extra element. This was handled with the `remainder` variable in `count_mpi.c`.
- **Ping-pong warm-up:** Without a warm-up round, the first iteration includes MPI connection-setup overhead, skewing latency measurements.
