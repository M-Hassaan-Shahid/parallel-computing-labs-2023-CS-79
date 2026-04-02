#include <mpi.h>
#include <stdio.h>
#include <math.h>

#define N 1000000000  // 1 billion intervals

double f(double x) {
    return 4.0 / (1.0 + x * x);
}

int main(int argc, char** argv) {
    int rank, size;
    double local_sum = 0.0, global_sum = 0.0;
    double start, end;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int n = N;
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    double h = 1.0 / n;
    int intervals_per_proc = n / size;
    int remainder = n % size;
    int start_index, end_index;

    if (rank < remainder) {
        start_index = rank * (intervals_per_proc + 1);
        end_index = start_index + intervals_per_proc + 1;
    } else {
        start_index = rank * intervals_per_proc + remainder;
        end_index = start_index + intervals_per_proc;
    }

    start = MPI_Wtime();
    for (int i = start_index; i < end_index; i++) {
        double x = (i + 0.5) * h;
        local_sum += f(x);
    }
    local_sum *= h;

    MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    end = MPI_Wtime();

    if (rank == 0) {
        printf("Pi = %.16f\n", global_sum);
        printf("Time with %d processes = %f seconds\n", size, end - start);
    }

    MPI_Finalize();
    return 0;
}
