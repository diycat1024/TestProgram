#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024
#define BACK_LOG 1000
char buffer[BUFFER_SIZE] = {0};
char path[] = "../namo_am";

void handleError(char* msg)
{
    perror(msg);
    exit(-1);
}

int32_t echo(int socket)
{
    int num_read, num_write = 0;
    while(true)
    {
        num_read = recv(socket, buffer, BUFFER_SIZE, 0);
        if (num_read == -1)
        {
            handleError("读取数据错误！");
            return -1;
        } else if (num_read == 0)
        {
            printf("客户端关闭链接!\n");
            close(socket);
            return -1;
        }
        printf("收到对端进程数据长度为%d, 开始echo\n", num_read);
        
        if (num_read > 0)
        {
            num_write = write(socket, buffer, num_read);
            printf("写入的结果为：%d\n", num_write);
        }
    }
}

int32_t handleRequest(int server_socket)
{
    int socket = accept(server_socket, NULL, NULL);
    if (socket == -1)
    {
        handleError("accept 错误");
        return -1;
    }
    puts("client 发起连接。。。\n");
    return echo(socket);
}

void bindToAddress(int server_socket)
{
    struct sockaddr_un address;
    address.sun_family = AF_UNIX;
    strncpy(address.sun_path, path, sizeof(path));
    if (remove(path) && errno != ENOENT)
    {
        handleError("删除失败！");
    }
    if (bind(server_socket, (struct sockaddr *)&address,sizeof(address)) == -1)
    {
        handleError("绑定地址失败!");
    }
}
int main()
{

    int server_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_socket == -1 )
    {
        handleError("创建socket失败！\n");
        return -1;
    }
    bindToAddress(server_socket);
    if (listen(server_socket, BACK_LOG) == -1)
    {
        handleError("监听失败！");
        return -1;
    }
    while(true)
    {
        if (handleRequest(server_socket) == -1)
            break;
    }


    return 1;
}
