#pragma once
#include <memory>
#include <map>

class TcpConnection
{
private:
    /* data */
public:
    TcpConnection(/* args */);
    ~TcpConnection();
};
typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;