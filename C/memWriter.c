/***********************************************************************
Program: memWriter.c

A simple example on shared memory.
Create/open a shared memory object, map to that object and write to it.
In addition, reads a pid sent through a pipe and kills the process with
that pid.

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
#include <signal.h>

#define SHARED_OBJ_PATH  "don1337"  // pathname to shared object       
#define MESSAGE_SIZE      100     // maximum length the message 
#define RESERVED_MEMORY_SIZE (sysconf(_SC_PAGE_SIZE)*5) // defines the size of the reserved memory space
#define PIPE_1     "/tmp/pipe_1" // define the location of the pipe

int main() {

    int mfd, fdin; //file descriptor for the shared object
    char *shared_msg;
    char *pagearea;  // buffer to fill and share
    char fill = 'A'; // character to fill the buffer with
    int pidToKill;  // pid that will be read from a pipe
	int pagesize;
	
	mkfifo(PIPE_1, 0666); // creates a new named pipe.

    // create the shared memory object with shm_open()  
    mfd = shm_open(SHARED_OBJ_PATH, O_CREAT | O_RDWR, S_IRWXU | S_IRWXG);
    if (mfd < 0) {
        perror("error in shm_open()");
        exit(1);
    }

    printf("I have created a lucky shared memory object:  %s\n", SHARED_OBJ_PATH);
  
    pagesize = (int) sysconf(_SC_PAGE_SIZE); // finds out the system page size
    printf("Page size on this system is:%d bytes\n", pagesize);

    pagearea = (char *) malloc(RESERVED_MEMORY_SIZE); // allocate enough memory for the shared space
    memset(pagearea, fill, RESERVED_MEMORY_SIZE);  // fill the pagearea with 'A'
    ftruncate(mfd, RESERVED_MEMORY_SIZE); // adjust shared memory size 

    // request the shared segment with mmap()    
    shared_msg = mmap(NULL, RESERVED_MEMORY_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, mfd, 0);
    if (shared_msg == NULL) {
        perror("error in mmap()");
        exit(1);
    }

    printf("Shared memory is now allocated %lu bytes\n", RESERVED_MEMORY_SIZE);
    memcpy(shared_msg, pagearea, RESERVED_MEMORY_SIZE); //put something onto the memory

    printf("Message content is: %s\n\nCause a segfault and I'll quit...\n", shared_msg);

	fdin = open(PIPE_1, O_RDONLY);	// open a named pipe 
    read(fdin, &pidToKill, 20); // read the contents of the pipe and store it as an int

    kill(pidToKill,SIGKILL);    // kill the process matching the pid read from the pipe
	printf("I've just killed PID %d!!!\n",pidToKill);

    shm_unlink(SHARED_OBJ_PATH); // unlink - better to add error check
	unlink(PIPE_1);   // unlink the named pipe
    return 0;
}
