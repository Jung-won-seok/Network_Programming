#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

void error_handling(char *message);
void cmdexe(int cmd_cnt, char *buf);

int main(int argc, char *argv[])
{
    int serv_sock;
    char buf[BUF_SIZE];
    int recv_len, cmd_cnt;
    struct sockaddr_in serv_adr, clnt_adr;
    socklen_t clnt_adr_sz;
    if(argc != 2){
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_DGRAM, 0);
    if(serv_sock == -1)
        error_handling("socket() error");

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
     error_handling("bind() error");
    clnt_adr_sz=sizeof(clnt_adr);

    while(1)
    {
        printf("Waiting new clinet message................\n");
        recv_len = recvfrom(serv_sock, buf, BUF_SIZE, 0, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);

        if(recv_len < 2)
        continue;
        cmd_cnt = buf[0];
        cmdexe(cmd_cnt, &buf[1]);

        printf("End of commands exe from client.............\n\n");
}
    
    close(serv_sock);
    return 0;
}

void cmdexe(int cmd_cnt, char *buf)
{
    char cmd[10];
    int cmd_len, i;
    int pos = 0;


    for(i=0 ; i<cmd_cnt ; i++)
    {
        cmd_len = buf[pos++];
        strncpy(cmd, &buf[pos], cmd_len);
        pos += cmd_len;
        cmd[cmd_len] = '\0';
        printf("[%s]================\n", cmd);
        system(cmd);
    }
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
