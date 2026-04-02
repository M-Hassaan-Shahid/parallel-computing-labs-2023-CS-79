#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

// Ring allgather: each process rotates data around the ring size-1 times.
// In each step, every process simultaneously sends to rank+1 and receives from rank-1.
// After size-1 steps every process holds all data in rank order.
void my_allgather(int *sendbuf, int n, int *recvbuf, int rank, int size) {
    // Place own data at the correct slot
    for (int i = 0; i < n; i++)
        recvbuf[rank * n + i] = sendbuf[i];

    int next = (rank + 1) % size;
    int prev = (rank - 1 + size) % size;

    for (int step = 0; step < size - 1; step++) {
        int send_slot = (rank - step + size) % size;
        int recv_slot = (rank - step - 1 + size) % size;

        // MPI_Sendrecv avoids deadlock by sending and receiving simultaneously
        MPI_Sendrecv(recvbuf + send_slot * n, n, MPI_INT, next, 0,
                     recvbuf + recv_slot * n, n, MPI_INT, prev, 0,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
}

int main(int argc, char** argv) {
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int n = 1;  // each process contributes 1 integer
    int sendbuf[1];
    sendbuf[0] = rank + 1;

    int *recvbuf  = (int*) malloc(size * n * sizeof(int));
    int *expected = (int*) malloc(size * n * sizeof(int));

    my_allgather(sendbuf, n, recvbuf, rank, size);

    printf("Process %d received: ", rank);
    for (int i = 0; i < size; i++) printf("%d ", recvbuf[i]);
    printf("\n");

    // Verify against built-in MPI_Allgather
    MPI_Allgather(sendbuf, n, MPI_INT, expected, n, MPI_INT, MPI_COMM_WORLD);

    int ok = 1;
    for (int i = 0; i < size * n; i++)
        if (recvbuf[i] != expected[i]) { ok = 0; break; }

    if (rank == 0)
        printf("Verification vs MPI_Allgather: %s\n", ok ? "PASSED" : "FAILED");

    free(recvbuf);
    free(expected);
    MPI_Finalize();
    return 0;
}
