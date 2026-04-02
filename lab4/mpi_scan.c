#include <stdio.h>
#include <mpi.h>

// Prefix sum using MPI_Scan (built-in inclusive scan).
// Each process i holds value = rank+1.
// MPI_Scan gives process i the sum of values from processes 0..i (inclusive).

int main(int argc, char** argv) {
    int rank, size;
    int value, result;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    value = rank + 1;

    MPI_Scan(&value, &result, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    printf("Process %d: value=%d  prefix_sum=%d\n", rank, value, result);

    MPI_Finalize();
    return 0;
}
