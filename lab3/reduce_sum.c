#include <stdio.h>
#include <mpi.h>

int main(int argc, char** argv) {
    int rank, size;
    int local_sum = 0, global_sum = 0;
    int startval, endval;
    double start_time, end_time;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Determine range for each process (same as Lab 2)
    startval = 1000 * rank / size + 1;
    endval   = 1000 * (rank + 1) / size;

    // Synchronise before timing
    MPI_Barrier(MPI_COMM_WORLD);
    start_time = MPI_Wtime();

    // Compute local sum
    for (int i = startval; i <= endval; i++) {
        local_sum += i;
    }

    // Reduce all local sums to global sum on rank 0
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    end_time = MPI_Wtime();

    if (rank == 0) {
        printf("The sum from 1 to 1000 is: %d\n", global_sum);
        printf("Time with %d processes: %f seconds\n", size, end_time - start_time);
        int expected = 1000 * 1001 / 2;
        printf("Expected sum: %d\n", expected);
    }

    MPI_Finalize();
    return 0;
}
