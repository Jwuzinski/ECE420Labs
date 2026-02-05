/*
 * Multithreaded Server
 * ECE420 Lab 2
 * 
 * This server handles concurrent read/write requests to a shared array of strings.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include "common.h"
#include "timer.h"

/* Global variables */
int NUM_STR;                        // Number of positions in the array
char **theArray;                    // The shared string array
pthread_rwlock_t *rwlocks;          // Per-position reader-writer locks
double *request_times;              // Array to store latency measurements
int request_counter = 0;            // Counter for completed requests
pthread_mutex_t counter_mutex;      // Mutex to protect counter

/* Thread function to handle client requests */
void *HandleClient(void *args) {
    int clientFileDescriptor = (int)(long)args;
    char str_msg[COM_BUFF_SIZE];
    char str_rcv[COM_BUFF_SIZE];
    ClientRequest rqst;
    double start, finish, elapsed;
    
    // Read the client request
    int bytes_read = read(clientFileDescriptor, str_msg, COM_BUFF_SIZE);
    if (bytes_read <= 0) {
        close(clientFileDescriptor);
        return NULL;
    }
    
    // Parse the client message
    ParseMsg(str_msg, &rqst);
    
    // Validate position
    if (rqst.pos < 0 || rqst.pos >= NUM_STR) {
        close(clientFileDescriptor);
        return NULL;
    }
    
    // Start timing for array access latency
    GET_TIME(start);
    
    if (rqst.is_read) {
        // Read operation - acquire read lock
        pthread_rwlock_rdlock(&rwlocks[rqst.pos]);
        getContent(str_rcv, rqst.pos, theArray);
        pthread_rwlock_unlock(&rwlocks[rqst.pos]);
    } else {
        // Write operation - acquire write lock
        pthread_rwlock_wrlock(&rwlocks[rqst.pos]);
        setContent(rqst.msg, rqst.pos, theArray);
        getContent(str_rcv, rqst.pos, theArray);
        pthread_rwlock_unlock(&rwlocks[rqst.pos]);
    }
    
    // End timing for array access latency
    GET_TIME(finish);
    elapsed = finish - start;
    
    // Record the latency
    pthread_mutex_lock(&counter_mutex);
    int current_index = request_counter;
    request_times[current_index] = elapsed;
    request_counter++;
    
    // Save times every 1000 requests
    if (request_counter == COM_NUM_REQUEST) {
        saveTimes(request_times, COM_NUM_REQUEST);
        request_counter = 0;  // Reset for next batch
    }
    pthread_mutex_unlock(&counter_mutex);
    
    // Send response back to client
    write(clientFileDescriptor, str_rcv, COM_BUFF_SIZE);
    
    // Close the client connection
    close(clientFileDescriptor);
    
    return NULL;
}

int main(int argc, char *argv[]) {
    struct sockaddr_in sock_var;
    int serverFileDescriptor;
    int clientFileDescriptor;
    int i;
    pthread_t thread_handles[COM_NUM_REQUEST];
    int thread_count = 0;
    
    /* Check command line arguments */
    if (argc != 4) {
        fprintf(stderr, "usage: %s <n positions> <server IP> <server port>\n", argv[0]);
        exit(1);
    }
    
    /* Parse command line arguments */
    NUM_STR = strtol(argv[1], NULL, 10);
    char *server_ip = argv[2];
    int server_port = strtol(argv[3], NULL, 10);
    
    /* Initialize the shared string array */
    theArray = (char **)malloc(NUM_STR * sizeof(char *));
    for (i = 0; i < NUM_STR; i++) {
        theArray[i] = (char *)malloc(COM_BUFF_SIZE * sizeof(char));
        sprintf(theArray[i], "String %d: the initial value", i);
    }
    
    /* Initialize per-position reader-writer locks */
    rwlocks = (pthread_rwlock_t *)malloc(NUM_STR * sizeof(pthread_rwlock_t));
    for (i = 0; i < NUM_STR; i++) {
        pthread_rwlock_init(&rwlocks[i], NULL);
    }
    
    /* Initialize request timing array and counter mutex */
    request_times = (double *)malloc(COM_NUM_REQUEST * sizeof(double));
    pthread_mutex_init(&counter_mutex, NULL);
    
    /* Create server socket */
    serverFileDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (serverFileDescriptor < 0) {
        perror("socket creation failed");
        exit(1);
    }
    
    /* Allow socket reuse to avoid "Address already in use" errors */
    int opt = 1;
    if (setsockopt(serverFileDescriptor, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt failed");
        exit(1);
    }
    
    /* Configure socket address */
    sock_var.sin_addr.s_addr = inet_addr(server_ip);
    sock_var.sin_port = server_port;
    sock_var.sin_family = AF_INET;
    
    /* Bind socket to address */
    if (bind(serverFileDescriptor, (struct sockaddr *)&sock_var, sizeof(sock_var)) < 0) {
        perror("bind failed");
        exit(1);
    }
    
    // Listen for incoming connections 
    if (listen(serverFileDescriptor, COM_NUM_REQUEST) < 0) {
        perror("listen failed");
        exit(1);
    }
    
    /* Main server loop - accept connections indefinitely */
    while (1) {
        /* Accept client connection */
        clientFileDescriptor = accept(serverFileDescriptor, NULL, NULL);
        if (clientFileDescriptor < 0) {
            continue;  // Skip failed connections
        }
        
        /* Create thread to handle the client request */
        if (pthread_create(&thread_handles[thread_count], NULL, HandleClient, 
                          (void *)(long)clientFileDescriptor) != 0) {
            close(clientFileDescriptor);
            continue;
        }
        
        /* Detach thread so resources are automatically freed */
        pthread_detach(thread_handles[thread_count]);
        
        thread_count = (thread_count + 1) % COM_NUM_REQUEST;
    }
    
    /* Cleanup (unreachable in this implementation) */
    close(serverFileDescriptor);
    pthread_mutex_destroy(&counter_mutex);
    for (i = 0; i < NUM_STR; i++) {
        pthread_rwlock_destroy(&rwlocks[i]);
        free(theArray[i]);
    }
    free(theArray);
    free(rwlocks);
    free(request_times);
    
    return 0;
}
