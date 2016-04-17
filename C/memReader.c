/***********************************************************************
Program: memReader.c

Open a shared a shared memory object, map to that object and read it.
Added functionality to attempt to write to shared memory (causing a segfault).
A signal handler catches the signal, writes the process id to a named pipe.

Name: John Gleeson 
ID  : 12162213
Date: 29/11/14
 ************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <signal.h>

#define SHARED_OBJ_PATH   "don1337"   // pathname to shared object  
#define MESSAGE_SIZE      100  // maximum length the message 
#define PIPE_1     "/tmp/pipe_1" // define the location of the pipe


char message[MESSAGE_SIZE];


// signal handler for SIGSEGV
void catchSig(int sig) {   
    int fdout, pid; 
    
    signal(SIGSEGV, catchSig);
    pid = getpid(); // gets the process pid

    // Open the public pipe
    if ((fdout = open(PIPE_1, O_WRONLY)) == -1) {

        perror(PIPE_1);
        exit(1);
    }

    write(fdout, &pid, sizeof (pid)); // writes the pid to the pipe
    close(fdout);

}

int main() {

    int mfd; //file descriptor for the shared object
    int pid = getpid(); // gets the pid of the process
    int seg_size = (sizeof (message)); //shared object sized to store message 
    char *shared_msg;

    // open the shared memory object using shm_open()    
    mfd = shm_open(SHARED_OBJ_PATH, O_RDWR, S_IRWXU | S_IRWXG);
    if (mfd < 0) {
        perror("error shm_open(): forgot to run memWriter?");
        exit(1);
    }

    printf("Opened the shared memory object %s\n", SHARED_OBJ_PATH);

    // request the shared segment with mmap()    
    shared_msg = mmap(NULL, seg_size, PROT_READ, MAP_SHARED, mfd, 0);
    if (shared_msg == NULL) {
        perror("error in  mmap()");
        exit(1);
    }

    printf("Shared memory size allocated is %d bytes\n", seg_size);
    printf("The message content actually read is: .... %s\n", shared_msg);
    printf("My PID is:%d\n", pid);      // displays pid to compare to the one the writer displays
    printf("Press return to cause a segfault!\n");

    signal(SIGSEGV, catchSig); // sets up the signal handler
    getchar();  // waits for user input to trigger the segfault

    memcpy(shared_msg, "BOO!", MESSAGE_SIZE); // causes a segfault by attempting to write to the shared memory
    shm_unlink(SHARED_OBJ_PATH); // unlink - better to add error check

    return 0;
}
