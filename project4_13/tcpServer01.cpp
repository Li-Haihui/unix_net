#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include <errno.h>  
#include <sys/types.h>  
#include <sys/socket.h>  
#include <netinet/in.h>
#include <arpa/inet.h>

#include <time.h>
#include <unistd.h>


#define SERV_PORT (9877)
#define LISTEN_NUM (10)
#define MAX_BUFFER (1024)

size_t writen(int fd, const void *vptr, size_t n)
{
    size_t nleft;
    size_t nwritten;
    const char *ptr;

    ptr = (char*)vptr;
    nleft = n;
    while(nleft > 0)
    {
        if((nwritten = write(fd, ptr, nleft)) <= 0)
        {
            if((nwritten < 0) && errno == EINTR)
            {
                nwritten = 0;
            }
            else
                return -1;
        }

        nleft = nleft - nwritten;
        ptr = ptr + nwritten;
    }
    return n;
}

int main(int argc, char **argv)
{
    int sockfd, connfd;
    int ret;
    struct sockaddr_in serverAddr, clientAddr;
    char readl[MAX_BUFFER], writel[MAX_BUFFER], buff[MAX_BUFFER];
    time_t ticks;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0)
    {
        printf("server create socket error: %s(errno: %d)\n",strerror(errno),errno);
        exit(0);
    }

    bzero(&serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(SERV_PORT);

    ret = bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if(ret < 0)
    {
        printf("bind socket error:%s(errno:%d)\n", strerror(errno),errno);
        close(sockfd);
        exit(0);
    }

    ret = listen(sockfd, LISTEN_NUM);
    if(ret < 0)
    {
        printf("server listen error:%s(errno:%d)\n", strerror(errno), errno);
        close(sockfd);
        exit(0);
    }

    while(1)
    {
        socklen_t clientAddrLen = sizeof(clientAddr);
        connfd = accept(sockfd, (struct sockaddr*)&clientAddr, &clientAddrLen);
        pid_t fpid = fork();
        if(fpid < 0)
        {
            printf("server fork error:%s(errno:%d)\n", strerror(errno), errno);
        }
        else if(fpid == 0)
        {
            while(1)
            {
                close(sockfd);
                printf("connection from %s, prot %d\n", 
                    inet_ntop(AF_INET, &clientAddr.sin_addr, buff, sizeof(buff)), ntohs(clientAddr.sin_port));

                int datalen = read(connfd, readl,MAX_BUFFER);
                printf("read data:%s", buff);
    
                ticks = time(NULL);
                snprintf(writel, sizeof(writel), "%.24s\r\n", ctime(&ticks));
                datalen = writen(connfd, writel, strlen(writel));
                if(datalen > 0)
                {
                    printf("writen data:%s(len:%d)\n", writel, datalen);
                }
                
            }      
        }
        else
        {
            close(connfd);
        }
    }

    close(sockfd);
}

