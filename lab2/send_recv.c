#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
int main(int argc, char *argv[]) {
int mynode, totalnodes;
int sender = 2;
int receiver = 4;
int tag = 123;
MPI_Status status;
MPI_Init(&argc, &argv);
MPI_Comm_size(MPI_COMM_WORLD, &totalnodes);
MPI_Comm_rank(MPI_COMM_WORLD, &mynode);
// For simplicity, we fix datasize
int datasize = 10;
double *databuffer = (double*) malloc(datasize *
sizeof(double));

if (mynode == sender) {
// Initialize some data
for (int i = 0; i < datasize; i++)
databuffer[i] = i * 1.0;
MPI_Send(databuffer, datasize, MPI_DOUBLE, receiver, tag,

MPI_COMM_WORLD);

printf("Process %d sent data to %d\n", mynode, receiver);
}
if (mynode == receiver) {
MPI_Recv(databuffer, datasize, MPI_DOUBLE, sender, tag,

MPI_COMM_WORLD, &status);

printf("Process %d received data from %d\n", mynode,

status.MPI_SOURCE);

// Print received data
for (int i = 0; i < datasize; i++)
printf("%f ", databuffer[i]);
printf("\n");
}
free(databuffer);
MPI_Finalize();
return 0;
}