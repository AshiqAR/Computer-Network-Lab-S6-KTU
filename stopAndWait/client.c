#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#define port 9889

int main(){
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd<0){
        perror("Socket creation failed");
        exit(1);
    }
    struct sockaddr_in serv_addr;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    int i=1;
    char buffer[512];
    memset(buffer,0, 512);

    struct sockaddr_in addr;
    int len = sizeof(addr);

    while(i<10){
        sprintf(buffer, "%d",i);
        printf("Packet %d sent...\n",i);
        int n = sendto(sockfd, buffer, 512, 0, (struct sockaddr*)&serv_addr ,sizeof(serv_addr));
        bzero(buffer,512);
        recvfrom(sockfd, buffer, 512, 0, (struct sockaddr*)&addr, &len);
        printf("Acknowledgement for Packet %d recieved\n",i);
        sleep(5);
        i++;
    }
    return 0;
}