#pragma once
#include "TcpConnection.h"
#include "Acceptor.h"

class EventLoop;
class TcpAddr;

class TcpServer
{
typedef std::map<std::string, TcpConnectionPtr> ConnectionMap;

public:
    TcpServer(EventLoop *loop, const TcpAddr& listenAddr);
    ~TcpServer();
private:
    std::unique_ptr<Acceptor> acceptor_;
    ConnectionMap connections_;
};
