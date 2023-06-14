#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>

#define port 8778

void main(){
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd< 0){
        perror("socket creation failed");
        exit(1);
    }

    struct sockaddr_in serv_addr ;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    char data[1024];
    strcpy(data, "What is the time now?");
    printf("Client : %s",data);
    sendto(sockfd , data, 1024, 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    bzero(data,1024);
    int len = sizeof(serv_addr);
    int byteRecieved = recvfrom(sockfd, data, 1024, 0, (struct sockaddr*)&serv_addr ,&len);
    data[byteRecieved] = '\0';
    printf("\nTime from server: %s\n",data );
}
