#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define N 1000  // total array size

int main(int argc, char** argv) {
    int rank, size;
    int *array = NULL;
    int *local_array;
    int local_n;
    int local_sum = 0, total_sum = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Determine size of each chunk (assume N divisible by size for simplicity)
    local_n = N / size;
    local_array = (int*) malloc(local_n * sizeof(int));

    if (rank == 0) {
        // Root initialises the full array
        array = (int*) malloc(N * sizeof(int));
        for (int i = 0; i < N; i++)
            array[i] = i + 1;  // 1,2,3,...,N
    }

    // Scatter the array from root to all processes
    MPI_Scatter(array, local_n, MPI_INT, local_array, local_n, MPI_INT,
                0, MPI_COMM_WORLD);

    // Compute local sum
    for (int i = 0; i < local_n; i++)
        local_sum += local_array[i];

    printf("Process %d: local sum = %d\n", rank, local_sum);

    // Option 1: Use MPI_Reduce to get total sum
    MPI_Reduce(&local_sum, &total_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    // Option 2: Use MPI_Gather to collect all local sums (just for practice)
    int *all_sums = NULL;
    if (rank == 0)
        all_sums = (int*) malloc(size * sizeof(int));

    MPI_Gather(&local_sum, 1, MPI_INT, all_sums, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Total sum via MPI_Reduce: %d\n", total_sum);
        printf("Collected sums: ");
        for (int i = 0; i < size; i++)
            printf("%d ", all_sums[i]);
        printf("\n");
        printf("Expected total sum = %d\n", N * (N + 1) / 2);

        free(array);
        free(all_sums);
    }

    free(local_array);
    MPI_Finalize();
    return 0;
}
