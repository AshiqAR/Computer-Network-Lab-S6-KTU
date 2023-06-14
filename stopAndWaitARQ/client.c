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
    if(sockfd<0){
        perror("socket error");
        exit(0);
    }

    struct sockaddr_in serv_addr;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    int len = sizeof(serv_addr);

    int n = 10;
    struct timeval timeout;
    timeout.tv_sec = 8;
    timeout.tv_usec = 0;

    int setsock_status = setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
    if(setsock_status < 0){
        perror("setsock error");
        exit(0);
    }

    char buffer[512];
    int i=1;

    while(i<n){
        bzero(buffer, sizeof(buffer));
        sprintf(buffer, "%d", i);
        sleep(3);
        sendto(sockfd, buffer , sizeof(buffer), 0, (struct sockaddr*)&serv_addr, len);
        printf("Packet %d sent\n",i);
        bzero(buffer, sizeof(buffer));
        int byterecieved = recvfrom (sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&serv_addr, &len);
        if(byterecieved < 0){
            printf("Time out error\nResending the packet\n");
        }
        else{
            printf("Ack for packet %s recieved\n", buffer);
            i++;
        }
    }
    close(sockfd);
}