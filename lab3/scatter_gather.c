#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define DATASET_SIZE 1000

int main(int argc, char** argv) {
    int proc_rank, proc_size;
    int *dataset = NULL;
    int *chunk_data;
    int chunk_size;
    int chunk_sum = 0, total_result = 0;
    double timer_start, timer_end;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &proc_size);

    // Verify even distribution is possible
    if (DATASET_SIZE % proc_size != 0) {
        if (proc_rank == 0)
            printf("Dataset size %d is not divisible by %d processes. Use scatterv instead.\n", DATASET_SIZE, proc_size);
        MPI_Finalize();
        return 1;
    }

    chunk_size = DATASET_SIZE / proc_size;
    chunk_data = (int*) malloc(chunk_size * sizeof(int));

    // Master process initializes the complete dataset
    if (proc_rank == 0) {
        dataset = (int*) malloc(DATASET_SIZE * sizeof(int));
        for (int i = 0; i < DATASET_SIZE; i++)
            dataset[i] = i + 1;
    }

    MPI_Barrier(MPI_COMM_WORLD);
    timer_start = MPI_Wtime();

    // Distribute dataset chunks to all processes
    MPI_Scatter(dataset, chunk_size, MPI_INT,
                chunk_data, chunk_size, MPI_INT,
                0, MPI_COMM_WORLD);

    // Each process computes sum of its chunk
    for (int i = 0; i < chunk_size; i++)
        chunk_sum += chunk_data[i];

    printf("Process %d: chunk sum = %d\n", proc_rank, chunk_sum);

    // Aggregate all chunk sums
    MPI_Reduce(&chunk_sum, &total_result, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    // Collect individual sums for verification
    int *collected_sums = NULL;
    if (proc_rank == 0)
        collected_sums = (int*) malloc(proc_size * sizeof(int));

    MPI_Gather(&chunk_sum, 1, MPI_INT,
               collected_sums, 1, MPI_INT,
               0, MPI_COMM_WORLD);

    timer_end = MPI_Wtime();

    // Master process displays results
    if (proc_rank == 0) {
        printf("Total sum via MPI_Reduce: %d\n", total_result);
        printf("Individual chunk sums via MPI_Gather: ");
        for (int i = 0; i < proc_size; i++)
            printf("%d ", collected_sums[i]);
        printf("\n");
        printf("Expected total sum = %d\n", DATASET_SIZE * (DATASET_SIZE + 1) / 2);
        printf("Execution time with %d processes: %.8f seconds\n", proc_size, timer_end - timer_start);
        free(dataset);
        free(collected_sums);
    }

    free(chunk_data);
    MPI_Finalize();
    return 0;
}
