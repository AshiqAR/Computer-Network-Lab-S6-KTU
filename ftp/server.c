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
    int bin;
    if(bin = bind(sockfd, (struct sockaddr*)&address, sizeof(address))<0){
        perror("bind failedd\n");
        exit(0);
    }
    printf("bind %d\n",bin);
    listen(sockfd, 10);
    struct sockaddr_in client_addr;
    int len = sizeof(client_addr);
    int cl_fd = accept(sockfd, (struct sockaddr*)&client_addr, &len);

    if(cl_fd<0){
        perror("coneenciton failed\n");
        exit(0);
    }
    printf("conected\n");
    char filename[512];
    bzero(filename, 512);
    int str_len;

    str_len = recv(cl_fd, filename, 512, 0);
    filename[str_len] = '\0';
    printf("Requested file: %s %ld\n", filename,strlen(filename));
    
    FILE *f  = fopen(filename, "r");
    if(f == NULL){
        printf("y");
        char output[2048];
        bzero(output,2048);
        sprintf(output, "no");
        send(cl_fd, output, sizeof(output), 0);
        printf("File doesnt exits\n");
    }
    else{
        printf("yes\n");
        char output[2048];
        int i=0;
        bzero(output, 2048);
        while(!feof(f)){
            output[i] = fgetc(f);
            printf("%c",output[i]);
            i++;
        }
        output[i-1] = '\0';
        send(cl_fd, output, strlen(output), 0);
        printf("%s",output);
        fclose(f);
    }
    printf("fjdkf");
    close(sockfd);
}
