#include <stdio.h>
#include <mpi.h>

int main(int argc, char** argv) {
    int mynode, totalnodes;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &totalnodes);
    MPI_Comm_rank(MPI_COMM_WORLD, &mynode);

    if (mynode % 2 == 0)
        printf("I am even (rank %d)\n", mynode);
    else
        printf("I am odd (rank %d)\n", mynode);

    MPI_Finalize();
    return 0;
}
