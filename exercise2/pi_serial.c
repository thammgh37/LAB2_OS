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
    int numberPoint = atoi(argv[1]);
    //init seed for random
    time_t t;
    srand((double) time(&t));
    // estimate pi
    double x,y;
    int inPoints = 0;
    for (int i = 0;i< numberPoint;i++){
        x = (double) rand()/RAND_MAX * 2 -1;
        y = (double) rand()/RAND_MAX * 2 -1;
        if ( x*x + y*y <= 1){
            inPoints++;
        }
    }
    double estimatePi = 4* (double)inPoints/numberPoint;
    printf("pi : %.5f\n", estimatePi);
    clock_t end = clock();
    double elapsed_time = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Time taken: %f seconds\n", elapsed_time);
    return 0;
}