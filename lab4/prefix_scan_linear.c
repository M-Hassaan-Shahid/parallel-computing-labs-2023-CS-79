#include <stdio.h>
#include <mpi.h>

// Linear pipeline prefix sum (scan).
// Each process i holds initial value = rank+1.
// Process 0 starts the pipeline; each process receives the accumulated sum
// from its left neighbour, adds its own value, prints, then forwards right.
// With 4 processes (values 1,2,3,4): results should be 1, 3, 6, 10.

int main(int argc, char** argv) {
    int rank, size;
    int value, prefix_sum;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    value = rank + 1;  // initial values: 1, 2, 3, ...

    if (rank == 0) {
        prefix_sum = value;
    } else {
        int received;
        MPI_Recv(&received, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        prefix_sum = received + value;
    }

    // Forward accumulated sum to next process
    if (rank < size - 1)
        MPI_Send(&prefix_sum, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);

    printf("Process %d: value=%d  prefix_sum=%d\n", rank, value, prefix_sum);

    MPI_Finalize();
    return 0;
}
