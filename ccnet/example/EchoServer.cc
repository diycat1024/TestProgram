#include "EchoServer.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

EchoServer::EchoServer(EventLoop *loop, const TcpAddr& addr)
:server_(loop,addr, "EchoServer")
{
}

EchoServer::~EchoServer()
{
}
void EchoServer::onConnect(const TcpConnectionPtr& conn)
{

}

void EchoServer::onMessage(const TcpConnectionPtr& conn,Buffer* buf)
{
    pritnf("recv from client msg: %s\n",  buf->peek());
    conn->send(buf);
}
