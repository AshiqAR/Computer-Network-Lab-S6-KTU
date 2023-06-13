#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>

#define port 9889

int main(){
    int sockfd = socket (AF_INET , SOCK_DGRAM , 0);
    if(sockfd<0){
        perror("Socket creation failed");
        exit(0);
    }
    printf("socket created\n");

    struct sockaddr_in address;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(port);
    address.sin_family = AF_INET;

    if(bind(sockfd, (struct sockaddr*) &address, sizeof(address)) < 0){
        perror("Bind failed");
        exit(0);
    }
    char buffer[512];
    bzero(buffer,512);
    struct sockaddr_in cl_addr;
    int len = sizeof(cl_addr);
    while(1){
        int bytesRecieved = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&cl_addr, &len);
        printf("Packet %s recieved...\n", buffer);
        sleep(5);
        printf("Sending ack for Packet %s...\n", buffer);
        sendto(sockfd, buffer, 512, 0, (struct sockaddr*)&cl_addr, sizeof(cl_addr));
    }
    return(0);
}