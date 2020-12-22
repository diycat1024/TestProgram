#pragma once
#include <memory>
#include "Channel.h"
#include "Socket.h"

class EventLoop;
class TcpAddr;

class Acceptor
{
public:
    Acceptor(EventLoop *loop, const TcpAddr& addr);
    ~Acceptor();
    int fd() const { return accept_socket_->fd();}

private:
    std::unique_ptr<Channel> accept_channel_;
    std::unique_ptr<Socket> accept_socket_;
};
    