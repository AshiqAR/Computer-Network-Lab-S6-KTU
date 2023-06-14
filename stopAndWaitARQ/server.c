#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <netinet/in.h>

#define port 8080

void main(){
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd < 0){
        perror("socket creation failed");
        exit(0);
    }
    struct sockaddr_in address,cl_addr;
    int len = sizeof(cl_addr);
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(port);
    address.sin_family = AF_INET;

    if(bind(sockfd, (struct sockaddr*)&address, sizeof(address)) < 0){
        perror("bind fail");
        exit(1);
    }

    char buffer[512];
    while(1){
        bzero(buffer, sizeof(buffer));
        recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&cl_addr, &len);
        printf("Packet%s recieved\n",buffer);
        sleep(5);
        if(rand()%2 != 0){
            printf("Ack for packet %s send\n",buffer);
            sendto(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&cl_addr, len);
        }
    }
    close(sockfd);
}