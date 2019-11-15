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


size_t readn(int fd, void *vptr, size_t n)
{
    size_t nleft;
    size_t nread;
    char *ptr;

    ptr = (char*)vptr;
    nleft = n;
    while(nleft > 0)
    {
        if((nread = read(fd, ptr, nleft)) < 0)
        {
            if(errno == EINTR)
            {
                nread = 0;
            }
            else
            {
                return -1;
            }
        }
        else if(nread = 0)
        {
            break;
        }

        nleft = nleft - nread;
        ptr = ptr + nread;
    }

    return n - nleft;
}

void str_client(FILE *fp, int sockfd)
{
    char sendl[MAX_BUFFER], recvl[MAX_BUFFER];
    while(fgets(sendl, MAX_BUFFER, fp) != NULL)
    {
        writen(sockfd, sendl, strlen(sendl));

        if(readn(sockfd, recvl, MAX_BUFFER) < 0)
        {
            printf("str_client read error:%s(errno:%d)\n", strerror(errno), errno);
        }
        else
        {
            printf("read from server:%s", recvl);
        }
    }
}

int main(int argc, char **argv)
{
    int sockfd, serverfd;
    struct sockaddr_in serverAddr;
    int ret = 0;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0)
    {
        printf("client create socket error: %s(errno: %d)\n",strerror(errno),errno);
        exit(0);
    }

    bzero(&serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERV_PORT);

    inet_pton(AF_INET, argv[1], &serverAddr.sin_addr);

    ret = connect(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if(ret < 0)
    {
        printf("client connect error:%s(errno:%d)", strerror(errno), errno);
        close(sockfd);
    }

    str_client(stdin, sockfd);

    return 0;
    
}

