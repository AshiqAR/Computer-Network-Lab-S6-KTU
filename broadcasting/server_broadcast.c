#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SA struct sockaddr_in
#define MAX_CLIENT 512
#define MSG_LEN 4096

int cl_cnt = 0;
int cl_socks[MAX_CLIENT];
pthread_mutex_t mutex;

void* serve_cl(void *arg);
void broadcast_msg(char *msg, int len);
void handle_err(char *err_msg);


int main(int argc, char *argv[]){
    int sockfd,cl_sock;
    struct sockaddr_in sv_addr,cl_addr;
    int cl_addr_sz;
    pthread_t t_id;

    if(argc != 2){
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    pthread_mutex_init(&mutex,NULL);

    sockfd = socket(PF_INET, SOCK_STREAM, 0);
    memset(&sv_addr, 0, sizeof(sv_addr));
    sv_addr.sin_family = AF_INET;
    sv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    sv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(sockfd, (struct sockaddr*)&sv_addr, sizeof(sv_addr)) == -1)
        handle_err("ERROR: bind() fail");
    
    if(listen(sockfd, 5) == -1)
        handle_err("ERROR: listen() fail");

    system("clear");

    printf("Group chat linux server running... (port %s)\n",argv[1]);

    while(1){
        cl_addr_sz = sizeof(cl_addr);
        cl_sock = accept(sockfd, (struct sockaddr*)&cl_addr, &cl_addr_sz);

        pthread_mutex_lock(&mutex);
        cl_socks[cl_cnt++] = cl_sock;
        pthread_mutex_unlock(&mutex);

        pthread_create(&t_id, NULL, serve_cl, (void *) &cl_sock);
        pthread_detach(t_id);
        printf("Connected to client IP: %s\n", inet_ntoa(cl_addr.sin_addr));

    }
    close(sockfd);
    return 0;
}

void* serve_cl(void *arg){
    int cl_sock = *((int *)arg);
    int str_len = 0,i;
    char msg[MSG_LEN];

    while((str_len = read(cl_sock, msg, sizeof(msg))) != 0)
        broadcast_msg(msg, str_len);
    pthread_mutex_lock(&mutex);
    for(i=0;i<cl_cnt;i++){
        if(cl_sock == cl_socks[i]){
            while(i<cl_cnt){
                cl_socks[i] = cl_socks[i+1];
                i++;
            }
        }
    }
    cl_cnt--;
    pthread_mutex_unlock(&mutex);
    close(cl_sock);
    return NULL;
}

void broadcast_msg(char *msg, int len){
    int i;
    pthread_mutex_lock(&mutex);
    for(i=0;i<cl_cnt;i++)
        write(cl_socks[i],msg,len);
    pthread_mutex_unlock(&mutex);
}

void handle_err(char *err){
    fputs(err,stderr);
    fputc('\n',stderr);
    exit(1);
}
