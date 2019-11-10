#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include <errno.h>  
#include <sys/types.h>  
#include <sys/socket.h>  
#include <netinet/in.h> 
#include <time.h>
#include <unistd.h>


//#include	<sys/types.h>	/* basic system data types */
//#include	<sys/socket.h>	/* basic socket definitions */

//#include	<netinet/in.h>	/* sockaddr_in{} and other Internet defns */
//#include	<arpa/inet.h>	/* inet(3) functions */

//#include <stdio.h>
//#include <string.h>
//#include <sys/stat.h>
//#include <fcntl.h>
//#include <unistd.h>
#include <time.h>

#define MAX_BUFFER (1024)
#define LISTEN_NUM (10)

int main(int argc, char **argv)
{
    int socketfd, connfd;
    struct sockaddr_in servaddr;
    char buff[MAX_BUFFER];
    time_t ticks;

    socketfd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(8000);

    bind(socketfd, (struct sockaddr*)&servaddr, sizeof(servaddr));

    listen(socketfd, LISTEN_NUM);

    for(; ;)
    {
        connfd = accept(socketfd, (struct sockaddr*)NULL, NULL);

        ticks = time(NULL);

        snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));

        write(connfd, buff, strlen(buff));

        close(connfd);
    }
}

