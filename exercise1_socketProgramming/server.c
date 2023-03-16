// reads a string as input from client
// server returns the string replacing the vowels with '$'

#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX 80
#define PORT 5004
#define SA struct sockaddr

void func(int connfd) {
    char buff[MAX];
    read(connfd, buff, sizeof(buff));
    for(int i=0;i<strlen(buff);i++){
        if(buff[i] == 'a' ||buff[i] == 'e' ||buff[i] == 'i' ||buff[i] == 'o' ||buff[i] == 'u' )
            buff[i] = '$';
    }
    write(connfd,buff,sizeof(buff));
}

int main(){
    int sockfd, connfd, len;
    struct sockaddr_in servaddr, cli;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
        printf("socket bind failed...\n");
        exit(0);
    }
    printf("Socket successfully binded..\n");

    if ((listen(sockfd, 5)) != 0) {
        printf("Listen failed...\n");
        exit(0);
    }
    printf("Server listening..\n");
    len = sizeof(cli);

    connfd = accept(sockfd, (SA*)&cli, &len);
    if (connfd < 0) {
        printf("server accept failed...\n");
        exit(0);
    }
    printf("server accept the client...\n");

    func(connfd);

    close(sockfd);
}
