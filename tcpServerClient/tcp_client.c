// tcp connection between server and client
// client and server communicate between each other

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include <netinet/in.h>

#define MSG_LEN 4096

void func(int sock_fd){
    int n ;
    char msg[MSG_LEN];
    // system("clear");
    printf("\n\n-- Enter 'quit' to exit --\n\n");
    while(strncmp(msg,"quit", 4) != 0 ){
        bzero(msg, MSG_LEN);
        printf("To server: ");

        n = 0;
        while((msg[n++] = getchar()) != '\n');

        write(sock_fd, msg, sizeof(msg));
        bzero(msg, MSG_LEN);
        read(sock_fd, msg, sizeof(msg));
        printf("[server] %s",msg);
    }
    printf("Client Exit\n");
}

void main(int argc, char *argv[]){
    int sock_fd;
    struct sockaddr_in sv_addr, cl_addr;

    if(argc != 2){
        printf("Usage: %s <port>\n",argv[0]);
        exit(0);
    }
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(sock_fd == -1){
        printf("socket creation failed...\n");
        exit(0);
    }
    printf("socket created...\n");

    sv_addr.sin_family = AF_INET;
    sv_addr.sin_port = htons(atoi(argv[1]));
    sv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if(connect(sock_fd, (struct sockaddr*)&sv_addr, sizeof(sv_addr)) == -1){
        printf("connection to the server failed...\n");
        exit(0);
    }
    printf("connected to server...\n");

    func(sock_fd);

    close(sock_fd);
}
