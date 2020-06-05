#include <stdio.h>
#include <winsock2.h>

int main()
{
    //初始化DLL
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2,2),& wsaData);

    //创建套接字
    SOCKET servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    //绑定套接字
    sockaddr_in sockAddr;
    memset(&sockAddr, 0, sizeof sockAddr);
    sockAddr.sin_family = PF_INET; //使用IPV4地址
    sockAddr.sin_addr.s_addr = inet_addr("0.0.0.0");// 具体的IP地址
    sockAddr.sin_port = htons(1234);
    bind(servSock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));

    //进入侦听状态
    listen(servSock, 20);

    //接受客户端请求
    SOCKADDR clntAddr;
    int nSize = sizeof(SOCKADDR);
    memset(&sockAddr, 0, nSize);

    //接受服务器传回的数据
    char szBuffer[MAXBYTE] = {0};
    printf("Start Accept...\n");
    while(1)
    {
        SOCKET clntSock = accept(servSock, (SOCKADDR*)&clntAddr, &nSize);
        // 向客户端发送数据
       recv(clntSock, szBuffer, MAXBYTE, NULL);
       send(clntSock,szBuffer, strlen(szBuffer), 0);

        //关闭套接字
        closesocket(clntSock);
    }
    closesocket(servSock);

    //终止DLL的使用
    WSACleanup();

    return -1;


}