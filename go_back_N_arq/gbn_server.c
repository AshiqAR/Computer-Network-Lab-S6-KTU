#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define port 3440

void main(){
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd < 0){
        perror("socket creation error");
        exit(0);
    }

    struct sockaddr_in address,cl_addr;
    int len = sizeof(cl_addr);
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(sockfd, (struct sockaddr*)&address, sizeof(address) ) < 0){
        perror("bind error");
        exit(0);
    }
    char buffer[512];
    // bzero(buffer, sizeof(buffer));
    // int byteRec = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&cl_addr, &len);
    // int window_size = atoi(buffer);
    // bzero(buffer, sizeof(buffer));
    // byteRec = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&cl_addr, &len);
    // int total_packets = atoi(buffer);

    int start;
    int end;
    int byteRec;
    int x,f;
    while(1){
        byteRec = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&cl_addr, &len);
        start = atoi(buffer);
        bzero(buffer,sizeof(buffer));
        byteRec = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&cl_addr, &len);
        end = atoi(buffer);
        bzero(buffer,sizeof(buffer));
        byteRec = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&cl_addr, &len);
        f = atoi(buffer);
        bzero(buffer,sizeof(buffer));
        if(f==1){
            for(int i=start;i<=end;i++){
                printf("Packet %d recieved\n",i);
            }
        }
        else{
            printf("packet %d recieved\n",end);
        }
        sleep(2);
        x = rand();
        if(x%3 == 0){
            sprintf(buffer, "%d", (rand()%(end-start+1))+start);
            sendto(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&cl_addr, len);
            printf("Ack for packed %s sent to client\n",buffer);
        }
        else if(x%3 == 1){
            sprintf(buffer, "%d", start);
            printf("Ack for packed %s sent to client\n",buffer);
            sendto(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&cl_addr, len);
        }
        else{

        }
        bzero(buffer, sizeof(buffer));
    }


    close(sockfd);
}
