#include <stdio.h>
#include <mpi.h>

int main(int argc, char** argv) {
    int proc_rank, proc_size;
    int partial_sum = 0, aggregated_sum = 0;
    int range_start, range_end;
    double timer_start, timer_end;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &proc_size);

    // Compute the range of numbers for this process
    range_start = 1000 * proc_rank / proc_size + 1;
    range_end   = 1000 * (proc_rank + 1) / proc_size;

    // Synchronize all processes before timing
    MPI_Barrier(MPI_COMM_WORLD);
    timer_start = MPI_Wtime();

    // Calculate partial sum for this process's range
    for (int idx = range_start; idx <= range_end; idx++) {
        partial_sum += idx;
    }

    // Aggregate all partial sums using reduction
    MPI_Reduce(&partial_sum, &aggregated_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    timer_end = MPI_Wtime();

    // Master process displays results
    if (proc_rank == 0) {
        printf("Aggregated sum from 1 to 1000: %d\n", aggregated_sum);
        printf("Execution time with %d processes: %.8f seconds\n", proc_size, timer_end - timer_start);
        int verification = 1000 * 1001 / 2;
        printf("Verification (expected sum): %d\n", verification);
    }

    MPI_Finalize();
    return 0;
}
