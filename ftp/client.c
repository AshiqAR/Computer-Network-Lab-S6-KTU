#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#define port 4339

void main(){
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in address;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(port);
    address.sin_family = AF_INET;
    int len = sizeof(address);

    int con_status = connect(sockfd, (struct sockaddr*)&address, len);

    if(con_status<0){
        perror("coneenciton failed");
        exit(0);
    }
    char filename[512];
    bzero(filename,512);
    printf("Enter the file name :");

    scanf("%s",filename);

    send(sockfd, filename, strlen(filename), 0);

    char output[2048];
    bzero(output, 2048);

    int byteRecieved = recv(sockfd, output, 2048, 0);
    printf("hello");

    // output[byteRecieved] = '\0';
    if(strcmp(output, "no") == 0){
        printf("file does not exist\n");
    }
    else{
        int i=0;
        FILE *f = fopen("recievedfile.txt","w");
        while(output[i] != '\0'){
            fputc(output[i], f);
            i++;
        }
        // do{
        //     fputc(output[i], f);
        //     i++;
        // }while(output[i] != '\0');
        fclose(f);
    }
    close(sockfd);
}

