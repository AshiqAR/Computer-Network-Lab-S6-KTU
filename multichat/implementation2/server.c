#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <stdlib.h>

#define port 9559

int count = 0;

typedef struct client{
    int index;
    int client_socket;
    struct sockaddr_in client_address;
}client;

client c[1024];
pthread_t thread[1024];

void *serve_client(void *client_details){
    client x = *((client *)client_details);
    int index = x.index;
    int client_socket = x.client_socket;

    printf("Client %d connected\n", index+1);
    char data[1024];
    char output[2000];

    while(1){

        int bytesRecieved = recv(client_socket, data, sizeof(data),0);
        if(bytesRecieved == 0){
            break;
        }
        data[bytesRecieved] = '\0';
        printf("\nClient %d: %s\n",index+1, data);
        sprintf(output, "Client %d: %s", index+1, data);
        for(int i=0;i<count;i++){
            if(i!=index)
                send(c[i].client_socket, output, sizeof(output),0);
        }
        bzero(data, sizeof(data));
        bzero(output, sizeof(output));
    }
}

void main(){
    // SOCKET
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0){
        perror("Socket creation failed");
        exit(1);
    }

    // configuration
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(port);

    // Bind
    int bind_status = bind(sockfd, (struct sockaddr*)&address, sizeof(address));
    if(bind_status<0){
        perror("Server socket bind error");
        exit(1);
    }

    // Listen to incoming communication
    listen(sockfd, 1024);
    printf("Listening for incoming communication\n");

    while(1){
        socklen_t len = sizeof(c[count].client_address);
        c[count].index = count;
        c[count].client_socket = accept(sockfd, (struct sockaddr*)&c[count].client_address, &len);

        pthread_create(&thread[count], NULL, serve_client, (void *)&c[count]);
        count ++;
    }
    for(int i=0;i<count;i++){
        pthread_join(thread[i],NULL);
    }
}
