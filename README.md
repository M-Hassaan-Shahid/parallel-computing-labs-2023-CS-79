# Parallel and Distributed Computing Lab Portfolio

## Student Information

- **Name:** M. Hassaan Shahid
- **Roll Number:** 2023-CS-79
- **Course:** Parallel and Distributed Computing
- **Instructor:** Waqas Ali

## Lab Overview

| Lab | Topic             | Key Concepts                                      |
|-----|-------------------|---------------------------------------------------|
| 0   | Foundations       | Pthreads, GIL, Amdahl's Law                       |
| 1   | MPI Introduction  | SPMD, point-to-point, parallel sum                |
| 2   | Point-to-Point    | MPI_Send, MPI_Recv, tags, wildcards               |
| 3   | Collectives       | Bcast, Reduce, Scatter, Gather                    |

## Repository Structure

```
parallel-computing-labs-2023cs79/
│
├── lab0/
│   ├── count_seq.c          # Sequential C baseline
│   ├── count_pthread.c      # POSIX threads implementation
│   ├── count_seq.py         # Sequential Python baseline
│   ├── count_threads.py     # Python threading (GIL demo)
│   ├── count_mp.py          # Python multiprocessing
│   ├── search_pthread.c     # Pthread search with early termination
│   └── README.md
│
├── lab1/
│   ├── hello_mpi.c          # MPI Hello World
│   ├── pingpong.c           # Ping-pong latency/bandwidth benchmark
│   ├── count_mpi.c          # Parallel counting with MPI_Reduce
│   ├── count_seq.c          # Sequential baseline
│   └── README.md
│
├── lab2/
│   ├── send_recv.c          # Basic MPI_Send / MPI_Recv
│   ├── parallel_sum.c       # Manual parallel sum via point-to-point
│   ├── custom_gather.c      # Ordered manual gather
│   ├── wildcard_gather.c    # Wildcard (MPI_ANY_SOURCE) gather
│   └── README.md
│
├── lab3/
│   ├── bcast.c              # MPI_Bcast demo
│   ├── reduce_sum.c         # MPI_Reduce parallel sum
│   ├── scatter_gather.c     # MPI_Scatter + MPI_Gather
│   ├── matvec.c             # Parallel matrix-vector multiplication
│   ├── scatterv_gatherv.c   # Uneven distribution with Scatterv/Gatherv
│   └── README.md
│
└── README.md                # This file
```

## How to Use This Repository

Each lab folder contains its own `README.md` with:
- An overview of what the lab covers
- A description of every source file
- Compilation and execution instructions
- Example output
- Challenges encountered and how they were resolved

### Prerequisites

```bash
# Ubuntu / Debian
sudo apt-get install build-essential openmpi-bin libopenmpi-dev python3

# Verify
gcc --version
mpicc --version
python3 --version
```

### Quick Compile (any lab)

```bash
# C programs (replace with actual filename)
gcc -O2 -pthread -o <binary> <file>.c        # pthreads
mpicc -O2 -o <binary> <file>.c               # MPI

# Python programs
python3 <file>.py
```
