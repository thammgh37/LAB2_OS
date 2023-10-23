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

struct threadData{
    long long start;
    long long end;
    long long sum;
};

void* runner(void* threadParam){
    struct threadData * data;
    data = (struct threadData*) threadParam;
    for (long long i = data->start ;i<= data->end;i++){
        data->sum += i;
    }
    pthread_exit(0);
}

int main(int argc, char *argv[]){
    clock_t start = clock();
    // input of number
    if (argc != 3){
        printf("Calculation failed, miss argument");
        return 0;
    }
    int numberThread = atoi(argv[1]);
    long long number = atoll(argv[2]);
    long long interval = number/numberThread;
    //create thread data and thread
    struct threadData thrData[numberThread];
    pthread_t tid[numberThread];
    for (long long i=0; i <= numberThread;i++){
        thrData[i].start = i*interval + 1;
        thrData[i].end = (1+i)*interval;
        thrData[i].sum = 0;
        int rc = pthread_create(&tid[i],NULL,runner,(void*)&thrData[i]);
        if (rc){
            printf("error thread create\n");
            exit(1);
        }
    }
    long long globalSum = 0;
    for (int i=0; i < numberThread;i++){
        pthread_join(tid[i],NULL);
        globalSum += thrData[i].sum;
    }
    // calculate sum
    printf("sum : %lld\n", globalSum);
    clock_t end = clock();
    double elapsed_time = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Time taken: %f seconds\n", elapsed_time);
    return 0;
}