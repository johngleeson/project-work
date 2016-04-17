// dupServer.c
// A server that will continuously read a message from a private pipe,
// convert the message to uppercase and return the message to a client
// through a 'private' pipe.
// 
// Name: John Gleeson 
// ID  : 12162213
// Date: 02/10/14
//////////////////////////////////////////////////
#include <sys/types.h>  
#include <sys/stat.h>  
#include <fcntl.h>  
#include <unistd.h>  
#include <string.h>  
#include <stdio.h>  
#include <errno.h>
#include <stdlib.h>
#include "pipeExample.h"  // A common header file for client and server

int main() {

    int i, fdin, status, numRead, fdout, serverToClient, dummyfifo;
    char buff[BUF_SIZE];
    struct message msg;

    // Create the public pipe with read and write access
    status = mkfifo(PIPE_1, 0666);

    // Check status for any error except an ‘exist’ error
    if ((status == -1) && (errno != EEXIST)) {
        perror("Error creating the named pipe - remove pipe if already exists");
        exit(1);
    }

    while (1) {

        // Open the public pipe for reading 
        fdin = open(PIPE_1, O_RDONLY);

        // Read the public pipe 
        numRead = read(fdin, &msg, BUF_SIZE);
        
        // dummy fd to keep the pipe open for writing
        dummyfifo = open(PIPE_1, O_WRONLY | O_NDELAY);

        if (numRead == 0) {
            printf("!!Write side of pipe_1 is closed, server now exits\n");
            exit(1);
        }

        printf("Got this string from pipe: %s\n", msg.messageInfo);

        // Convert all characters of the message portion of the 
        // msg struct to upper case 
        for (i = 0; i < sizeof (msg.messageInfo); i++) {
            if (islower(msg.messageInfo[i]))
                msg.messageInfo[i] = toupper(msg.messageInfo[i]);
        }
		
		// Create the 'private' pipe based on the contents of the 
		// privateName portion of the struct
		mkfifo(msg.privateName, 0666);
		
        printf("Now converted uppercase string: %s\n", msg.messageInfo);
        
        //Copy msg.messageInfo into the buffer
        strcpy(buff,msg.messageInfo);
        
        // fd to open the private pipe for writing
        serverToClient = open(msg.privateName, O_WRONLY);
        
        // Write contents of the buffer to the 
        // private pipe
        write(serverToClient, buff, BUF_SIZE);
        
        // Close the fd for the private pipe
        close(serverToClient);
        sleep(1);
        
        // Unlink the private pipe. This won't make much difference as 
        // this code is constantly looping
        unlink(msg.privateName);

    }
    // Clean up
    close(fdin);
    unlink(PIPE_1);
    return (0);

}
