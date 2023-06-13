#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <arpa/inet.h>  

#define port 9559

void *recieve_message_from_server(void *socket){
    int client_socket = *((int *) socket);
    while(1){
        char data[1024];
        int byteRecieved = recv(client_socket, data, sizeof(data),0);
        if(byteRecieved == 0){
            continue;
        }
        data[byteRecieved] = '\0';
        printf("%s\n",data);
    }
}

void main(){
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(client_socket<0){
        perror("client socket creation failed");
        exit(0);
    }

    struct sockaddr_in server_address;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(port);
    server_address.sin_family = AF_INET;

    int connection_status = connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address));
    if(connection_status<0){
        perror("Connecting to server failed");
        exit(1);
    }
    printf("Connected to server...\n");
    pthread_t thread;
    pthread_create(&thread, NULL, recieve_message_from_server, (void *)&client_socket);

    while(1){
        char data[1024];
        scanf(" %[^\n]",data);
        if(strcmp(data, "QUIT") == 0){
            break;
        }
        send(client_socket, data, 1024,0);
        printf("You : %s\n", data);
    }
    close(client_socket);
}
