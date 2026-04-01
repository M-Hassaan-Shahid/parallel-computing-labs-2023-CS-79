#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define DATASET_SIZE 1000

int main(int argc, char** argv) {
    int proc_rank, proc_size;
    int *dataset = NULL;
    int *chunk_data;
    int *distribution_counts = NULL;
    int *distribution_offsets = NULL;
    int *gather_counts = NULL;
    int *gather_offsets = NULL;
    int *collected_results = NULL;
    int chunk_size;
    int chunk_sum = 0, total_result = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &proc_size);

    // Master process prepares distribution parameters
    if (proc_rank == 0) {
        dataset = (int*) malloc(DATASET_SIZE * sizeof(int));
        for (int i = 0; i < DATASET_SIZE; i++)
            dataset[i] = i + 1;

        distribution_counts = (int*) malloc(proc_size * sizeof(int));
        distribution_offsets = (int*) malloc(proc_size * sizeof(int));

        int offset_tracker = 0;
        for (int i = 0; i < proc_size; i++) {
            distribution_counts[i] = DATASET_SIZE / proc_size;
            if (i < DATASET_SIZE % proc_size) distribution_counts[i]++;
            distribution_offsets[i] = offset_tracker;
            offset_tracker += distribution_counts[i];
        }

        printf("Distribution counts: ");
        for (int i = 0; i < proc_size; i++) printf("%d ", distribution_counts[i]);
        printf("\n");
    }

    // Broadcast chunk size to all processes
    MPI_Scatter(distribution_counts, 1, MPI_INT,
                &chunk_size, 1, MPI_INT,
                0, MPI_COMM_WORLD);

    chunk_data = (int*) malloc(chunk_size * sizeof(int));

    // Distribute dataset using variable-length scatter
    MPI_Scatterv(dataset, distribution_counts, distribution_offsets, MPI_INT,
                 chunk_data, chunk_size, MPI_INT,
                 0, MPI_COMM_WORLD);

    // Each process computes sum of its chunk
    for (int i = 0; i < chunk_size; i++)
        chunk_sum += chunk_data[i];

    printf("Process %d: chunk_size = %d, chunk sum = %d\n", proc_rank, chunk_size, chunk_sum);

    // Aggregate all chunk sums
    MPI_Reduce(&chunk_sum, &total_result, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    // Collect individual sums using variable-length gather
    if (proc_rank == 0) {
        collected_results = (int*) malloc(proc_size * sizeof(int));
        gather_counts = (int*) malloc(proc_size * sizeof(int));
        gather_offsets = (int*) malloc(proc_size * sizeof(int));
        for (int i = 0; i < proc_size; i++) {
            gather_counts[i] = 1;
            gather_offsets[i] = i;
        }
    }

    MPI_Gatherv(&chunk_sum, 1, MPI_INT,
                collected_results, gather_counts, gather_offsets, MPI_INT,
                0, MPI_COMM_WORLD);

    // Master process displays results
    if (proc_rank == 0) {
        printf("Total sum = %d\n", total_result);
        printf("Expected  = %d\n", DATASET_SIZE * (DATASET_SIZE + 1) / 2);
        printf("All chunk sums via Gatherv: ");
        for (int i = 0; i < proc_size; i++) printf("%d ", collected_results[i]);
        printf("\n");
        free(dataset);
        free(distribution_counts);
        free(distribution_offsets);
        free(collected_results);
        free(gather_counts);
        free(gather_offsets);
    }

    free(chunk_data);
    MPI_Finalize();
    return 0;
}
