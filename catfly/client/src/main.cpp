#include <stdio.h>
#include <winsock2.h>

int main()
{
    //初始化DLL
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2,2),& wsaData);

    //创建套接字
    SOCKET sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    //连接服务器
    sockaddr_in sockAddr;
    memset(&sockAddr, 0, sizeof(sockAddr));
    sockAddr.sin_family = PF_INET;
    sockAddr.sin_addr.s_addr = inet_addr("192.168.0.106");
    sockAddr.sin_port = htons(1234);

    connect(sock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));

    int i = 0;
    //接受服务器传回的数据
    char szBuffer[MAXBYTE] = {0};
    while(true)
    {
        i++;
        sprintf(szBuffer,"Hello World: %d", i);
        send(sock,szBuffer, strlen(szBuffer)+sizeof(char), 0);
        //输出接收到的数据
        recv(sock, szBuffer, MAXBYTE, NULL);
        printf("Message form server: %s\n", szBuffer);
    }

    //关闭套接字
    closesocket(sock);

    //终止使用DLL
    WSACleanup();

    system("pause");
    return 0;
}