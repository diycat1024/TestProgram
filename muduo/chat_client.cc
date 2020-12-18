#include "muduo/net/TcpClient.h"
#include "muduo/net/EventLoopThread.h"
#include "muduo/base/Logging.h"
#include "muduo/base/Mutex.h"

#include <iostream>

using namespace muduo;
using namespace muduo::net;

class ChatClient
{
public:
    ChatClient(EventLoop* loop, const InetAddress serverAddr)
    : client_(loop,  serverAddr, "ChatClient")
    {
        client_.setConnectionCallback(std::bind(&ChatClient::onConnection, this, _1));
        client_.setMessageCallback(std::bind(&ChatClient::onMessage,this, _1, _2, _3));
    }
    ~ChatClient()
    {

    }

    void connect()
    {
        client_.connect();
    }

    void disconnect()
    {
        client_.disconnect();
    }

    void  write(const std::string& msg)
    {
        MutexLockGuard lock(mutex_);
        if(connection_)
        {
             muduo::net::Buffer buf;
            int32_t len = static_cast<int32_t>(msg.size());
            int32_t be32 = htonl(len);
            buf.append(&be32,sizeof(be32));
            buf.append(msg.data(),  msg.size());
            connection_->send(&buf);
        }
    }
private:
    void onConnection(const TcpConnectionPtr& conn)
    {
        MutexLockGuard lock(mutex_);
        if (conn->connected())
        {
            connection_ = conn;
        }
        else
        {
            connection_.reset();
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
                muduo::string message(buf->peek(), len);
                LOG_INFO << "<<< " << message;
                buf->retrieve(len);
            }else
            {
                break;
            }
        }
    }
private:
    TcpClient  client_;
    MutexLock mutex_;
    TcpConnectionPtr connection_ GUARDED_BY(mutex_);
};



int main(int argc, char* argv[])
{
    LOG_INFO << "pid = " << getpid();
    if (argc < 2)
    {
        printf("Usage: %s host_ip port\n", argv[0]);
        return -1;
    }

    EventLoopThread loopThread;
    uint16_t port = static_cast<uint16_t>(atoi(argv[2]));
    InetAddress serverAddr(argv[1], port);

    ChatClient client(loopThread.startLoop(),serverAddr);
    client.connect();

    std::string line;
    while(std::getline(std::cin, line))
    {
        client.write(line);
    }
    client.disconnect();
    printf("client disconnect");

    CurrentThread::sleepUsec(1000*1000);

    return 1;
}