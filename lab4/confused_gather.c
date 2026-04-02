#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Part 5.3: Confused Gather
// Bug: root loops i = 0..size-1 calling MPI_Recv for every i including i==root.
// But root never sends to itself, so the receive for i==root hangs (deadlock).
// Fix: when i == root, copy data directly with memcpy instead of receiving.
void my_Gather(int* sendbuf, int sendcount, int* recvbuf, int recvcount, int root, MPI_Comm comm) {
    int rank, size;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);

    if (rank == root) {
        for (int i = 0; i < size; i++) {
            if (i == root) {
                // Fix: copy own data directly (root cannot receive from itself)
                memcpy(recvbuf + i * recvcount, sendbuf, sendcount * sizeof(int));
            } else {
                MPI_Recv(recvbuf + i * recvcount, recvcount, MPI_INT,
                         i, 0, comm, MPI_STATUS_IGNORE);
            }
        }
    } else {
        MPI_Send(sendbuf, sendcount, MPI_INT, root, 0, comm);
    }
}

int main(int argc, char** argv) {
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int send_val = rank + 1;
    int *recvbuf = NULL;

    if (rank == 0)
        recvbuf = (int*) malloc(size * sizeof(int));

    my_Gather(&send_val, 1, recvbuf, 1, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Gathered: ");
        for (int i = 0; i < size; i++) printf("%d ", recvbuf[i]);
        printf("\n");
        free(recvbuf);
    }

    MPI_Finalize();
    return 0;
}
