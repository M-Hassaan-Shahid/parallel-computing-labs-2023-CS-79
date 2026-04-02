#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define M 8  // number of rows
#define N 8  // number of columns

int main(int argc, char** argv) {
    int rank, size;
    double *A = NULL;       // full matrix on root
    double *local_A;        // local rows
    double *x = NULL;       // vector (replicated on all processes)
    double *local_y;        // local result for local rows
    double *y = NULL;       // full result on root
    int local_rows;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (M % size != 0) {
        if (rank == 0)
            printf("Number of rows must be divisible by number of processes.\n");
        MPI_Finalize();
        return 1;
    }

    local_rows = M / size;

    // Allocate local storage
    local_A = (double*) malloc(local_rows * N * sizeof(double));
    local_y = (double*) malloc(local_rows * sizeof(double));

    // Root initialises matrix and vector
    if (rank == 0) {
        A = (double*) malloc(M * N * sizeof(double));
        x = (double*) malloc(N * sizeof(double));
        y = (double*) malloc(M * sizeof(double));

        // Initialise A as a simple matrix (e.g., all 1's)
        for (int i = 0; i < M * N; i++)
            A[i] = 1.0;

        // Initialise x as a vector of 1's
        for (int i = 0; i < N; i++)
            x[i] = 1.0;
    } else {
        // Other processes allocate x (will be filled by broadcast)
        x = (double*) malloc(N * sizeof(double));
    }

    // Broadcast vector x to all processes
    MPI_Bcast(x, N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Scatter rows of A
    MPI_Scatter(A, local_rows * N, MPI_DOUBLE,
                local_A, local_rows * N, MPI_DOUBLE,
                0, MPI_COMM_WORLD);

    // Compute local matrix-vector product
    for (int i = 0; i < local_rows; i++) {
        local_y[i] = 0.0;
        for (int j = 0; j < N; j++) {
            local_y[i] += local_A[i * N + j] * x[j];
        }
    }

    // Gather all local_y into y on root
    MPI_Gather(local_y, local_rows, MPI_DOUBLE,
               y, local_rows, MPI_DOUBLE,
               0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Result vector y:\n");
        for (int i = 0; i < M; i++)
            printf("%f ", y[i]);
        printf("\n");

        free(A);
        free(y);
    }

    free(local_A);
    free(local_y);
    free(x);
    MPI_Finalize();
    return 0;
}
