#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N 100000000
#define TARGET 42
#define NUM_THREADS 4

int *arr;
int found_index = -1;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

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

int main() {
    arr = (int*) malloc(N * sizeof(int));
    if (!arr) { perror("malloc"); return 1; }

    // Fill array with random numbers 0-99
    srand(42);
    for (long i = 0; i < N; i++)
        arr[i] = rand() % 100;

    pthread_t threads[NUM_THREADS];
    long starts[NUM_THREADS];
    long chunk_size = N / NUM_THREADS;

    // Create threads
    for (int t = 0; t < NUM_THREADS; t++) {
        starts[t] = t * chunk_size;
        pthread_create(&threads[t], NULL, search_chunk, &starts[t]);
    }

    // Wait for all threads to finish
    for (int t = 0; t < NUM_THREADS; t++)
        pthread_join(threads[t], NULL);

    if (found_index != -1)
        printf("Target %d found at index %d\n", TARGET, found_index);
    else
        printf("Target %d not found\n", TARGET);

    free(arr);
    pthread_mutex_destroy(&lock);
    return 0;
}
