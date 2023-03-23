#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <pthread.h>

#define MSG_LEN 4096
#define NAME_LEN 64

void* send_msg(void *arg);
void* recv_msg(void *arg);
void handle_err(char *err_msg);

char name[NAME_LEN];
char msg[MSG_LEN];
char info_msg[MSG_LEN];

int main(int argc,char *argv[]){
    int sock;
    struct sockaddr_in sv_addr;
    pthread_t send_thread,recv_thread;
    void *thread_return;

    if(argc != 4){
        printf("Usage: %s <serverIP> <port> <name>\n",argv[0]);
        exit(1);
    }
    sprintf(name, "%s",argv[3]);

    sock = socket(PF_INET, SOCK_STREAM, 0);

    memset(&sv_addr, 0, sizeof(sv_addr));
    sv_addr.sin_family = AF_INET;
    sv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    sv_addr.sin_port = htons(atoi(argv[2]));

    if(connect(sock, (struct sockaddr*)&sv_addr , sizeof(sv_addr)) == -1)
        handle_err("ERROR: connect() fail");

    system("clear");
    printf("Welcome to group chat! (Q/q to quit)\n\n");
    sprintf(msg,"\n+++++ %s has joined! +++++\n\n",name);
    write(sock,msg,strlen(msg));

    pthread_create(&send_thread, NULL, send_msg, (void*)&sock);
    pthread_create(&recv_thread, NULL, recv_msg, (void*)&sock);
    pthread_join(send_thread, &thread_return);
    pthread_join(recv_thread, &thread_return);
    close(sock);
    return 0;
}

void* send_msg(void *arg){
    int sock = *((int *)arg);
    char name_msg[MSG_LEN+NAME_LEN+2];
    while(1){
        fgets(msg, MSG_LEN, stdin);

        if( strcmp(msg,"q\n") == 0 || strcmp(msg,"Q\n") == 0 ){
            sprintf(msg,"----- %s has left! -----\n", name);
            write(sock, msg, strlen(msg));
            
            close(sock);
            exit(0);
        }
        sprintf(name_msg, "[%s] %s",name,msg);
        write(sock, msg, strlen(msg));
    }
}

void* recv_msg(void *arg){
    int sock = *((int *)arg);
    char name_msg[NAME_LEN + MSG_LEN + 2];
    char msg[MSG_LEN];
    int str_len;
    while(1){
        str_len = read(sock, msg, MSG_LEN);
        if(str_len == -1)
            return (void*)-1;
        
        msg[str_len] = 0;
        fputs(name_msg, stdout);
    }
}

void handle_err(char *err_msg){
    fputs(err_msg, stderr);
    fputc('\n',stderr);
    exit(1);
}
