#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

// Part 6: Global Prefix Sum on distributed array.
// N integers split evenly among P processes (each has N/P elements).
// Algorithm:
//   1. Each process computes its local prefix sums.
//   2. MPI_Scan on local totals gives inclusive cumulative sum up to this process.
//   3. offset = scan_result - local_total (exclusive prefix: sum of all previous processes).
//   4. Each local prefix value += offset to get global prefix sums.

#define N 8  // total array size (must be divisible by number of processes)

int main(int argc, char** argv) {
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (N % size != 0) {
        if (rank == 0) printf("N must be divisible by number of processes.\n");
        MPI_Finalize();
        return 1;
    }

    int local_n = N / size;
    int *local  = (int*) malloc(local_n * sizeof(int));
    int *prefix = (int*) malloc(local_n * sizeof(int));

    // Initialise: process i holds values [i*local_n+1 .. (i+1)*local_n]
    for (int i = 0; i < local_n; i++)
        local[i] = rank * local_n + i + 1;

    printf("Process %d initial values: ", rank);
    for (int i = 0; i < local_n; i++) printf("%d ", local[i]);
    printf("\n");

    // Step 1: local prefix sums
    prefix[0] = local[0];
    for (int i = 1; i < local_n; i++)
        prefix[i] = prefix[i-1] + local[i];

    // Step 2: inclusive scan of local totals
    int local_total = prefix[local_n - 1];
    int scan_result;
    MPI_Scan(&local_total, &scan_result, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    // Step 3: exclusive offset = sum of all elements in processes 0..rank-1
    int offset = scan_result - local_total;

    // Step 4: shift local prefix sums by offset
    for (int i = 0; i < local_n; i++)
        prefix[i] += offset;

    printf("Process %d prefix sums:  ", rank);
    for (int i = 0; i < local_n; i++) printf("%d ", prefix[i]);
    printf("\n");

    free(local);
    free(prefix);
    MPI_Finalize();
    return 0;
}
