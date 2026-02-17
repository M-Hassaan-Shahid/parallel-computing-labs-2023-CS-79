/**
 * Pthread implementation for counting occurrences of a target value in a large array.
 * Uses multiple threads with mutex synchronization for thread-safe counting.
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define N 100000000
#define TARGET 42
#define NUM_THREADS 4

int *arr;
long global_count = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

/**
 * Structure to pass start and end indices to a thread
 */
typedef struct {
    long start;
    long end;
} ThreadArgs;

/**
 * Thread function to count occurrences in a chunk of the array
 * 
 * @param arg Pointer to ThreadArgs structure containing chunk boundaries
 * @return NULL
 */
void* count_chunk(void* arg) {
    ThreadArgs *args = (ThreadArgs*) arg;
    long local_count = 0;
    
    // Count in this thread's chunk
    for (long i = args->start; i < args->end; i++) {
        if (arr[i] == TARGET)
            local_count++;
    }
    
    // Add local count to global count (critical section)
    pthread_mutex_lock(&mutex);
    global_count += local_count;
    pthread_mutex_unlock(&mutex);
    
    return NULL;
}

int main() {
    // Allocate memory for array
    arr = malloc(N * sizeof(int));
    if (!arr) {
        perror("malloc");
        return 1;
    }
    
    // Fill array with random numbers
    srand(time(NULL));
    for (long i = 0; i < N; i++)
        arr[i] = rand() % 100;
    
    pthread_t threads[NUM_THREADS];
    ThreadArgs args[NUM_THREADS];
    long chunk_size = N / NUM_THREADS;
    
    clock_t start = clock();
    
    // Create threads
    for (int t = 0; t < NUM_THREADS; t++) {
        args[t].start = t * chunk_size;
        // Last thread takes the remainder to handle cases where N not divisible
        args[t].end = (t == NUM_THREADS - 1) ? N : (t + 1) * chunk_size;
        pthread_create(&threads[t], NULL, count_chunk, &args[t]);
    }
    
    // Wait for all threads to finish
    for (int t = 0; t < NUM_THREADS; t++) {
        pthread_join(threads[t], NULL);
    }
    
    clock_t end = clock();
    double elapsed = (double)(end - start) / CLOCKS_PER_SEC;
    
    printf("Count = %ld\n", global_count);
    printf("Time with %d threads = %.2f seconds\n", NUM_THREADS, elapsed);
    
    // Cleanup
    free(arr);
    pthread_mutex_destroy(&mutex);
    
    return 0;
}
