#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<pthread.h>
#include "common.h"
#include "timer.h"

static char **theArray;
static pthread_rwlock_t *rwlocks;
static int n;

void *ServerEcho(void *arg) {
    	int clientFileDescriptor = *(int*)arg;
	free(arg);
	char forward_message[COM_BUFF_SIZE];
	char return_message[COM_BUFF_SIZE];
	//Need to read client request
	Clientrequest request;
	/* Clientrequest structure for reference
		typedef struct{
    			int pos;
    			int is_read;
    			char msg[COM_BUFF_SIZE];
		} ClientRequest; // To store the parsed client message
	*/

	read(clientFileDescriptor, forward_message, COM_BUFF_SIZE);
	//use ParseMsg
	ParseMsg(forward_message, &request);
	//Need to check what string
	//Read or write?
	//Need to get the time it takes to read/write

	//An idea was to have a read/write lock for every line in the
	//array so that it only locks that specific line and blocks
	//so that all reads/writes to other lines don't become blocked.

	/* If the request was a read do:
	*	pthread_rwlock_rdlock at particular position
	*	getContent, set string in return_message
	*	pthread_rwlock_unlock at particular position
	*/

	/* If the request was a write do:
        *       pthread_rwlock_wrlock at particular position
        *       setContent, set string to whatever the request was
	*	getContent, set string in return_message
        *       pthread_rwlock_unlock at particular position
        */

	write(clientFileDescriptor, return_message, COM_BUFF_SIZE);
	close(clientFileDescriptor);
    	return NULL;
}



int main(int argc, char* argv[]){
	if(argc != 4){ //Usage error, decide how to handle
		printf("Usage: %s <n> <ip> <port>\n", argv[0]);
		return -1;
	}
	//Initialize array, this is the critical data.
	//Needs to be initialized on the heap.
	thearray = malloc(n*sizeof(char*));
	for(int i = 0; i < n; i++){
		theArray[i] = malloc(COM_BUFF_SIZE);
		printf(theArray[i], "String %d: the initial value", i);
	}
	//Create socket, not sure how most of this works.
	//Pulled from simpleServer.c and chatGPT
	struct sockaddr_in sock_var;
    	int serverFileDescriptor=socket(AF_INET,SOCK_STREAM,0);
	n = atoi(argv[1]);
	char *ip = argv[2]
	int port = atoi(argv[3]);
	int serverFileDescriptor = socket(AF_INET, SOCK_STREAM, 0);
	sock_var.sin_addr.s_addr=inet_addr(ip);
	sock_var.sin_port=htons(port);
	sock_var.sin_family=AF_INET;

	if(bind(serverFileDescriptor,(struct sockaddr*)&sock_var,sizeof(sock_var))>=0)
    	{
        	printf("socket has been created\n");
        	listen(serverFileDescriptor,2000);
        	while(1){
			int *pclient = malloc(sizeof(int));
			*pclient = accept(serverFileDescriptor, NULL, NULL);
			pthread_t pthread_handle;
			pthread_create(&pthread_handle, NULL, ServerEcho, pclient);
			pthread_detach(pthread_handle);
		}
    	}
    	else{
        	printf("socket creation failed\n");
    	}
    	return 0;
}
