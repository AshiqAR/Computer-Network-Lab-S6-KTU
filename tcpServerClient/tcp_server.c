#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include <netinet/in.h>

#define MSG_LEN 4096

void call_func(int cl_fd){
    
}

void main(int argc, char *argv[]){
    int sock_fd;
    int cl_fd;
    struct sockaddr_in sv_addr, cl_addr;
    if(argc != 2){
        printf("Usage: %s <port>",argv[0]);
        exit(0);
    }

    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(sock_fd == -1){
        printf("socket creation failed...\n");
        exit(1);
    }
    printf("socket created successfully\n");

    sv_addr.sin_family = AF_INET;
    sv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    sv_addr.sin_port = htons(atoi(argv[1]));

    if( bind(sock_fd, (struct sockaddr*)&sv_addr, sizeof(sv_addr)) == 0){
        printf("socket bind failed...\n");
        exit(0);
    }
    printf("socket bind successful...\n");

    if( listen(sock_fd, 5) == -1){
        printf("listen failed...\n");
        exit(0);
    }
    printf("server listening...\n");

    cl_fd = accept(sock_fd, (struct sockaddr*)&cl_addr, sizeof(cl_addr));
    if(cl_fd < 0){
        printf("server accept failed\n");
        exit(0);
    }
    printf("server accept the client\n");
    call_func(cl_fd);

    close(sock_fd);
}
