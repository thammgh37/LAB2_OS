#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

static int pipefd1[2], pipefd2[2];
char *str1 = "Child send message to parent! \n";
char *str2 = "Parent send message to child! \n";
char buffer[100];

void INIT(void) {
    if (pipe( pipefd1 )<0 || pipe (pipefd2)<0){
        perror ("pipe");
        exit (EXIT_FAILURE);
    }
}
void WRITE_TO_PARENT() {
    close(pipefd2[0]);
    write(pipefd2[1],str1,strlen(str1)+1);
    printf( "Child_send_message_to_parent! \n" );
}
void READ_FROM_PARENT() {
    // TO DO
    close(pipefd1[1]);
    read(pipefd1[0],buffer,sizeof(buffer));
    printf( "Child_receive_message_from_parent! \n%s \n",buffer);
}
void WRITE_TO_CHILD() {
    close(pipefd1[0]);
    write(pipefd1[1],str2,strlen(str2)+1);
    printf( "Parent_send_message_to_child! \n" ) ;
}
void READ_FROM_CHILD() {
    close(pipefd2[1]);
    read(pipefd2[0],buffer,sizeof(buffer));
    printf( "Parent_receive_message_from_child! \n" ) ;
    printf("%s\n",buffer);
}
int main() {
    INIT ( ) ;
    pid_t pid ;
    pid = fork ( );
    //set a timer , process will end after 1 second.

    alarm(10);
    if ( pid==0){
        while (1) {
            sleep ( rand()%2+1);
            WRITE_TO_CHILD() ;
            READ_FROM_CHILD() ;
        }
    }else{
        while (1) {
            sleep ( rand ( )%2+1);
            READ_FROM_PARENT();
            WRITE_TO_PARENT();
        }
    }
    return 0 ;
}