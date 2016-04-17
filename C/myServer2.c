////////////////////////////////////////////////////////////////////////
// Program: myServer2.c
// Program to demonstrate a multi-threaded server.
//
// Name: John Gleeson
// ID: 	 12162213	
// Date: 24/10/2014
/////////////////////////////////////////////////////////////////////////

// Here are the necessary includes
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <time.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <pthread.h>

void *processActivity(void *arg); // function declaration for the threaded function
int counter; // a simple global variable
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;


// define struct to pass fds to processActivity function
typedef struct fdStruct {
    int connFd;
    int sockFd;
} fdStruct;

// MAIN

int main() {

    pthread_t processThread;
    struct sockaddr_in servAddr; // declare socket address structure
    struct fdStruct myFdStruct;  // declare struct to contain fds 

    memset(&servAddr, 0, sizeof (servAddr)); // clear the socket address structure

    // Create an unnamed socket ... name will be binded in later
    myFdStruct.sockFd = socket(PF_INET, SOCK_STREAM, 0); // Note PF_, not AF_

    if (myFdStruct.sockFd == -1) {
        perror("did not create the socket");
        exit(EXIT_FAILURE);
    }

    // Fill in the address structure
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(5000);

    if (inet_pton(AF_INET, "127.0.0.1", &servAddr.sin_addr) == -1) {
        perror("inet_pton problem");
        exit(EXIT_FAILURE);
    }

    // Name the socket by binding it to the servAddr structure's data
    if (bind(myFdStruct.sockFd, (struct sockaddr*) &servAddr, sizeof (servAddr)) == -1) {
        perror("did not bind socket properly");
        close(myFdStruct.sockFd);
        exit(EXIT_FAILURE);
    }

    // Set up 'listen' queue for say 60 client connections

    if (listen(myFdStruct.sockFd, 60) == -1) {
        perror("listen failure");
        close(myFdStruct.sockFd);
        exit(EXIT_FAILURE);
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////
    // Server is now set up, print a starting message from server
    printf("\nI am the server starting up. Hit Ctrl C to get rid of me!\n");
    printf("\nServer info: (IP: %s, port: %d)\n\n", inet_ntoa(servAddr.sin_addr), ntohs(servAddr.sin_port));


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Now – continuously loop accepting and processing client requests

    while (1) {
        // wait for a client and process the client
        myFdStruct.connFd = accept(myFdStruct.sockFd, (struct sockaddr*) NULL, NULL);

        if (myFdStruct.connFd < 0) {
            perror("accept call failed");
            close(myFdStruct.connFd);
            close(myFdStruct.sockFd);
            exit(EXIT_FAILURE);
        }

        printf("I have just dealt with a client!\n"); // message to the user
        
        // Create new thread, passing the fds using myFdStruct
        pthread_create(&processThread, NULL, &processActivity, (void*) &myFdStruct);


    } // end of while() loop

    pthread_join(processThread, NULL); // Join the thread
    close(myFdStruct.sockFd); // Close the socket
    pthread_mutex_destroy(&mutex1); // Destroy the mutex
    return (EXIT_SUCCESS);

} // end of main()

/*//////////////////////////////////////////////////////////////////////
					THREADED FUNCTION BIT BELOW
*///////////////////////////////////////////////////////////////////////
void *processActivity(void *arg) {

    fdStruct threadFdStruct; // Declare struct to keep it in scope of function
    threadFdStruct = *((fdStruct *) (arg)); // Assign the passed fds to a new struct

    while (1) {

        // use some buffers for messages
        char transmitBuff[1024] = {0};
        char receiveBuff[1024] = {0};

        // Fill tramsmitter buffer and clear the receiver buffer
        sprintf(transmitBuff, "Hello client, the server is using this fd for you: %d\n", threadFdStruct.connFd);
        memset(receiveBuff, 0, sizeof (receiveBuff)); // clear the receive buffer
        
        // Read from the connection
        if ((read(threadFdStruct.connFd, receiveBuff, sizeof (receiveBuff))) == -1) {
            perror("read from connection failure");
            close(threadFdStruct.connFd);
            close(threadFdStruct.sockFd);
            exit(EXIT_FAILURE);
        }

        pthread_mutex_lock(&mutex1); // Lock the mutex to prevent race condiion on counter
        printf("This server to date has an aggregate of %d reads for all clients\n", counter++);
        pthread_mutex_unlock(&mutex1); // Unlock the mutex

        printf("I just read this from a client: %s\n", receiveBuff);

        // Write to the connection
        if ((write(threadFdStruct.connFd, transmitBuff, strlen(transmitBuff))) == -1) {
            perror("write to connection failure");
            close(threadFdStruct.connFd);
            close(threadFdStruct.sockFd);
            exit(EXIT_FAILURE);
        }

        // If the received string is "End" than close the connection for this client
        if (strcmp(receiveBuff, "End\r\n") == 0) {
            printf("Closing down this client connection\n");
            close(threadFdStruct.connFd); // Close  the connection
            pthread_exit(NULL); // Thread exits. This line needs to be right here.
								// Otherwise "Bad file descriptor" occurs and server stops :(            
        }

    } // end while () loop

} // end processActivity function
