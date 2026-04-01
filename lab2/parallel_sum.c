#include <stdio.h>
#include <mpi.h>
int main(int argc, char *argv[]) {
int mynode, totalnodes;

int sum, startval, endval, accum;
MPI_Status status;
double start_time, end_time;
MPI_Init(&argc, &argv);
MPI_Comm_size(MPI_COMM_WORLD, &totalnodes);
MPI_Comm_rank(MPI_COMM_WORLD, &mynode);
// Start timing after initialization (optional: synchronize with barrier)
MPI_Barrier(MPI_COMM_WORLD);
start_time = MPI_Wtime();
sum = 0;
// Determine the range of numbers for this process
startval = 1000 * mynode / totalnodes + 1;
endval = 1000 * (mynode + 1) / totalnodes;
for (int i = startval; i <= endval; i++) {
sum += i;
}
if (mynode != 0) {
// Worker sends its partial sum to process 0
MPI_Send(&sum, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
} else {
// Master receives from all workers
for (int j = 1; j < totalnodes; j++) {
MPI_Recv(&accum, 1, MPI_INT, j, 1, MPI_COMM_WORLD,

&status);

sum += accum;
}
// Master now has the total sum
}
end_time = MPI_Wtime();
if (mynode == 0) {
printf("The sum from 1 to 1000 is: %d\n", sum);
printf("Execution time with %d processes: %f seconds\n",
    totalnodes, end_time - start_time);

}
MPI_Finalize();
return 0;
}