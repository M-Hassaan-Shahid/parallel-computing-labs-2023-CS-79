#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define N 100000000 // total number of elements
#define TARGET 42
int main(int argc, char** argv) {
int rank, size;
long local_count = 0;
long global_count = 0;
double start, end;
MPI_Init(&argc, &argv);
MPI_Comm_rank(MPI_COMM_WORLD, &rank);
MPI_Comm_size(MPI_COMM_WORLD, &size);
// Determine how many elements each process gets
long elements_per_proc = N / size;
long remainder = N % size;
long local_n;
if (rank < remainder) {
local_n = elements_per_proc + 1;
} else {
local_n = elements_per_proc;
}
// Allocate local array
int *local_arr = (int*) malloc(local_n * sizeof(int));
if (!local_arr) {
fprintf(stderr, "Process %d: malloc failed\n", rank);
MPI_Abort(MPI_COMM_WORLD, 1);
}
// Seed random number generator differently for each process
srand(time(NULL) + rank);
// Fill local array with random numbers 0-99
for (long i = 0; i < local_n; i++) {
local_arr[i] = rand() % 100;
}
// Synchronise before timing
MPI_Barrier(MPI_COMM_WORLD);
start = MPI_Wtime();
// Count occurrences of TARGET in local chunk
for (long i = 0; i < local_n; i++) {
if (local_arr[i] == TARGET)
local_count++;
}
// Reduce all local counts to global count on rank 0
MPI_Reduce(&local_count, &global_count, 1, MPI_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
end = MPI_Wtime();
if (rank == 0) {
printf("Total count = %ld\n", global_count);
printf("Time with %d processes = %f seconds\n", size, end - start);
}
free(local_arr);
MPI_Finalize();
return 0;
}
