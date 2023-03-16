// server connects to two clients
// client1 sends a character to the server
// server increments the character by 1
// returns the new character to client2

#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX 80
#define PORT 5003
#define SA struct sockaddr


void func(int connfd1,int connfd2) {
    char buff[MAX];

    read(connfd1, buff, sizeof(buff));
    printf("from client1: %c\n",buff[0] );
    if(buff[0] != 'z' && buff[0] != 'Z')
        buff[0]++;
    else if(buff[0] == 'z')
        buff[0] = 'a';
    else if(buff[0] == 'Z')
        buff[0] = 'A';
    else 
        buff[0]++;

    write(connfd2, buff,sizeof(buff));
}


int main(){
    int sockfd, connfd1,connfd2, len;
    struct sockaddr_in servaddr, cli;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
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

    connfd1 = accept(sockfd, (SA*)&cli, &len);
    if (connfd1 < 0) {
        printf("server accept failed...\n");
        exit(0);
    }
    printf("server accepted the client1...\n");

    connfd2 = accept(sockfd, (SA*)&cli, &len);
    if (connfd2 < 0) {
        printf("server accept failed...\n");
        exit(0);
    }
    else
        printf("server accepted the client2...\n");

    func(connfd1,connfd2);

    close(sockfd);
}
