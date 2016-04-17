// dupClient.c
// A client that will continuously write a message to a public pipe,
// and constantly read a private pipe that will contain the sent message 
// converted to uppercase.
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
#include "pipeExample.h" // A common header file for client and server

int main() {
    int fdout, clientFromServer;
    char buff[BUF_SIZE];
    struct message msg;

    // Use sprintf to make message text along with PID info and put it in
    // the messageInfo part of the msgstruct
    sprintf(msg.messageInfo, "This is a client, my PID is: %d", getpid());
    
    // Use sprintf to put the pid of the client into the privateInfo of 
    // the msg struct
    sprintf(msg.privateName, "%d", getpid());

    // Open the public pipe
    if ((fdout = open(PIPE_1, O_WRONLY)) == -1) {

        perror(PIPE_1);
        exit(1);
    }

    while (1) {
        write(fdout, &msg, sizeof (msg)); // Simply write to the pipe
        sleep(1);

		// Create the private pipe through which the return message will 
		// be received.
		// I shouldn't have to create a private pipe in both the client and 
		// the server but I do for some reason...
		// If I don't create the private pipe in the server, the message is 
		// not returned to the client. Odd.
        mkfifo(msg.privateName, 0666);
        
        // Open the private pipe read-only
        clientFromServer = open(msg.privateName, O_RDONLY);
        sleep(1);

		// Read the contents of the pipe to a buffer
        read(clientFromServer, buff, sizeof (buff));
        sleep(1);

		// Print the contents of the pipe
        printf("Got this from server: %s\n\n", buff);
        
        // Close the private pipe
        close(clientFromServer);
        sleep(1);
        
        // Unlink the private pipe
        unlink(msg.privateName);
	
    }
    close(fdout);

    return (0);



}
