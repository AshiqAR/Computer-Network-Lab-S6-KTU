#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/time.h>

#define port 3440


int total_packets = 10;

void send_window_packets(int start, int end, int sockfd, struct sockaddr_in serv_address,int f){
    char buffer[512];
    bzero(buffer, sizeof(buffer));
    sprintf(buffer, "%d",start);
    sleep(2);
    sendto(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&serv_address, sizeof(serv_address));
    bzero(buffer, sizeof(buffer));
    sprintf(buffer, "%d",end);
    sendto(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&serv_address, sizeof(serv_address));
    bzero(buffer, sizeof(buffer));
    sprintf(buffer, "%d",f);
    sendto(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&serv_address, sizeof(serv_address));
    if(f==1){
        for(int i=start;i<=end;i++){
            printf("Packet %d sent\n", i);
        }
    }
    else{
        printf("Packet %d sent\n", end);
    }
}

void main(){
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd < 0){
        perror("Socket creation failed\n");
        exit(0);
    }
    struct sockaddr_in serv_addr;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(port);
    serv_addr.sin_family = AF_INET;
    int len = sizeof(serv_addr);
    int window_size =3 ;
    // printf("Enter hte window size: ");
    // scanf("%d", &window_size);
    int start = 1;
    int end = 3;

    struct timeval timeout;
    timeout.tv_sec = 5;
    timeout.tv_usec = 0;

    int sock_status = setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
    char buffer[512];
    // bzero(buffer, sizeof(buffer));
    // sprintf(buffer, "%d",window_size);
    // sendto(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    // bzero(buffer, sizeof(buffer));
    // sprintf(buffer, "%s",total_packets);
    // sendto(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    int byteRec;
    int n;
    int f=1;
    while(start<=total_packets){
        send_window_packets(start, end, sockfd, serv_addr,f);
        bzero(buffer, sizeof(buffer));
        byteRec = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&serv_addr, &len);
        if(byteRec < 0){
            printf("Timeout error\nResending the window\n");
            f=1;
            continue;
        }
        n = atoi(buffer);
        printf("Acknowledgemnet for packet %d recieved\n",n);
        if(n== start){
            f = 0;
            if(start == total_packets){
                printf("All packets sent\n");
                exit(0);
            }
            if(end == total_packets){
                start++;
            }
            else{
                start = start+1;
                end = end+1;
            }
        }
        else{
            f = 1;
            printf("Timeout error\n");
            printf("Resending the window\n");
        }
    }
    

}

