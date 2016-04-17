/******************************************************************
pipe_1.c
Demonstrates the use of unnamed pipes in C.

  Name: John Gleeson 
  ID  : 12162213
  Date: 10/04/14
  
 *****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define READ 0
#define WRITE 1

main() {
    int pid, child_pid, status;
    int fd[2], fd2[2], num_bytesRead;
    int i,j;

    pipe(fd);     // Create the 1st unnamed pipe
    pipe(fd2);	  // Create the 2nd unnamed pipe
    printf("I am main, my PID is %d\n", getpid());
    pid = fork();  // Fork a new process, original continues to run

    //THE CHILD PROCESS
    if (pid == 0)  // Pid of zero defines the new child process
     {
        printf("I am child: %d, my parent is %d \n", getpid(), getppid());
        char childBuff [200];		// Make a local buffer

		// For loop to send a message to the parent 10 times
        for (j = 0; j < 10; j++) {
            strcpy(childBuff, "Hello parent!");  // Put the string in the buffer
            write(fd[WRITE], childBuff, 200);    // Write to the buffer
            
        } // End for loop
		
        strcpy(childBuff, "Last Message!");  // Put the last message in the buffer
        close(fd[READ]);					 // Close the read end of the 1st pipe 
        write(fd[WRITE], childBuff, 200);	 // Write the buffer to the 1st pipe
        close(fd[WRITE]);					 // Close the write end of the 1st pipe

	
        close(fd2[WRITE]);					 // Close the write end of 2nd pipe
        read(fd2[READ], childBuff, 200);	 // Read the contents of the 2nd pipe
        close(fd2[READ]);					 // Close the read end of the 2nd pipe

		// If statement to compare the contents of the buffer to the 'trigger' string
        if (strcmp("Closing now!", childBuff) == 0) {
            printf("Got a message from parent: %s \n", childBuff);
            
            exit(0);            // Child process exits
            
        }	// End if block
        
    } // End if block       
    
    
    
    // THE PARENT PROCESS
    else {
        char myBuff[200];				// Make a local buffer

		// While loop to cause the parent process to constantly loop
        while (1) {
            num_bytesRead = read(fd[READ], myBuff, 200);  // Read the 1st pipe

		    // If statement to compare the contents of the buffer to the 'trigger' string
            if (strcmp("Last Message!", myBuff) == 0) {
                printf("Got a message from child: %s \n", myBuff);
                strcpy(myBuff, "Closing now!");		// Put the string in the buffer
                close(fd2[READ]);				    // Close the read end of the 2nd pipe
                write(fd2[WRITE], myBuff, 200);     // Write the buffer to the 2nd pipe
                close(fd2[WRITE]);				    //Close the write end of the 2nd pipe
                break;
                
            }   // End if block
            
            printf("Got a message from child: %s \n", myBuff);
            
        }  // End while block

        child_pid = wait(&status);    // Parent waits for child
        exit(0);					  // Parent process exits
        
    } // End else block
}
