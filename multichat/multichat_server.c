#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX_CLIENT 512
#define MSG_LEN 4096

int cl_cnt = 0;
int cl_socks[MAX_CLIENT];
pthread_mutex_t mutex;

struct client{
    int sock;
    char name[50];
};

struct client c[MAX_CLIENT];

void* serve_cl(void *arg);
void forward_msg(char *msg, int len, int sender_sock);
void handle_err(char *err_msg);
int search_sock(char name[]){
    for(int i=0;i<cl_cnt;i++){
        if(strcmp(c[i].name,name) == 0)
            return i;
    }
    return -1;
}


int main(int argc, char *argv[]){
    int sockfd,cl_sock;
    struct sockaddr_in sv_addr,cl_addr;
    int cl_addr_sz;
    pthread_t t_id;
    char name_of_cl[50];

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
        c[cl_cnt].sock = cl_sock;
        read(cl_sock, name_of_cl, sizeof(name_of_cl));
        strcpy(c[cl_cnt].name, name_of_cl);
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
        forward_msg(msg, str_len, cl_sock);
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

void forward_msg(char *msg, int len, int sender_sock){
    int i,k=0,dest_sock;
    char name[50];
    pthread_mutex_lock(&mutex);
    if(strncmp("++++",msg,4) == 0 || strncmp("----",msg,4) == 0){
        for(i=0;i<cl_cnt;i++)
            write(cl_socks[i],msg,len);
    }
    else if(strncmp("SEND",msg,4) == 0){
        for(i = 0;i<len;i++){
            if(msg[i] == ' ')
                break;
        }
        i++;
        do{
            name[k++] = msg[i++];
        }while((char)i != ' ');
        name[--k] = '\0';
    }
    if(dest_sock = search_sock(name) != -1){
        write(dest_sock, msg, len);
    }
    else{
        write(sender_sock, "invalid input\n To send a msg: 'SEND' <name_of_reciever>\n",sizeof("invalid input\n To send a msg: 'SEND' <name_of_reciever>\n"));
    }
    pthread_mutex_unlock(&mutex);
}

void handle_err(char *err){
    fputs(err,stderr);
    fputc('\n',stderr);
    exit(1);
}
