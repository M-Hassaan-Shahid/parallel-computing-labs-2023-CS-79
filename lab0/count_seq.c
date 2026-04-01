#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define N 100000000 // 100 million elements
#define TARGET 42
int main() {
int *arr = malloc(N * sizeof(int));
if (!arr) { perror("malloc"); return 1; }
// Fill array with random numbers 0-99
srand(time(NULL));
for (long i = 0; i < N; i++)
arr[i] = rand() % 100;
clock_t start = clock();
long count = 0;
for (long i = 0; i < N; i++)
if (arr[i] == TARGET) count++;
clock_t end = clock();
double elapsed = (double)(end - start) / CLOCKS_PER_SEC;
printf("Count = %ld\n", count);
printf("Time = %.2f seconds\n", elapsed);
free(arr);
return 0;
}
