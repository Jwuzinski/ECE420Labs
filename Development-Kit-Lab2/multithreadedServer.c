#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<pthread.h>


void *ServerEcho(void *arg) {
    	int client_fd = *(int*)arg;
    	free(arg);
	//loop
    	//read request
	//parse
	//lock
	//get/set
	//unlock
	//write
	//close when done
    	return NULL;
}



int main(int argc, char* argv[]){
	struct sockaddr_in sock_var;
    	int serverFileDescriptor=socket(AF_INET,SOCK_STREAM,0);
    	int clientFileDescriptor;
    	int i;

	pthread_t t[1000];

	sock_var.sin_addr.s_addr=inet_addr("127.0.0.1");
	sock_var.sin_port=htons(3000);
	sock_var.sin_family=AF_INET;
	if(bind(serverFileDescriptor,(struct sockaddr*)&sock_var,sizeof(sock_var))>=0)
    	{
        	printf("socket has been created\n");
        	listen(serverFileDescriptor,2000);
        	while(1){
			int *pclient = malloc(sizeof(int));
			*pclient = accept(server_fd, null, null);

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
