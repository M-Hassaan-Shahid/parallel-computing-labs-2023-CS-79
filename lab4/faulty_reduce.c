#include <mpi.h>
#include <stdio.h>

// Part 5.2: Faulty Reduce
// Bug: *global += temp always performs addition regardless of the op parameter.
// MPI_MAX, MPI_MIN, MPI_PROD etc. are silently ignored.
// Fix: check op and apply the correct operation.
void my_Reduce(int* local, int* global, int count, MPI_Op op, int root, MPI_Comm comm) {
    int rank, size;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);

    if (rank == root) {
        *global = *local;
        for (int i = 0; i < size; i++) {
            if (i != root) {
                int temp;
                MPI_Recv(&temp, 1, MPI_INT, i, 0, comm, MPI_STATUS_IGNORE);
                // Fixed: apply the correct operation based on op
                if (op == MPI_SUM)
                    *global += temp;
                else if (op == MPI_MAX)
                    *global = (*global > temp) ? *global : temp;
                else if (op == MPI_MIN)
                    *global = (*global < temp) ? *global : temp;
                else
                    *global += temp;  // default to sum
            }
        }
    } else {
        MPI_Send(local, 1, MPI_INT, root, 0, comm);
    }
}

int main(int argc, char** argv) {
    int rank, size, local_val, result = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    local_val = rank + 1;

    my_Reduce(&local_val, &result, 1, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0)
        printf("Sum = %d (expected %d)\n", result, size * (size + 1) / 2);

    MPI_Finalize();
    return 0;
}
