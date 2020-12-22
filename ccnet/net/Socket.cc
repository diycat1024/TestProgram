#include "Socket.h"

Socket::Socket(const TcpAddr& addr)
{
    fd_ = ::socket(addr.family(), SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP);
    if (fd_ == -1)
    {
        perror("sock error\n"); return;
    }
    if ( bind(fd_, (struct sockaddr*)&addr.addr(),sizeof(addr.addr())) <= 0)
    {
        perror("bind error\n");
        return;
    }

    if (listen(fd_, 5) <= 0)
    {
        perror("listen error\n");
        return;
    }
}

Socket::~Socket()
{
}

