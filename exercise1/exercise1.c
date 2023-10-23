#include "stdio.h"
#include "stdlib.h"
#include "sys/types.h"
#include "string.h"
#include "unistd.h"
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h> 

#define SHM_KEY 0x125
#define numberMovie 1683
struct ratingMoviePair
{
    int id;
    int Rating;
    int numRating;
};
void writeSharedMemory(struct ratingMoviePair* data){
    struct ratingMoviePair* sharedMemory;
    int sharedID = shmget(SHM_KEY,numberMovie*sizeof(struct ratingMoviePair),0644 | IPC_CREAT);
    if ( sharedID < 0) {
        perror ( "shmget error1" ) ;
        exit(1);
    }else {
        printf ( "shared memory : %d\n" , sharedID ) ;
    }
    sharedMemory = shmat(sharedID,NULL,0);
    if(sharedMemory == (struct ratingMoviePair*) -1){
        perror("shmat error");
        exit(1);
    }
    memcpy (sharedMemory,data,numberMovie*sizeof(struct ratingMoviePair));
    if (shmdt(sharedMemory) == -1){
        perror("shmdt error");
        return;
    }
}
void readSharedMemory(struct ratingMoviePair* data){
    struct ratingMoviePair* sharedMemory;
    int sharedID = shmget(SHM_KEY,numberMovie*sizeof(struct ratingMoviePair),0644 | IPC_CREAT);
    if ( sharedID < 0) {
        perror ( "shmget error2" ) ;
        exit(1);
    }else {
        printf ( "shared memory : %d\n" , sharedID ) ;
    }
    sharedMemory = shmat(sharedID,NULL,0);
    if(sharedMemory == (struct ratingMoviePair*) -1){
        perror("shmat error");
        exit(1);
    }
    memcpy (data,sharedMemory,numberMovie*sizeof(struct ratingMoviePair));
    if (shmdt(sharedMemory) == -1){
        perror("shmdt error");
        return;
    }
}
void destroySharedMemory(){
    int shmid = shmget(SHM_KEY,numberMovie*sizeof(struct ratingMoviePair) ,0644 | IPC_CREAT);
    if ( shmid < 0) {
        perror ( "shmget error3" ) ;
        exit(1);
    }
    if (shmctl(shmid,IPC_RMID,0) == -1)
        perror("shmctl error");
        return;
    
}
void readFile(char* filename, struct ratingMoviePair* sharedArrayStruct){
    for (int i  =  1;i<numberMovie;i++){
        sharedArrayStruct[i].id = 0;
        sharedArrayStruct[i].Rating = 0;
        sharedArrayStruct[i].numRating = 0;

    }
    char buffer[1024];
    FILE* file = fopen(filename, "r");
    if (file == NULL){
        perror("Couldn't open file ");
        return;
    }
    char* token;
    int idMovie;
    int rating;
    int userID;
    int timeStamp;
    while(fgets(buffer, sizeof(buffer), file) != NULL){
        token = strtok(buffer, "\t");
        userID = atoi(token);
        token = strtok(NULL, "\t");
        idMovie = atoi(token);
        token = strtok(NULL, "\t");
        rating = atoi(token);
        token = strtok(NULL, "\t");
        timeStamp = atoi(token);
        sharedArrayStruct[idMovie].id = idMovie;
        sharedArrayStruct[idMovie].numRating++;
        sharedArrayStruct[idMovie].Rating+= rating;
    }
    fclose(file);
}
void writeFile(struct ratingMoviePair* fullData){
    FILE* file;
    file = fopen("output.txt","w");
    for (int i = 0 ;i < numberMovie;i++){
        float tmp;
        if (fullData[i].numRating == 0){
            tmp = 0;
        }
        else{
            tmp = (float)fullData[i].Rating/(fullData[i].numRating);
        }    
        fprintf(file,"ID Movie :%d : %.2f\n",i, tmp);
    }
    fclose(file);
}
int main(){
    int pid1,pid2;
    int status;
    pid1 = fork();
    if (pid1 == 0){
        printf("child1");
        struct ratingMoviePair data[numberMovie];
        readFile("movie-100k-split/movie-100k_1.txt",data);
        writeSharedMemory(data);
        exit(0);
    }
    else{
        pid2 = fork();
        if ( pid2 == 0){
            usleep(100000);
            printf("child2");
            struct ratingMoviePair data1[numberMovie];
            struct ratingMoviePair data2[numberMovie];
            readFile("movie-100k-split/movie-100k_2.txt",data2);
            readSharedMemory(data1);
            for (int i = 0;i < numberMovie;i++){
                data1[i].Rating += data2[i].Rating;
                data1[i].numRating += data2[i].numRating;
            }
            writeSharedMemory(data1);
            exit(0);
        }
        else {
            printf("child3");
            wait(NULL);
            wait(NULL);
            struct ratingMoviePair fullData[numberMovie];
            readSharedMemory(fullData);
            writeFile(fullData);
            destroySharedMemory();
        }
        
    }
}