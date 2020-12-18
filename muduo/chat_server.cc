#include  <iostream>
#include "muduo/base/Logging.h"
#include "muduo/base/Mutex.h"
#include "muduo/net/EventLoop.h"
#include "muduo/net/TcpServer.h"
#include <set>

using namespace muduo;
using namespace muduo::net;

class ChatServer
{
    typedef std::set<TcpConnectionPtr> Connectionlist;
public:
    ChatServer(EventLoop*loop,const InetAddress& listenAAddr)
    :server_(loop,listenAAddr, "ChatServer")
    {
        server_.setConnectionCallback(std::bind(&ChatServer::onConnection, this, _1));
        server_.setMessageCallback(std::bind(&ChatServer::onMessage,this, _1, _2, _3));
    }
    ~ChatServer()
    {

    }

    void start()
    {
        server_.start();
    }

private:
    void onConnection(const TcpConnectionPtr& conn)
    {
        LOG_INFO << conn->peerAddress().toIpPort() << "->"
                 << conn->localAddress().toIpPort() << " is "
                 << (conn->connected() ? "UP" :  "DOWN");
        if  (conn->connected())
        {
            connections_.insert(conn);
        }
        else
        {
            connections_.erase(conn);
        }
    }
    void onMessage(const TcpConnectionPtr& conn,
                            Buffer* buf,
                            Timestamp)
    {
        while(buf->readableBytes() >= 4)
        {
            const void* data = buf->peek();
            int32_t be32 = *static_cast<const int32_t*>(data);
            const int32_t len = ntohl(be32);
            if (len > 65536 || len < 0)
            {
                LOG_ERROR << "Invalid length " << len;
                conn->shutdown(); // FIXME: disable reading
                break;
            }
            else if (buf->readableBytes() >= len + 4)
            {
                buf->retrieve(4);
                muduo::string message(buf->peek(),len);
                send(message);
                buf->retrieve(len);
            }else
            {
                break;
            }
        }
    }

    void send(const muduo::string& msg)
    {
        for  (auto&conn : connections_)
        {
            muduo::net::Buffer buf;
            int32_t len = static_cast<int32_t>(msg.size());
            int32_t be32 = htonl(len);
            buf.append(&be32,sizeof(be32));
            buf.append(msg.data(),  msg.size());
            conn->send(&buf);
        }
    }

    
private:
    /* data */
    TcpServer server_;
    Connectionlist connections_;
};



int main(int argc, char* argv[])
{
    LOG_INFO <<  "pid=" << getpid();
    if (argc > 1)
    {
        EventLoop loop;
        InetAddress  listenAddr(atoi(argv[1]));
        ChatServer server(&loop, listenAddr);
        server.start();
        loop.loop();
    }
    else
    {
        printf("Usage: %s port\n", argv[0])        ;
    }
    
}