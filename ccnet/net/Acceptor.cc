#include "Acceptor.h"

Acceptor::Acceptor(EventLoop *loop, const TcpAddr& addr)
:accept_socket_(new Socket(addr)),
accept_channel_(new Channel(loop))
{
     
}

Acceptor::~Acceptor()
{
}
