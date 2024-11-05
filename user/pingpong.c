//pipe system call in C
//shared by Parent and Child

//For printf()
#include <stdio.h>

//For system calls -> fork(), pipe(), read() and write()
#include <unistd.h>

//For exit()
#include <stdlib.h>

//For strlen() to determine the length of string
#include <string.h>

//For wait() to synchronize parent and child processes
#include <sys/wait.h>

//Define MSGSIZE with a value of 16
#define MSGSIZE 16


int main(){

    //Store 2 file descriptors 
    //fd[0] will be read end of the pipe
    //fd[1] will be write end of the pipe
    int fd[2];

    //buf is for storing messages read from the pipe
    char buf[MSGSIZE];
    
    //pipe(fd) create a pipe.
    //if successful, initialize fd[0] for reading and fd[1] for writing
    //On success, pipe(fd) returns 0;
    //On failure, pipe(fd) returns -1;
    if(pipe(fd) == -1){
        //print error message
        perror("pipe failed");
        //exit(1) -> indicating that the program exited due to a error
        exit(1);
    }
    
    //Get the id of the new processer
    int pid = fork();

    //pid == -1 indicates an error in creating the new process
    if(pid == -1){
        perror("fork failed");
        //exit(1) -> indicating that the program exited due to a error
        exit(1);
    }
    
    //Parent
    if(pid > 0){
        printf("I am Parent\n");

        //strlen("ping")+1, includes the null terminator (\0) so that child can receive complete string
        write(fd[1], "ping", strlen("ping")+1);

        //wait() -> let parent goes into blocking state
        //wait() will automatically returns control to the parent as soon as the child has exited
        wait(NULL);

        //Read the child's response from pipe
        read(fd[0], buf, MSGSIZE);
        printf("Parent process received: %s\n", buf);

        //fd[0] and fd[1] in the parent process are no longer used, close them
        close(fd[0]);
        close(fd[1]);

        exit(0);

    //Child
    }else{
        printf("I am child\n");

        //Child reads the message sent by the parent from fd[0] into buf
        read(fd[0], buf, MSGSIZE);
        printf("Child process received: %s\n", buf);

        //Child writes the message into pipe
        write(fd[1], "pong", strlen("pong")+1);

        //fd[0] and fd[1] in the child process are no longer used, close them
        close(fd[0]);
        close(fd[1]);

        exit(0);
    }
    return 0;
}