#include "EventLoop.h"
#include "TcpAddr.h"
#include "TcpServer.h"

class EchoServer
{
public:
    EchoServer(EventLoop *loop, const TcpAddr& addr);
    ~EchoServer();

    void onConnect(const TcpConnectionPtr& conn);
    void onMessage(const TcpConnectionPtr& conn,Buffer* buf);
private:
    TcpServer server_;

};

