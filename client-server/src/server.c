#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctype.h>
#include <pthread.h>
 
#define CHUNK_SIZE 1024
#define MAX_THREADS 5

void *handle_client(void *arg);

/* created threads information */
struct threadInfo{
	int client_socket;
	pthread_t thread_id;
};
 
/* mutex and condition for allowing multiple threads */
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_var = PTHREAD_COND_INITIALIZER;

/* shared data between multiple threads */
int active_client = 0;
int filenum = 0;

int main(int argc, char *argv[]) {
    
    /* user should give 2 arguments as input */
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <PORT>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    
    /* creating a socket for server*/
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        fprintf(stderr, "Error creating socket");
        exit(EXIT_FAILURE);
    }
    printf("\nsocket created.\n");
    
    /* creating a binding for memory assign */
    const int PORT = atoi(argv[1]);
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;
 
    /* binding request accept then return 0 */
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        fprintf(stderr, "Error binding");
        close(server_socket);
        exit(EXIT_FAILURE);
    }
    printf("\nbinidng completed.\n");

    /* server is ready to listen */ 
    if (listen(server_socket, MAX_THREADS) == -1) {
        fprintf(stderr, "Error listening");
        close(server_socket);
        exit(EXIT_FAILURE);
    }
    printf("Server listening on port : %d\n", PORT);
 
    /* creating a threads */ 
    struct threadInfo threadUser[MAX_THREADS];
    
    /* set client_socket of all threads to -1 for future use */
    for(int i = 0; i < MAX_THREADS; i++){
    	threadUser[i].client_socket = -1;
    }
    
    /* creating and destroying the threads */
    while(1){
            
            /* accept request from one of multiple clients */
    	    int client_socket = accept(server_socket, NULL, NULL);
	    if (client_socket == -1) {
		    fprintf(stderr, "Error accepting connection");
		    close(server_socket);
		    exit(EXIT_FAILURE);
	    }
	    
	    /* lock the mutex for accessing the critical sections or variables */
	    pthread_mutex_lock(&mutex);
	    
	    /* if client active more than limit then wait for spaces for other client */
	    while(active_client >= MAX_THREADS){
	    	pthread_cond_wait(&cond_var, &mutex);
	    }
	    
	    /* creating a thread which is not used */
	    for (int i = 0; i < MAX_THREADS; ++i) {
	        
	        /* create thread and assign thread function for thread execution */
		if(threadUser[i].client_socket == -1){
		    printf("\nCreating a thread\n");
		    threadUser[i].client_socket = client_socket;
		    pthread_create(&threadUser[i].thread_id, NULL, handle_client, (void *)&threadUser[i]);
		    active_client++;
		    printf("\nActive Client is : %d\n", active_client);
		    break;
		}
	    }
	    pthread_mutex_unlock(&mutex);
    }
    
     /* wait for all threads to complete */
     for(int i = 0; i < MAX_THREADS; ++i){
	   	if(threadUser[i].client_socket != -1){
	   	    pthread_join(threadUser[i].thread_id, NULL);
	   	}
     }
    
    /* close the socket of server */
    close(server_socket); 
    
    return 0;
}

void *handle_client(void *arg) {

    printf("\nthread handler start.\n");
    
    /* retrive info of client socket from thread structure. */
    struct threadInfo *thread_info = (struct threadInfo *) arg;
    int client_socket = thread_info->client_socket; 
    char buffer[CHUNK_SIZE];
    ssize_t bytes_received;
    char filename[50];
    
    /* creating a file name */
    filenum++;
    sprintf(filename, "file%d.txt", filenum);
    
    /* opening a file which name is created */ 
    FILE *input_file = fopen(filename, "w");
    if (input_file == NULL) {
        fprintf(stderr, "Error opening received file");
        close(client_socket);
        pthread_exit(NULL);
    }
    
    /* writing data from to opened file which is received from client */
    printf("\nfile receiving from client.\n");
    while (1) {
        bytes_received = read(client_socket, buffer, CHUNK_SIZE);
 
        /* Client disconnected or error */
        if (bytes_received <= 0) {
            if (bytes_received == 0) {
                printf("Client disconnected.\n");
            } else {
                fprintf(stderr, "Error receiving data");
            }
            break;
        }
 
        /* Write received chunk to the file */
        fwrite(buffer, 1, bytes_received, input_file);
 
        /* Check for end of file */
        if (bytes_received < CHUNK_SIZE) {
            break;
        }
    }
    fclose(input_file);
    printf("\nfile receiving from client. successful.\n");
    
    /* Open the received file for reading */
    FILE *output_file = fopen(filename, "r");
    if (output_file == NULL) {
        fprintf(stderr, "Error opening received file for reading");
        close(client_socket);
        pthread_exit(NULL);
    }
    printf("\nfile sending to client.\n"); 
    
    /* Convert the content to uppercase and send it back to the client */
    while (1) {
        size_t bytes_read = fread(buffer, 1, CHUNK_SIZE, output_file);
 
        if (bytes_read > 0) {
            /* Convert the chunk to uppercase */
            for (size_t i = 0; i < bytes_read; i++) {
                buffer[i] = toupper(buffer[i]);
            }
 
            /* Send the modified chunk back to the client */
            ssize_t bytes_sent = write(client_socket, buffer, bytes_read);
            if (bytes_sent <= 0) {
                fprintf(stderr, "Error sending data");
                fclose(output_file);
                close(client_socket);
                pthread_exit(NULL);
            }
        }
 
        /* Check for end of file */
        if (bytes_read < CHUNK_SIZE) {
            break;
        }
    }
    
    fclose(output_file);
    printf("\nfile sending to client. successful.\n");
    
    /* wait for termination signal receive from client */
    while(1){
    	bytes_received = read(client_socket, buffer, CHUNK_SIZE);
    	if(strcmp(buffer, "CLIENT_TERMINATED")){
    		break;
    	}
    }
    
    /* lock the mutex for accessing the critical sections or variables */
    pthread_mutex_lock(&mutex);
    active_client--;
    thread_info->client_socket = -1;
    pthread_cond_signal(&cond_var);
    pthread_mutex_unlock(&mutex);
    
    close(client_socket);
    printf("\nthread handler over.\n");
    printf("\nActive Client is : %d\n", active_client);
    pthread_exit(NULL);
}
