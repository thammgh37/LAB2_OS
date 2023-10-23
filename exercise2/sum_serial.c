#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h> 
#include <time.h>


int main(int argc, char *argv[]){
    clock_t start = clock();
    // input of number point
    if (argc != 2){
        printf("Calculation failed, miss argument");
        return 0;
    }
    long long number = atoll(argv[1]);
    // estimate pi
    long long sum = 0;
    for (long long i = 1;i<= number;i++){
        sum += i;
    }
    printf("sum : %lld\n", sum);
    clock_t end = clock();
    double elapsed_time = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Time taken: %f seconds\n", elapsed_time);
    return 0;
}