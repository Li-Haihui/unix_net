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



//#include "unp.h"
//#include	<sys/types.h>	/* basic system data types */
//#include	<sys/socket.h>	/* basic socket definitions */

//#include	<netinet/in.h>	/* sockaddr_in{} and other Internet defns */
//#include	<arpa/inet.h>	/* inet(3) functions */

//#include <stdio.h>
//#include <string.h>
//#include <sys/stat.h>
//#include <fcntl.h>
//#include<unistd.h>


#define MAX_BUFFER (1024)

int main(int argc, char **argv)
{
    int socketfd, dataLen;
    char recvData[MAX_BUFFER + 1];

    struct sockaddr_in servaddr;

    if (argc != 2)
    {
        printf("usage argv error\n");
        //err_quit("usage argv error");    
    }

    if((socketfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("create socket error: %s(errno: %d)\n",strerror(errno),errno);
        exit(0);
    }

    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    //servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(8000);

    if(inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
    {
        printf("inet_pton %s error\n", argv[1]);
    }

    if(connect(socketfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
    {
        printf("connect error: %s(errno: %d)\n",strerror(errno),errno);
        exit(0);
    }

    while((dataLen = read(socketfd, recvData, MAX_BUFFER)) > 0)
    {
        recvData[dataLen] = 0;
        if(fputs(recvData, stdout) == EOF)
        {
            printf("fputs error\n");
        }
    }

    if(dataLen < 0)
    {
        printf("read error\n");
    }

    return 0;

}
    
