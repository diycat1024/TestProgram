#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>

#include <errno.h>
#include <string.h>

int main(int argc, char const *argv[])
{
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd == -1)
    {
        fprintf(stderr, "socket error %s!!!\n", strerror(errno));
        exit(1);
    }
    struct sockaddr_in addr;
    addr.sin_port = htons(8883);
    addr.sin_addr.s_addr = inet_addr("192.168.6.143");
    addr.sin_family = AF_INET;

    int ret = connect(sock_fd,(struct sockaddr*)&addr,sizeof(struct sockaddr));
    if (ret == -1)
    {
        fprintf(stderr,"connect error %s\n", strerror(errno));
        exit(1);
    }

    char buff[1024] = {0};
    sprintf(buff, "%s", "helloworld\n");
    printf("send msg %s\n", buff);
    send(sock_fd, buff, strlen(buff),0);
    recv(sock_fd,buff,sizeof(buff), 0);
    printf("recv msg %s\n", buff);
    return 0;
}
