#include <stdio.h>
#include <mpi.h>

// Binary tree reduction: processes pair up, higher sends to lower, repeat
// Returns the reduced value on rank 0; other ranks return -1 after sending
int tree_reduce_sum(int local_val, int rank, int size) {
    int result = local_val;

    for (int step = 1; step < size; step *= 2) {
        if (rank % (2 * step) == 0) {
            int partner = rank + step;
            if (partner < size) {
                int temp;
                MPI_Recv(&temp, 1, MPI_INT, partner, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                result += temp;
            }
        } else if ((rank / step) % 2 == 1) {
            int parent = rank - step;
            MPI_Send(&result, 1, MPI_INT, parent, 0, MPI_COMM_WORLD);
            return -1;  // this process is done
        }
    }
    return result;  // only rank 0 reaches here with the final sum
}

int main(int argc, char** argv) {
    int rank, size;
    int value, result;
    double start, end;
    int ITERS = 1000;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    value = rank + 1;

    // Time binary tree reduction
    MPI_Barrier(MPI_COMM_WORLD);
    start = MPI_Wtime();
    for (int it = 0; it < ITERS; it++)
        result = tree_reduce_sum(value, rank, size);
    end = MPI_Wtime();
    if (rank == 0)
        printf("tree_reduce: result=%d avg_time=%.6f s (%d iters)\n",
               result, (end-start)/ITERS, ITERS);

    // Time MPI_Reduce
    MPI_Barrier(MPI_COMM_WORLD);
    start = MPI_Wtime();
    for (int it = 0; it < ITERS; it++)
        MPI_Reduce(&value, &result, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    end = MPI_Wtime();
    if (rank == 0)
        printf("MPI_Reduce:  result=%d avg_time=%.6f s (%d iters)\n",
               result, (end-start)/ITERS, ITERS);

    if (rank == 0)
        printf("Expected sum = %d\n", size * (size + 1) / 2);

    MPI_Finalize();
    return 0;
}
