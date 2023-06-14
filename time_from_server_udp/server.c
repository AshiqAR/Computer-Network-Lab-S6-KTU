#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>


#define port 8778

void main(){
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd<0){
        perror("Sokcet failed");
        exit(0);
    }

    struct sockaddr_in address ;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_family = AF_INET;
    address.sin_port = htons(port);

    if(bind(sockfd, (struct sockaddr*)&address, sizeof(address)) < 0){
        perror("bind failed");
        exit(0);
    }

    struct sockaddr_in client_address;
    int len = sizeof(client_address);
    char buffer[1024];
    bzero(buffer, 1024);

    int byteRecieved = recvfrom(sockfd, buffer, 1024, 0, (struct sockaddr*)&client_address, &len );
    // buffer[byteRecieved] = '\0';
    printf("Client: %s\n",buffer);
    bzero(buffer, 1024);

    time_t t;
    t = time(NULL);
    strcpy(buffer, ctime(&t));


    printf("Time to client: %s",buffer);;
    sendto(sockfd, buffer, 1024, 0,(struct sockaddr*)&client_address, len);
}