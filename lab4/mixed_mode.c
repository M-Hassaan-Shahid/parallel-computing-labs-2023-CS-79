#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    int rank, size;
    int a = 0, b = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) a = 5;

    MPI_Bcast(&a, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        MPI_Send(&a, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Recv(&b, 1, MPI_INT, 1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    } else if (rank == 1) {
        MPI_Recv(&b, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        b += a;
        MPI_Send(&b, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
    }

    printf("Rank %d: b = %d\n", rank, b);
    MPI_Finalize();
    return 0;
}
