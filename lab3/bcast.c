#include <stdio.h>
#include <mpi.h>

int main(int argc, char** argv) {
    int proc_rank, proc_size;
    int shared_value;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &proc_size);

    // Master process initializes the shared value
    if (proc_rank == 0) {
        shared_value = 1000;
        printf("Master Process %d is broadcasting value %d to all workers\n", proc_rank, shared_value);
    }

    // All processes participate in the broadcast operation
    MPI_Bcast(&shared_value, 1, MPI_INT, 0, MPI_COMM_WORLD);
    printf("Worker Process %d has received shared value = %d\n", proc_rank, shared_value);

    MPI_Finalize();
    return 0;
}
