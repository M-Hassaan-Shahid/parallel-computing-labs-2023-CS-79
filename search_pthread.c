/**
 * Pthread implementation for searching a target value in a large array.
 * Uses multiple threads with early termination when target is found.
 * Demonstrates thread synchronization and cooperative cancellation.
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N 100000000
#define TARGET 42
#define NUM_THREADS 4

int *arr;
int found_index = -1;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

/**
 * Thread function to search for target in a chunk of the array
 * 
 * @param arg Pointer to starting index for this thread's chunk
 * @return NULL
 */
void* search_chunk(void* arg) {
    long start = *(long*)arg;
    long end = start + N / NUM_THREADS;
    
    // Last chunk handles remainder
    if (start == (NUM_THREADS - 1) * N / NUM_THREADS)
        end = N;
    
    for (long i = start; i < end; i++) {
        // Check if target already found by another thread
        pthread_mutex_lock(&lock);
        int stop = (found_index != -1);
        pthread_mutex_unlock(&lock);
        
        if (stop)
            break;
        
        if (arr[i] == TARGET) {
            pthread_mutex_lock(&lock);
            // First thread to find sets index
            if (found_index == -1)
                found_index = i;
            pthread_mutex_unlock(&lock);
            break;
        }
    }
    
    return NULL;
}
