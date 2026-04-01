# Lab 2: Point-to-Point Communication

## Overview

This lab focuses on MPI point-to-point communication primitives: `MPI_Send` and `MPI_Recv`. It explores message tags, wildcard receives (`MPI_ANY_SOURCE`, `MPI_ANY_TAG`), and a manual parallel sum that uses explicit send/receive instead of collective operations.

## Files

- `send_recv.c` – Demonstrates basic `MPI_Send`/`MPI_Recv` between a fixed sender (rank 2) and receiver (rank 4); requires at least 5 processes
- `parallel_sum.c` – Computes the sum 1…1000 across all processes using point-to-point communication; workers send partial sums to rank 0
- `custom_gather.c` – Manual gather pattern: each worker sends an array to process 0 using ordered receives (deterministic)
- `wildcard_gather.c` – Same gather pattern but using `MPI_ANY_SOURCE` and `MPI_ANY_TAG` (non-deterministic order)

## Compilation and Execution

```bash
# send_recv (needs >= 5 processes)
mpicc -o send_recv send_recv.c
mpirun -np 5 ./send_recv

# parallel_sum
mpicc -O2 -o parallel_sum parallel_sum.c
mpirun -np 4 ./parallel_sum

# custom_gather (ordered)
mpicc -o custom_gather custom_gather.c
mpirun -np 4 ./custom_gather

# wildcard_gather (non-deterministic order)
mpicc -o wildcard_gather wildcard_gather.c
mpirun -np 4 ./wildcard_gather
```

## Example Output

```
# send_recv (5 processes)
Process 2 sent data to 4
Process 4 received data from 2
0.000000 1.000000 2.000000 ... 9.000000

# parallel_sum (4 processes)
The sum from 1 to 1000 is: 500500
Execution time with 4 processes: 0.000012 seconds

# wildcard_gather (order varies)
Process 0 is receiving arrays using MPI_ANY_SOURCE:
Process 0 received from 3 (tag=3): 300 301 302 ...
Process 0 received from 1 (tag=1): 100 101 102 ...
Process 0 received from 2 (tag=2): 200 201 202 ...
```

## Challenges Faced

- **Tag matching:** Using unique tags equal to the sender's rank allowed the master to identify the source of each message unambiguously.
- **Wildcard vs ordered receives:** `MPI_ANY_SOURCE` makes the program simpler but the arrival order is non-deterministic; ordered receives guarantee deterministic output but can introduce head-of-line blocking.
- **Minimum process count for send_recv:** The program hard-codes sender=2 and receiver=4, so running with fewer than 5 processes causes it to hang; this was documented explicitly.
