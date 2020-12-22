#include "TcpServer.h"


TcpServer::TcpServer(EventLoop *loop, const TcpAddr& listenAddr)
: acceptor_(new Acceptor(loop, listenAddr))
{
    
}

TcpServer::~TcpServer()
{
}
