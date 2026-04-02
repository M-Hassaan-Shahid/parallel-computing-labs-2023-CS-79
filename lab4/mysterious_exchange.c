#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    int rank, size;
    int a = 1, b = 2, c = 3;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 2) {
        if (rank == 0) printf("Run with 2 processes.\n");
        MPI_Finalize();
        return 0;
    }

    if (rank == 0) {
        MPI_Send(&a, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Recv(&b, 1, MPI_INT, 1, 1, MPI_COMM_WORLD, &status);
        printf("P0: b = %d\n", b);
    } else {
        MPI_Recv(&c, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Send(&c, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
        printf("P1: c = %d\n", c);
    }

    MPI_Finalize();
    return 0;
}
