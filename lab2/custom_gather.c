#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int mynode, totalnodes;
    MPI_Status status;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &totalnodes);
    MPI_Comm_rank(MPI_COMM_WORLD, &mynode);
    
    int array_size = 10;
    int *send_array = (int*) malloc(array_size * sizeof(int));
    
    if (mynode != 0) {
        // Worker processes: generate array and send to process 0
        for (int i = 0; i < array_size; i++) {
            send_array[i] = mynode * 100 + i;
        }
        
        // Send array to process 0 with tag = rank
        MPI_Send(send_array, array_size, MPI_INT, 0, mynode, MPI_COMM_WORLD);
        printf("Process %d sent its array to process 0\n", mynode);
        
    } else {
        // Process 0: master - receive from all workers
        int *recv_buffer = (int*) malloc(array_size * sizeof(int));
        
        printf("Process 0 is receiving arrays from all workers:\n");
        printf("=================================================\n");
        
        for (int source = 1; source < totalnodes; source++) {
            // Receive from each worker in order
            MPI_Recv(recv_buffer, array_size, MPI_INT, source, source, 
                     MPI_COMM_WORLD, &status);
            
            // Print received data
            printf("Process 0 received from %d: ", source);
            for (int i = 0; i < array_size; i++) {
                printf("%d ", recv_buffer[i]);
            }
            printf("\n");
        }
        
        free(recv_buffer);
    }
    
    free(send_array);
    MPI_Finalize();
    return 0;
}
