//pipe system call in C
//shared by Parent and Child

#include "user.h"

//Define MSGSIZE with a value of 16
#define MSGSIZE 16


int main(){

    //Store 2 file descriptors 
    //fd[0] will be read end of the pipe
    //fd[1] will be write end of the pipe
    int ParentToChild[2];
    int ChildToParent[2];

    //buf is for storing messages read from the pipe
    char buf[MSGSIZE];
    
    //pipe(fd) create a pipe.
    //if successful, initialize fd[0] for reading and fd[1] for writing
    //On success, pipe(fd) returns 0;
    //On failure, pipe(fd) returns -1;
    if(pipe(ParentToChild) == -1 || pipe(ChildToParent) == -1){
        //print error message
        printf("pipe failed");
        //exit(1) -> indicating that the program exited due to a error
        exit(1);
    }
    
    //Get the id of the new processer
    int pid = fork();

    //pid == -1 indicates an error in creating the new process
    if(pid == -1){
        printf("fork failed");
        //exit(1) -> indicating that the program exited due to a error
        exit(1);
    }
    
    //Parent
    if(pid > 0){
        
        close(ParentToChild[0]);
        close(ChildToParent[1]);

        //printf("I am Parent\n");

        //strlen("ping")+1, includes the null terminator (\0) so that child can receive complete string
        if(write(ParentToChild[1], "ping", strlen("ping")+1) == -1){
            printf("write to pipe failed");
            exit(1);
        }

        //wait() -> let parent goes into blocking state
        //wait() will automatically returns control to the parent as soon as the child has exited
        wait(0);

        //Read the child's response from pipe
        if(read(ChildToParent[0], buf, MSGSIZE) == -1){
            printf("read from pipe failed");
            exit(1);
        }
        printf("%d: received pong\n", getpid());

        //fd[0] and fd[1] in the parent process are no longer used, close them
        close(ParentToChild[0]);
        close(ChildToParent[1]);

        exit(0);

    //Child
    }else if(pid == 0){
        
        close(ParentToChild[1]);
        close(ChildToParent[0]);

        //printf("I am child\n");

        //Child reads the message sent by the parent from fd[0] into buf
        if(read(ParentToChild[0], buf, MSGSIZE) == -1){
            printf("child read from pipe failed");
            exit(1);
        };
        printf("%d: received ping\n", getpid());

        //Child writes the message into pipe
        if(write(ChildToParent[1], "pong", strlen("pong")+1) == -1){
            printf("child write to pipe failed");
            exit(1);
        }

        //fd[1] and f[0] in the child process are no longer used, close them
        close(ParentToChild[0]);
        close(ChildToParent[1]);
        exit(0);
    }
    return 0;
}