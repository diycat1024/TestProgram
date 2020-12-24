#include <iostream>
#include "EchoServer.h"

int main()
{
    EventLoop loop;
    TcpAddr addr(8883);
    TcpServer server(&loop,addr, "reearae");
    server.start();

    loop.loop();
}