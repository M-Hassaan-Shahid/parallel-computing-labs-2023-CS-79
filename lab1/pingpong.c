#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define NUM_ITERS  100
#define MSG_SIZE   1000

int main(int argc, char **argv) {
    int rank;
    double start, end;
    double *buffer;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    buffer = (double *) malloc(MSG_SIZE * sizeof(double));
    if (!buffer) {
        fprintf(stderr, "malloc failed\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    /* Warm-up */
    if (rank == 0) {
        MPI_Send(buffer, MSG_SIZE, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD);
        MPI_Recv(buffer, MSG_SIZE, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    } else if (rank == 1) {
        MPI_Recv(buffer, MSG_SIZE, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Send(buffer, MSG_SIZE, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    start = MPI_Wtime();

    for (int i = 0; i < NUM_ITERS; i++) {
        if (rank == 0) {
            MPI_Send(buffer, MSG_SIZE, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD);
            MPI_Recv(buffer, MSG_SIZE, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        } else if (rank == 1) {
            MPI_Recv(buffer, MSG_SIZE, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(buffer, MSG_SIZE, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
        }
    }

    end = MPI_Wtime();

    if (rank == 0) {
        double total_bytes = 2.0 * NUM_ITERS * MSG_SIZE * sizeof(double);
        double elapsed     = end - start;
        double latency_us  = (elapsed / (2.0 * NUM_ITERS)) * 1e6;
        double bandwidth   = total_bytes / elapsed / 1e6;  /* MB/s */

        printf("Ping-Pong results (%d iterations, %d doubles per message):\n",
               NUM_ITERS, MSG_SIZE);
        printf("  Total time : %.6f s\n", elapsed);
        printf("  Latency    : %.3f us (one-way)\n", latency_us);
        printf("  Bandwidth  : %.2f MB/s\n", bandwidth);
    }

    free(buffer);
    MPI_Finalize();
    return 0;
}
