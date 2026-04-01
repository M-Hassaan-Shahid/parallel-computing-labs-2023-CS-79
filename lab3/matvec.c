#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define MATRIX_ROWS 8   /* number of rows    */
#define MATRIX_COLS 8   /* number of columns */

int main(int argc, char** argv) {
    int proc_rank, proc_size;
    double *matrix_A       = NULL;
    double *local_matrix;
    double *vector_x       = NULL;
    double *local_result;
    double *result_vector  = NULL;
    int rows_per_process;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &proc_size);

    // Verify even row distribution
    if (MATRIX_ROWS % proc_size != 0) {
        if (proc_rank == 0)
            printf("Matrix rows %d must be divisible by number of processes (%d).\n", MATRIX_ROWS, proc_size);
        MPI_Finalize();
        return 1;
    }

    rows_per_process = MATRIX_ROWS / proc_size;
    local_matrix = (double*) malloc(rows_per_process * MATRIX_COLS * sizeof(double));
    local_result = (double*) malloc(rows_per_process * sizeof(double));

    // Master process initializes matrix and vector
    if (proc_rank == 0) {
        matrix_A = (double*) malloc(MATRIX_ROWS * MATRIX_COLS * sizeof(double));
        vector_x = (double*) malloc(MATRIX_COLS * sizeof(double));
        result_vector = (double*) malloc(MATRIX_ROWS * sizeof(double));

        // Initialize matrix: A[i][j] = (i+1)
        // Initialize vector: x[j] = 1.0
        // Expected result: y[i] = MATRIX_COLS * (i+1)
        for (int i = 0; i < MATRIX_ROWS; i++)
            for (int j = 0; j < MATRIX_COLS; j++)
                matrix_A[i * MATRIX_COLS + j] = (double)(i + 1);

        for (int j = 0; j < MATRIX_COLS; j++)
            vector_x[j] = 1.0;
    } else {
        // Non-master processes allocate vector space
        vector_x = (double*) malloc(MATRIX_COLS * sizeof(double));
    }

    // Broadcast vector to all processes
    MPI_Bcast(vector_x, MATRIX_COLS, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Distribute matrix rows to all processes
    MPI_Scatter(matrix_A, rows_per_process * MATRIX_COLS, MPI_DOUBLE,
                local_matrix, rows_per_process * MATRIX_COLS, MPI_DOUBLE,
                0, MPI_COMM_WORLD);

    // Compute local matrix-vector product
    for (int i = 0; i < rows_per_process; i++) {
        local_result[i] = 0.0;
        for (int j = 0; j < MATRIX_COLS; j++)
            local_result[i] += local_matrix[i * MATRIX_COLS + j] * vector_x[j];
    }

    // Collect results from all processes
    MPI_Gather(local_result, rows_per_process, MPI_DOUBLE,
               result_vector, rows_per_process, MPI_DOUBLE,
               0, MPI_COMM_WORLD);

    // Master process displays results
    if (proc_rank == 0) {
        printf("Result vector y (expected: 8, 16, 24, ...):\n");
        for (int i = 0; i < MATRIX_ROWS; i++)
            printf("y[%d] = %6.1f\n", i, result_vector[i]);
        free(matrix_A); 
        free(result_vector);
    }

    free(local_matrix);
    free(local_result);
    free(vector_x);
    MPI_Finalize();
    return 0;
}
