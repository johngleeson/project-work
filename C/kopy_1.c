/******************************************************************
kopy_1.c
An example Unix-like shell copy command/utility.
Copies the first file to the second. Three flags can be set.
Uppercase flag : Changes all text of a given text file to uppercase
Size flag : Prints the size of the file being copied to the screen
Rate : Prints the transfer rate of the copy process in kb/s

  Name: John Gleeson 
  ID  : 12162213
  Date: 10/04/14
  
 *****************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

    int i;
    int fileArg = 0;           // Count for files on command line
    int infd, outfd;           // File descriptor for input and output file
    int num_read, num_write;   // Numbers of characters returned by a file access
    char buff [1024];          // Buffer for use in file access
    char* fileName[2] = {};    // Point to file names
    int uppercase_flag = 0;    // Stores uppercase_flag
    int size_flag = 0;	       // Stores size_flag
    int rate_flag = 0;         // Stores rate_flag
    int buffer_iterations = 0; // Stores buffer_iterations
    double start, end;         // Two variables represent number of microseconds
    unsigned long elapsed = 0; // variable to represent elapsed time
    struct timeval startVal;   
    struct timeval endVal;     
    struct rusage ru;		   
    float rate;				   // Stores rate

    printf("\n");
    
    // Parse the command line arguments
    for (i = 1; i < argc; i++) {
		
        // If statement to check for the uppercase flag
        if (strcmp("-u", argv[i]) == 0) {
            uppercase_flag = 1;
            printf("Uppercase flag is enabled\n");
            continue;
        }

        // If statement to check for the size flag
        if (strcmp("-s", argv[i]) == 0) {
            size_flag = 1;
            printf("Size flag is enabled\n");
            continue;
        }

        // If statement to check for the rate flag
        if (strcmp("-r", argv[i]) == 0) {
            rate_flag = 1;
            printf("Rate flag is enabled\n");
            continue;
        }
        fileName[fileArg++] = argv[i];
    }

    // Open the first file for reading
    infd = open(fileName[0], O_RDONLY);
    if (infd == -1) {
        printf("Failed to open first file %s\n", fileName[0]);
        return 1;
    }


    // Open/create the second file for writing
    outfd = open(fileName[1], O_CREAT | O_WRONLY, 0600);
    if (outfd == -1) {
        printf("Failed to open second file %s\n", fileName[1]);
        close(infd);
        return 1;
    }

	// Get the time when just before the while loop starts 
	gettimeofday(&startVal, NULL);
	
    // Now copy the contents of the first file to the second file
    while ((num_read = read(infd, buff, sizeof (buff))) > 0) {

        // Checks to see if the uppercase flag is 1 and changes the 
        // contents of the buffer to  uppercase
        if (uppercase_flag == 1) {
            for (i = 0; i<sizeof (buff); i++) {
                if (islower(buff[i]))
                    buff[i] = toupper(buff[i]);
            }
        }

        // Increment the buffer_iterations variable for each iteration of the while loop
        buffer_iterations++;
        num_write = write(outfd, buff, num_read);

        // Check for basic failure
        if (num_write != num_read) {
            printf("Problem - mismatch between write size and read size!\n");
        }
    }

    // Get the time when the while loop is finished    
    gettimeofday(&endVal, NULL);

	// if the size flag is selected this loop is processed
    if (size_flag == 1) {
		
		// if the buffer size is 1kb,  buffer_iterations is printed out unmodified
        if (sizeof (buff) == 1024) {
            printf("File Size is: %dkBytes\n", buffer_iterations);
        }
        
        // if the buffer size is 4kb, a 4x modifier is applied to buffer_iterations
        // this scales the file size to remain correct
        if (sizeof (buff) == 4096) {
            printf("File Size is: %dkBytes\n", buffer_iterations * 4);
        }
    }

	// if construct to check if the rate flag is set
    if (rate_flag == 1) {

		// Stores the time since epoch of gettimeofday(&startVal);
        start = startVal.tv_sec * 1000000 + startVal.tv_usec;
        printf("Start time: %f\n", start);

		// Stores the time since epoch of gettimeofday(&endVal);
        end = endVal.tv_sec * 1000000 + endVal.tv_usec;
        printf("End time: %f\n", end);

		// Subtracts start from end and stores this value as the elapsed time in microseconds
        elapsed = (unsigned long) (end - start);
        printf("Elapsed time: %lu\n", elapsed);
        
        // if there is a 1kb buffer
        if (sizeof (buff) == 1024) {
			
			// rate is (1kb / elapsed microseconds) * 1000000 i.e kb/s
            rate = (float) ((float) (buffer_iterations / (float) elapsed) * 1000000);
        }
        
        // if buffer is 4kb
        if (sizeof (buff) == 4096) {
			
			// rate is (4kb / elapsed microseconds) * 4000000 i.e (I hope) kb/s
            rate = (float) ((float) (buffer_iterations / (float) elapsed) * 4000000);
        }
        
        printf("Buffer iterations: %d\n", buffer_iterations);
        printf("The transfer rate was: %.2f kBytes/sec\n\n", rate);
    }

	// Gets the resource usage time value
    getrusage(RUSAGE_SELF, &ru);
    printf("Usage results for this program are:\n\n");
    printf("User space CPU time used: %6lu %6lu\n", ru.ru_utime.tv_sec, ru.ru_utime.tv_usec);
    printf("Kernel CPU time used:     %6lu %6lu\n", ru.ru_stime.tv_sec, ru.ru_stime.tv_usec);
    
    // Remember to close all open files before exit
    close(outfd);
    close(infd);

    return 0;
}
