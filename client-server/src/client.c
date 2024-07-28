#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define CHUNK_SIZE 1024
const char *termination_signal = "CLIENT_TERMINATED";

int main(int argc, char *argv[]) {
    /* user should give 4 arguments as input */
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <IP address> <PORT> <input file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    
    /* retrive ip address, port number and input file name from arguments*/
    const char *ip_address = argv[1];
    const int PORT = atoi(argv[2]);
    const char *input_file_path = argv[3];
    
    /* creating a socket for client*/
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        fprintf(stderr, "Error creating socket");
        exit(EXIT_FAILURE);
    }
    
    /* creating a connect request with server */
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(ip_address);
    
    /* server accept request then return 0 */
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        fprintf(stderr, "Error connecting to server");
        close(client_socket);
        exit(EXIT_FAILURE);
    }
    
    /* opening a file which client want to send */
    FILE *input_file = fopen(input_file_path, "r");
    if (input_file == NULL) {
        fprintf(stderr, "Error opening input file");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    /* initialize a buffer for 1024 CHUNK */
    char buffer[CHUNK_SIZE];
    size_t bytes_read;

    
    /* Send the input file to the server */
    printf("\nFile sending to server.\n");
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), input_file)) > 0) {
        
        ssize_t bytes_sent = write(client_socket, buffer, bytes_read);

        if (bytes_sent <= 0) {
            fprintf(stderr, "Error sending data");
            fclose(input_file);
            close(client_socket);
            exit(EXIT_FAILURE);
        }
    }

    /* Send an empty chunk to signal the end of the file */
    ssize_t bytes_sent = write(client_socket, "", 1);
    if (bytes_sent <= 0) {
        fprintf(stderr, "Error sending data");
        fclose(input_file);
        close(client_socket);
        exit(EXIT_FAILURE);
    }
    printf("\nFile sending to server. Completed.\n");
    fclose(input_file);


    /* Open a file to receive the modified content from the server */
    FILE *output_file = fopen("modified_output.txt", "w");
    if (output_file == NULL) {
        fprintf(stderr, "Error opening output file");
        close(client_socket);
        exit(EXIT_FAILURE);
    }
    
    /* receiving file from server */
    printf("\nFile receiving from server.\n");
    while (1) {
        ssize_t bytes_received = read(client_socket, buffer, CHUNK_SIZE);
        
        /* Server disconnected or error */
        if (bytes_received <= 0) {
            if (bytes_received == 0) {
                printf("Server disconnected.\n");
            } else {
                fprintf(stderr, "Error receiving data");
            }
            break;
        }

        /* Write received chunk to the file */
        fwrite(buffer, 1, bytes_received, output_file);

        /* Check for end of file */
        if (bytes_received < CHUNK_SIZE) {
            break;
        }
    }

    printf("\nFile receiving from server. completed.\n");
    fclose(output_file);
    
    /* send termination signal to server */
    write(client_socket, termination_signal, strlen(termination_signal));
    
    close(client_socket);

    return 0;
}
