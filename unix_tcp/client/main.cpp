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

int main()
{
    int client_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    if (client_socket == -1)
    {
        handleError("创建socket失败！");
        return -1;
    }
    struct sockaddr_un addr;
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, path, sizeof(addr.sun_path));

    if (connect(client_socket, (struct sockaddr*)&addr, sizeof(addr)) == -1)
    {
        handleError("服务器连接失败！");
        return -1;
    }
    while (true)
    {
        fgets(buffer, BUFFER_SIZE, stdin);
        if (send(client_socket, buffer, strlen(buffer), 0)== -1)
        {
            handleError("发送失败");
            return -1;
        }
        int num_read = recv(client_socket, buffer, BUFFER_SIZE, 0);
        if (num_read == -1)
        {
            handleError("对端已经关闭");
        }
        buffer[num_read] = '\0';
        printf("%s\n", buffer);
    }
}
