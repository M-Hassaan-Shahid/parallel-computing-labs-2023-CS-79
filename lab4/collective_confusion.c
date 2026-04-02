#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    int rank, size;
    int data;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) data = 99;
    MPI_Bcast(&data, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 1) data = 100;  // modify after broadcast?
    MPI_Bcast(&data, 1, MPI_INT, 1, MPI_COMM_WORLD);  // root = 1?

    printf("Rank %d: data = %d\n", rank, data);
    MPI_Finalize();
    return 0;
}
