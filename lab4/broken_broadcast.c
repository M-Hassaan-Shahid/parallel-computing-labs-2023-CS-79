#include <mpi.h>
#include <stdio.h>

// Part 5.1: Broken Broadcast
// Bug: Root sends to each process sequentially in a loop — this is O(P) latency.
// A correct tree-based broadcast would be O(log P).
// The function produces correct results but is inefficient for large P.
void my_Bcast(void* data, int count, MPI_Datatype datatype, int root, MPI_Comm comm) {
    int rank, size;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);

    if (rank == root) {
        for (int i = 0; i < size; i++) {
            if (i != root) {
                MPI_Send(data, count, datatype, i, 0, comm);
            }
        }
    } else {
        MPI_Recv(data, count, datatype, root, 0, comm, MPI_STATUS_IGNORE);
    }
}

int main(int argc, char** argv) {
    int rank, size, data;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) data = 42;

    my_Bcast(&data, 1, MPI_INT, 0, MPI_COMM_WORLD);

    printf("Rank %d: data = %d\n", rank, data);

    MPI_Finalize();
    return 0;
}
