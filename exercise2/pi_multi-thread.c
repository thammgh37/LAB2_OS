#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h> 
#include <pthread.h>
#include <time.h>
#define numberThread 4

struct threadData{
    int totalPoints;
    int inPoints;
};
struct threadData arrayThreadData[numberThread];

void* runner(void* threadParam){
    struct threadData * data;
    data = (struct threadData*) threadParam;
    int numberPoint = data->totalPoints;
    double x,y;
    for (int i = 0;i< numberPoint;i++){
        x = (double) rand()/RAND_MAX * 2 -1;
        y = (double) rand()/RAND_MAX * 2 -1;
        if ( x*x + y*y <= 1){
            data->inPoints++;
        }
    }
    pthread_exit(0);
}

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
    //create thread data and thread
    struct threadData thrData[numberThread];
    pthread_t tid[numberThread];
    for (int i=0; i < numberThread;i++){
        thrData[i].inPoints = 0;
        thrData[i].totalPoints = numberPoint/numberThread;
        int rc = pthread_create(&tid[i],NULL,runner,(void*)&thrData[i]);
        if (rc){
            printf("error thread create\n");
            exit(1);
        }
    }
    long globalCount = 0;
    for (int i=0; i < numberThread;i++){
        pthread_join(tid[i],NULL);
        globalCount += thrData[i].inPoints;
    }
    // estimate pi
    double estimatePi = 4* (double)globalCount/(double)numberPoint;
    printf("pi : %.5f\n", estimatePi);
    clock_t end = clock();
    double elapsed_time = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Time taken: %f seconds\n", elapsed_time);
    return 0;
}