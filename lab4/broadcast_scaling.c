#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

// Manual linear broadcast: root sends to each process one by one (O(P) sequential sends)
void manual_bcast(void *buf, int count, MPI_Datatype dtype, int root, MPI_Comm comm) {
    int rank, size;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);

    if (rank == root) {
        for (int i = 0; i < size; i++) {
            if (i != root)
                MPI_Send(buf, count, dtype, i, 0, comm);
        }
    } else {
        MPI_Recv(buf, count, dtype, root, 0, comm, MPI_STATUS_IGNORE);
    }
}

int main(int argc, char** argv) {
    int rank, size;
    double start, end;
    int msg_sizes[] = {1, 1000, 10000};
    int ITERS = 100;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    for (int s = 0; s < 3; s++) {
        int n = msg_sizes[s];
        int *buf = (int*) malloc(n * sizeof(int));

        if (rank == 0)
            for (int i = 0; i < n; i++) buf[i] = i;

        // Time MPI_Bcast
        MPI_Barrier(MPI_COMM_WORLD);
        start = MPI_Wtime();
        for (int it = 0; it < ITERS; it++)
            MPI_Bcast(buf, n, MPI_INT, 0, MPI_COMM_WORLD);
        end = MPI_Wtime();
        if (rank == 0)
            printf("MPI_Bcast    n=%-6d procs=%d avg_time=%.6f s\n", n, size, (end-start)/ITERS);

        // Time manual bcast
        MPI_Barrier(MPI_COMM_WORLD);
        start = MPI_Wtime();
        for (int it = 0; it < ITERS; it++)
            manual_bcast(buf, n, MPI_INT, 0, MPI_COMM_WORLD);
        end = MPI_Wtime();
        if (rank == 0)
            printf("manual_bcast n=%-6d procs=%d avg_time=%.6f s\n", n, size, (end-start)/ITERS);

        free(buf);
    }

    MPI_Finalize();
    return 0;
}
