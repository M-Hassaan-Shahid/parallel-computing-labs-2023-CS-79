#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char** argv) {
    int rank, size;
    double start, end;
    int sizes[] = {1000, 10000, 100000};
    int ITERS = 50;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    for (int s = 0; s < 3; s++) {
        int N = sizes[s];
        int local_n = N / size;
        int *array = NULL;
        int *local = (int*) malloc(local_n * sizeof(int));

        if (rank == 0) {
            array = (int*) malloc(N * sizeof(int));
            for (int i = 0; i < N; i++) array[i] = i + 1;
        }

        // Time MPI_Scatter
        MPI_Barrier(MPI_COMM_WORLD);
        start = MPI_Wtime();
        for (int it = 0; it < ITERS; it++)
            MPI_Scatter(array, local_n, MPI_INT, local, local_n, MPI_INT, 0, MPI_COMM_WORLD);
        end = MPI_Wtime();
        if (rank == 0)
            printf("MPI_Scatter    N=%-7d procs=%d avg_time=%.6f s\n", N, size, (end-start)/ITERS);

        // Time manual sends
        MPI_Barrier(MPI_COMM_WORLD);
        start = MPI_Wtime();
        for (int it = 0; it < ITERS; it++) {
            if (rank == 0) {
                for (int i = 0; i < local_n; i++) local[i] = array[i];
                for (int p = 1; p < size; p++)
                    MPI_Send(array + p * local_n, local_n, MPI_INT, p, 0, MPI_COMM_WORLD);
            } else {
                MPI_Recv(local, local_n, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
        }
        end = MPI_Wtime();
        if (rank == 0)
            printf("manual_scatter N=%-7d procs=%d avg_time=%.6f s\n", N, size, (end-start)/ITERS);

        if (rank == 0) free(array);
        free(local);
    }

    MPI_Finalize();
    return 0;
}
